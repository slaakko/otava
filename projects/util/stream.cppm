export module util.stream;

import std;

export namespace util {

enum class Origin
{
    seekSet, seekCur, seekEnd
};

class StreamObserver;

class Stream
{
public:
    Stream() noexcept;
    virtual ~Stream();
    virtual int ReadByte() = 0;
    virtual std::int64_t Read(std::uint8_t* buf, std::int64_t count) = 0;
    virtual void Write(std::uint8_t x) = 0;
    virtual void Write(std::uint8_t* buf, std::int64_t count) = 0;
    virtual void Flush();
    virtual void Seek(std::int64_t pos, Origin origin);
    virtual std::int64_t Tell();
    void CopyTo(Stream& destination);
    void CopyTo(Stream& destination, std::int64_t bufferSize);
    inline std::int64_t Position() const noexcept { return position; }
    void SetPosition(std::int64_t position_);
    void AddObserver(StreamObserver* observer) noexcept;
    void RemoveObserver(StreamObserver* observer) noexcept;
private:
    std::int64_t position;
    std::vector<StreamObserver*> observers;
};

class StreamObserver
{
public:
    StreamObserver() noexcept;
    virtual ~StreamObserver();
    virtual void PositionChanged(Stream* stream) {}
};

class Streams
{
public:
    Streams() noexcept;
    Streams(Streams&& that) noexcept;
    Streams(const Streams&) = delete;
    Streams& operator=(Streams&& that) noexcept;
    Streams& operator=(const Streams&) = delete;
    ~Streams();
    void Add(Stream* stream);
    Stream* Get(int index) const noexcept;
    Stream& Back() const;
    inline int Count() const noexcept { return static_cast<int>(streams.size()); }
private:
    std::vector<std::unique_ptr<Stream>> streams;
};

} // util
