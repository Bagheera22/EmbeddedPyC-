#include <Function.h>
#include <Python.h>
#include <Module.h>
#include <exception.hpp>
#include <PyCast.h>
#include <Dic.h>
#include <Tuple.h>

namespace python
{
    
    Function::Function(const Module& module, const std::string& attr)
    : m_pyObject(std::make_shared<PythonNativeRef>(PyObject_GetAttrString( module.GetPy() , attr.c_str() ) ))
    {
        if ( m_pyObject.get() == NULL )
        {
            Exception::translateException() ;
        }
    }
    std::unique_ptr<IObject> Function::CallObj(const Tuple& args, const Dic& kywdArgs)
    {
        // call the object
        PyObject* pResultObj = PyObject_Call( m_pyObject->Get() , args.GetPy() , kywdArgs.GetPy() ) ;
        if ( pResultObj == NULL )
        {
            Exception::translateException() ;
        }
        return PyCast<IObject>::Cast(pResultObj);
    }

    PyObject* Function::GetPy() const
    {
        return m_pyObject->Get();
    }
    
};
