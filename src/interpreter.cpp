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

#include "interpreter.hpp"
#include "exception.hpp"

namespace python {

/* --- CONSTRUCTORS --------------------------------------------------- */

Interpreter::Interpreter()
{
    // create a new interpreter 
    PyEval_AcquireLock() ; // nb: get the GIL
    mpThreadState = Py_NewInterpreter() ;
    if ( mpThreadState == NULL )
        Exception::translateException() ;
    PyEval_ReleaseThread( mpThreadState ) ; // nb: this also releases the GIL
}

/* --- DESTRUCTOR ----------------------------------------------------- */

Interpreter::~Interpreter()
{
    // clean up
    PyEval_AcquireThread( mpThreadState ) ; // nb: this also locks the GIL
    Py_EndInterpreter( mpThreadState ) ;
    PyEval_ReleaseLock() ; // nb: release the GIL
}

/* -------------------------------------------------------------------- */

void
Interpreter::runSimpleString( const char* pCode )
{
    // run the string 
    int rc = PyRun_SimpleString( pCode ) ;
    if ( rc != 0 )
        Exception::translateException() ;
}

/* -------------------------------------------------------------------- */

void
Interpreter::runSimpleFile( const char* pFilename )
{
    // open the file 
    FILE* fp = fopen( pFilename , "r" );
    if ( fp == NULL )
        throw Exception( "Can't open file." ) ;

    // run the script
    int rc2 = PyRun_SimpleFile( fp , pFilename ) ;
    if ( rc2 != 0 )
        Exception::translateException() ;
}

/* -------------------------------------------------------------------- */

void 
Interpreter::lockInterpreter()
{
    // swap the interpreter in
    PyEval_AcquireThread( mpThreadState ) ;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

void 
Interpreter::unlockInterpreter()
{
    // swap the interpreter out 
    PyEval_ReleaseThread( mpThreadState ) ;
}

/* --- InterpreterLock ------------------------------------------------ */

InterpreterLock::InterpreterLock( Interpreter& i ) : mrInterpreter(i) { mrInterpreter.lockInterpreter() ; }
InterpreterLock::~InterpreterLock() { mrInterpreter.unlockInterpreter() ; }

/* -------------------------------------------------------------------- */

} // namespace python
