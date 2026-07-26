export module soul.spg.array_generator;

import std;
import soul.ast.spg;
import util.code_formatter;

export namespace soul::spg {

void GenerateArrays(soul::ast::spg::ParserFile& parserFile, util::CodeFormatter& formatter, int& sn);

} // namespace soul::spg
