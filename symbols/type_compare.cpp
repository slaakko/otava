// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.type_compare;

import otava.symbols.type_symbol;
import otava.symbols.classes;
import otava.symbols.class_group_symbol;
import otava.symbols.class_templates;
import otava.symbols.compound_type_symbol;
import otava.symbols.context;
import otava.symbols.function_type_symbol;
import otava.symbols.variable_symbol;
import otava.symbols.templates;

namespace otava::symbols {

bool TypeIdLess::operator()(TypeSymbol* left, TypeSymbol* right) const noexcept
{
    return left->Id() < right->Id();
}

bool TypesEqual(TypeSymbol* left, TypeSymbol* right, Context* context) noexcept
{
    if (left->IsForwardClassDeclarationSymbol())
    {
        ForwardClassDeclarationSymbol* fwdLeft = static_cast<ForwardClassDeclarationSymbol*>(left);
        if (fwdLeft->GetClassTypeSymbol(context))
        {
            left = fwdLeft->GetClassTypeSymbol(context);
        }
    }
    if (right->IsForwardClassDeclarationSymbol())
    {
        ForwardClassDeclarationSymbol* fwdRight = static_cast<ForwardClassDeclarationSymbol*>(right);
        if (fwdRight->GetClassTypeSymbol(context))
        {
            right = fwdRight->GetClassTypeSymbol(context);
        }
    }
    if (left == right) return true;
    if (left->Id() == right->Id()) return true;
    if (left->IsClassTemplateSpecializationSymbol() && right->IsClassTemplateSpecializationSymbol())
    {
        ClassTemplateSpecializationSymbol* leftSpecialization = static_cast<ClassTemplateSpecializationSymbol*>(left);
        ClassTemplateSpecializationSymbol* rightSpecialization = static_cast<ClassTemplateSpecializationSymbol*>(right);
        if (!TypesEqual(leftSpecialization->ClassTemplate(context), rightSpecialization->ClassTemplate(context), context)) return false;
        if (leftSpecialization->TemplateArguments(context).size() != rightSpecialization->TemplateArguments(context).size()) return false;
        Cardinality n = Cardinality(leftSpecialization->TemplateArguments(context).size());
        for (Index i = Index(0); i < Index(n); ++i)
        {
            Symbol* leftTemplateArg = leftSpecialization->TemplateArguments(context)[ToUnderlying(i)];
            Symbol* rightTemplateArg = rightSpecialization->TemplateArguments(context)[ToUnderlying(i)];
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
        if (TypesEqual(leftCompound->GetBaseType(context), rightCompound->GetBaseType(context), context) && 
            leftCompound->GetDerivations() == rightCompound->GetDerivations()) return true;
    }
    if (left->IsTemplateParameterSymbol() && right->IsTemplateParameterSymbol())
    {
        TemplateParameterSymbol* leftTemplateParam = static_cast<TemplateParameterSymbol*>(left);
        TemplateParameterSymbol* rightTemplateParam = static_cast<TemplateParameterSymbol*>(right);
        return leftTemplateParam->GetIndex() == rightTemplateParam->GetIndex();
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
            if (classGroupType->GetClassGroup()->Name() == classType->Group(context)->Name())
            {
                return true;
            }
        }
        else if (left->IsClassTypeSymbol() && right->IsClassGroupTypeSymbol())
        {
            ClassGroupTypeSymbol* classGroupType = static_cast<ClassGroupTypeSymbol*>(right);
            ClassTypeSymbol* classType = static_cast<ClassTypeSymbol*>(left);
            if (classGroupType->GetClassGroup()->Name() == classType->Group(context)->Name())
            {
                return true;
            }
        }
    }
    return false;
}

} // namespace otava::symbols
