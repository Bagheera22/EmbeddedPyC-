#pragma once
#include "IObject.h"
#include "PythonNativeRef.h"
#include <string>

namespace python
{
class Module;
class Code : public IObject
{
public:
    Code(const std::string& code);
    
    std::unique_ptr<Module> ExecCodeModule(const std::string& moduleName);
    
    PyObject* GetPy() const override;
    
private:
    PythonNativeRef m_pyObject;
};

};
