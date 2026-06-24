// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.class_info;

import std;
import util.binary_stream_reader;
import util.binary_stream_writer;

export namespace otava::symbols {

enum class class_id : std::uint32_t {};

enum class class_key : std::uint8_t
{
    cls = 0, strct = 1, uni = 2
};

constexpr std::uint32_t to_underlying(class_id cid)
{
    return std::uint32_t(cid);
}

class class_info
{
public:
    class_info();
    class_info(class_id id_, class_key key_, const std::string& name_, std::int64_t size_);
    inline const class_id& get_id() const noexcept { return id; }
    inline class_key get_key() const noexcept { return key; }
    inline const std::string& get_name() const noexcept { return name; }
    inline std::int64_t get_size() const noexcept { return size; }
    inline void set_size(std::int64_t size_) noexcept { size = size_; }
    void add_base(const class_id& id);
    inline const std::vector<class_id>& base_class_ids() const noexcept { return bases; }
    void read(util::BinaryStreamReader& reader);
    void write(util::BinaryStreamWriter& writer);
private:
    class_id id;
    class_key key;
    std::string name;
    std::int64_t size;
    std::vector<class_id> bases;
};

class class_index
{
public:
    class_index();
    void import(const class_index& that);
    void add_class(std::unique_ptr<class_info>& info);
    class_info* get_class_info(const class_id& id);
    void read(util::BinaryStreamReader& reader);
    void write(util::BinaryStreamWriter& writer);
private:
    std::vector<std::unique_ptr<class_info>> infos;
    std::map<class_id, class_info*> map;
};

} // namespace otava::symbols
