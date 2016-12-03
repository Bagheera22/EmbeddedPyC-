#include <PyCast.h>
#include <Value.h>
#include <python.h>
#include <IObject.h>

namespace python {


PyObject* PyCast<long>::Cast(const long value)
{
    return PyLong_FromLong(value);
}
long PyCast<long>::Cast(PyObject* value)
{
    return PyLong_AsLong(value);
}

PyObject* PyCast<double>::Cast(const double value)
{
    return PyFloat_FromDouble(value);
}
double PyCast<double>::Cast(PyObject* value)
{
    return PyFloat_AsDouble(value);
}

PyObject* PyCast<std::string>::Cast(const std::string& value)
{
    return PyUnicode_FromString(value.c_str());
}
std::string PyCast<std::string>::Cast(PyObject* value)
{
    return PyUnicode_AsUTF8(value);
}
    
PyObject* PyCast<IObject>::Cast(const IObject* value)
{
    return value->GetPy();
}


std::unique_ptr<IObject> PyCast<IObject>::Cast(PyObject* value)
{
    const char* type = value->ob_type->tp_name;
    
    if(strcmp(type,"int") == 0)
    {
        return std::unique_ptr<IObject>(new Value<long>(value));
    }
    if(strcmp(type,"str") == 0)
    {
        return std::unique_ptr<IObject>(new Value<std::string>(value));
    }
    printf("%s\n",value->ob_type->tp_name);
    //todo
    return nullptr;
}
    
}
