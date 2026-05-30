// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.util;

import util.unicode;
import util.text_util;
import util.unicode;

namespace otava::ast {

std::string UniversalCharacterName(char32_t c)
{
    std::string s;
    s.append("\\");
    if (c >= 0 && c <= 0xFFFF)
    {
        s.append("u").append(util::ToHexString(static_cast<std::uint16_t>(c)));
    }
    else
    {
        s.append("").append(util::ToHexString(static_cast<std::uint32_t>(c)));
    }
    return s;
}

std::string ToUniversalId(const std::string& id)
{
    std::string s;
    std::u32string u32s = util::ToUtf32(id);
    for (char32_t c : id)
    {
        if (c >= 32 && c < 127)
        {
            s.append(1, char(c));
        }
        else
        {
            s.append(UniversalCharacterName(c));
        }
    }
    return s;
}

} // namespace otava::ast
