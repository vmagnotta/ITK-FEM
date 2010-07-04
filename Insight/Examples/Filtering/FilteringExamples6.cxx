/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: FilteringExamples6.cxx,v $
  Language:  C++
  Date:      $Date: 2009-03-16 21:52:49 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
// this file defines the FilterExamples for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

#include "itkConfigure.h"
#include "itkTestMain.h" 


void RegisterTests()
{
  REGISTER_TEST(FFTDirectInverseTest);
#if defined(USE_FFTWF)
  REGISTER_TEST(FFTDirectInverse2Test);
#endif
}
#undef main
#define main FFTDirectInverseTest
#include "FFTDirectInverse.cxx"

#if defined(USE_FFTWF)
#undef main
#define main FFTDirectInverse2Test
#include "FFTDirectInverse2.cxx"
#endif
