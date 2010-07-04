/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: RegistrationExamples11.cxx,v $
  Language:  C++
  Date:      $Date: 2008-03-16 22:38:20 $
  Version:   $Revision: 1.3 $

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
#include "itkTestMain.h" 


void RegisterTests()
{
  REGISTER_TEST(ImageRegistration8Test);
  REGISTER_TEST(ImageRegistration9Test);
  REGISTER_TEST(DeformableRegistration4Test);
}

#undef main
#define main  ImageRegistration8Test
#undef CommandIterationUpdate
#define CommandIterationUpdate CommandIterationUpdate08
#include "ImageRegistration8.cxx"

#undef main
#define main  ImageRegistration9Test
#undef CommandIterationUpdate
#define CommandIterationUpdate CommandIterationUpdate09
#include "ImageRegistration9.cxx"

#undef main
#define main  DeformableRegistration4Test
#undef CommandIterationUpdate
#define CommandIterationUpdate CommandIterationUpdate10
#include "DeformableRegistration4.cxx"

