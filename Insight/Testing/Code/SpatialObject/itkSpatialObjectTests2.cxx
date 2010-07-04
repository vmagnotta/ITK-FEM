/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSpatialObjectTests2.cxx,v $
  Language:  C++
  Date:      $Date: 2008-05-13 16:50:02 $
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
  REGISTER_TEST(itkPlaneSpatialObjectTest);
  REGISTER_TEST(itkPolygonGroupSpatialObjectTest);
  REGISTER_TEST(itkPolygonSpatialObjectTest);
  REGISTER_TEST(itkSceneSpatialObjectTest);
  REGISTER_TEST(itkSpatialObjectTreeContainerTest);
  REGISTER_TEST(itkSurfaceSpatialObjectTest);
  REGISTER_TEST(itkTubeSpatialObjectTest);
}
