/******************************************************************************
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
//******************************************************************************/

#ifndef _Template_H
   #define _Template_H

//***************************************************************
//
// Definitions and Constants
//
//***************************************************************
#define DIM(x) (sizeof(x) / sizeof(x[0])

//***************************************************************
//
// @class Template | Layout for a C++ class
//
//***************************************************************
class CTemplate
{
   //*******************************************************************
   //
   // Life cycle
   //
   //*******************************************************************
   public:

      // @cmember,mfunc Constructor
      CTemplate();

      // @cmember,mfunc Copy cnstructor
      CTemplate(const CTemplate&);

      // @cmember,mfunc Destructor
      virtual ~CTemplate();

   //*******************************************************************
   //
   // Operators, implementation
   //
   //*******************************************************************
   public:

      // @cmember,mfunc Assignment operator
      CTemplate& operator=(const CTemplate&);

      bool IsEqual( const CTemplate& );

      // OPERATIONS
      // ACCESS
      // INQUIRY

   //*******************************************************************
   //
   // Attributes and member variables
   //
   //*******************************************************************
   public:
   // protected:

      // @cmember Member variable description
      int m_nMemberVar;

   private:

};

#endif                     // _TEMPLATE_H

