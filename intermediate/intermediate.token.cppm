
// this file has been automatically generated from 'D:/work/otava/intermediate/intermediate.token' using soul lexer generator version 5.2.0

export module otava.intermediate.token;

import std;

export namespace otava::intermediate::token {

constexpr std::int32_t tokenSetID = 1238242560;

constexpr std::int64_t ID = (static_cast<std::int64_t>(tokenSetID) << 32) | 1;
constexpr std::int64_t TYPES = (static_cast<std::int64_t>(tokenSetID) << 32) | 2;
constexpr std::int64_t DATA = (static_cast<std::int64_t>(tokenSetID) << 32) | 3;
constexpr std::int64_t FUNCTION = (static_cast<std::int64_t>(tokenSetID) << 32) | 4;
constexpr std::int64_t EXTERN = (static_cast<std::int64_t>(tokenSetID) << 32) | 5;
constexpr std::int64_t INLINE = (static_cast<std::int64_t>(tokenSetID) << 32) | 6;
constexpr std::int64_t LINK_ONCE = (static_cast<std::int64_t>(tokenSetID) << 32) | 7;
constexpr std::int64_t CHILD = (static_cast<std::int64_t>(tokenSetID) << 32) | 8;
constexpr std::int64_t METADATA = (static_cast<std::int64_t>(tokenSetID) << 32) | 9;
constexpr std::int64_t STORE = (static_cast<std::int64_t>(tokenSetID) << 32) | 10;
constexpr std::int64_t ARG = (static_cast<std::int64_t>(tokenSetID) << 32) | 11;
constexpr std::int64_t JMP = (static_cast<std::int64_t>(tokenSetID) << 32) | 12;
constexpr std::int64_t BRANCH = (static_cast<std::int64_t>(tokenSetID) << 32) | 13;
constexpr std::int64_t CALL = (static_cast<std::int64_t>(tokenSetID) << 32) | 14;
constexpr std::int64_t RET = (static_cast<std::int64_t>(tokenSetID) << 32) | 15;
constexpr std::int64_t SWITCH = (static_cast<std::int64_t>(tokenSetID) << 32) | 16;
constexpr std::int64_t NOP = (static_cast<std::int64_t>(tokenSetID) << 32) | 17;
constexpr std::int64_t NOT = (static_cast<std::int64_t>(tokenSetID) << 32) | 18;
constexpr std::int64_t NEG = (static_cast<std::int64_t>(tokenSetID) << 32) | 19;
constexpr std::int64_t SIGNEXTEND = (static_cast<std::int64_t>(tokenSetID) << 32) | 20;
constexpr std::int64_t ZEROEXTEND = (static_cast<std::int64_t>(tokenSetID) << 32) | 21;
constexpr std::int64_t FPEXTEND = (static_cast<std::int64_t>(tokenSetID) << 32) | 22;
constexpr std::int64_t TRUNCATE = (static_cast<std::int64_t>(tokenSetID) << 32) | 23;
constexpr std::int64_t BITCAST = (static_cast<std::int64_t>(tokenSetID) << 32) | 24;
constexpr std::int64_t INTTOFLOAT = (static_cast<std::int64_t>(tokenSetID) << 32) | 25;
constexpr std::int64_t FLOATTOINT = (static_cast<std::int64_t>(tokenSetID) << 32) | 26;
constexpr std::int64_t INTTOPTR = (static_cast<std::int64_t>(tokenSetID) << 32) | 27;
constexpr std::int64_t PTRTOINT = (static_cast<std::int64_t>(tokenSetID) << 32) | 28;
constexpr std::int64_t ADD = (static_cast<std::int64_t>(tokenSetID) << 32) | 29;
constexpr std::int64_t SUB = (static_cast<std::int64_t>(tokenSetID) << 32) | 30;
constexpr std::int64_t MUL = (static_cast<std::int64_t>(tokenSetID) << 32) | 31;
constexpr std::int64_t DIV = (static_cast<std::int64_t>(tokenSetID) << 32) | 32;
constexpr std::int64_t MOD = (static_cast<std::int64_t>(tokenSetID) << 32) | 33;
constexpr std::int64_t AND = (static_cast<std::int64_t>(tokenSetID) << 32) | 34;
constexpr std::int64_t OR = (static_cast<std::int64_t>(tokenSetID) << 32) | 35;
constexpr std::int64_t XOR = (static_cast<std::int64_t>(tokenSetID) << 32) | 36;
constexpr std::int64_t SHL = (static_cast<std::int64_t>(tokenSetID) << 32) | 37;
constexpr std::int64_t SHR = (static_cast<std::int64_t>(tokenSetID) << 32) | 38;
constexpr std::int64_t EQUAL = (static_cast<std::int64_t>(tokenSetID) << 32) | 39;
constexpr std::int64_t LESS = (static_cast<std::int64_t>(tokenSetID) << 32) | 40;
constexpr std::int64_t PARAM = (static_cast<std::int64_t>(tokenSetID) << 32) | 41;
constexpr std::int64_t LOCAL = (static_cast<std::int64_t>(tokenSetID) << 32) | 42;
constexpr std::int64_t PLOCAL = (static_cast<std::int64_t>(tokenSetID) << 32) | 43;
constexpr std::int64_t LOAD = (static_cast<std::int64_t>(tokenSetID) << 32) | 44;
constexpr std::int64_t ELEMADDR = (static_cast<std::int64_t>(tokenSetID) << 32) | 45;
constexpr std::int64_t PTROFFSET = (static_cast<std::int64_t>(tokenSetID) << 32) | 46;
constexpr std::int64_t PTRDIFF = (static_cast<std::int64_t>(tokenSetID) << 32) | 47;
constexpr std::int64_t GETRBP = (static_cast<std::int64_t>(tokenSetID) << 32) | 48;
constexpr std::int64_t TYPE = (static_cast<std::int64_t>(tokenSetID) << 32) | 49;
constexpr std::int64_t OFFSET = (static_cast<std::int64_t>(tokenSetID) << 32) | 50;
constexpr std::int64_t SIZE = (static_cast<std::int64_t>(tokenSetID) << 32) | 51;
constexpr std::int64_t ALIGNMENT = (static_cast<std::int64_t>(tokenSetID) << 32) | 52;
constexpr std::int64_t VOID = (static_cast<std::int64_t>(tokenSetID) << 32) | 53;
constexpr std::int64_t BOOL = (static_cast<std::int64_t>(tokenSetID) << 32) | 54;
constexpr std::int64_t SBYTE = (static_cast<std::int64_t>(tokenSetID) << 32) | 55;
constexpr std::int64_t BYTE = (static_cast<std::int64_t>(tokenSetID) << 32) | 56;
constexpr std::int64_t SHORT = (static_cast<std::int64_t>(tokenSetID) << 32) | 57;
constexpr std::int64_t USHORT = (static_cast<std::int64_t>(tokenSetID) << 32) | 58;
constexpr std::int64_t INT = (static_cast<std::int64_t>(tokenSetID) << 32) | 59;
constexpr std::int64_t UINT = (static_cast<std::int64_t>(tokenSetID) << 32) | 60;
constexpr std::int64_t LONG = (static_cast<std::int64_t>(tokenSetID) << 32) | 61;
constexpr std::int64_t ULONG = (static_cast<std::int64_t>(tokenSetID) << 32) | 62;
constexpr std::int64_t FLOAT = (static_cast<std::int64_t>(tokenSetID) << 32) | 63;
constexpr std::int64_t DOUBLE = (static_cast<std::int64_t>(tokenSetID) << 32) | 64;
constexpr std::int64_t TRUE = (static_cast<std::int64_t>(tokenSetID) << 32) | 65;
constexpr std::int64_t FALSE = (static_cast<std::int64_t>(tokenSetID) << 32) | 66;
constexpr std::int64_t NULL_TOKEN = (static_cast<std::int64_t>(tokenSetID) << 32) | 67;
constexpr std::int64_t CONV = (static_cast<std::int64_t>(tokenSetID) << 32) | 68;
constexpr std::int64_t LPAREN = (static_cast<std::int64_t>(tokenSetID) << 32) | 69;
constexpr std::int64_t RPAREN = (static_cast<std::int64_t>(tokenSetID) << 32) | 70;
constexpr std::int64_t COMMA = (static_cast<std::int64_t>(tokenSetID) << 32) | 71;
constexpr std::int64_t LBRACE = (static_cast<std::int64_t>(tokenSetID) << 32) | 72;
constexpr std::int64_t RBRACE = (static_cast<std::int64_t>(tokenSetID) << 32) | 73;
constexpr std::int64_t AST = (static_cast<std::int64_t>(tokenSetID) << 32) | 74;
constexpr std::int64_t COLON = (static_cast<std::int64_t>(tokenSetID) << 32) | 75;
constexpr std::int64_t SEMICOLON = (static_cast<std::int64_t>(tokenSetID) << 32) | 76;
constexpr std::int64_t LBRACKET = (static_cast<std::int64_t>(tokenSetID) << 32) | 77;
constexpr std::int64_t RBRACKET = (static_cast<std::int64_t>(tokenSetID) << 32) | 78;
constexpr std::int64_t ASSIGN = (static_cast<std::int64_t>(tokenSetID) << 32) | 79;
constexpr std::int64_t AT = (static_cast<std::int64_t>(tokenSetID) << 32) | 80;
constexpr std::int64_t DOLLAR = (static_cast<std::int64_t>(tokenSetID) << 32) | 81;
constexpr std::int64_t EXCLAMATION = (static_cast<std::int64_t>(tokenSetID) << 32) | 82;
constexpr std::int64_t TYPEID = (static_cast<std::int64_t>(tokenSetID) << 32) | 83;
constexpr std::int64_t CLSID = (static_cast<std::int64_t>(tokenSetID) << 32) | 84;
constexpr std::int64_t STRING = (static_cast<std::int64_t>(tokenSetID) << 32) | 85;
constexpr std::int64_t NUMBER = (static_cast<std::int64_t>(tokenSetID) << 32) | 86;
constexpr std::int64_t HEXNUM = (static_cast<std::int64_t>(tokenSetID) << 32) | 87;

} // otava::intermediate::token
