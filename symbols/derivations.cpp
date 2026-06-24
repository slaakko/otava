// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.derivations;

import otava.symbols.writer;
import otava.symbols.reader;

namespace otava::symbols {

Derivations Merge(Derivations left, Derivations right) noexcept
{
    Derivations merged = Derivations::none;
    if (HasDerivation(left, Derivations::constDerivation))
    {
        merged = merged | Derivations::constDerivation;
    }
    else if (HasDerivation(right, Derivations::constDerivation))
    {
        merged = merged | Derivations::constDerivation;
    }
    if (HasDerivation(left, Derivations::volatileDerivation))
    {
        merged = merged | Derivations::volatileDerivation;
    }
    else if (HasDerivation(right, Derivations::volatileDerivation))
    {
        merged = merged | Derivations::volatileDerivation;
    }
    int leftPointerCount = PointerCount(left);
    int rightPointerCount = PointerCount(right);
    int pointerCount = leftPointerCount + rightPointerCount;
    merged = SetPointerCount(merged, pointerCount);
    if (HasDerivation(left, Derivations::lvalueRefDerivation))
    {
        merged = merged | Derivations::lvalueRefDerivation;
    }
    else if (HasDerivation(right, Derivations::lvalueRefDerivation))
    {
        merged = merged | Derivations::lvalueRefDerivation;
    }
    if (HasDerivation(left, Derivations::rvalueRefDerivation))
    {
        merged = merged | Derivations::rvalueRefDerivation;
    }
    else if (HasDerivation(right, Derivations::rvalueRefDerivation))
    {
        merged = merged | Derivations::rvalueRefDerivation;
    }
    return merged;
}

Derivations UnifyDerivations(Derivations left, Derivations right) noexcept
{
    Derivations result = Derivations::none;
    if (HasDerivation(left, Derivations::constDerivation) || HasDerivation(right, Derivations::constDerivation))
    {
        result = result | Derivations::constDerivation;
    }
    int pointerCount = PointerCount(left) + PointerCount(right);
    result = SetPointerCount(result, pointerCount);
    if (HasDerivation(left, Derivations::lvalueRefDerivation))
    {
        result = result | Derivations::lvalueRefDerivation;
    }
    else if (HasDerivation(left, Derivations::rvalueRefDerivation))
    {
        result = result | Derivations::rvalueRefDerivation;
    }
    else if (HasDerivation(right, Derivations::lvalueRefDerivation))
    {
        result = result | Derivations::lvalueRefDerivation;
    }
    else if (HasDerivation(right, Derivations::rvalueRefDerivation))
    {
        result = result | Derivations::rvalueRefDerivation;
    }
    return result;
}

int CountMatchingDerivations(Derivations left, Derivations right) noexcept
{
    int matchingDerivationsCount = 0;
    if (HasDerivation(left, Derivations::constDerivation) && HasDerivation(right, Derivations::constDerivation))
    {
        ++matchingDerivationsCount;
    }
    if (HasDerivation(left, Derivations::volatileDerivation) && HasDerivation(right, Derivations::volatileDerivation))
    {
        ++matchingDerivationsCount;
    }
    int leftPointerCount = PointerCount(left);
    int rightPointerCount = PointerCount(right);
    matchingDerivationsCount += std::min(leftPointerCount, rightPointerCount);
    if (HasDerivation(left, Derivations::lvalueRefDerivation) && HasDerivation(right, Derivations::lvalueRefDerivation))
    {
        ++matchingDerivationsCount;
    }
    else if (HasDerivation(left, Derivations::rvalueRefDerivation) && HasDerivation(right, Derivations::rvalueRefDerivation))
    {
        ++matchingDerivationsCount;
    }
    return matchingDerivationsCount;
}

void Write(Writer& writer, Derivations derivations)
{
    writer.GetBinaryStreamWriter().Write(ToUnderlying(derivations));
}

void Read(Reader& reader, Derivations& derivations)
{
    derivations = static_cast<Derivations>(reader.CurrentReader().ReadByte());
}

} // namespace otava::symbols

