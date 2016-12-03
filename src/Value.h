#pragma once
#include <IObject.h>
#include <PythonNativeRef.h>
#include <PyCast.h>

namespace python {

template<typename T>
class Value : public IObject
{
public:
    explicit Value( PyObject* val )
    : m_pyObject(std::make_shared<PythonNativeRef>(val))
    {}
    
    explicit Value( const T& val )
    : m_pyObject(std::make_shared<PythonNativeRef>(PyCast<T>::Cast( val )))
    {}
    T Get() const
    {
        return PyCast<T>::Cast(m_pyObject->Get());
    }
    PyObject* GetPy() const override
    {
        return m_pyObject->Get();
    }
private:
    PythonNativeRef_ptr m_pyObject;
};

};
