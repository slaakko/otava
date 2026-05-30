// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.fundamental_type_symbol;

import otava.symbols.context;
import otava.symbols.emitter;
import otava.symbols.exception;
import otava.symbols.modules;
import otava.symbols.reader;
import otava.symbols.writer;
import util.binary_stream_writer;

namespace otava::symbols {

int Rank(FundamentalTypeKind fundamentalTypeKind) noexcept
{
    switch (fundamentalTypeKind)
    {
    case FundamentalTypeKind::boolType:
    {
        return 0;
    }
    case FundamentalTypeKind::charType:
    case FundamentalTypeKind::signedCharType:
    case FundamentalTypeKind::unsignedCharType:
    case FundamentalTypeKind::char8Type:
    {
        return 1;
    }
    case FundamentalTypeKind::char16Type:
    case FundamentalTypeKind::shortIntType:
    case FundamentalTypeKind::unsignedShortIntType:
    {
        return 2;
    }
    case FundamentalTypeKind::char32Type:
    case FundamentalTypeKind::wcharType:
    case FundamentalTypeKind::intType:
    case FundamentalTypeKind::unsignedIntType:
    {
        return 3;
    }
    case FundamentalTypeKind::longIntType:
    case FundamentalTypeKind::unsignedLongIntType:
    {
        return 4;
    }
    case FundamentalTypeKind::longLongIntType:
    case FundamentalTypeKind::unsignedLongLongIntType:
    {
        return 5;
    }
    }
    return -1;
}

bool IsSignedIntegerType(FundamentalTypeKind fundamentalTypeKind) noexcept
{
    switch (fundamentalTypeKind)
    {
    case FundamentalTypeKind::signedCharType:
    case FundamentalTypeKind::shortIntType:
    case FundamentalTypeKind::intType:
    case FundamentalTypeKind::longIntType:
    case FundamentalTypeKind::longLongIntType:
    {
        return true;
    }
    }
    return false;
}

bool IsUnsignedIntegerType(FundamentalTypeKind fundamentalTypeKind) noexcept
{
    switch (fundamentalTypeKind)
    {
    case FundamentalTypeKind::unsignedCharType:
    case FundamentalTypeKind::unsignedShortIntType:
    case FundamentalTypeKind::unsignedIntType:
    case FundamentalTypeKind::unsignedLongIntType:
    case FundamentalTypeKind::unsignedLongLongIntType:
    {
        return true;
    }
    }
    return false;
}

class FundamentalTypeFlagMapper
{
public:
    static FundamentalTypeFlagMapper& Instance();
    FundamentalTypeKind GetFundamentalTypeKind(DeclarationFlags flags) const;
private:
    FundamentalTypeFlagMapper();
    std::map<DeclarationFlags, FundamentalTypeKind> flagMap;
};

FundamentalTypeFlagMapper& FundamentalTypeFlagMapper::Instance()
{
    static FundamentalTypeFlagMapper instance;
    return instance;
}

FundamentalTypeFlagMapper::FundamentalTypeFlagMapper()
{
    flagMap[DeclarationFlags::voidFlag] = FundamentalTypeKind::voidType;
    flagMap[DeclarationFlags::charFlag] = FundamentalTypeKind::charType;
    flagMap[DeclarationFlags::char8Flag] = FundamentalTypeKind::char8Type;
    flagMap[DeclarationFlags::char16Flag] = FundamentalTypeKind::char16Type;
    flagMap[DeclarationFlags::char32Flag] = FundamentalTypeKind::char32Type;
    flagMap[DeclarationFlags::wcharFlag] = FundamentalTypeKind::wcharType;
    flagMap[DeclarationFlags::boolFlag] = FundamentalTypeKind::boolType;
    flagMap[DeclarationFlags::shortFlag] = FundamentalTypeKind::shortIntType;
    flagMap[DeclarationFlags::shortFlag | DeclarationFlags::intFlag] = FundamentalTypeKind::shortIntType;
    flagMap[DeclarationFlags::intFlag] = FundamentalTypeKind::intType;
    flagMap[DeclarationFlags::longFlag] = FundamentalTypeKind::longIntType;
    flagMap[DeclarationFlags::longFlag | DeclarationFlags::intFlag] = FundamentalTypeKind::longIntType;
    flagMap[DeclarationFlags::longLongFlag] = FundamentalTypeKind::longLongIntType;
    flagMap[DeclarationFlags::longLongFlag | DeclarationFlags::intFlag] = FundamentalTypeKind::longLongIntType;
    flagMap[DeclarationFlags::signedFlag] = FundamentalTypeKind::intType;
    flagMap[DeclarationFlags::signedFlag | DeclarationFlags::charFlag] = FundamentalTypeKind::signedCharType;
    flagMap[DeclarationFlags::signedFlag | DeclarationFlags::longFlag] = FundamentalTypeKind::longIntType;
    flagMap[DeclarationFlags::signedFlag | DeclarationFlags::shortFlag] = FundamentalTypeKind::shortIntType;
    flagMap[DeclarationFlags::signedFlag | DeclarationFlags::intFlag] = FundamentalTypeKind::intType;
    flagMap[DeclarationFlags::signedFlag | DeclarationFlags::longLongFlag] = FundamentalTypeKind::longLongIntType;
    flagMap[DeclarationFlags::signedFlag | DeclarationFlags::longFlag | DeclarationFlags::intFlag] = FundamentalTypeKind::longIntType;
    flagMap[DeclarationFlags::signedFlag | DeclarationFlags::longLongFlag | DeclarationFlags::intFlag] = FundamentalTypeKind::longLongIntType;
    flagMap[DeclarationFlags::signedFlag | DeclarationFlags::shortFlag | DeclarationFlags::intFlag] = FundamentalTypeKind::shortIntType;
    flagMap[DeclarationFlags::unsignedFlag] = FundamentalTypeKind::unsignedIntType;
    flagMap[DeclarationFlags::unsignedFlag | DeclarationFlags::charFlag] = FundamentalTypeKind::unsignedCharType;
    flagMap[DeclarationFlags::unsignedFlag | DeclarationFlags::longFlag] = FundamentalTypeKind::unsignedLongIntType;
    flagMap[DeclarationFlags::unsignedFlag | DeclarationFlags::shortFlag] = FundamentalTypeKind::unsignedShortIntType;
    flagMap[DeclarationFlags::unsignedFlag | DeclarationFlags::intFlag] = FundamentalTypeKind::unsignedIntType;
    flagMap[DeclarationFlags::unsignedFlag | DeclarationFlags::shortFlag | DeclarationFlags::intFlag] = FundamentalTypeKind::unsignedShortIntType;
    flagMap[DeclarationFlags::unsignedFlag | DeclarationFlags::longFlag | DeclarationFlags::intFlag] = FundamentalTypeKind::unsignedLongIntType;
    flagMap[DeclarationFlags::unsignedFlag | DeclarationFlags::longLongFlag] = FundamentalTypeKind::unsignedLongLongIntType;
    flagMap[DeclarationFlags::unsignedFlag | DeclarationFlags::longLongFlag | DeclarationFlags::intFlag] = FundamentalTypeKind::unsignedLongLongIntType;
    flagMap[DeclarationFlags::floatFlag] = FundamentalTypeKind::floatType;
    flagMap[DeclarationFlags::doubleFlag] = FundamentalTypeKind::doubleType;
    flagMap[DeclarationFlags::longFlag | DeclarationFlags::doubleFlag] = FundamentalTypeKind::longDoubleType;
    flagMap[DeclarationFlags::autoFlag] = FundamentalTypeKind::autoType;
}

FundamentalTypeKind FundamentalTypeFlagMapper::GetFundamentalTypeKind(DeclarationFlags flags) const
{
    auto it = flagMap.find(flags);
    if (it != flagMap.cend())
    {
        return it->second;
    }
    else
    {
        return FundamentalTypeKind::none;
    }
}

std::string MakeFundamentalTypeName(FundamentalTypeKind fundamentalTypeKind)
{
    switch (fundamentalTypeKind)
    {
    case FundamentalTypeKind::none: return "none";
    case FundamentalTypeKind::boolType: return "bool";
    case FundamentalTypeKind::charType: return "char";
    case FundamentalTypeKind::signedCharType: return "signed char";
    case FundamentalTypeKind::unsignedCharType: return "unsigned char";
    case FundamentalTypeKind::char8Type: return "char8_t";
    case FundamentalTypeKind::char16Type: return "char16_t";
    case FundamentalTypeKind::shortIntType: return "short int";
    case FundamentalTypeKind::unsignedShortIntType: return "unsigned short int";
    case FundamentalTypeKind::char32Type: return "char32_t";
    case FundamentalTypeKind::wcharType: return "wchar_t";
    case FundamentalTypeKind::intType: return "int";
    case FundamentalTypeKind::unsignedIntType: return "unsigned int";
    case FundamentalTypeKind::longIntType: return "long int";
    case FundamentalTypeKind::unsignedLongIntType: return "unsigned long int";
    case FundamentalTypeKind::longLongIntType: return "long long int";
    case FundamentalTypeKind::unsignedLongLongIntType: return "unsigned long long int";
    case FundamentalTypeKind::floatType: return "float";
    case FundamentalTypeKind::doubleType: return "double";
    case FundamentalTypeKind::longDoubleType: return "long double";
    case FundamentalTypeKind::voidType: return "void";
    case FundamentalTypeKind::autoType: return "auto";
    case FundamentalTypeKind::nullPtrType: return "nullptr_t";
    }
    return "<fundamental_type>";
}

FundamentalTypeSymbol::FundamentalTypeSymbol(Module* module_, SymbolId symbolId_) : TypeSymbol(module_, symbolId_), fundamentalTypeKind(FundamentalTypeKind::none)
{
}

FundamentalTypeSymbol::FundamentalTypeSymbol(Module* module_, SymbolId symbolId_, FundamentalTypeKind fundamentalTypeKind_) :
    TypeSymbol(module_, symbolId_, MakeFundamentalTypeName(fundamentalTypeKind_)), fundamentalTypeKind(fundamentalTypeKind_)
{
}

void FundamentalTypeSymbol::Write(Writer& writer) 
{
    TypeSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(fundamentalTypeKind));
}

