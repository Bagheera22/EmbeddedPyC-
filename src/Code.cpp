#include "Code.h"
#include "exception.hpp"
#include "Module.h"

#include <Python.h>
#include <memory>

namespace python
{

Code::Code(const std::string& code)
: m_pyObject(Py_CompileString(code.c_str(),"",Py_file_input))
{
    if(m_pyObject.Get() == NULL)
    {
        Exception::translateException() ;
    }
}
std::unique_ptr<Module> Code::ExecCodeModule(const std::string& moduleName)
{
    return std::unique_ptr<Module>(new Module(moduleName, *this));
}
PyObject* Code::GetPy() const
{
    return m_pyObject.Get();
}


};
