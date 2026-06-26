// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.class_info;

import util.binary_resource_ptr;
import util.path;
import util.system;
import util.memory_stream;

namespace otava::symbols {

class_info::class_info() : id(), key(class_key::cls), name(), bases(), size(0)
{
}

class_info::class_info(class_id id_, class_key key_, const std::string& name_, std::int64_t size_) : id(id_), key(key_), name(name_), size(size_)
{
}

void class_info::add_base(const class_id& id)
{
    bases.push_back(id);
}

void class_info::read(util::BinaryStreamReader& reader)
{
    id = class_id(reader.ReadUInt());
    key = static_cast<class_key>(reader.ReadByte());
    name = reader.ReadUtf8String();
    size = reader.ReadLong();
    std::int32_t nb = reader.ReadInt();
    for (int i = 0; i < nb; ++i)
    {
        std::uint32_t b = reader.ReadUInt();
        add_base(class_id(b));
    }
}

void class_info::write(util::BinaryStreamWriter& writer)
{
    std::uint32_t cid = to_underlying(id);
    writer.Write(cid);
    writer.Write(static_cast<std::uint8_t>(key));
    writer.Write(name);
    writer.Write(size);
    std::int32_t nb = std::int32_t(bases.size());
    writer.Write(nb);
    for (int i = 0; i < nb; ++i)
    {
        std::uint32_t b = to_underlying(bases[i]);
        writer.Write(b);
    }
}

class_index::class_index()
{
}

void class_index::add_class(std::unique_ptr<class_info>& info)
{
    class_info* prev = get_class_info(info->get_id());
    if (!prev)
    {
        map[info->get_id()] = info.get();
        infos.push_back(std::unique_ptr<class_info>(info.release()));
    }
}

void class_index::import(const class_index& that)
{
    for (const auto& info : that.infos)
    {
        if (!get_class_info(info->get_id()))
        {
            std::unique_ptr<class_info> i(new class_info(*info.get()));
            add_class(i);
        }
    }
}

void class_index::read(util::BinaryStreamReader& reader)
{
    std::int32_t n = reader.ReadInt();
    for (std::int32_t i = 0; i < n; ++i)
    {
        std::unique_ptr<class_info> info(new class_info());
        info->read(reader);
        add_class(info);
    }
}

void class_index::write(util::BinaryStreamWriter& writer)
{
    std::int32_t n = std::int32_t(infos.size());
    writer.Write(n);
    for (std::int32_t i = 0; i < n; ++i)
    {
        class_info* info = infos[i].get();
        info->write(writer);
    }
}

class_info* class_index::get_class_info(const class_id& id)
{
    auto it = map.find(id);
    if (it != map.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

} // namespace otava::symbols
