export module soul.slg.lexer_generator;

import std;
import soul.ast.slg;
import soul.lexer;

export namespace soul::slg {

void GenerateLexer(soul::ast::slg::SlgFile* slgFile, bool verbose, soul::lexer::FileMap& fileMap, bool debug);

} // namespace soul::slg
