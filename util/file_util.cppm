// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.file.util;

import std;

export namespace util {

void CopyFile(const std::string& source, const std::string& dest, bool force, bool verbose);

} // namespace util
