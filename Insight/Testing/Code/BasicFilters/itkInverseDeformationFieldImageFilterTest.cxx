/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkInverseDeformationFieldImageFilterTest.cxx,v $
  Language:  C++
  Date:      $Date: 2007-08-10 14:34:02 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkImage.h"
#include "itkVector.h"
#include "itkInverseDeformationFieldImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageFileWriter.h"
#include "itkFilterWatcher.h"


int itkInverseDeformationFieldImageFilterTest( int argc, char * argv[] )
{

  if( argc < 2 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " outputImage" << std::endl;
    return EXIT_FAILURE;
    }

  const     unsigned int   Dimension = 2;
  typedef   float          VectorComponentType;

  typedef   itk::Vector< VectorComponentType, Dimension >    VectorType;

  typedef itk::Image< VectorType,  Dimension >   DeformationFieldType;

  typedef itk::InverseDeformationFieldImageFilter<
                                    DeformationFieldType, 
                                    DeformationFieldType 
                                             >  FilterType;

  FilterType::Pointer filter = FilterType::New();

  FilterWatcher watcher(filter);

  // Creating an input deformation field
  DeformationFieldType::Pointer field = DeformationFieldType::New();

  DeformationFieldType::SpacingType spacing;
  spacing.Fill( 1.0 );

  DeformationFieldType::PointType origin;
  origin.Fill( 0.0 );

  DeformationFieldType::RegionType     region;
  DeformationFieldType::SizeType       size;
  DeformationFieldType::IndexType      start;

  size[0] = 128;
  size[1] = 128;

  start[0] = 0;
  start[1] = 0;

  region.SetSize( size );
  region.SetIndex( start );


  field->SetOrigin( origin );
  field->SetSpacing( spacing );
  field->SetRegions( region );
  field->Allocate();

  VectorType pixelValue;

  itk::ImageRegionIteratorWithIndex< DeformationFieldType > it( field, region );

  // Fill the field with some vectors
  it.GoToBegin();
  while( !it.IsAtEnd() )
    {
    DeformationFieldType::IndexType index = it.GetIndex();
    pixelValue[0] = index[0] * 2.0;
    pixelValue[1] = index[1] * 2.0;
    it.Set( pixelValue );
    ++it;
    }



  // Use the same geometry for the inverse field.
  // This is for simplicity here, in general a 
  // different geometry should be used.
  filter->SetOutputSpacing( spacing );


  // keep the origin
  filter->SetOutputOrigin( origin );

  // set the size
  filter->SetSize( size );


  filter->SetInput( field ); 


  filter->SetSubsamplingFactor( 16 );

  try
    {
    filter->UpdateLargestPossibleRegion();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception thrown " << std::endl;
    std::cerr << excp << std::endl;
    }

  // Write an image for regression testing
  typedef itk::ImageFileWriter<  DeformationFieldType  > WriterType;

  WriterType::Pointer writer = WriterType::New();

  writer->SetInput (filter->GetOutput());
  writer->SetFileName( argv[1] );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception thrown by writer" << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }
  
  return EXIT_SUCCESS;

}

