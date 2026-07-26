export module soul.spg.code_generator;

import std;
import soul.ast.spg;
import soul.lexer.file_map;

export namespace soul::spg {

void GenerateCode(soul::ast::spg::SpgFile* spgFile, bool verbose, bool noDebugSupport, const std::string& version, soul::lexer::FileMap& fileMap);

} // namespace soul::spg
