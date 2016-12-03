#pragma once

#include <string>
typedef struct _object PyObject;

namespace python {

class IObject;
    
template <typename T>
class PyCast;

template<>
class PyCast<long>
{
public:
    static PyObject* Cast(const long value);
    static long Cast(PyObject* value);
};

template<>
class PyCast<double>
{
public:
    static PyObject* Cast(const double value);
    static double Cast(PyObject* value);
};

template<>
class PyCast<std::string>
{
public:
    static PyObject* Cast(const std::string& value);
    static std::string Cast(PyObject* value);
};
    
template<>
class PyCast<IObject>
{
public:
    static PyObject* Cast(const IObject* value);
    static std::unique_ptr<IObject> Cast(PyObject* value);
};

};
