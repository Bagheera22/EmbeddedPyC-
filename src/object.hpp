/*
 * COPYRIGHT:   (c) Awasu Pty. Ltd. 2014 (all rights reserved).
 *              Unauthorized use of this code is prohibited.
 *
 * LICENSE:     This software is provided 'as-is', without any express
 *              or implied warranty.
 *
 *              In no event will the author be held liable for any damages
 *              arising from the use of this software.
 *
 *              Permission is granted to anyone to use this software
 *              for any non-commercial purpose and to alter it and
 *              redistribute it freely, subject to the following restrictions:
 *
 *              - The origin of this software must not be misrepresented;
 *                you must not claim that you wrote the original software.
 *                If you use this software, an acknowledgement is requested
 *                but not required.
 *
 *              - Altered source versions must be plainly marked as such,
 *                and must not be misrepresented as being the original software.
 *                Altered source is encouraged to be submitted back to
 *                the original author so it can be shared with the community.
 *                Please share your changes.
 *
 *              - This notice may not be removed or altered from any
 *                source distribution.
 */

#ifndef PYTHON_OBJECT_HPP
#define PYTHON_OBJECT_HPP

#include <string>

typedef struct _object PyObject ; // nb: from python.h 

namespace python {

/* --- CLASS DEFINITION Object ---------------------------------------- */

// This class wraps a PyObject* returned to us by Python.
// NOTE: It is not thread-safe!

class Object
{
public: // constructors/destructor
    Object( _object* pPyObject , bool decRef ) ;
    explicit Object( const int val ) ;
    explicit Object( const char* pVal ) ;
    virtual ~Object() ;
public: // Python methods
    Object* callObj() ;
    Object* callObj( Object& args ) ;
    Object* callObj( Object& args , Object& kywdArgs ) ;
    Object* getAttr( const char* pAttrName ) const ;
    void setTupleItem( size_t pos , Object& obj ) ;
    std::string reprVal() const ;
public: // factory methods:
    static Object* compileString( const char* pCodeBuf ) ;
    static Object* execCodeModule( const char* pModuleName , Object& codeObj ) ;
    static Object* newTuple( size_t nItems ) ;
    static Object* newDict() ;
public: // miscellaneous methods
    PyObject* pPyObject() const ;
private: // data members
    PyObject* mpPyObject ; // Python object being wrapped
    bool mDecRef ; // flags if we should decref 
} ;

/* -------------------------------------------------------------------- */

} // namespace python

#endif // PYTHON_OBJECT_HPP

