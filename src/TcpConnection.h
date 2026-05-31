#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <functional>

class TcpConnection {
public:
    TcpConnection();
    ~TcpConnection();

    // Клиентские методы
    bool connect(const std::string& host, int port);
    bool send(const std::string& data);

    // Серверные методы
    bool listen(int port, int backlog = 10);
    bool acceptConnection(TcpConnection& newConnection);

    // Общие методы
    std::string receive();
    void disconnect();
    bool isConnected() const;

    int getSocket() const { return socket_; }

private:
    int socket_;
    bool connected_;
    std::string host_;
    int port_;

#ifdef _WIN32
    static bool winsockInitialized_;
    static void initializeWinsock();
#endif
};