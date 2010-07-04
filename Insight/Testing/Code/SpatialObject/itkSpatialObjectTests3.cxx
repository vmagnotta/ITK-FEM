/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSpatialObjectTests3.cxx,v $
  Language:  C++
  Date:      $Date: 2005-08-08 14:24:01 $
  Version:   $Revision: 1.5 $

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
  REGISTER_TEST(itkLandmarkSpatialObjectTest);
  REGISTER_TEST(itkLineSpatialObjectTest);
  REGISTER_TEST(itkMeshSpatialObjectTest);
  REGISTER_TEST(itkSpatialObjectPrintTest);
  REGISTER_TEST(itkBoxSpatialObjectTest);
  REGISTER_TEST(itkSpatialObjectDuplicatorTest);
}
