module util.file_mapping;

namespace util {

FileMapping::FileMapping(const std::string& fileName_) : fileName(fileName_), fileHandle(nullptr), fileMappingHandle(nullptr), start(nullptr), length(-1)
{
    bool succeeded = ort_create_file_mapping(fileName.c_str(), fileHandle, fileMappingHandle, start, length);
    if (!succeeded)
    {
        std::string msg = "could not create file mapping for file '" + fileName_ + "'";
        throw std::runtime_error(msg);
    }
}

FileMapping::~FileMapping()
{
    ort_destruct_file_mapping(fileHandle, fileMappingHandle, start);
}

} // util
