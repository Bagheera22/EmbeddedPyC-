#pragma once
#include <memory>

typedef struct _object PyObject;

namespace python {

class PythonNativeRef
{
public:
    PythonNativeRef(PyObject* pyReference);
    ~PythonNativeRef();
    void IncRef();
    PyObject* Get() const;
private:
    PyObject* m_pyReference;
    bool m_shouldDeleteIt;
};

typedef std::shared_ptr<PythonNativeRef> PythonNativeRef_ptr;

};
