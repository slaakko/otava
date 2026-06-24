module std.utilities.shared_ptr;

import std.new_delete_op;

namespace std::detail {

counter_base::counter_base() : use_cnt(1), weak_cnt(1)
{
}

counter_base::~counter_base()
{
}

void counter_base::destruct()
{
    delete this;
}

void counter_base::release()
{
    --use_cnt;
    if (use_cnt != 0)
    {
        --weak_cnt;
        return;
    }
    dispose();
    weak_release();
}

void counter_base::weak_release()
{
    --weak_cnt;
    if (weak_cnt == 0)
    {
        destruct();
    }
}

} // namespace std::detail
