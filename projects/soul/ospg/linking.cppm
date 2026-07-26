export module soul.spg.linking;

import std;
import soul.ast.spg;
import soul.lexer.file_map;

export namespace soul::spg {

void Link(soul::ast::spg::SpgFile* spgFile, bool verbose, soul::lexer::FileMap& fileMap);

} // namespace soul::spg
