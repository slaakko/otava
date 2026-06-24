import std;

enum class rb_node_color
{
    red, black
};

int main()
{
    rb_node_color x = rb_node_color::black;
    x = rb_node_color::red;
}
