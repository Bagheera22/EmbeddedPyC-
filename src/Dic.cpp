#include <Dic.h>
#include <Python.h>

namespace python
{

Dic::Dic()
: m_pyObject(std::make_shared<PythonNativeRef>(PyDict_New()))
{}

PyObject* Dic::GetPy() const
{
    return m_pyObject->Get();
}

};
