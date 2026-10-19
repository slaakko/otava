// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.ast.common;

import util;

namespace soul::ast::common {

std::string ToNamespaceName(const std::string& qualifiedId)
{
    std::string namespaceName;
    std::vector<std::string> components = util::Split(qualifiedId, '.');
    bool first = true;
    for (const auto& component : components)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            namespaceName.append("::");
        }
        namespaceName.append(component);
    }
    return namespaceName;
}

ExportModule::ExportModule(const std::string& moduleName_) : moduleName(moduleName_)
{
}

std::string ExportModule::NamespaceName() const
{
    std::string namespaceName;
    std::vector<std::string> components = util::Split(moduleName, '.');
    bool first = true;
    for (const auto& component : components)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            namespaceName.append("::");
        }
        namespaceName.append(component);
    }
    return namespaceName;
}

Import::Import(const std::string& moduleName_, ImportPrefix prefix_) : moduleName(moduleName_), prefix(prefix_)
{
}

File::File(FileKind kind_, const std::string& filePath_) : kind(kind_), filePath(filePath_), external(false)
{
}

File::~File()
{
}

void File::Accept(Visitor& visitor)
{
}

Collection::Collection(CollectionKind kind_, const std::string& name_) : kind(kind_), name(name_), file(nullptr)
{
}

Collection::~Collection()
{
}

Token::Token(std::int64_t id_, const std::string& name_, const std::string& info_) : id(id_), name(name_), info(info_), collection(nullptr)
{
}

Token::Token(const std::string& name_, const std::string& info_) : id(-1), name(name_), info(info_), collection(nullptr)
{
}

Token* Token::Clone() const
{
    Token* clone = new Token(id, name, info);
    return clone;
}

std::string Token::FullName() const
{
    Collection* collection = GetCollection();
    if (collection)
    {
        return collection->Name() + "." + Name();
    }
    else
    {
        return Name();
    }
}

std::string Token::FullCppId() const
{
    std::string fullCppId;
    std::vector<std::string> components = util::Split(FullName(), '.');
    bool first = true;
    for (const std::string& component : components)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            fullCppId.append("::");
        }
        fullCppId.append(component);
    }
    return fullCppId;
}

TokenCollection::TokenCollection(const std::string& name_) : 
    Collection(CollectionKind::tokenCollection, name_), initialized(false), id(std::hash<std::string>()(Name()) & 0x7FFFFFFF)
{
}

void TokenCollection::AddToken(Token* token)
{
    token->SetCollection(this);
    if (token->Id() == -1)
    {
        std::int64_t tokenId = (std::int64_t(Id()) << 32) | (std::int64_t(tokens.size()) + 1);
        token->SetId(tokenId);
    }
    tokens.push_back(std::unique_ptr<Token>(token));
    tokenMap[token->Id()] = token;
    token->SetCollection(this);
}

Token* TokenCollection::GetToken(std::int64_t id) const noexcept
{
    auto it = tokenMap.find(id);
    if (it != tokenMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

TokenCollection* TokenCollection::Clone() const
{
    TokenCollection* clone = new TokenCollection(Name());
    for (const auto& token : tokens)
    {
        clone->AddToken(token->Clone());
    }
    return clone;
}

TokenFile::TokenFile(const std::string& filePath_) : File(FileKind::tokenFile, filePath_)
{
}

TokenFile* TokenFile::Clone() const
{
    TokenFile* clone = new TokenFile(FilePath());
    clone->SetTokenCollection(tokenCollection->Clone());
    return clone;
}

void TokenFile::SetTokenCollection(TokenCollection* tokenCollection_)
{
    tokenCollection.reset(tokenCollection_);
    tokenCollection->SetFile(this);
}

void TokenFile::Accept(soul::ast::common::Visitor& visitor)
{
    visitor.Visit(*this);
}

TokenMap::TokenMap() : any(-1, "*", "<any>"), epsilon(-2, "#", "<epsilon>")
{
}

void TokenMap::AddUsingToken(Token* usingToken)
{
    usingTokenMap[usingToken->Name()] = usingToken;
}

void TokenMap::AddToken(Token* token)
{
    auto it = tokenFullNameMap.find(token->FullName());
    if (it != tokenFullNameMap.end())
    {
        throw std::runtime_error("token name '" + token->FullName() + "' not unique");
    }
    tokenFullNameMap[token->FullName()] = token;
    std::vector<Token*>& tokens = tokenMap[token->Name()];
    if (std::find(tokens.begin(), tokens.end(), token) == tokens.end())
    {
        tokens.push_back(token);
    }
}

std::vector<Token*> TokenMap::GetTokens(const std::string& tokenName) const
{
    auto uit = usingTokenMap.find(tokenName);
    if (uit != usingTokenMap.end())
    {
        std::vector<Token*> tokens;
        tokens.push_back(uit->second);
        return tokens;
    }
    auto it = tokenMap.find(tokenName);
    if (it != tokenMap.end())
    {
        return it->second;
    }
    else
    {
        return std::vector<Token*>();
    }
}

Token* TokenMap::GetToken(const std::string& tokenFullName) const noexcept
{
    auto it = tokenFullNameMap.find(tokenFullName);
    if (it != tokenFullNameMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

Visitor::Visitor()
{
}

Visitor::~Visitor()
{
}

} // namespace soul::ast::common
