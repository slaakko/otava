// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.util;

import util.unicode;
import util.text.util;

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
        s.append("U").append(util::ToHexString(static_cast<std::uint32_t>(c)));
    }
    return s;
}

std::u32string ToUniversalId(const std::u32string& id)
{
    std::u32string s;
    for (char32_t c : id)
    {
        if (c >= 32 && c < 127)
        {
            s.append(1, c);
        }
        else
        {
            s.append(util::ToUtf32(UniversalCharacterName(c)));
        }
    }
    return s;
}

} // namespace otava::ast
