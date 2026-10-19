export module soul.spg.xml_printer;

import std;
import soul.ast.spg;

export namespace soul::spg {

void PrintXml(soul::ast::spg::SpgFile* spgFile, bool verbose, bool opt);

} // namespace soul::spg
