/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPhasedArray3DSpecialCoordinatesImageTest.cxx,v $
  Language:  C++
  Date:      $Date: 2004-10-26 13:24:45 $
  Version:   $Revision: 1.1 $

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
#include "itkPhasedArray3DSpecialCoordinatesImage.h"
#include "vnl/vnl_math.h"

int itkPhasedArray3DSpecialCoordinatesImageTest(int, char* [] )
{
  bool passed = true;

  typedef itk::PhasedArray3DSpecialCoordinatesImage<float> Image;
  typedef Image::RegionType RegionType;
  typedef Image::SizeType SizeType;
  typedef Image::PointType PointType;
  typedef Image::IndexType IndexType;
  typedef itk::ContinuousIndex<double,3> ContinuousIndexType;
  
  Image::Pointer image = Image::New();
  //image->DebugOn();
  //image->GetSource();
  SizeType size;
  size.Fill(5); // 5x5x5 sampling of data
  RegionType region;
  region.SetSize(size);
  image->SetRegions(region);
  image->DisconnectPipeline();
  
  /**  Set the number of radians between each azimuth unit.   **/
  image->SetAzimuthAngularSeparation( 15.0*2.0*vnl_math::pi/360.0 );
  
  /**  Set the number of radians between each elevation unit.   **/
  image->SetElevationAngularSeparation( 15.0*2.0*vnl_math::pi/360.0 );
  
  /**  Set the number of cartesian units between each unit along the R .  **/
  image->SetRadiusSampleSize( 1 );
  
  /**  Set the distance to add to the radius. */
  image->SetFirstSampleDistance( 2 );
  
  image->Print(std::cout);
  std::cout<<std::endl;
  
  
  PointType point;
  IndexType index;
  ContinuousIndexType continuousIndex;
  
  point.Fill(0.05); point[2]=3.1;
  image->TransformPhysicalPointToIndex(point, index);
  std::cout<<"Point "<<point<<" -> Index "<<index<<std::endl;
  image->TransformPhysicalPointToContinuousIndex(point, continuousIndex);
  std::cout<<"Point "<<point<<" -> Continuous Index "<<continuousIndex<<std::endl;
  
  if(     index[0] != 2
      ||  index[1] != 2
      ||  index[2] != 1 )
  {
    passed = false;
  }
  
  index.Fill(1);
  image->TransformIndexToPhysicalPoint(index, point);
  std::cout<<"Index "<<index<<" -> Point "<<point<<std::endl;
  
  continuousIndex.Fill(3.5);
  image->TransformContinuousIndexToPhysicalPoint(continuousIndex, point);
  std::cout<<"Continuous Index "<<continuousIndex<<" -> Point "<<point<<std::endl;
  
  continuousIndex.Fill(2.0);
  image->TransformContinuousIndexToPhysicalPoint(continuousIndex, point);
  std::cout<<"Continuous Index "<<continuousIndex<<" -> Point "<<point<<std::endl;
  
  if(     point[0] < -0.001 || point[0] > 0.001
      ||  point[1] < -0.001 || point[1] > 0.001
      ||  point[2] < -4.001 || point[2] > 4.001 )
  {
    passed = false;
  }
  
  std::cout<<std::endl;
  
  if (passed)
  {
    std::cout << "PhasedArray3DSpecialCoordinatesImage tests passed" << std::endl;
    return EXIT_SUCCESS;
  }
  else
  {
    std::cout << "PhasedArray3DSpecialCoordinatesImage tests failed" << std::endl;
    return EXIT_FAILURE;
  }
}
