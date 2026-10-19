
// this file has been automatically generated from 'D:/work/soul-mod/tools/otava/projects/soul/token/soul.cpp.token' using soul lexer generator version 5.0.0

export module soul.cpp.token;

import std;

export namespace soul::cpp::token {

constexpr std::int32_t tokenSetID = 1960179546;

constexpr std::int64_t AUTO = (static_cast<std::int64_t>(tokenSetID) << 32) | 1;
constexpr std::int64_t REGISTER = (static_cast<std::int64_t>(tokenSetID) << 32) | 2;
constexpr std::int64_t STATIC = (static_cast<std::int64_t>(tokenSetID) << 32) | 3;
constexpr std::int64_t EXTERN = (static_cast<std::int64_t>(tokenSetID) << 32) | 4;
constexpr std::int64_t MUTABLE = (static_cast<std::int64_t>(tokenSetID) << 32) | 5;
constexpr std::int64_t EXPORT = (static_cast<std::int64_t>(tokenSetID) << 32) | 6;
constexpr std::int64_t IMPORT = (static_cast<std::int64_t>(tokenSetID) << 32) | 7;
constexpr std::int64_t MODULE = (static_cast<std::int64_t>(tokenSetID) << 32) | 8;
constexpr std::int64_t CHAR = (static_cast<std::int64_t>(tokenSetID) << 32) | 9;
constexpr std::int64_t CHAR8T = (static_cast<std::int64_t>(tokenSetID) << 32) | 10;
constexpr std::int64_t CHAR16T = (static_cast<std::int64_t>(tokenSetID) << 32) | 11;
constexpr std::int64_t CHAR32T = (static_cast<std::int64_t>(tokenSetID) << 32) | 12;
constexpr std::int64_t WCHART = (static_cast<std::int64_t>(tokenSetID) << 32) | 13;
constexpr std::int64_t BOOL = (static_cast<std::int64_t>(tokenSetID) << 32) | 14;
constexpr std::int64_t SHORT = (static_cast<std::int64_t>(tokenSetID) << 32) | 15;
constexpr std::int64_t INT = (static_cast<std::int64_t>(tokenSetID) << 32) | 16;
constexpr std::int64_t LONG = (static_cast<std::int64_t>(tokenSetID) << 32) | 17;
constexpr std::int64_t SIGNED = (static_cast<std::int64_t>(tokenSetID) << 32) | 18;
constexpr std::int64_t UNSIGNED = (static_cast<std::int64_t>(tokenSetID) << 32) | 19;
constexpr std::int64_t FLOAT = (static_cast<std::int64_t>(tokenSetID) << 32) | 20;
constexpr std::int64_t DOUBLE = (static_cast<std::int64_t>(tokenSetID) << 32) | 21;
constexpr std::int64_t VOID = (static_cast<std::int64_t>(tokenSetID) << 32) | 22;
constexpr std::int64_t TYPEDEF = (static_cast<std::int64_t>(tokenSetID) << 32) | 23;
constexpr std::int64_t CONST = (static_cast<std::int64_t>(tokenSetID) << 32) | 24;
constexpr std::int64_t VOLATILE = (static_cast<std::int64_t>(tokenSetID) << 32) | 25;
constexpr std::int64_t USING = (static_cast<std::int64_t>(tokenSetID) << 32) | 26;
constexpr std::int64_t NAMESPACE = (static_cast<std::int64_t>(tokenSetID) << 32) | 27;
constexpr std::int64_t THROW = (static_cast<std::int64_t>(tokenSetID) << 32) | 28;
constexpr std::int64_t SIZEOF = (static_cast<std::int64_t>(tokenSetID) << 32) | 29;
constexpr std::int64_t OPERATOR = (static_cast<std::int64_t>(tokenSetID) << 32) | 30;
constexpr std::int64_t DYNAMIC_CAST = (static_cast<std::int64_t>(tokenSetID) << 32) | 31;
constexpr std::int64_t STATIC_CAST = (static_cast<std::int64_t>(tokenSetID) << 32) | 32;
constexpr std::int64_t REINTERPRET_CAST = (static_cast<std::int64_t>(tokenSetID) << 32) | 33;
constexpr std::int64_t CONST_CAST = (static_cast<std::int64_t>(tokenSetID) << 32) | 34;
constexpr std::int64_t TYPEID = (static_cast<std::int64_t>(tokenSetID) << 32) | 35;
constexpr std::int64_t THIS = (static_cast<std::int64_t>(tokenSetID) << 32) | 36;
constexpr std::int64_t NEW = (static_cast<std::int64_t>(tokenSetID) << 32) | 37;
constexpr std::int64_t DELETE = (static_cast<std::int64_t>(tokenSetID) << 32) | 38;
constexpr std::int64_t TRUE = (static_cast<std::int64_t>(tokenSetID) << 32) | 39;
constexpr std::int64_t FALSE = (static_cast<std::int64_t>(tokenSetID) << 32) | 40;
constexpr std::int64_t NULLPTR = (static_cast<std::int64_t>(tokenSetID) << 32) | 41;
constexpr std::int64_t CASE = (static_cast<std::int64_t>(tokenSetID) << 32) | 42;
constexpr std::int64_t DEFAULT = (static_cast<std::int64_t>(tokenSetID) << 32) | 43;
constexpr std::int64_t IF = (static_cast<std::int64_t>(tokenSetID) << 32) | 44;
constexpr std::int64_t ELSE = (static_cast<std::int64_t>(tokenSetID) << 32) | 45;
constexpr std::int64_t SWITCH = (static_cast<std::int64_t>(tokenSetID) << 32) | 46;
constexpr std::int64_t WHILE = (static_cast<std::int64_t>(tokenSetID) << 32) | 47;
constexpr std::int64_t DO = (static_cast<std::int64_t>(tokenSetID) << 32) | 48;
constexpr std::int64_t FOR = (static_cast<std::int64_t>(tokenSetID) << 32) | 49;
constexpr std::int64_t BREAK = (static_cast<std::int64_t>(tokenSetID) << 32) | 50;
constexpr std::int64_t CONTINUE = (static_cast<std::int64_t>(tokenSetID) << 32) | 51;
constexpr std::int64_t RETURN = (static_cast<std::int64_t>(tokenSetID) << 32) | 52;
constexpr std::int64_t GOTO = (static_cast<std::int64_t>(tokenSetID) << 32) | 53;
constexpr std::int64_t TRY = (static_cast<std::int64_t>(tokenSetID) << 32) | 54;
constexpr std::int64_t CATCH = (static_cast<std::int64_t>(tokenSetID) << 32) | 55;
constexpr std::int64_t ID = (static_cast<std::int64_t>(tokenSetID) << 32) | 56;
constexpr std::int64_t INTEGER_LITERAL = (static_cast<std::int64_t>(tokenSetID) << 32) | 57;
constexpr std::int64_t FLOATING_LITERAL = (static_cast<std::int64_t>(tokenSetID) << 32) | 58;
constexpr std::int64_t CHAR_LITERAL = (static_cast<std::int64_t>(tokenSetID) << 32) | 59;
constexpr std::int64_t STRING_LITERAL = (static_cast<std::int64_t>(tokenSetID) << 32) | 60;

} // soul::cpp::token
