#pragma once
#include <IObject.h>
#include <PythonNativeRef.h>

namespace python {

class Tuple : public IObject
{
public:
    Tuple(std::size_t n);

    std::size_t GetSize() const;
    
    std::unique_ptr<IObject> GetItem(const std::size_t& index) const;
    
    void SetItem(const std::size_t& index, const IObject& object);

    PyObject* GetPy() const override;
private:
    PythonNativeRef_ptr m_pyObject;
};
};
