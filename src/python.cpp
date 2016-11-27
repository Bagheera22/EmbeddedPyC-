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

#include <cassert>
#include "python.h"

#include "python.hpp"

namespace python {

/* --- LOCAL DATA ----------------------------------------------------- */

static bool gIsOpen = false ;
static PyThreadState* gpMainThreadState = NULL ;

/* -------------------------------------------------------------------- */

void 
openPython( bool enableThreads )
{
    assert( !gIsOpen ) ;

    // initialize Python
    Py_Initialize() ;
    gIsOpen = true ;

    // initialize threading 
    if ( enableThreads )
    {
        PyEval_InitThreads() ; // nb: creates and locks the GIL

        // NOTE: We save the current thread state, and restore it when we unload,
        // so that we can clean up properly.
        gpMainThreadState = PyEval_SaveThread() ; // nb: this also releases the GIL
        assert( gpMainThreadState != NULL ) ;
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

void 
closePython()
{
    assert( gIsOpen ) ;

    // clean up threading
    if ( gpMainThreadState != NULL )
    {
        PyEval_RestoreThread( gpMainThreadState ) ; // nb: this also locks the GIL
        gpMainThreadState = NULL ;
    }

    // clean up Python
    Py_Finalize() ;
    gIsOpen = false ;
}

/* -------------------------------------------------------------------- */

} // namespace python

#include <iostream>
#include <sstream>
#include "object.hpp"
int main()
{
    //PyObject a;
    //a.ob_type
    using namespace python;
    using namespace std;
    openPython(false);
    {
    using namespace std;
    stringstream buf ;
    buf << "def add( n1 , n2 ) :" << endl
    << "    return n1+n2" << endl ;
    auto_ptr<Object> pCompiledFn( Object::compileString( buf.str().c_str() ) ) ;
    auto_ptr<Object> pModule( Object::execCodeModule( "test" , *pCompiledFn ) ) ;
    auto_ptr<Object> pAddFn( pModule->getAttr( "add" ) ) ;
    
    // prepare the function parameters
    auto_ptr<Object> pPosArgs( Object::newTuple( 2 ) ) ;
    Object a(2);
    Object b(15);
    //Object c("adas");
    pPosArgs->setTupleItem( 0 , a ) ;
    pPosArgs->setTupleItem( 1 , b ) ;
    
    // call the function
    auto_ptr<Object> pKywdArgs( Object::newDict() ) ;
    auto_ptr<Object> pResult( pAddFn->callObj( *pPosArgs , *pKywdArgs ) ) ;
    
    cout << "The answer: " << pResult->reprVal() << endl ;
    }
    closePython();
}
