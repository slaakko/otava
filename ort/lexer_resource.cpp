// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module lexer_resource;

import util.path;
import util.system;
import util.binary_resource_ptr;
import util.memory_stream;
import util.binary_stream_reader;
import util.deflate_stream;
import util.compression;
import soul.lexer.classmap;

std::int32_t* read_lexer_resource(const char* resource_name, std::int32_t& size)
{
    std::string currentExecutableName = util::Path::GetFileName(util::GetFullPath(util::GetPathToExecutable()));
    util::BinaryResourcePtr resource(currentExecutableName, resource_name);
    util::MemoryStream memoryStream(resource.Data(), resource.Size());
    util::BinaryStreamReader rawReader(memoryStream);
    size = rawReader.ReadInt();
    std::int32_t* data = new std::int32_t[size];
    util::DeflateStream compressedStream(util::CompressionMode::decompress, memoryStream);
    util::BinaryStreamReader reader(compressedStream);
    for (std::int64_t i = 0; i < size; ++i)
    {
        std::int32_t x = reader.ReadInt();
        data[i] = x;
    }
    return data;
}

void free_lexer_resource(std::int32_t* lexer_resource)
{
    delete[](lexer_resource);
}