void FundamentalTypeSymbol::Read(Reader& reader) 
{
    TypeSymbol::Read(reader);
    fundamentalTypeKind = FundamentalTypeKind(reader.CurrentReader().ReadByte());
}

otava::intermediate::Type* FundamentalTypeSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    switch (fundamentalTypeKind)
    {
    case FundamentalTypeKind::charType:
    case FundamentalTypeKind::unsignedCharType:
    case FundamentalTypeKind::char8Type:
    {
        return emitter.GetByteType();
    }
    case FundamentalTypeKind::signedCharType:
    {
        return emitter.GetSByteType();
    }
    case FundamentalTypeKind::shortIntType:
    {
        return emitter.GetShortType();
    }
    case FundamentalTypeKind::char16Type:
    case FundamentalTypeKind::wcharType:
    case FundamentalTypeKind::unsignedShortIntType:
    {
        return emitter.GetUShortType();
    }
    case FundamentalTypeKind::intType:
    case FundamentalTypeKind::longIntType:
    {
        return emitter.GetIntType();
    }
    case FundamentalTypeKind::char32Type:
    case FundamentalTypeKind::unsignedIntType:
    case FundamentalTypeKind::unsignedLongIntType:
    {
        return emitter.GetUIntType();
    }
    case FundamentalTypeKind::boolType:
    {
        return emitter.GetBoolType();
    }
    case FundamentalTypeKind::longLongIntType:
    {
        return emitter.GetLongType();
    }
    case FundamentalTypeKind::unsignedLongLongIntType:
    {
        return emitter.GetULongType();
    }
    case FundamentalTypeKind::floatType:
    {
        return emitter.GetFloatType();
    }
    case FundamentalTypeKind::doubleType:
    {
        return emitter.GetDoubleType();
    }
    case FundamentalTypeKind::voidType:
    {
        return emitter.GetVoidType();
    }
    case FundamentalTypeKind::nullPtrType:
    {
        return emitter.MakePtrType(emitter.GetVoidType());
    }
    default:
    {
        ThrowException("unsupported fundamental type", fullSpan, context);
    }
    }
    return nullptr;
}

TypeSymbol* GetFundamentalType(DeclarationFlags fundamentalTypeFlags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    FundamentalTypeKind fundamentalTypeKind = FundamentalTypeFlagMapper::Instance().GetFundamentalTypeKind(fundamentalTypeFlags);
    if (fundamentalTypeKind != FundamentalTypeKind::none)
    {
        return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(fundamentalTypeKind, context);
    }
    else
    {
        ThrowException("invalid combination of fundamental type specifiers", fullSpan, context);
        return nullptr;
    }
}

} // namespace otava::symbols
