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

 
#ifndef PYTHON_EXCEPTION_HPP
#define PYTHON_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace python {

/* --- CLASS DEFINITION: Exception ------------------------------------ */
    
// This class represents a Python error.
// If a Python API call returns an error, we translate it into a C++ exception.

class Exception : public std::runtime_error
{
public: // constructors/destructor
    Exception( const char* pExceptionMsg ) ;
protected: // constructors
    Exception( const std::string& errorMsg , 
               const std::string& excType , const std::string& excValue , const std::string& excTraceback 
             ) ;
public: // miscellaneous methods
    static void translateException() ;
private: // data members:
    std::string mExcType ;
    std::string mExcValue ;
    std::string mExcTraceback ;
} ;

/* -------------------------------------------------------------------- */

} // namespace python

#endif // PYTHON_EXCEPTION_HPP
