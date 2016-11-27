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

#ifndef PYTHON_INTERPRETER_HPP
#define PYTHON_INTERPRETER_HPP

typedef struct _ts PyThreadState ; // nb: from python.h

namespace python {

/* --- CLASS DEFINITION Interpreter ----------------------------------- */

// This class represents a Python interpreter.
// NOTE: It is not thread-safe! Don't use across multiple threads.

class Interpreter
{
public: // constructors/destructor
    Interpreter() ;
    virtual ~Interpreter() ;
public: // Python methods:
    static void runSimpleString( const char* pCode ) ;
    static void runSimpleFile( const char* pFilename ) ;
public: // interpreter methods:
    void lockInterpreter() ;
    void unlockInterpreter() ;
private: // data members
    PyThreadState* mpThreadState ;
} ;

/* --- CLASS DEFINITION InterpreterLock ------------------------------- */

// Helper class and macros to manage locking/unlocking Interpreter's.

class InterpreterLock
{
public:
    InterpreterLock( Interpreter& interp ) ;
    ~InterpreterLock() ;
private:
    Interpreter& mrInterpreter ;
} ;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

#define BEGIN_PYTHON_INTERPRETER( interp ) \
{ \
    python::InterpreterLock _interpreterLock_( (interp) ) ;

#define END_PYTHON_INTERPRETER() \
}

/* -------------------------------------------------------------------- */

} // namespace python

#endif // PYTHON_INTERPRETER_HPP

