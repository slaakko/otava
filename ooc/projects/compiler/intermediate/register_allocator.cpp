// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.intermediate.register_allocator;

namespace otava::intermediate {

RegisterAllocator::RegisterAllocator()
{
}

RegisterAllocator::~RegisterAllocator()
{
}

SpillData::SpillData() noexcept : registerGroupToSpill(nullptr), instToSpill(nullptr)
{
}

} // otava::intermediate
