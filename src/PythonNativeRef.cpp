#include <PythonNativeRef.h>
#include <python.h>

namespace python {
    
PythonNativeRef::PythonNativeRef(PyObject* pyReference)
: m_pyReference(pyReference)
, m_shouldDeleteIt(true)
{
    assert( pyReference != NULL ) ;
}
void PythonNativeRef::IncRef()
{
    Py_IncRef(m_pyReference);
}
PythonNativeRef::~PythonNativeRef()
{
    if(m_shouldDeleteIt)
    {
        Py_DecRef( m_pyReference ) ;
    }
}
PyObject* PythonNativeRef::Get() const
{
    return m_pyReference;
}

};
