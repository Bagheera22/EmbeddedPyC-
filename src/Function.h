#pragma once
#include <IObject.h>
#include <PythonNativeRef.h>
#include <string>

namespace python
{
    class Tuple;
    class Dic;
    class Module;
    class Function : public IObject
    {
    public:
        Function(const Module& module, const std::string& attr);
        std::unique_ptr<IObject> CallObj(const Tuple& args, const Dic& kywdArgs);
        PyObject* GetPy() const override;
    private:
        PythonNativeRef_ptr m_pyObject;
    };
    
}
