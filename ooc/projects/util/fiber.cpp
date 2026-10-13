module util.fiber;

namespace util {

void* ConvertThreadToFiber(void* param)
{
    return ort_convert_thread_to_fiber(param);
}

void* CreateFiber(std::uint64_t stackSize, void* startAddress, void* param)
{
    return ort_create_fiber(stackSize, startAddress, param);
}

void SwitchToFiber(void* fiber)
{
    ort_switch_to_fiber(fiber);
}

void* GetFiberData()
{
    return ort_get_fiber_data();
}

void DeleteFiber(void* fiber)
{
    ort_delete_fiber(fiber);
}

} // namespace util
