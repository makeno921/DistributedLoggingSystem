#include "TcpConnection.h"
#include <iostream>
#include <cstring>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
bool TcpConnection::winsockInitialized_ = false;
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#endif

TcpConnection::TcpConnection() : socket_(INVALID_SOCKET), connected_(false), port_(0) {
#ifdef _WIN32
    initializeWinsock();
#endif
}

TcpConnection::~TcpConnection() {
    disconnect();
}

#ifdef _WIN32
void TcpConnection::initializeWinsock() {
    if (!winsockInitialized_) {
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
            std::cerr << "[TcpConnection] WSAStartup failed!\n";
        }
        winsockInitialized_ = true;
    }
}
#endif

bool TcpConnection::connect(const std::string& host, int port) {
    host_ = host;
    port_ = port;

    socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ == INVALID_SOCKET) {
        std::cerr << "[TcpConnection] Failed to create socket\n";
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr) <= 0) {
        std::cerr << "[TcpConnection] Invalid address: " << host << "\n";
        return false;
    }

    if (::connect(socket_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "[TcpConnection] Connection failed to " << host << ":" << port << "\n";
        return false;
    }

    connected_ = true;
    std::cout << "[TcpConnection] Successfully connected to " << host << ":" << port << "\n";
    return true;
}

bool TcpConnection::listen(int port, int backlog) {
    socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ == INVALID_SOCKET) {
        std::cerr << "[TcpConnection] Failed to create listening socket\n";
        return false;
    }

    int opt = 1;
    setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (::bind(socket_, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "[TcpConnection] Bind failed on port " << port << "\n";
        return false;
    }

    if (::listen(socket_, backlog) == SOCKET_ERROR) {
        std::cerr << "[TcpConnection] Listen failed\n";
        return false;
    }

    connected_ = true;
    std::cout << "[TcpConnection] Server listening on port " << port << "...\n";
    return true;
}

bool TcpConnection::acceptConnection(TcpConnection& newConnection) {
    sockaddr_in clientAddr{};
    socklen_t addrLen = sizeof(clientAddr);

    int clientSocket = ::accept(socket_, (sockaddr*)&clientAddr, &addrLen);
    if (clientSocket == INVALID_SOCKET) {
        return false;
    }

    newConnection.socket_ = clientSocket;
    newConnection.connected_ = true;
    std::cout << "[TcpConnection] New client connected!\n";
    return true;
}

std::string TcpConnection::receive() {
    if (!connected_ || socket_ == INVALID_SOCKET) return "";

    char buffer[8192] = {0};
    int bytes = ::recv(socket_, buffer, sizeof(buffer) - 1, 0);
    if (bytes > 0) {
        return std::string(buffer, bytes);
    }
    return "";
}

bool TcpConnection::send(const std::string& data) {
    if (!connected_ || socket_ == INVALID_SOCKET) {
        std::cerr << "[TcpConnection] Cannot send - not connected\n";
        return false;
    }

    if (::send(socket_, data.c_str(), data.length(), 0) == SOCKET_ERROR) {
        std::cerr << "[TcpConnection] Send failed\n";
        return false;
    }
    return true;
}

void TcpConnection::disconnect() {
    if (connected_ && socket_ != INVALID_SOCKET) {
#ifdef _WIN32
        closesocket(socket_);
#else
        close(socket_);
#endif
        socket_ = INVALID_SOCKET;
        connected_ = false;
    }
}

bool TcpConnection::isConnected() const {
    return connected_;
}