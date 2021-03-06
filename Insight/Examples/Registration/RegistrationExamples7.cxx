/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: RegistrationExamples7.cxx,v $
  Language:  C++
  Date:      $Date: 2006-05-14 12:16:23 $
  Version:   $Revision: 1.13 $

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

#include <iostream>
#include "vnl/vnl_sample.h"
#include "itkTestMain.h" 


void RegisterTests()
{
  vnl_sample_reseed(8775070);
  REGISTER_TEST(ImageRegistration13Test);
}

#undef main
#define main  ImageRegistration13Test
#undef CommandIterationUpdate
#define CommandIterationUpdate CommandIterationUpdate5
#include "ImageRegistration13.cxx"
