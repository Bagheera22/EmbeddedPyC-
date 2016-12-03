#include <IObject.h>
#include <Value.h>
#include <Python.h>

namespace python {

std::string IObject::ToString() const
{
    Value<std::string>str(PyObject_Repr(GetPy()));
    return str.Get();
}

    
};
