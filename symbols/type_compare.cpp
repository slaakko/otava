// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.type_compare;

import otava.symbols.type_symbol;
import otava.symbols.classes;

namespace otava::symbols {

bool TypeIdLess::operator()(TypeSymbol* left, TypeSymbol* right) const noexcept
{
    return left->Id() < right->Id();
}

bool TypesEqual(TypeSymbol* left, TypeSymbol* right, Context* context) noexcept
{
/*
    if (left->IsForwardClassDeclarationSymbol())
    {
        ForwardClassDeclarationSymbol* fwdLeft = static_cast<ForwardClassDeclarationSymbol*>(left);
        if (fwdLeft->GetClassTypeSymbol())
        {
            left = fwdLeft->GetClassTypeSymbol();
        }
    }
    if (right->IsForwardClassDeclarationSymbol())
    {
        ForwardClassDeclarationSymbol* fwdRight = static_cast<ForwardClassDeclarationSymbol*>(right);
        if (fwdRight->GetClassTypeSymbol())
        {
            right = fwdRight->GetClassTypeSymbol();
        }
    }
    if (left == right) return true;
    if (left->Id() == right->Id()) return true;
    if (left->IsClassTemplateSpecializationSymbol() && right->IsClassTemplateSpecializationSymbol())
    {
        ClassTemplateSpecializationSymbol* leftSpecialization = static_cast<ClassTemplateSpecializationSymbol*>(left);
        ClassTemplateSpecializationSymbol* rightSpecialization = static_cast<ClassTemplateSpecializationSymbol*>(right);
        if (!TypesEqual(leftSpecialization->ClassTemplate(), rightSpecialization->ClassTemplate(), context)) return false;
        if (leftSpecialization->TemplateArguments().size() != rightSpecialization->TemplateArguments().size()) return false;
        int n = leftSpecialization->TemplateArguments().size();
        for (int i = 0; i < n; ++i)
        {
            Symbol* leftTemplateArg = leftSpecialization->TemplateArguments()[i];
            Symbol* rightTemplateArg = rightSpecialization->TemplateArguments()[i];
            if (leftTemplateArg->IsTypeSymbol() && rightTemplateArg->IsTypeSymbol())
            {
                TypeSymbol* leftTemplateArgType = static_cast<TypeSymbol*>(leftTemplateArg);
                TypeSymbol* rightTemplateArgType = static_cast<TypeSymbol*>(rightTemplateArg);
                if (!TypesEqual(leftTemplateArgType, rightTemplateArgType, context)) return false;
            }
            else
            {
                if (leftTemplateArg != rightTemplateArg) return false;
            }
        }
        return true;
    }
    if (left->IsCompoundTypeSymbol() && right->IsCompoundTypeSymbol())
    {
        CompoundTypeSymbol* leftCompound = static_cast<CompoundTypeSymbol*>(left);
        CompoundTypeSymbol* rightCompound = static_cast<CompoundTypeSymbol*>(right);
        if (TypesEqual(leftCompound->BaseType(), rightCompound->BaseType(), context) && leftCompound->GetDerivations() == rightCompound->GetDerivations()) return true;
    }
    if (left->IsTemplateParameterSymbol() && right->IsTemplateParameterSymbol())
    {
        TemplateParameterSymbol* leftTemplateParam = static_cast<TemplateParameterSymbol*>(left);
        TemplateParameterSymbol* rightTemplateParam = static_cast<TemplateParameterSymbol*>(right);
        return leftTemplateParam->Index() == rightTemplateParam->Index();
    }
    if (left->IsFunctionType() && right->IsFunctionType())
    {
        FunctionTypeSymbol* leftFnType = static_cast<FunctionTypeSymbol*>(left);
        FunctionTypeSymbol* rightFnType = static_cast<FunctionTypeSymbol*>(right);
        return FunctionTypesEqual(leftFnType, rightFnType, context);
    }
    if (context && context->GetFlag(ContextFlags::matchClassGroup))
    {
        if (left->IsClassGroupTypeSymbol() && right->IsClassTypeSymbol())
        {
            ClassGroupTypeSymbol* classGroupType = static_cast<ClassGroupTypeSymbol*>(left);
            ClassTypeSymbol* classType = static_cast<ClassTypeSymbol*>(right);
            if (classGroupType->ClassGroup()->Name() == classType->Group()->Name())
            {
                return true;
            }
        }
        else if (left->IsClassTypeSymbol() && right->IsClassGroupTypeSymbol())
        {
            ClassGroupTypeSymbol* classGroupType = static_cast<ClassGroupTypeSymbol*>(right);
            ClassTypeSymbol* classType = static_cast<ClassTypeSymbol*>(left);
            if (classGroupType->ClassGroup()->Name() == classType->Group()->Name())
            {
                return true;
            }
        }
    }
*/
    return false;
}

} // namespace otava::symbols
