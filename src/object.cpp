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
 
#include "python.h"

#include "object.hpp"
#include "exception.hpp"
#include <iostream>

using namespace std ;

namespace python {

/* --- CONSTRUCTORS --------------------------------------------------- */

Object::Object( PyObject* pPyObject , bool decRef )
    : mpPyObject(pPyObject) , mDecRef(decRef)
{
    std::cout <<"Object( PyObject* )"<< mpPyObject->ob_type->tp_name<<"\n";
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

Object::Object( const int val )
{
    // create a Python int object 
    mpPyObject = PyLong_FromLong( val ) ;
    assert( mpPyObject != NULL ) ;
    mDecRef = true ;
    std::cout <<"Object( const int val )"<< mpPyObject->ob_type->tp_name<<"\n";
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

Object::Object( const char* pVal )
{
    // create a Python string object 
    mpPyObject = PyUnicode_FromString( pVal ) ;
    assert( mpPyObject != NULL ) ;
    mDecRef = true ;
    std::cout <<"Object( const char* val )"<< mpPyObject->ob_type->tp_name<<"\n";
}

/* --- DESTRUCTOR ----------------------------------------------------- */

Object::~Object()
{
    // clean up
    if ( mDecRef && mpPyObject != nullptr ){
        Py_DecRef( mpPyObject ) ;
        mpPyObject = nullptr;
    }
}

/* -------------------------------------------------------------------- */

void 
Object::setTupleItem( size_t pos , Object& obj )
{
    // set the tuple item
    int rc = PyTuple_SetItem( pPyObject() , pos , obj.pPyObject() ) ;
    if ( rc != 0 )
        Exception::translateException() ;
    // NOTE: The tuple takes ownership of the object, so it doesn't need to decref itself.
    obj.mDecRef = false ;
}

/* -------------------------------------------------------------------- */

Object*
Object::callObj( Object& args , Object& kywdArgs ) 
{
    // call the object
    PyObject* pResultObj = PyObject_Call( pPyObject() , args.pPyObject() , kywdArgs.pPyObject() ) ;
    if ( pResultObj == NULL )
        Exception::translateException() ;
    return new Object( pResultObj , true ) ; 
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

Object* Object::callObj() { return callObj( *newTuple(0) , *newDict() ) ; }
Object* Object::callObj( Object& a ) { return callObj( a , *newDict() ) ; }

/* -------------------------------------------------------------------- */

Object*
Object::getAttr( const char* pAttrName ) const
{ 
    // return the specified attribute
    PyObject* pAttrObj = PyObject_GetAttrString( pPyObject() , pAttrName ) ;
    if ( pAttrObj == NULL )
        Exception::translateException() ;
    return new Object( pAttrObj , true ) ; 
}

/* -------------------------------------------------------------------- */

Object*
Object::compileString( const char* pCodeBuf )
{
    // compile the string 
    PyObject* pCodeObj = Py_CompileString( pCodeBuf , "" , Py_file_input ) ;
    if ( pCodeObj == NULL )
        Exception::translateException() ;
    return new Object( pCodeObj , true ) ; 
}

/* -------------------------------------------------------------------- */

Object*
Object::execCodeModule( const char* pModuleName , Object& codeObj ) 
{
    // execute the code module 
    PyObject* pModuleObj = PyImport_ExecCodeModule( (char*)pModuleName , codeObj.pPyObject() ) ;
    if ( pModuleObj == NULL )
        Exception::translateException() ;
    return new Object( pModuleObj , true ) ; 
}

/* -------------------------------------------------------------------- */

Object* Object::newTuple( size_t n ) { PyObject* p=PyTuple_New(n) ; assert(p != NULL) ; return new Object(p,true) ; }
Object* Object::newDict() { PyObject* p=PyDict_New() ; assert(p != NULL) ; return new Object(p,true) ; }

/* -------------------------------------------------------------------- */

string
Object::reprVal() const 
{
    // return the "repr" value for the Python object
    // NOTE: We have to return a std::string since PyString_AsString() returns
    //  a char* for the "repr" object, which will be destroyed when we return.
    Object repr( PyObject_Repr(mpPyObject) , true ) ;
    return PyUnicode_AsUTF8( repr.pPyObject() ) ;
}

/* -------------------------------------------------------------------- */

PyObject* Object::pPyObject() const { return mpPyObject ; }

/* -------------------------------------------------------------------- */

} // namespace python
