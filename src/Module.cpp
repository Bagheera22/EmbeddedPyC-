#include <Module.h>
#include <exception.hpp>
#include <Python.h>
#include <Code.h>
#include <Function.h>

namespace python
{
Module::Module(PyObject* pyModule)
: m_pyObject(pyModule)
{
}
    
Module::Module(const std::string& moduleName, const Code& code)
    : m_pyObject(PyImport_ExecCodeModule(moduleName.c_str() , code.GetPy()))
{
    if ( m_pyObject.Get() ==  nullptr )
    {
        Exception::translateException() ;
    }
}
std::unique_ptr<Function> Module::GetAttr(const std::string& attr)
{
    return std::unique_ptr<Function>(new Function(*this, attr));
}
    
PyObject* Module::GetPy() const
{
    return m_pyObject.Get();
}

};
