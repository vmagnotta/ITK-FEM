/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOrientedImage2DTest.cxx,v $
  Language:  C++
  Date:      $Date: 2007-12-20 18:37:03 $
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

#include "itkImageFileReader.h"
#include "itkOrientedImage.h"
#include "itkCentralDifferenceImageFunction.h"

int itkOrientedImage2DTest( int ac, char * av[] )
{

  if( ac < 12 )
    {
    std::cerr << "Usage: " << av[0] 
    << " InputImage  "
    << "corner1x corner1y "
    << "corner2x corner2y "
    << "corner3x corner3y "
    << "derivative1x derivative1y "
    << "derivative2x derivative2y "
    << std::endl;
    return EXIT_FAILURE;
    }
  
  const unsigned int Dimension = 2;
  typedef unsigned char PixelType;

  typedef itk::OrientedImage<PixelType, Dimension>    ImageType;
  typedef itk::ImageFileReader< ImageType >           ReaderType;

  typedef ImageType::IndexType                        IndexType;
  typedef ImageType::PointType                        PointType;
  typedef IndexType::IndexValueType                   IndexValueType;

  ReaderType::Pointer reader = ReaderType::New();
  
  reader->SetFileName( av[1] );

  try
    {
    reader->Update();
    }
  catch (itk::ExceptionObject & e)
    {
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
    }
  
  ImageType::ConstPointer image = reader->GetOutput();
   
  ImageType::DirectionType directionCosines = image->GetDirection();

  std::cout << directionCosines << std::endl;

  unsigned int element = 2;
  const double tolerance = 1e-3;

  ImageType::RegionType region = image->GetLargestPossibleRegion();
  ImageType::SizeType   size   = region.GetSize();

  const int numberOfPointsToTest = 3;

  IndexType index[numberOfPointsToTest];
  PointType physicalPoint;

  index[0][0] = 0;
  index[0][1] = 0;

  index[1][0] = size[0];
  index[1][1] = 0;

  index[2][0] = 0;
  index[2][1] = size[1];

  image->Print( std::cout );
  std::cout << std::endl;
  std::cout << std::endl;

  for( int pointId=0; pointId < numberOfPointsToTest; ++pointId )
    {

    image->TransformIndexToPhysicalPoint( index[pointId], physicalPoint );

    std::cout << index[pointId] << " : " << physicalPoint << std::endl;

    for( unsigned int dim=0; dim < Dimension; ++dim )
      {
      const double expectedValue = atof( av[ element++ ] );
      const double currentValue = physicalPoint[dim];
      const double difference = currentValue - expectedValue;
      if( vnl_math_abs( difference ) > tolerance )
        {
        std::cerr << "Error: " << std::endl;
        std::cerr << "in Point # " << pointId << std::endl;
        std::cerr << "Expected      = " << expectedValue << std::endl;
        std::cerr << "Read          = " << currentValue << std::endl;
        std::cerr << "Index         = " << index[pointId] << std::endl;
        std::cerr << "PhysicalPoint = " << physicalPoint << std::endl;
        return EXIT_FAILURE;
        }
      }
    }

  //
  // Select a point in the middle of the image and compute its
  // derivative using the image orientation.
  //
  IndexType centralIndex;
  centralIndex[0] = static_cast< IndexValueType >( size[0] / 2.0 );
  centralIndex[1] = static_cast< IndexValueType >( size[1] / 2.0 );

  typedef itk::CentralDifferenceImageFunction< ImageType, double >   CentralDifferenceImageFunctionType;

  CentralDifferenceImageFunctionType::Pointer gradientHelper1 = CentralDifferenceImageFunctionType::New();
  gradientHelper1->SetInputImage( image );

  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "Image Direction" << std::endl;
  std::cout << image->GetDirection() << std::endl;

  { // Compute gradient value without taking image direction into account
  gradientHelper1->UseImageDirectionOff();
  CentralDifferenceImageFunctionType::OutputType gradient1a = gradientHelper1->EvaluateAtIndex( centralIndex ); 

  std::cout << "Gradient without Direction" << std::endl;
  std::cout << gradient1a << std::endl;

  for( unsigned int dim=0; dim < Dimension; ++dim )
    {
    const double expectedValue = atof( av[ element++ ] );
    const double currentValue = gradient1a[dim];
    const double difference = currentValue - expectedValue;
    if( vnl_math_abs( difference ) > tolerance )
      {
      std::cerr << "Error: " << std::endl;

      std::cerr << "Expected      = " << expectedValue << std::endl;
      std::cerr << "Read          = " << currentValue << std::endl;
      return EXIT_FAILURE;
      }
    }
  }


  { // Compute gradient value taking image direction into account
  gradientHelper1->UseImageDirectionOn();
  CentralDifferenceImageFunctionType::OutputType gradient1b = gradientHelper1->EvaluateAtIndex( centralIndex ); 

  std::cout << std::endl;
  std::cout << "Gradient with Direction" << std::endl;
  std::cout << gradient1b << std::endl;

  for( unsigned int dim=0; dim < Dimension; ++dim )
    {
    const double expectedValue = atof( av[ element++ ] );
    const double currentValue = gradient1b[dim];
    const double difference = currentValue - expectedValue;
    if( vnl_math_abs( difference ) > tolerance )
      {
      std::cerr << "Error: " << std::endl;
      std::cerr << "Expected      = " << expectedValue << std::endl;
      std::cerr << "Read          = " << currentValue << std::endl;
      return EXIT_FAILURE;
      }
    }
  }

  return EXIT_SUCCESS;
}