export module conversion_table;

import std;

class Module
{
public:
    Module() {}
};

class ConversionTable
{
public:
    ConversionTable(Module* module_);
private:
    Module* module;
    bool tableRead;
};
