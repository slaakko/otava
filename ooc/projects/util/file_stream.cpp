module util.file_stream;

import std;
import util.text_util;

namespace util {

FileStream::FileStream(int handle) : filePath(), file(nullptr), needToClose(false)
{
    switch (handle)
    {
        case 0:
        {
            file = stdin;
            filePath = "STDIN";
            break;
        }
        case 1:
        {
            file = stdout;
            filePath = "STDOUT";
            break;
        }
        case 2:
        {
            file = stderr;
            filePath = "STDERR";
            break;
        }
    }
    throw std::runtime_error("invalid handle value");
}

FileStream::FileStream(const std::string& filePath_, OpenMode openMode) : filePath(filePath_), file(), needToClose(false)
{
    std::string mode;
    if ((openMode & OpenMode::read) != OpenMode::none && (openMode & OpenMode::write) == OpenMode::none)
    {
        mode += "r";
    }
    if ((openMode & OpenMode::write) != OpenMode::none)
    {
        mode += "w";
        if ((openMode & OpenMode::read) != OpenMode::none)
        {
            mode += "+";
        }
    }
    if ((openMode & OpenMode::append) != OpenMode::none)
    {
        if ((openMode & (OpenMode::read | OpenMode::write)) != OpenMode::none)
        {
            std::string msg("could not open file '");
            msg.append(filePath).append("': open mode not supported");
            throw std::runtime_error(msg);
        }
        else
        {
            mode += "a";
        }
    }
    if ((openMode & OpenMode::binary) != OpenMode::none)
    {
        mode += "b";
    }
    file = std::fopen(filePath.c_str(), mode.c_str());
    if (!file)
    {
        std::string msg("could not open file '");
        int errorNumber = ort_error_number();
        msg.append(filePath).append("': ").append(ort_error_str(errorNumber));
        throw std::runtime_error(msg);
    }
    needToClose = true;
}

FileStream::~FileStream()
{
    if (file && needToClose)
    {
        std::fclose(file);
    }
}

int FileStream::ReadByte()
{
    int result = std::fgetc(file);
    if (result == EOF)
    {
        if (std::feof(file))
        {
            return -1;
        }
        else
        {
            std::string msg("could not read from file '");
            int errorNumber = ort_error_number();
            msg.append(filePath).append("': ").append(ort_error_str(errorNumber));
            throw std::runtime_error(msg);
        }
    }
    SetPosition(Position() + 1);
    return result;
}

std::int64_t FileStream::Read(std::uint8_t* buf, std::int64_t count)
{
    std::int64_t result = std::fread(buf, 1, count, file);
    if (std::ferror(file))
    {
        std::string msg("could not read from file '");
        int errorNumber = ort_error_number();
        msg.append(filePath).append("': ").append(ort_error_str(errorNumber));
        throw std::runtime_error(msg);
    }
    SetPosition(Position() + result);
    return result;
}

void FileStream::Write(std::uint8_t x)
{
    int result = std::fputc(x, file);
    if (result == EOF)
    {
        std::string msg("could not write to file '");
        int errorNumber = ort_error_number();
        msg.append(filePath).append("': ").append(ort_error_str(errorNumber));
        throw std::runtime_error(msg);
    }
    SetPosition(Position() + 1);
}

void FileStream::Write(std::uint8_t* buf, std::int64_t count)
{
    std::int64_t result = std::fwrite(buf, 1, count, file);
    if (result != count)
    {
        std::string msg("could not write to file '");
        int errorNumber = ort_error_number();
        msg.append(filePath).append("': ").append(ort_error_str(errorNumber));
        throw std::runtime_error(msg);
    }
    SetPosition(Position() + result);
}

void FileStream::Flush()
{
    int result = std::fflush(file);
    if (result != 0)
    {
        std::string msg("could not flush file '");
        int errorNumber = ort_error_number();
        msg.append(filePath).append("': ").append(ort_error_str(errorNumber));
        throw std::runtime_error(msg);
    }
}

void FileStream::Seek(std::int64_t pos, Origin origin)
{
    int seekOrigin = SEEK_SET;
    switch (origin)
    {
        case Origin::seekCur:
        {
            seekOrigin = SEEK_CUR;
            break;
        }
        case Origin::seekEnd:
        {
            seekOrigin = SEEK_END;
            break;
        }
        case Origin::seekSet:
        {
            seekOrigin = SEEK_SET;
            break;
        }
    }
    int result = std::fseek(file, long(pos), seekOrigin);
    if (result != 0)
    {
        std::string msg("could not seek file '");
        int errorNumber = ort_error_number();
        msg.append(filePath).append("' ").append(ort_error_str(errorNumber));
        throw std::runtime_error(msg);
    }
    switch (origin)
    {
        case Origin::seekCur:
        {
            SetPosition(Position() + pos);
            break;
        }
        case Origin::seekEnd:
        {
            SetPosition(Size() + pos);
            break;
        }
        case Origin::seekSet:
        {
            SetPosition(pos);
            break;
        }
    }
}

std::int64_t FileStream::Tell()
{
    std::int64_t result = std::ftell(file);
    if (result == -1)
    {
        std::string msg("could not tell file '");
        int errorNumber = ort_error_number();
        msg.append(filePath).append("': ").append(ort_error_str(errorNumber));
        throw std::runtime_error(msg);
    }
    return result;
}

std::uint64_t FileStream::Size() const
{
    return ort_file_size(filePath.c_str());
}

std::string ReadFile(const std::string& filePath)
{
    return ReadFile(filePath, false);
}

std::string ReadFile(const std::string& filePath, bool doNotSkipBOM)
{
    std::string s;
    FileStream fs(filePath, OpenMode::read | OpenMode::binary);
    std::int64_t n = fs.Size();
    for (std::int64_t i = 0; i < n; ++i)
    {
        int c = fs.ReadByte();
        bool skip = false;
        if (!doNotSkipBOM && i <= 2)
        {
            if (i == 0 && c == 0xEF)
            {
                skip = true;
            }
            else if (i == 1 && c == 0xBB)
            {
                skip = true;
            }
            else if (i == 2 && c == 0xBF)
            {
                skip = true;
            }
        }
        if (!skip)
        {
            if (c != -1)
            {
                s.append(1, static_cast<char>(c));
            }
            else
            {
                std::string msg("unexpected end of '");
                msg.append(filePath).append("'");
                throw std::runtime_error(msg);
            }
        }
    }
    return s;
}

} // util
