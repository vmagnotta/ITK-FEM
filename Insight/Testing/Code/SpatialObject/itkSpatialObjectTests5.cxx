/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSpatialObjectTests5.cxx,v $
  Language:  C++
  Date:      $Date: 2010-06-10 12:38:35 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "itkTestMain.h" 


void RegisterTests()
{
  REGISTER_TEST(itkContourSpatialObjectTest);
  REGISTER_TEST(itkCylinderSpatialObjectTest);
  REGISTER_TEST(itkEllipseSpatialObjectTest);
  REGISTER_TEST(itkGaussianSpatialObjectTest);
  REGISTER_TEST(itkMetaArrowConverterTest);
}
