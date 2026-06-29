export module soul.ast.common;

import std;

export namespace soul::ast::common {

std::string ToNamespaceName(const std::string& qualifiedId);

class ExportModule
{
public:
    ExportModule(const std::string& moduleName_);
    inline const std::string& ModuleName() const noexcept { return moduleName; }
    std::string NamespaceName() const;
private:
    std::string moduleName;
};

enum class ImportPrefix
{
    interfacePrefix, implementationPrefix
};

class Import
{
public:
    Import(const std::string& moduleName_, ImportPrefix prefix_);
    inline const std::string& ModuleName() const noexcept { return moduleName; }
    inline ImportPrefix Prefix() const noexcept { return prefix; }
private:
    std::string moduleName;
    ImportPrefix prefix;
};

enum class FileKind
{
    tokenFile, keywordFile, expressionFile, lexerFile, slgFile, parserFile, spgFile
};

class Visitor;

class File
{
public:
    File(FileKind kind_, const std::string& filePath_);
    virtual ~File();
    inline FileKind Kind() const noexcept { return kind; }
    inline const std::string& FilePath() const noexcept { return filePath; }
    inline bool IsExternal() const noexcept { return external; }
    inline void SetExternal() noexcept { external = true; }
    virtual void Accept(Visitor& visitor);
private:
    FileKind kind;
    std::string filePath;
    bool external;
};

enum class CollectionKind
{
    tokenCollection, keywordCollection, expressionCollection, lexer
};

class Collection
{
public:
    Collection(CollectionKind kind_, const std::string& name_);
    virtual ~Collection();
    inline CollectionKind Kind() const noexcept { return kind; }
    inline const std::string& Name() const noexcept { return name; }
    inline void SetFile(File* file_) noexcept { file = file_; }
    inline File* GetFile() const noexcept { return file; }
private:
    CollectionKind kind;
    std::string name;
    File* file;
};

class Token
{
public:
    Token(std::int64_t id_, const std::string& name_, const std::string& info_);
    Token(const std::string& name_, const std::string& info_);
    inline void SetCollection(Collection* collection_) noexcept { collection = collection_; }
    inline Collection* GetCollection() const noexcept { return collection; }
    inline std::int64_t Id() const noexcept { return id; }
    inline void SetId(std::int64_t id_) noexcept { id = id_; }
    inline const std::string& Name() const noexcept { return name; }
    inline const std::string& Info() const noexcept { return info; }
    inline bool IsAny() const noexcept { return id == -1; }
    inline bool IsEpsilon() const noexcept { return id == -2; }
    std::string FullName() const;
    std::string FullCppId() const;
    Token* Clone() const;
private:
    std::int64_t id;
    std::string name;
    std::string info;
    Collection* collection;
};

class TokenCollection : public Collection
{
public:
    TokenCollection(const std::string& name_);
    ~TokenCollection();
    inline bool Initialized() const noexcept { return initialized; }
    inline void SetInitialized() noexcept { initialized = true; }
    void AddToken(Token* token);
    inline const std::vector<std::unique_ptr<Token>>& GetTokens() const { return tokens; }
    inline std::int32_t Id() const noexcept { return id; }
    Token* GetToken(std::int64_t id) const noexcept;
    TokenCollection* Clone() const;
private:
    bool initialized;
    std::int32_t id;
    std::vector<std::unique_ptr<Token>> tokens;
    std::map<std::int64_t, Token*> tokenMap;
};

class TokenFile : public File
{
public:
    TokenFile(const std::string& filePath_);
    void SetTokenCollection(TokenCollection* tokenCollection_);
    inline TokenCollection* GetTokenCollection() const noexcept { return tokenCollection.get(); }
    void Accept(Visitor& visitor) override;
    TokenFile* Clone() const;
private:
    std::unique_ptr<TokenCollection> tokenCollection;
};

class TokenMap
{
public:
    TokenMap();
    void AddUsingToken(Token* usingToken);
    void AddToken(Token* token);
    std::vector<Token*> GetTokens(const std::string& tokenName) const;
    Token* GetToken(const std::string& tokenFullName) const noexcept;
    const Token* Any() const noexcept { return &any; }
    const Token* Epsilon() const noexcept { return &epsilon; }
private:
    std::map<std::string, Token*> usingTokenMap;
    std::map<std::string, std::vector<Token*>> tokenMap;
    std::map<std::string, Token*> tokenFullNameMap;
    Token any;
    Token epsilon;
};

class Visitor
{
public:
    Visitor();
    virtual ~Visitor();
    virtual void Visit(TokenFile& tokenFile) {}
};

} // namespace soul::ast::common
