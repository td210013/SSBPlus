//******************************************************************************
// Copyright ?1988-2019
// NCR Self Serve Checkout (SSCO)
// All Rights Reserved
//******************************************************************************
// @doc
//
// @module     Template.h | Module description
//
// @comm
//
// @devnote
//
// author      tdonnan
//
// @date       01/01/2017
//
// @end
//
//******************************************************************************
// $HeadURL$
//
// $Rev::                            $:  Revision of last commit
// $Author::                         $:  Author of last commit
// $Date::                           $:  Date of last commit
//
//******************************************************************************
#include "StdAfx.h"
#include "Template.h"

// #include <string.h>     // system
// #include <example.h>    // local

//***************************************************************
//
// Module Level Definitions and Constants
//
//***************************************************************

//***************************************************************
//
// Module Level Static Data
//
//***************************************************************

//***************************************************************
//
// @func    constructor
//
// @rdesc   None
//
// @devnote Developer only notes.
//
//***************************************************************

CTemplate::CTemplate() :
   m_nMemberVar(4)
{
}

//***************************************************************
//
// @func    destructor
//
// @rdesc   None
//
// @devnote Developer only notes.
//
//***************************************************************

CTemplate::~CTemplate()
{
}


//***************************************************************
//
// @func    copy constructor
//
// @rdesc   None
//
// @devnote Developer only notes.
//
//***************************************************************
CTemplate::CTemplate(const CTemplate& a_oRhs)
{
    *this = a_oRhs;
}

//***************************************************************
//
// @func    copy constructor
//
// @rdesc   None
//
// @devnote Developer only notes.
//
//***************************************************************
CTemplate & CTemplate::operator = (const CTemplate & a_oRhs)
{
    m_nMemberVar = a_oRhs.m_nMemberVar;

    return *this;
}


//***************************************************************
//
// @func    IsEqual - returns true if this is equal to rhs
//
// @rdesc   None
//
// @devnote Developer only notes.
//
//***************************************************************
bool CTemplate::IsEqual( const CTemplate & a_oRhs )
{
    bool bIsEqual = false;

    if ( m_nMemberVar == a_oRhs.m_nMemberVar )
    {
        bIsEqual = true;
    }

    return bIsEqual;
}

//***************************************************************
//
// @func    Function/method description
//
// @rdesc   Description of returned value
// @flag    value | description
//
// @comm    Detailed comments on function/method.
//
// @devnote Developer only notes.
//
//***************************************************************

int Function(
   int iinputValue1,    // @parm Input value description
   int iinputValue2)    // @parm Input value description
{
   return TRUE;
}
// DGH 2/1/2001 T#7768 - POS DB Phase I


static char FAR * RCSID_TEMPLATE_CPP = "$Header: http://subversion.sweng.ncr.com/svn/repos/pa/etrans/trunk/common/MISC/Template.cpp 13514 2013-11-16 02:43:36Z sb210046 $";
