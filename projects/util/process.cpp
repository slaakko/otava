module util.process;

namespace util {

int RunProcess(const char* commandLine)
{
    return ort_run_process(commandLine);
}

} // namespace util
