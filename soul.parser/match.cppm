// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.parser.match;

import std;

export namespace soul::parser {

struct Match
{
    Match() noexcept : hit(false), value(nullptr) {}
    Match(bool hit_) noexcept : hit(hit_), value(nullptr) {}
    Match(bool hit_, void* value_) noexcept : hit(hit_), value(value_) {}
    bool hit;
    void* value;
};

} // namespace soul::parser;
