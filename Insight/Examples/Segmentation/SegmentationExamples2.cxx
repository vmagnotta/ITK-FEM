/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: SegmentationExamples2.cxx,v $
  Language:  C++
  Date:      $Date: 2009-03-17 21:44:43 $
  Version:   $Revision: 1.17 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
// this file defines the SegmentationExamples for the test driver
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
  REGISTER_TEST(IsolatedConnectedImageFilterTest);
  REGISTER_TEST(NeighborhoodConnectedImageFilterTest);
  REGISTER_TEST(ShapeDetectionLevelSetFilterTest);
  REGISTER_TEST(ThresholdSegmentationLevelSetImageFilterTest);
}

#undef main
#define main IsolatedConnectedImageFilterTest
#include "IsolatedConnectedImageFilter.cxx"

#undef main
#define main NeighborhoodConnectedImageFilterTest
#include "NeighborhoodConnectedImageFilter.cxx"

#undef main
#define main ShapeDetectionLevelSetFilterTest
#include "ShapeDetectionLevelSetFilter.cxx"

#undef main
#define main ThresholdSegmentationLevelSetImageFilterTest
#include "ThresholdSegmentationLevelSetImageFilter.cxx"
