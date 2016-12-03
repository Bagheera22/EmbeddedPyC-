#pragma once
#include <IObject.h>
#include <PythonNativeRef.h>
#include <string>

namespace python
{
    class Code;
    class Function;
    
class Module : public IObject
{
public:
    Module(PyObject* pyModule);
    Module(const std::string& moduleName, const Code& code);
    
    std::unique_ptr<Function> GetAttr(const std::string& attr);
    PyObject* GetPy() const override;
private:
    PythonNativeRef m_pyObject;
};

};

