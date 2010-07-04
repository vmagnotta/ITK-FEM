/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: IOExamples.cxx,v $
  Language:  C++
  Date:      $Date: 2009-06-25 14:06:15 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// this file defines the IOExamples for the test driver
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
  vnl_sample_reseed(8775070);
  REGISTER_TEST(VisibleHumanStreamReadWriteTest);
  REGISTER_TEST(VisibleHumanPasteWriteTest);
}

#undef main
#define main  VisibleHumanStreamReadWriteTest
#include "VisibleHumanStreamReadWrite.cxx"

#undef main
#define main  VisibleHumanPasteWriteTest
#include "VisibleHumanPasteWrite.cxx"
