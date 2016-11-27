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
#include <memory>
#include "python.h"

#include "exception.hpp"
#include "object.hpp"

using namespace std ;

namespace python {

/* --- CONSTRUCTORS --------------------------------------------------- */

Exception::Exception( const char* pExceptionMsg )
    : runtime_error( pExceptionMsg )
{
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

Exception::Exception( const string& errorMsg , const string& excType , const string& excValue , const string& excTraceback )
    : runtime_error( errorMsg )
    , mExcType(excType) , mExcValue(excValue) , mExcTraceback(excTraceback)
{
}

/* -------------------------------------------------------------------- */

void
Exception::translateException()
{
    // get the Python error details
    string excType , excValue , excTraceback ;
    PyObject *pExcType , *pExcValue , *pExcTraceback ;
    PyErr_Fetch( &pExcType , &pExcValue , &pExcTraceback ) ;
    if ( pExcType != NULL )
    {
        Object obj( pExcType , true ) ;
        auto_ptr<Object> attrObj( obj.getAttr( "__name__" ) ) ;
        excType = attrObj->reprVal() ;
    }
    if ( pExcValue != NULL )
    {
        Object obj( pExcValue , true ) ;
        excValue = obj.reprVal() ;
    }
    if ( pExcTraceback != NULL )
    {
        Object obj( pExcTraceback , true ) ;
        excTraceback = obj.reprVal() ;
    }

    // translate the error into a C++ exception
    stringstream buf ;
    buf << (excType.empty() ? "???" : excType) ;
    if ( ! excValue.empty() )
        buf << ": " << excValue ;
    throw Exception( buf.str() , excType , excValue , excTraceback ) ;
}

/* -------------------------------------------------------------------- */

} // namespace python
