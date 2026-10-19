export module soul.spg.optimizer;

import std;
import soul.ast.spg;
import soul.lexer.file_map;

export namespace soul::spg {

std::unique_ptr<soul::ast::spg::SpgFile> Optimize(soul::ast::spg::SpgFile* spgFile, bool verbose, bool xml, soul::lexer::FileMap& fileMap);

} // namespace soul::spg

