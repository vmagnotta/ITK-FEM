/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: RegistrationExamples13.cxx,v $
  Language:  C++
  Date:      $Date: 2008-03-30 03:14:05 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// this file defines the RegistrationExamples for the test driver
// and all it expects is that you have a function called RegisterTests

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

#include <iostream>
#include "vnl/vnl_sample.h"
#include "itkTestMain.h" 


void RegisterTests()
{
  REGISTER_TEST(DeformableRegistration12Test);
  REGISTER_TEST(DeformableRegistration13Test);
}

#undef main
#define main  DeformableRegistration12Test
#undef CommandIterationUpdate
#define CommandIterationUpdate CommandIterationUpdate13
#include "DeformableRegistration12.cxx"

#undef main
#define main  DeformableRegistration13Test
#undef CommandIterationUpdate
#define CommandIterationUpdate CommandIterationUpdate14
#include "DeformableRegistration13.cxx"

