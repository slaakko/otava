// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.lexeme;

import std;

export namespace soul::lexer {

template<typename Char>
struct Lexeme
{
    inline Lexeme() noexcept : begin(nullptr), end(nullptr) {}
    inline Lexeme(const Char* begin_, const Char* end_) noexcept : begin(begin_), end(end_) {}
    inline std::basic_string<Char> ToString() const noexcept { return std::basic_string<Char>(begin, end); }
    inline int Length() const noexcept { return static_cast<int>(end - begin); }
    const Char* begin;
    const Char* end;
};

template<typename Char>
bool operator==(const Lexeme<Char>& left, const Lexeme<Char>& right) noexcept
{
    if (left.end - left.begin != right.end - right.begin) return false;
    const Char* p = left.begin;
    const Char* q = right.begin;
    while (p != left.end)
    {
        if (*p != *q) return false;
        ++p;
        ++q;
    }
    return true;
}

template<typename Char>
bool operator<(const Lexeme<Char>& left, const Lexeme<Char>& right) noexcept
{
    const Char* p = left.begin;
    const Char* q = right.begin;
    while (p != left.end && q != right.end)
    {
        if (*p < *q) return true;
        if (*p > *q) return false;
        ++p;
        ++q;
    }
    if (p == left.end) return q != right.end;
    return false;
}

template<typename Char>
struct LexemeCompare
{
    inline bool operator()(const Lexeme<Char>& left, const Lexeme<Char>& right) const noexcept
    {
        return left < right;
    }
};

} // namespace soul::lexer

