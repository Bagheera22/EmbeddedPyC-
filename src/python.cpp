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
#include <Code.h>
#include <Module.h>
#include <Tuple.h>
#include <Value.h>
#include <Dic.h>
#include <Function.h>

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
    Code pCompiledFn(buf.str());
    std::unique_ptr<Module> pModule = pCompiledFn.ExecCodeModule("test");
    
    std::unique_ptr<Function> pAddFn = pModule->GetAttr( "add" );
    
    // prepare the function parameters
    Tuple pPosArgs(2) ;
    Value<long> a(12);
    Value<long> b(15);
    pPosArgs.SetItem( 0 , a ) ;
    pPosArgs.SetItem( 1 , b ) ;
    
    // call the function
    Dic pKywdArgs;
    std::unique_ptr<IObject> pResult = pAddFn->CallObj( pPosArgs , pKywdArgs );
    cout << "The answer: " << pResult->ToString() << endl ;
    }
    closePython();
}
