#include <Tuple.h>
#include <Python.h>
#include <PyCast.h>

namespace python {

Tuple::Tuple(std::size_t n)
: m_pyObject(std::make_shared<PythonNativeRef>(PyTuple_New(n)))
{}

std::size_t Tuple::GetSize() const
{
    return PyTuple_Size(m_pyObject->Get());
}
std::unique_ptr<IObject> Tuple::GetItem(const std::size_t& index) const
{
    return PyCast<IObject>::Cast(PyTuple_GetItem(m_pyObject->Get(),index));
}
void Tuple::SetItem(const std::size_t& index, const IObject& object)
{
    Py_IncRef(object.GetPy());
    PyTuple_SetItem(m_pyObject->Get(), index, object.GetPy());
}
PyObject* Tuple::GetPy() const
{
    return m_pyObject->Get();
}

};
