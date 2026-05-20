// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.socket_stream;

import util.stream;
import util.socket;
import std;

export namespace util {

class SocketStream : public Stream
{
public:
    SocketStream(TcpSocket&& socket_);
    inline const TcpSocket& Socket() const { return socket; }
    inline TcpSocket& Socket() { return socket; }
    int ReadByte() override;
    std::int64_t Read(std::uint8_t* buf, std::int64_t count) override;
    void Write(std::uint8_t x) override;
    void Write(std::uint8_t* buf, std::int64_t count) override;
private:
    TcpSocket socket;
};

} // util
