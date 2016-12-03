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

#include <sstream>
#include <iostream>
#include <memory>
#include "python.h"

#include "exception.hpp"
#include "PyCast.h"
#include "IObject.h"
#include "Module.h"
#include "Function.h"
#include "Value.h"

namespace python {

/* --- CONSTRUCTORS --------------------------------------------------- */

Exception::Exception( const char* pExceptionMsg )
    : runtime_error( pExceptionMsg )
{
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

    Exception::Exception( const std::string& errorMsg , const std::string& excType , const std::string& excValue , const std::string& excTraceback )
    : runtime_error( errorMsg )
    , mExcType(excType) , mExcValue(excValue) , mExcTraceback(excTraceback)
{
}

/* -------------------------------------------------------------------- */

void
Exception::translateException()
{
    //PyErr_Print();
    // get the Python error details
    std::string excType , excValue , excTraceback ;
    PyObject *pExcType , *pExcValue , *pExcTraceback ;
    PyErr_Fetch( &pExcType , &pExcValue , &pExcTraceback ) ;
    if ( pExcType != NULL )
    {
        
        Module obj(pExcType) ;
        std::unique_ptr<Function> attrObj = obj.GetAttr( "__name__" ) ;
        excType = attrObj->ToString() ;
    }
    if ( pExcValue != NULL )
    {
        std::unique_ptr<IObject> obj = PyCast<IObject>::Cast(pExcValue);
        excValue = obj->ToString() ;
    }
    if ( pExcTraceback != NULL )
    {
        PyObject * sysModule = PyImport_AddModule ("sys");
        PyObject * handle = PyObject_GetAttrString (sysModule, "stderr");
        
        std::cout << PyTraceBack_Print(pExcTraceback,handle)<<"\n";
        //todo
    }
    PyErr_Restore(pExcType, pExcValue, pExcTraceback);
    
    // translate the error into a C++ exception
    std::stringstream buf ;
    buf << (excType.empty() ? "???" : excType) ;
    if ( ! excValue.empty() )
        buf << ": " << excValue ;
    throw Exception( buf.str() , excType , excValue , excTraceback ) ;
}

/* -------------------------------------------------------------------- */

} // namespace python
