// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.socket;

import std;

export namespace util {

enum class ShutdownMode
{
    receive = 0, send = 1, both = 2
};

std::int64_t CreateSocket();
void BindSocket(std::int64_t socketHandle, int port);
void ListenSocket(std::int64_t socketHandle, int backLog);
std::int64_t AcceptSocket(std::int64_t socketHandle);
void CloseSocket(std::int64_t socketHandle);
void ShutdownSocket(std::int64_t socketHandle, ShutdownMode mode);
std::int64_t ConnectSocket(const std::string& node, const std::string& service);
int SendSocket(std::int64_t socketHandle, const std::uint8_t* buf, int len, int flags);
int ReceiveSocket(std::int64_t socketHandle, std::uint8_t* buf, int len, int flags);
void SocketInit();
void SocketDone();

class TcpSocket
{
public:
    TcpSocket();
    TcpSocket(const std::string& node, const std::string& service);
    TcpSocket(std::int64_t handle_) noexcept;
    TcpSocket(const TcpSocket&) = delete;
    TcpSocket& operator=(const TcpSocket&) = delete;
    TcpSocket(TcpSocket&& that) noexcept;
    TcpSocket& operator=(TcpSocket&& that) noexcept;
    ~TcpSocket();
    void Close();
    void Connect(const std::string& node, const std::string& service);
    void Bind(int port);
    void Listen(int backlog);
    TcpSocket Accept();
    void Shutdown(ShutdownMode mode);
    void Send(const std::uint8_t* buffer, int count);
    int Receive(std::uint8_t* buffer, int count);
private:
    std::int64_t handle;
    bool connected;
    bool shutdown;
};

void Write(TcpSocket& socket, const std::string& s);
std::string ReadStr(TcpSocket& socket);

} // namespace util
