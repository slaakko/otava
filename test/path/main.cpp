import std;
import util.path;

int main()
{
    std::string p = util::GetFullPath(util::Path::Combine("foo", "bar"));
    std::cout << p << "\n";
}
