export module soul.ast.lexer_pos_pair;

import std;

export namespace soul::ast::lexer::pos::pair {

struct LexerPosPair
{
    LexerPosPair() noexcept : start(-1), end(-1) {}
    LexerPosPair(std::int64_t start_, std::int64_t end_) noexcept : start(start_), end(end_) {}
    bool IsValid() const noexcept { return start != -1 && end != -1; }
    std::int64_t start;
    std::int64_t end;
};

} // namespace soul::ast::lexer::pos::pair
