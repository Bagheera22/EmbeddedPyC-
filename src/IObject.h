#pragma once
#include <string>
typedef struct _object PyObject;

namespace python {

class IObject
{
public:
    std::string ToString() const;
    virtual PyObject* GetPy() const = 0;
};

};
