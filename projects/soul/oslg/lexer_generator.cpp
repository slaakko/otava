module soul.slg.lexer_generator;

import util.path;
import util.code_formatter;
import util.file_stream;
import util.buffered_stream;
import util.binary_stream_writer;
import util.unicode;
import util.binary_resource_ptr;
import soul.slg.classmap;
import soul.slg.file_parsers;
import soul.slg.token.generator;
import soul.slg.regular.expression.parser;
import soul.slg.code_modifier;
import soul.lexer;
import soul.ast.re;
import soul.ast.cpp;
import soul.ast.common;
import soul.ast.source_pos;

namespace soul::slg {

void MakeNfas(soul::ast::re::LexerContext& lexerContext)
{
    for (auto& rule : lexerContext.GetLexer()->Rules())
    {
        soul::ast::re::Nfa* nfa = lexerContext.GetExprParser()->Parse(rule->Expr(), &lexerContext, lexerContext.FileName(), rule->Line());
        nfa->End()->SetRuleIndex(rule->Index());
        int nfaIndex = lexerContext.AddNfa(nfa);
        rule->SetNfaIndex(nfaIndex);
    }
    if (lexerContext.Debug())
    {
        std::string nfaFilePath = util::Path::Combine(lexerContext.Base(), "NFA.debug.txt");
        std::ofstream nfaFile(nfaFilePath);
        if (!nfaFile)
        {
            throw std::runtime_error("could not create file '" + nfaFilePath + "'");
        }
        util::CodeFormatter formatter(nfaFile);
        for (soul::ast::re::NfaState* state : lexerContext.NfaStates())
        {
            state->Print(formatter);
        }
        if (lexerContext.Verbose())
        {
            std::cout << "==> " << nfaFilePath << "\n";
        }
    }
}

void MakeCanonicalClasses(soul::ast::re::LexerContext& lexerContext)
{
    std::vector<soul::ast::re::Symbol*> s = lexerContext.Symbols();
    for (soul::ast::re::Symbol* symbol : s)
    {
        if (symbol->Contained()) continue;
        if (symbol->Kind() == soul::ast::re::SymbolKind::classSymbol)
        {
            soul::ast::re::Class* cls = static_cast<soul::ast::re::Class*>(symbol);
            soul::ast::re::Class* canonicalClass = cls->MakeCanonical(lexerContext);
            lexerContext.AddCanonicalClass(canonicalClass);
        }
        else if (symbol->Kind() == soul::ast::re::SymbolKind::charSymbol)
        {
            soul::ast::re::CharSymbol* chr = static_cast<soul::ast::re::CharSymbol*>(symbol);
            soul::ast::re::Class* canonicalClass = new soul::ast::re::Class(-1);
            canonicalClass->Ranges().push_back(soul::ast::re::Range(chr->Chr(), chr->Chr()));
            lexerContext.AddCanonicalClass(canonicalClass);
        }
        else if (symbol->Kind() == soul::ast::re::SymbolKind::anySymbol)
        {
            soul::ast::re::Symbol* range = lexerContext.MakeRange(static_cast<char32_t>(1), static_cast<char32_t>(0x10FFFF));
            soul::ast::re::Class* canonicalClass = new soul::ast::re::Class(-1);
            canonicalClass->Ranges().push_back(soul::ast::re::Range(static_cast<char32_t>(1), static_cast<char32_t>(0x10FFFF)));
            lexerContext.AddCanonicalClass(canonicalClass);
        }
        else if (symbol->Kind() == soul::ast::re::SymbolKind::rangeSymbol)
        {
            soul::ast::re::Range* range = static_cast<soul::ast::re::Range*>(symbol);
            soul::ast::re::Class* canonicalClass = new soul::ast::re::Class(-1);
            canonicalClass->Ranges().push_back(*range);
            lexerContext.AddCanonicalClass(canonicalClass);
        }
    }
    int index = 0;
    for (soul::ast::re::Class* canonicalClass : lexerContext.CanonicalClasses())
    {
        canonicalClass->SetIndex(index++);
        for (const soul::ast::re::Range& range : canonicalClass->Ranges())
        {
            if (canonicalClass->Chars().empty())
            {
                canonicalClass->AddChar(range.Start());
            }
            break;
        }
    }
    if (lexerContext.Debug())
    {
        std::string classFilePath = util::Path::Combine(lexerContext.Base(), "CLASS.debug.txt");
        std::ofstream classFile(classFilePath);
        if (!classFile)
        {
            throw std::runtime_error("could not create file '" + classFilePath + "'");
        }
        util::CodeFormatter formatter(classFile);
        for (soul::ast::re::Class* canonicalClass : lexerContext.CanonicalClasses())
        {
            canonicalClass->Print(formatter);
        }
        if (lexerContext.Verbose())
        {
            std::cout << "==> " << classFilePath << "\n";
        }
    }
}

void MakeClassPartition(soul::ast::re::LexerContext& lexerContext)
{
    std::list<soul::ast::re::Class*> classes;
    for (soul::ast::re::Class* cls : lexerContext.CanonicalClasses())
    {
        classes.push_back(cls);
    }
    auto classIt = classes.begin();
    while (classIt != classes.end())
    {
        bool classChanged = false;
        soul::ast::re::Class* cls = *classIt;
        auto sourceIt = classes.begin();
        while (!cls->Ranges().empty() && sourceIt != classes.end())
        {
            if (sourceIt == classIt)
            {
                ++sourceIt;
                continue;
            }
            bool sourceChanged = false;
            soul::ast::re::Class* source = *sourceIt;
            std::unique_ptr<soul::ast::re::Class> splitClass(new soul::ast::re::Class(-1));
            auto leftIt = cls->Ranges().begin();
            while (leftIt != cls->Ranges().end())
            {
                bool leftChanged = false;
                auto rightIt = source->Ranges().begin();
                while (leftIt != cls->Ranges().end() && rightIt != source->Ranges().end())
                {
                    bool rightChanged = false;
                    if (soul::ast::re::Intersect(*leftIt, *rightIt))
                    {
                        std::vector<soul::ast::re::Range> leftRanges = *leftIt - *rightIt;
                        std::vector<soul::ast::re::Range> rightRanges = *rightIt - *leftIt;
                        soul::ast::re::Range intersection = *leftIt & *rightIt;
                        for (const soul::ast::re::Range& leftRange : leftRanges)
                        {
                            if (leftRange.IsEmpty()) continue;
                            auto it = leftIt;
                            ++it;
                            cls->Ranges().insert(it, leftRange);
                        }
                        leftIt = cls->Ranges().erase(leftIt);
                        leftChanged = true;
                        if (!intersection.IsEmpty())
                        {
                            splitClass->Ranges().push_back(intersection);
                        }
                        for (const soul::ast::re::Range& rightRange : rightRanges)
                        {
                            if (rightRange.IsEmpty()) continue;
                            auto it = rightIt;
                            ++it;
                            source->Ranges().insert(it, rightRange);
                        }
                        rightIt = source->Ranges().erase(rightIt);
                        rightChanged = true;
                    }
                    if (!rightChanged)
                    {
                        ++rightIt;
                    }
                }
                if (!leftChanged)
                {
                    ++leftIt;
                }
            }
            if (source->Ranges().empty())
            {
                sourceIt = classes.erase(sourceIt);
                sourceChanged = true;
            }
            if (!splitClass->Ranges().empty())
            {
                classes.insert(sourceIt, splitClass.get());
                lexerContext.AddToSymbols(splitClass.release());
                sourceChanged = true;
            }
            if (!sourceChanged)
            {
                ++sourceIt;
            }
        }
        if (cls->Ranges().empty())
        {
            classIt = classes.erase(classIt);
            classChanged = true;
        }
        if (!classChanged)
        {
            ++classIt;
        }
    }
    for (soul::ast::re::Class* cls : classes)
    {
        cls->MakeMinimal(lexerContext);
        lexerContext.AddToPartition(cls);
    }
    for (int i = 0; i < lexerContext.Partition().size(); ++i)
    {
        soul::ast::re::Class* cls = lexerContext.Partition()[i];
        cls->SetIndex(i);
    }
    if (lexerContext.Debug())
    {
        std::string partitionFilePath = util::Path::Combine(lexerContext.Base(), "PARTITION.debug.txt");
        std::ofstream partitionFile(partitionFilePath);
        if (!partitionFile)
        {
            throw std::runtime_error("could not create file '" + partitionFilePath + "'");
        }
        util::CodeFormatter formatter(partitionFile);
        for (int i = 0; i < lexerContext.Partition().size(); ++i)
        {
            soul::ast::re::Class* cls = lexerContext.Partition()[i];
            cls->Print(formatter);
        }
        if (lexerContext.Verbose())
        {
            std::cout << "==> " << partitionFilePath << "\n";
        }
    }
}

void CreateClassMap(soul::ast::re::LexerContext& lexerContext, const std::string& rootDir, bool verbose)
{
    std::vector<std::int32_t> classMapVec(0x110000, -1);
    for (soul::ast::re::Class* cls : lexerContext.Partition())
    {
        for (const soul::ast::re::Range& range : cls->Ranges())
        {
            for (char32_t i = range.Start(); i <= range.End(); ++i)
            {
                if (classMapVec[i] == -1)
                {
                    classMapVec[static_cast<std::int32_t>(i)] = cls->Index();
                }
            }
        }
    }
    std::string moduleName;
    soul::ast::slg::Lexer* lexer = lexerContext.GetLexer();
    soul::ast::common::File* file = lexer->GetFile();
    if (file->Kind() == soul::ast::common::FileKind::lexerFile)
    {
        soul::ast::slg::LexerFile* lexerFile = static_cast<soul::ast::slg::LexerFile*>(file);
        soul::ast::common::ExportModule* mod = lexerFile->GetExportModule();
        moduleName = mod->ModuleName();
    }
    else
    {
        throw std::runtime_error("lexer file expected");
    }
    std::string classMapName = moduleName + ".classmap";
    std::string classMapFileName = util::Path::Combine(rootDir, classMapName);
    util::FileStream fileStream(classMapFileName, util::OpenMode::binary | util::OpenMode::write);
    util::BufferedStream bufferedStream(fileStream);
    util::BinaryStreamWriter writer(bufferedStream);
    std::int32_t size = static_cast<std::int32_t>(classMapVec.size());
    writer.Write(size);
    // size = 0x110000
    for (std::int32_t x : classMapVec)
    {
        writer.Write(x);
    }
    if (verbose)
    {
        std::cout << "==> " << classMapFileName << "\n";
    }
    if (lexerContext.Debug())
    {
        std::string classMapFilePath = util::Path::Combine(lexerContext.Base(), "CLASSMAP.debug.txt");
        std::ofstream classMapFile(classMapFilePath);
        if (!classMapFile)
        {
            throw std::runtime_error("could not create file '" + classMapFilePath + "'");
        }
        util::CodeFormatter formatter(classMapFile);
        formatter.WriteLine("CLASSMAP:");
        int32_t i = 0;
        for (std::int32_t x : classMapVec)
        {
            formatter.WriteLine(std::to_string(i) + " : " + std::to_string(x));
            ++i;
        }
        if (lexerContext.Verbose())
        {
            std::cout << "==> " << classMapFilePath << "\n";
        }
    }
}

void CompressClassMap(soul::ast::re::LexerContext& lexerContext, const std::string& rootDir, bool verbose)
{
    std::string moduleName;
    soul::ast::slg::Lexer* lexer = lexerContext.GetLexer();
    soul::ast::common::File* file = lexer->GetFile();
    if (file->Kind() == soul::ast::common::FileKind::lexerFile)
    {
        soul::ast::slg::LexerFile* lexerFile = static_cast<soul::ast::slg::LexerFile*>(file);
        soul::ast::common::ExportModule* mod = lexerFile->GetExportModule();
        moduleName = mod->ModuleName();
    }
    else
    {
        throw std::runtime_error("lexer file expected");
    }
    std::string classMapName = moduleName + ".classmap";
    MakeCompressedClassMap(rootDir, classMapName, verbose);
    MakeResourceFile(rootDir, classMapName, verbose);
}

void MakeMasterNfa(soul::ast::re::LexerContext& lexerContext)
{
    soul::ast::re::Nfa* masterNfa = new soul::ast::re::Nfa();
    int masterNfaIndex = lexerContext.AddNfa(masterNfa);
    lexerContext.SetMasterNfaIndex(masterNfaIndex);
    soul::ast::re::NfaState* start = lexerContext.MakeNfaState();
    masterNfa->SetStart(start);
    soul::ast::slg::Lexer* lexer = lexerContext.GetLexer();
    for (const auto& rule : lexer->Rules())
    {
        soul::ast::re::Nfa* ruleNfa = lexerContext.GetNfa(rule->NfaIndex());
        start->AddEdge(soul::ast::re::NfaEdge(lexerContext.MakeEpsilon(), ruleNfa->Start()));
    }
}
 
void MakeDfa(soul::ast::re::LexerContext& lexerContext)
{
    soul::ast::re::Nfa* masterNfa = lexerContext.GetNfa(lexerContext.MasterNfaIndex());
    soul::ast::re::Dfa dfa = soul::ast::re::Compile(lexerContext, *masterNfa);
    lexerContext.SetDfa(std::move(dfa));
    if (lexerContext.Debug())
    {
        std::string dfaFilePath = util::Path::Combine(lexerContext.Base(), "DFA.debug.txt");
        std::ofstream dfaFile(dfaFilePath);
        if (!dfaFile)
        {
            throw std::runtime_error("could not create file '" + dfaFilePath + "'");
        }
        util::CodeFormatter formatter(dfaFile);
        lexerContext.GetDfa().Print(lexerContext, formatter);
        if (lexerContext.Verbose())
        {
            std::cout << "==> " << dfaFilePath << "\n";
        }
    }
}
std::string GetTokenCppId(soul::ast::slg::LexerFile* lexerFile, soul::lexer::FileMap& fileMap, const soul::ast::SourcePos& sourcePos, const std::string& tokenName)
{
    soul::ast::common::TokenMap* tokenMap = lexerFile->GetTokenMap();
    if (tokenMap)
    {
        std::vector<soul::ast::common::Token*> tokens = tokenMap->GetTokens(tokenName);
        int n = static_cast<int>(tokens.size());
        if (n == 1)
        {
            soul::ast::common::Token* token = tokens.front();
            return token->FullCppId();
        }
        else if (n > 1)
        {
            std::string tokenString;
            for (int i = 0; i < n; ++i)
            {
                soul::ast::common::Token* token = tokens[i];
                if (i > 0)
                {
                    tokenString.append(" or\n");
                }
                tokenString.append(token->FullName());
            }
            std::string errorMessage = soul::lexer::MakeMessage("error", "ambiguous reference to token '" + tokenName + "' in lexer file '" +
                lexerFile->FilePath() + "':\n" + tokenString, sourcePos, fileMap);
            throw std::runtime_error(errorMessage);
        }
        else
        {
            std::string errorMessage = soul::lexer::MakeMessage("error", "token '" + tokenName + "' not found in lexer file '" +
                lexerFile->FilePath(), sourcePos, fileMap);
            throw std::runtime_error(errorMessage);
        }
    }
    else
    {
        throw std::runtime_error("token map not set");
    }
}

void WriteVariables(soul::ast::re::LexerContext& lexerContext, util::CodeFormatter& interfaceFormatter, util::CodeFormatter sourceFormatter)
{
    soul::ast::slg::Lexer* lexer = lexerContext.GetLexer();
    std::string variableClassName = lexer->Name() + "_Variables";
    lexer->SetVariableClassName(variableClassName);
    interfaceFormatter.WriteLine("struct " + variableClassName + " : public soul::lexer::Variables");
    interfaceFormatter.WriteLine("{");
    interfaceFormatter.IncIndent();
    interfaceFormatter.WriteLine(variableClassName + "();");
    bool hasVariables = false;
    for (const auto& var : lexer->Variables())
    {
        var->Type()->Write(interfaceFormatter);
        interfaceFormatter.Write(" ");
        interfaceFormatter.WriteLine(var->Name() + ";");
        hasVariables = true;
    }
    interfaceFormatter.DecIndent();
    interfaceFormatter.WriteLine("};");
    sourceFormatter.WriteLine();
    sourceFormatter.Write(variableClassName + "::" + variableClassName + "()");
    if (hasVariables)
    {
        sourceFormatter.Write(" :");
    }
    sourceFormatter.WriteLine();
    sourceFormatter.IncIndent();
    bool first = true;
    for (const auto& var : lexer->Variables())
    {
        if (first)
        {
            first = false;
        }
        else
        {
            sourceFormatter.Write(", ");
        }
        sourceFormatter.Write(var->Name() + "()");
    }
    if (hasVariables)
    {
        sourceFormatter.WriteLine();
    }
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("{");
    sourceFormatter.WriteLine("}");
}

void WriteLexer(soul::ast::re::LexerContext& lexerContext, soul::ast::slg::SlgFile* slgFile, const std::string& rootDir, bool verbose, soul::lexer::FileMap& fileMap)
{
    std::string interfaceFilePath;
    interfaceFilePath = util::GetFullPath(util::Path::Combine(rootDir, lexerContext.FileName() + ".cppm"));
    std::string sourceFilePath = util::GetFullPath(util::Path::Combine(rootDir, lexerContext.FileName() + ".cpp"));
    std::ofstream interfaceStream(interfaceFilePath);
    util::CodeFormatter interfaceFormatter(interfaceStream);
    interfaceFormatter.WriteLine("// this file has been automatically generated from '" + lexerContext.FileName() + "' using soul lexer generator oslg version " +
        util::OtavaVersionStr());
    interfaceFormatter.WriteLine();
    std::ofstream sourceStream(sourceFilePath);
    util::CodeFormatter sourceFormatter(sourceStream);
    sourceFormatter.WriteLine("// this file has been automatically generated from '" + lexerContext.FileName() + "' using soul lexer generator oslg version " +
        util::OtavaVersionStr());
    sourceFormatter.WriteLine();
    soul::ast::slg::Lexer* lexer = lexerContext.GetLexer();
    soul::ast::common::File* file = lexer->GetFile();
    soul::ast::slg::LexerFile* lexerFile = nullptr;
    std::string moduleName;
    if (file->Kind() == soul::ast::common::FileKind::lexerFile)
    {
        lexerFile = static_cast<soul::ast::slg::LexerFile*>(file);
        soul::ast::common::ExportModule* mod = lexerFile->GetExportModule();
        moduleName = mod->ModuleName();
    }
    else
    {
        throw std::runtime_error("lexer file expected");
    }
    sourceFormatter.WriteLine("module " + moduleName + ";");
    sourceFormatter.WriteLine();
    std::string includeGuard;
    interfaceFormatter.WriteLine("export module " + moduleName + ";");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("import std;");
    interfaceFormatter.WriteLine("import soul.lexer.cls;");
    interfaceFormatter.WriteLine("import soul.lexer.base;");
    interfaceFormatter.WriteLine("import soul.lexer.lexeme;");
    interfaceFormatter.WriteLine("import soul.lexer.token;");
    interfaceFormatter.WriteLine("import soul.lexer.classmap;");
    interfaceFormatter.WriteLine("import soul.lexer.keyword;");
    interfaceFormatter.WriteLine("import soul.ast.slg;");
    interfaceFormatter.WriteLine("import soul.ast.common;");
    interfaceFormatter.WriteLine("import util.binary_resource_ptr;");
    for (const auto& imprt : lexerFile->Imports())
    {
        soul::ast::common::Collection* collection = slgFile->GetCollection(imprt->ModuleName());
        if (collection)
        {
            if (collection->Kind() == soul::ast::common::CollectionKind::tokenCollection)
            {
                interfaceFormatter.WriteLine("import " + collection->Name() + ";");
            }
        }
    }
    interfaceFormatter.WriteLine();
    sourceFormatter.WriteLine();
    interfaceFormatter.WriteLine("export namespace " + soul::ast::common::ToNamespaceName(moduleName) + " {");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("std::mutex& MakeLexerMtx();");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("template<typename Char>");
    interfaceFormatter.WriteLine("struct " + lexer->Name() + ";");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("template<typename Char>");
    interfaceFormatter.WriteLine("soul::lexer::Lexer<" + lexer->Name() + "<Char>, Char> MakeLexer(const Char* start, const Char* end, const std::string& fileName);");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("template<typename Char>");
    interfaceFormatter.WriteLine("soul::lexer::Lexer<" + lexer->Name() +
        "<Char>, Char> MakeLexer(const std::string& moduleFileName, util::ResourceFlags resourceFlags, const Char* start, const Char* end, const std::string& fileName);");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("soul::ast::common::TokenCollection* GetTokens();");
    interfaceFormatter.WriteLine();
    sourceFormatter.WriteLine("namespace " + soul::ast::common::ToNamespaceName(moduleName) + " {");
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("soul::ast::common::TokenCollection* GetTokens()");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("static soul::ast::common::TokenCollection tokens(\"" + moduleName + ".tokens\");");
    sourceFormatter.WriteLine("if (!tokens.Initialized())");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("tokens.SetInitialized();");
    soul::ast::slg::Tokens* tokens = lexerContext.GetTokens();
    for (const auto* token : tokens->GetTokens())
    {
        sourceFormatter.WriteLine("tokens.AddToken(new soul::ast::common::Token(" + token->FullCppId() + ", \"" + token->Name() + "\", \"" + token->Info() + "\"));");
    }
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");
    sourceFormatter.WriteLine("return &tokens;");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");
    WriteVariables(lexerContext, interfaceFormatter, sourceFormatter);
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("template<typename Char>");
    interfaceFormatter.WriteLine("struct " + lexer->Name());
    interfaceFormatter.WriteLine("{");
    interfaceFormatter.IncIndent();
    interfaceFormatter.WriteLine("using Variables = " + lexer->VariableClassName() + ";");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("static std::int32_t NextState(std::int32_t state, Char chr, soul::lexer::LexerBase<Char>& lexer)");
    interfaceFormatter.WriteLine("{");
    interfaceFormatter.IncIndent();
    interfaceFormatter.WriteLine("soul::lexer::ClassMap<Char>* classmap = lexer.GetClassMap();");
    interfaceFormatter.WriteLine("std::int32_t cls = classmap->GetClass(chr);");
    interfaceFormatter.WriteLine("switch (state)");
    interfaceFormatter.WriteLine("{");
    interfaceFormatter.IncIndent();
    const soul::ast::re::Dfa& dfa = lexerContext.GetDfa();
    for (const auto* state : dfa.States())
    {
        interfaceFormatter.WriteLine("case " + std::to_string(state->Id()) + ":");
        interfaceFormatter.WriteLine("{");
        interfaceFormatter.IncIndent();
        if (state->Accept())
        {
            interfaceFormatter.WriteLine("auto& token = lexer.CurrentToken();");
            interfaceFormatter.WriteLine("auto prevMatch = token.match;");
            interfaceFormatter.WriteLine("token.match = lexer.CurrentLexeme();");
            interfaceFormatter.WriteLine("std::int64_t tokenId = GetTokenId(" + std::to_string(state->RuleIndex()) + ", lexer);");
            interfaceFormatter.WriteLine("if (tokenId == soul::lexer::CONTINUE_TOKEN)");
            interfaceFormatter.WriteLine("{");
            interfaceFormatter.IncIndent();
            interfaceFormatter.WriteLine("token.id = soul::lexer::CONTINUE_TOKEN;");
            interfaceFormatter.WriteLine("return -1;");
            interfaceFormatter.DecIndent();
            interfaceFormatter.WriteLine("}");
            interfaceFormatter.WriteLine("else if (tokenId != soul::lexer::INVALID_TOKEN)");
            interfaceFormatter.WriteLine("{");
            interfaceFormatter.IncIndent();
            interfaceFormatter.WriteLine("token.id = tokenId;");
            interfaceFormatter.DecIndent();
            interfaceFormatter.WriteLine("}");
            interfaceFormatter.WriteLine("else");
            interfaceFormatter.WriteLine("{");
            interfaceFormatter.IncIndent();
            interfaceFormatter.WriteLine("token.match = prevMatch;");
            interfaceFormatter.DecIndent();
            interfaceFormatter.WriteLine("}");
        }
        std::map<std::int32_t, std::set<std::int32_t>> m;
        for (soul::ast::re::Class* cls : lexerContext.Partition())
        {
            soul::ast::re::DfaState* next = state->Next(cls->Index());
            if (next != nullptr)
            {
                m[next->Id()].insert(cls->Index());
            }
        }
        if (m.empty())
        {
            interfaceFormatter.WriteLine("return -1;");
        }
        else
        {
            interfaceFormatter.WriteLine("switch (cls)");
            interfaceFormatter.WriteLine("{");
            interfaceFormatter.IncIndent();
            for (const auto& p : m)
            {
                std::int32_t n = p.first;
                for (std::int32_t k : p.second)
                {
                    interfaceFormatter.WriteLine("case " + std::to_string(k) + ":");
                }
                interfaceFormatter.WriteLine("{");
                interfaceFormatter.IncIndent();
                interfaceFormatter.WriteLine("return " + std::to_string(n) + ";");
                interfaceFormatter.DecIndent();
                interfaceFormatter.WriteLine("}");
            }
            interfaceFormatter.WriteLine("default:");
            interfaceFormatter.WriteLine("{");
            interfaceFormatter.IncIndent();
            interfaceFormatter.WriteLine("return -1;");
            interfaceFormatter.DecIndent();
            interfaceFormatter.WriteLine("}");
            interfaceFormatter.DecIndent();
            interfaceFormatter.WriteLine("}");
        }
        interfaceFormatter.DecIndent();
        interfaceFormatter.WriteLine("}");
    }
    interfaceFormatter.DecIndent();
    interfaceFormatter.WriteLine("}");
    interfaceFormatter.WriteLine("return -1;");
    interfaceFormatter.DecIndent();
    interfaceFormatter.WriteLine("}");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("static std::int64_t GetTokenId(std::int32_t ruleIndex, soul::lexer::LexerBase<Char>& lexer)");
    interfaceFormatter.WriteLine("{");
    interfaceFormatter.IncIndent();
    interfaceFormatter.WriteLine("switch (ruleIndex)");
    interfaceFormatter.WriteLine("{");
    interfaceFormatter.IncIndent();
    for (const auto& rule : lexer->Rules())
    {
        interfaceFormatter.WriteLine("case " + std::to_string(rule->Index()) + ":");
        soul::ast::common::TokenMap* tokenMap = lexerFile->GetTokenMap();
        if (tokenMap)
        {
            ModifyCode(rule->Code(), *tokenMap, lexerFile, fileMap);
        }
        else
        {
            throw std::runtime_error("token map not set");
        }
        soul::ast::SourcePos sourcePos; // todo
        rule->Code()->InsertFront(
            new soul::ast::cpp::ExpressionStatementNode(sourcePos,
                new soul::ast::cpp::InvokeNode(sourcePos,
                    new soul::ast::cpp::IdExprNode(sourcePos, "lexer.Retract"))));
        if (rule->GetAction() != -1)
        {
            soul::ast::slg::Action* action = lexer->GetActions().GetAction(rule->GetAction());
            if (action != nullptr)
            {
                soul::ast::cpp::CodeEvaluationVisitor visitor;
                action->Code()->Accept(visitor);
                int n = action->Code()->Statements().size();
                for (int i = n - 1; i >= 0; --i)
                {
                    soul::ast::cpp::StatementNode* s = action->Code()->Statements()[i].get();
                    rule->Code()->InsertFront(static_cast<soul::ast::cpp::StatementNode*>(s->Clone()));
                }
                if (visitor.HasVars())
                {
                    soul::ast::cpp::DeclarationStatementNode* declarationStmt = new soul::ast::cpp::DeclarationStatementNode(sourcePos,
                        new soul::ast::cpp::InitDeclaratorNode(sourcePos, "auto vars",
                            new soul::ast::cpp::InitializerNode(sourcePos,
                                new soul::ast::cpp::AssignInitNode(sourcePos,
                                    new soul::ast::cpp::IdExprNode(sourcePos, "static_cast<Variables*>(lexer.GetVariables())")))));
                    rule->Code()->InsertFront(declarationStmt);
                }
            }
            else
            {
                throw std::runtime_error("action " + std::to_string(rule->GetAction()) + " not found");
            }
        }
        rule->Code()->Add(new soul::ast::cpp::BreakStatementNode(sourcePos));
        rule->Code()->Write(interfaceFormatter);
    }
    interfaceFormatter.DecIndent();
    interfaceFormatter.WriteLine("}");
    interfaceFormatter.WriteLine("return soul::lexer::CONTINUE_TOKEN;");
    interfaceFormatter.DecIndent();
    interfaceFormatter.WriteLine("}");
    interfaceFormatter.DecIndent();
    interfaceFormatter.WriteLine("};");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("template<typename Char>");
    interfaceFormatter.WriteLine("soul::lexer::ClassMap<Char>* GetClassMap()");
    interfaceFormatter.WriteLine("{");
    interfaceFormatter.IncIndent();
    interfaceFormatter.WriteLine("static std::unique_ptr<soul::lexer::ClassMap<Char>> classmap(soul::lexer::MakeClassMap<Char>(\"" + moduleName + ".classmap\"));");
    interfaceFormatter.WriteLine("return classmap.get();");
    interfaceFormatter.DecIndent();
    interfaceFormatter.WriteLine("}");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("template<typename Char>");
    interfaceFormatter.WriteLine("soul::lexer::ClassMap<Char>* GetClassMap(const std::string& moduleFileName, util::ResourceFlags resourceFlags)");
    interfaceFormatter.WriteLine("{");
    interfaceFormatter.IncIndent();
    interfaceFormatter.WriteLine("static std::unique_ptr<soul::lexer::ClassMap<Char>> classmap(soul::lexer::MakeClassMap<Char>(moduleFileName, \"" + moduleName +
        ".classmap\", resourceFlags));");
    interfaceFormatter.WriteLine("return classmap.get();");
    interfaceFormatter.DecIndent();
    interfaceFormatter.WriteLine("}");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("template<typename Char>");
    interfaceFormatter.WriteLine("soul::lexer::KeywordMap<Char>* GetKeywords();");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("template<>");
    interfaceFormatter.WriteLine("soul::lexer::KeywordMap<char>* GetKeywords<char>();");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("template<>");
    interfaceFormatter.WriteLine("soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>();");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("template<>");
    interfaceFormatter.WriteLine("soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>();");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("template<>");
    interfaceFormatter.WriteLine("soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>();");
    interfaceFormatter.WriteLine();

    interfaceFormatter.WriteLine("template<typename Char>");
    interfaceFormatter.WriteLine("soul::lexer::Lexer<" + lexer->Name() + "<Char>, Char> MakeLexer(const Char* start, const Char* end, const std::string& fileName)");
    interfaceFormatter.WriteLine("{");
    interfaceFormatter.IncIndent();
    interfaceFormatter.WriteLine("std::lock_guard<std::mutex> lock(MakeLexerMtx());");
    interfaceFormatter.WriteLine("auto lxr = soul::lexer::Lexer<" + lexer->Name() + "<Char>, Char>(start, end, fileName);");
    interfaceFormatter.WriteLine("lxr.SetClassMap(GetClassMap<Char>());");
    interfaceFormatter.WriteLine("lxr.SetTokenCollection(GetTokens());");
    interfaceFormatter.WriteLine("lxr.SetKeywordMap(GetKeywords<Char>());");
    interfaceFormatter.WriteLine("return lxr;");
    interfaceFormatter.DecIndent();
    interfaceFormatter.WriteLine("}");
    interfaceFormatter.WriteLine();

    interfaceFormatter.WriteLine("template<typename Char>");
    interfaceFormatter.WriteLine("soul::lexer::Lexer<" + lexer->Name() +
        "<Char>, Char> MakeLexer(const std::string& moduleFileName, util::ResourceFlags resourceFlags, const Char* start, const Char* end, const std::string& fileName)");
    interfaceFormatter.WriteLine("{");
    interfaceFormatter.IncIndent();
    interfaceFormatter.WriteLine("std::lock_guard<std::mutex> lock(MakeLexerMtx());");
    interfaceFormatter.WriteLine("auto lxr = soul::lexer::Lexer<" + lexer->Name() + "<Char>, Char>(start, end, fileName);");
    interfaceFormatter.WriteLine("lxr.SetClassMap(GetClassMap<Char>(moduleFileName, resourceFlags));");
    interfaceFormatter.WriteLine("lxr.SetTokenCollection(GetTokens());");
    interfaceFormatter.WriteLine("lxr.SetKeywordMap(GetKeywords<Char>());");
    interfaceFormatter.WriteLine("return lxr;");
    interfaceFormatter.DecIndent();
    interfaceFormatter.WriteLine("}");
    interfaceFormatter.WriteLine();

    interfaceFormatter.WriteLine("} // namespace " + soul::ast::common::ToNamespaceName(moduleName));
    if (verbose)
    {
        std::cout << "==> " << interfaceFilePath << "\n";
    }

    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("std::mutex mtx;");
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("std::mutex& MakeLexerMtx() { return mtx; }");
    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("template<>");
    sourceFormatter.WriteLine("soul::lexer::KeywordMap<char>* GetKeywords<char>()");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("static const soul::lexer::Keyword<char> keywords[] = {");
    sourceFormatter.IncIndent();
    bool first = true;
    auto keywords = lexerContext.GetKeywords();
    bool hasKw = false;
    for (const auto* keyword : keywords->GetKeywords())
    {
        if (first)
        {
            first = false;
        }
        else
        {
            sourceFormatter.WriteLine(",");
        }
        sourceFormatter.Write(" { \"" + keyword->Str() + "\", " + GetTokenCppId(lexerFile, fileMap, keyword->GetSourcePos(), keyword->TokenName()) + " }");
        hasKw = true;
    }
    if (hasKw)
    {
        sourceFormatter.WriteLine(",");
    }
    sourceFormatter.WriteLine(" { nullptr, -1 }");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("};");
    sourceFormatter.WriteLine("static soul::lexer::KeywordMap<char> keywordMap(keywords);");
    sourceFormatter.WriteLine("return &keywordMap;");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");

    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("template<>");
    sourceFormatter.WriteLine("soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>()");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("static const soul::lexer::Keyword<char8_t> keywords[] = {");
    sourceFormatter.IncIndent();
    bool first8 = true;
    bool hasKw8 = false;
    auto keywords8 = lexerContext.GetKeywords();
    for (const auto* keyword : keywords8->GetKeywords())
    {
        if (first8)
        {
            first8 = false;
        }
        else
        {
            sourceFormatter.WriteLine(",");
        }
        sourceFormatter.Write(" { u8\"" + keyword->Str() + "\", " + GetTokenCppId(lexerFile, fileMap, keyword->GetSourcePos(), keyword->TokenName()) + " }");
        hasKw8 = true;
    }
    if (hasKw8)
    {
        sourceFormatter.WriteLine(",");
    }
    sourceFormatter.WriteLine(" { nullptr, -1 }");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("};");
    sourceFormatter.WriteLine("static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);");
    sourceFormatter.WriteLine("return &keywordMap;");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");

    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("template<>");
    sourceFormatter.WriteLine("soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>()");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("static const soul::lexer::Keyword<char16_t> keywords[] = {");
    sourceFormatter.IncIndent();
    bool first16 = true;
    bool hasKw16 = false;
    auto keywords16 = lexerContext.GetKeywords();
    for (const auto* keyword : keywords16->GetKeywords())
    {
        if (first16)
        {
            first16 = false;
        }
        else
        {
            sourceFormatter.WriteLine(",");
        }
        sourceFormatter.Write(" { u\"" + keyword->Str() + "\", " + GetTokenCppId(lexerFile, fileMap, keyword->GetSourcePos(), keyword->TokenName()) + " }");
        hasKw16 = true;
    }
    if (hasKw16)
    {
        sourceFormatter.WriteLine(",");
    }
    sourceFormatter.WriteLine(" { nullptr, -1 }");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("};");
    sourceFormatter.WriteLine("static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);");
    sourceFormatter.WriteLine("return &keywordMap;");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");

    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("template<>");
    sourceFormatter.WriteLine("soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>()");
    sourceFormatter.WriteLine("{");
    sourceFormatter.IncIndent();
    sourceFormatter.WriteLine("static const soul::lexer::Keyword<char32_t> keywords[] = {");
    sourceFormatter.IncIndent();
    bool first32 = true;
    bool hasKw32 = false;
    auto keywords32 = lexerContext.GetKeywords();
    for (const auto* keyword : keywords32->GetKeywords())
    {
        if (first32)
        {
            first32 = false;
        }
        else
        {
            sourceFormatter.WriteLine(",");
        }
        sourceFormatter.Write(" { U\"" + keyword->Str() + "\", " + GetTokenCppId(lexerFile, fileMap, keyword->GetSourcePos(), keyword->TokenName()) + " }");
        hasKw32 = true;
    }
    if (hasKw32)
    {
        sourceFormatter.WriteLine(",");
    }
    sourceFormatter.WriteLine(" { nullptr, -1 }");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("};");
    sourceFormatter.WriteLine("static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);");
    sourceFormatter.WriteLine("return &keywordMap;");
    sourceFormatter.DecIndent();
    sourceFormatter.WriteLine("}");

    sourceFormatter.WriteLine();
    sourceFormatter.WriteLine("} // namespace " + soul::ast::common::ToNamespaceName(moduleName));

    if (verbose)
    {
        std::cout << "==> " << sourceFilePath << "\n";
    }
}

void GenerateLexer(soul::ast::slg::SlgFile* slgFile, soul::ast::slg::LexerFile* lexerFile, bool verbose, soul::lexer::FileMap& fileMap, bool debug)
{
    soul::ast::re::LexerContext lexerContext;
    if (verbose) lexerContext.SetVerbose();
    if (debug) lexerContext.SetDebug();
    lexerContext.SetFileName(lexerFile->FilePath());
    lexerContext.SetBase(util::Path::GetDirectoryName(lexerContext.FileName()));
    RegularExpressionParser regularExpressionParser;
    lexerContext.SetExprParser(&regularExpressionParser);
    soul::ast::slg::Tokens tokens;
    soul::ast::slg::Keywords keywords;
    soul::ast::slg::Expressions expressions;
    for (const auto& imprt : lexerFile->Imports())
    {
        soul::ast::common::Collection* collection = slgFile->GetCollection(imprt->ModuleName());
        if (collection)
        {
            switch (collection->Kind())
            {
                case soul::ast::common::CollectionKind::tokenCollection:
                {
                    soul::ast::common::TokenCollection* tokenCollection = static_cast<soul::ast::common::TokenCollection*>(collection);
                    for (const auto& token : tokenCollection->GetTokens())
                    {
                        tokens.AddToken(token.get());
                        lexerFile->GetTokenMap()->AddToken(token.get());
                    }
                    break;
                }
                case soul::ast::common::CollectionKind::keywordCollection:
                {
                    soul::ast::slg::KeywordCollection* keywordCollection = static_cast<soul::ast::slg::KeywordCollection*>(collection);
                    for (const auto& keyword : keywordCollection->GetKeywords())
                    {
                        keywords.AddKeyword(keyword.get());
                    }
                    break;
                }
                case soul::ast::common::CollectionKind::expressionCollection:
                {
                    soul::ast::slg::ExpressionCollection* expressionCollection = static_cast<soul::ast::slg::ExpressionCollection*>(collection);
                    for (const auto& expression : expressionCollection->GetExpressions())
                    {
                        expressions.AddExpression(expression.get());
                    }
                    break;
                }
            }
        }
    }
    lexerContext.SetTokens(&tokens);
    lexerContext.SetKeywords(&keywords);
    lexerContext.SetExpressions(&expressions);
    lexerContext.SetLexer(lexerFile->GetLexer());
    MakeNfas(lexerContext);
    MakeCanonicalClasses(lexerContext);
    MakeClassPartition(lexerContext);
    std::string rootDir = util::Path::GetDirectoryName(slgFile->FilePath());
    CreateClassMap(lexerContext, rootDir, verbose);
    CompressClassMap(lexerContext, rootDir, verbose);
    MakeMasterNfa(lexerContext);
    MakeDfa(lexerContext);
    WriteLexer(lexerContext, slgFile, rootDir, verbose, fileMap);
}

void GenerateLexer(soul::ast::slg::SlgFile* slgFile, bool verbose, soul::lexer::FileMap& fileMap, bool debug)
{
    if (verbose)
    {
        std::cout << "generating lexers for project '" << slgFile->ProjectName() << "'..." << "\n";
    }
    std::string filePath = util::GetFullPath(slgFile->FilePath());
    std::string dir = util::Path::GetDirectoryName(filePath);
    for (const auto& declaration : slgFile->Declarations())
    {
        switch (declaration->Kind())
        {
            case soul::ast::slg::SlgFileDeclarationKind::tokenFileDeclaration:
            {
                soul::ast::slg::TokenFileDeclaration* tokenFileDeclaration = static_cast<soul::ast::slg::TokenFileDeclaration*>(declaration.get());
                std::string tokenFilePath = util::GetFullPath(util::Path::Combine(dir, tokenFileDeclaration->FilePath()));
                if (verbose)
                {
                    std::cout << "> " << tokenFilePath << "\n";
                }
                std::unique_ptr<soul::ast::common::TokenFile> tokenFile = ParseTokenFile(tokenFilePath, tokenFileDeclaration->External(), fileMap);
                GenerateTokenModule(tokenFile.get(), verbose);
                slgFile->AddTokenFile(tokenFile.release());
                break;
            }
            case soul::ast::slg::SlgFileDeclarationKind::keywordFileDeclaration:
            {
                std::string keywordFilePath = util::GetFullPath(util::Path::Combine(dir, declaration->FilePath()));
                if (verbose)
                {
                    std::cout << "> " << keywordFilePath << "\n";
                }
                std::unique_ptr<soul::ast::slg::KeywordFile> keywordFile = ParseKeywordFile(keywordFilePath, fileMap);
                slgFile->AddKeywordFile(keywordFile.release());
                break;
            }
            case soul::ast::slg::SlgFileDeclarationKind::expressionFileDeclaration:
            {
                std::string expressionFilePath = util::GetFullPath(util::Path::Combine(dir, declaration->FilePath()));
                if (verbose)
                {
                    std::cout << "> " << expressionFilePath << "\n";
                }
                std::unique_ptr<soul::ast::slg::ExpressionFile> expressionFile = ParseExpressionFile(expressionFilePath, fileMap);
                slgFile->AddExpressionFile(expressionFile.release());
                break;
            }
            case soul::ast::slg::SlgFileDeclarationKind::lexerFileDeclararation:
            {
                std::string lexerFilePath = util::GetFullPath(util::Path::Combine(dir, declaration->FilePath()));
                if (verbose)
                {
                    std::cout << "> " << lexerFilePath << "\n";
                }
                std::unique_ptr<soul::ast::slg::LexerFile> lexerFile = ParseLexerFile(lexerFilePath, fileMap);
                slgFile->AddLexerFile(lexerFile.release());
                break;
            }
        }
    }
    for (const auto& lexerFile : slgFile->LexerFiles())
    {
        GenerateLexer(slgFile, lexerFile.get(), verbose, fileMap, debug);
    }
    if (verbose)
    {
        std::cout << "lexers for project '" << slgFile->ProjectName() << "' generated successfully." << "\n";
    }
}

} // namespace soul::slg
