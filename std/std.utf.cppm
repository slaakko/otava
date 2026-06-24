export module std.utf;

import std.type.fundamental;
import std.basic_string;

export namespace std {

inline const string& to_utf8(const string& s) { return s; }
string to_utf8(const u16string& s);
string to_utf8(const u32string& s);
inline const u16string& to_utf16(const u16string& s) { return s; }
u16string to_utf16(const string& s);
u16string to_utf16(const u32string& s);
inline const u32string& to_utf32(const u32string& s) { return s; }
u32string to_utf32(const string& s);
u32string to_utf32(const u16string& s);

} // namespace std
