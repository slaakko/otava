// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module lexer_resource;

import std;

export extern "C" std::int32_t* read_lexer_resource(const char* resource_name, std::int32_t& size);
export extern "C" void free_lexer_resource(std::int32_t* lexer_resource);

