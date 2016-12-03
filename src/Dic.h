#pragma once
#include <IObject.h>
#include <PythonNativeRef.h>

namespace python
{

class Dic : public IObject
{
public:
    Dic();
    
    PyObject* GetPy() const override;
private:
    PythonNativeRef_ptr m_pyObject;
};

}
