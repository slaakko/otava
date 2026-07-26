export module soul.slg.classmap;

import std;

export namespace soul::slg {

void MakeCompressedClassMap(const std::string& rootDir, const std::string& classMapName, bool verbose);
void MakeResourceFile(const std::string& rootDir, const std::string& classMapName, bool verbose);

} // namespce soul::slg
