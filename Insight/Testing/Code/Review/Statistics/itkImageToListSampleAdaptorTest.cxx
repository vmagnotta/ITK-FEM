/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageToListSampleAdaptorTest.cxx,v $
  Language:  C++
  Date:      $Date: 2009-05-02 05:44:02 $
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
#include "itkImageToListSampleAdaptor.h"
#include "itkRandomImageSource.h"
#include "itkImageRegionIterator.h"
#include "itkScalarToArrayCastImageFilter.h"
#include "itkFixedArray.h"

int itkImageToListSampleAdaptorTest(int, char* [] ) 
{
  std::cout << "ImageToListSampleAdaptor Test \n \n"; 

  std::string whereFail = "";

  typedef itk::Image< float, 3 > FloatImage;

  // Now generate a random image
  typedef itk::RandomImageSource<FloatImage> SourceType;
  SourceType::Pointer source = SourceType::New();
  unsigned long size[3] = {17, 8, 20};
  unsigned long totalSize = size[0] * size[1] * size[2];

  source->SetSize(size);
  float minValue = -100.0;
  float maxValue = 1000.0;

  source->SetMin( static_cast< FloatImage::PixelType >( minValue ) );
  source->SetMax( static_cast< FloatImage::PixelType >( maxValue ) );
  source->Update();

  // creates a sample
  typedef  itk::Statistics::ImageToListSampleAdaptor< FloatImage >
    ImageToListSampleAdaptorType;

  ImageToListSampleAdaptorType::Pointer sample = ImageToListSampleAdaptorType::New();

  //Test if the methods throw exceptions if invoked before setting the image
  try
    {
    // purposely call Size() method prematurely in order to trigger an exception.
    sample->Size();
    std::cerr << "Exception should have been thrown since the input image \
                  is not set yet" << std::endl;
    }
  catch ( itk::ExceptionObject & excp )
    {
    std::cerr << "Caught expected exception: " << excp << std::endl;
    }
  try
    {
    // purposely call GetTotalFrequency() method prematurely in order to trigger an exception.
    sample->GetTotalFrequency();
    std::cerr << "Exception should have been thrown since the input image \
                  is not set yet" << std::endl;
    }
  catch ( itk::ExceptionObject & excp )
    {
    std::cerr << "Caught expected exception: " << excp << std::endl;
    }
 
  try
    {
    ImageToListSampleAdaptorType::MeasurementVectorType m = sample->GetMeasurementVector( 0 );
    std::cerr << "Exception should have been thrown since the input image \
                  is not set yet" << std::endl;
    }
  catch ( itk::ExceptionObject & excp )
    {
    std::cerr << "Caught expected exception: " << excp << std::endl;
    }
 
  try
    {
    ImageToListSampleAdaptorType::ImageConstPointer image = sample->GetImage( );
    std::cerr << "Exception should have been thrown since the input image \
                  is not set yet" << std::endl;
    }
  catch ( itk::ExceptionObject & excp )
    {
    std::cerr << "Caught expected exception: " << excp << std::endl;
    }
 
  try
    {
    // purposely call GetFrequency() method prematurely in order to trigger an exception.
    sample->GetFrequency(0 );
    std::cerr << "Exception should have been thrown since the input image \
                  is not set yet" << std::endl;
    }
  catch ( itk::ExceptionObject & excp )
    {
    std::cerr << "Caught expected exception: " << excp << std::endl;
    }
 

  sample->SetImage(source->GetOutput());

  // tests begin

  //check size
  if (totalSize != sample->Size())
    {
    std::cerr << "Size() is not returning the correct size"<< std::endl;
    return EXIT_FAILURE;
    }

  //check frequency
  if (totalSize != sample->GetTotalFrequency())
    {
    std::cerr << "GetTotalFrequency() is not returning the correct frequency"<< std::endl;
    return EXIT_FAILURE;
    }


  sample->Print( std::cout );

  FloatImage::IndexType index;
  FloatImage::PixelType pixel;

  ImageToListSampleAdaptorType::InstanceIdentifier iid;

  for ( unsigned int i=0; i < size[2]; i++ )
    {
    for ( unsigned int j=0; j < size[1]; j++ )
      {
      for ( unsigned int k=0; k < size[0]; k++ )  
        {
        index[0]=k;
        index[1]=j;
        index[2]=i;

        pixel = sample->GetImage()->GetPixel( index );
        iid = sample->GetImage()->ComputeOffset( index );
        if ( sample->GetMeasurementVector(iid)[0] != pixel )
          {
          std::cerr << "Error in pixel value accessed using the adaptor" << std::endl;  
          return EXIT_FAILURE;
          }
        }
      }
    }

  // iterator tests
  //
  std::cerr << "Iterators..." << std::endl;
    {
    // forward iterator
    typedef ImageToListSampleAdaptorType::Iterator IteratorType;
    
    IteratorType s_iter = sample->Begin();
    
    // copy constructor
    IteratorType bs_iter(s_iter);
    if (bs_iter != s_iter)
      {
      std::cerr << "Iterator::Copy Constructor failed" << std::endl;
      return EXIT_FAILURE;
      }
    
    // assignment operator 
    IteratorType assignment_iter( bs_iter );
    assignment_iter = s_iter;
    if (assignment_iter != s_iter)
      {
      std::cerr << "Iterator::assignment operator failed" << std::endl;
      return EXIT_FAILURE;
      }

    ImageToListSampleAdaptorType::InstanceIdentifier iid2 = 0;
    while (s_iter != sample->End())
      {
      if (sample->GetMeasurementVector(iid2) != 
          s_iter.GetMeasurementVector())
        {
        std::cerr << "Iterator::GetMeasurementVector (forward) failed" 
                  << std::endl;
        return EXIT_FAILURE;
        }
      if (iid2 != s_iter.GetInstanceIdentifier())
        {
        std::cerr << "Iterator::GetInstanceIdentifier (forward) failed" 
                  << std::endl;
        return EXIT_FAILURE;
        }
      if (s_iter.GetFrequency() != 1)
        {
        std::cerr << "Iterator::GetFrequency (forward) failed" << std::endl;
        return EXIT_FAILURE;
        }
      if (sample->GetFrequency(iid2) != 1)
        {
        std::cerr << "GetFrequency (forward) failed" << std::endl;
        return EXIT_FAILURE;
        }
      ++iid2;
      ++s_iter;
      }
    
    if (s_iter != sample->End())
      {
      std::cerr << "Iterator::End (forward) failed" << std::endl;
      return EXIT_FAILURE;
      }
    
    }

  //Test the iterators
  std::cerr << "Const Iterators..." << std::endl;
    {
    // forward iterator
    typedef ImageToListSampleAdaptorType::ConstIterator  ConstIteratorType;
    
    ConstIteratorType s_iter = sample->Begin();
    
    // copy constructor
    ConstIteratorType bs_iter(s_iter);
    if (bs_iter != s_iter)
      {
      std::cerr << "Iterator::Copy Constructor (from const) failed" 
                << std::endl;
      return EXIT_FAILURE;
      }

    // assignment operator
    ConstIteratorType assignment_iter( bs_iter );
    assignment_iter = s_iter;
    if (assignment_iter != s_iter)
      {
      std::cerr << "Const Iterator::operator= () failed" 
                << std::endl;
      return EXIT_FAILURE;
      }

    // copy from non-const iterator
    ImageToListSampleAdaptorType::Iterator nonconst_iter = sample->Begin();
    ImageToListSampleAdaptorType::ConstIterator s2_iter(nonconst_iter);
    if (s2_iter != s_iter)
      {
      std::cerr << "Iterator::Copy Constructor (from non-const) failed" 
                << std::endl;
      return EXIT_FAILURE;
      }
    // assignment from non-const iterator
    s2_iter = nonconst_iter;
    if (s2_iter != s_iter)
      {
      std::cerr << "Iterator::assignment (from non-const) failed" << std::endl;
      return EXIT_FAILURE;
      }
    
    ImageToListSampleAdaptorType::InstanceIdentifier iid3 = 0;
    while (s_iter != sample->End())
      {
      if (sample->GetMeasurementVector(iid3) != 
          s_iter.GetMeasurementVector())
        {
        std::cerr << "Iterator::GetMeasurementVector (forward) failed" 
                  << std::endl;
        return EXIT_FAILURE;
        }
      if (iid3 != s_iter.GetInstanceIdentifier())
        {
        std::cerr << "Iterator::GetInstanceIdentifier (forward) failed" 
                  << std::endl;
        return EXIT_FAILURE;
        }
      if (s_iter.GetFrequency() != 1)
        {
        std::cerr << "Iterator::GetFrequency (forward) failed" << std::endl;
        return EXIT_FAILURE;
        }
      ++iid3;
      ++s_iter;
      }
    
    if (s_iter != sample->End())
      {
      std::cerr << "Iterator::End (forward) failed" << std::endl;
      return EXIT_FAILURE;
      }
    
    }


  //Test the Use_PixelContainer boolean

  sample->SetUsePixelContainer( false );
  if ( sample->GetUsePixelContainer() != false )
    {
    std::cerr << "Error in Set/Get UsePixelContainer methods" << std::endl;
    return EXIT_FAILURE;
    }

  sample->UsePixelContainerOn(  );
  if ( sample->GetUsePixelContainer() != true )
    {
    std::cerr << "Error in Set/Get UsePixelContainerOn method" << std::endl;
    return EXIT_FAILURE;
    }

  //Get measurement vector from the pixel container and using ComputeIndex and compare
  //the result
  ImageToListSampleAdaptorType::MeasurementVectorType  v1 = sample->GetMeasurementVector( 4 );
  sample->UsePixelContainerOff();
  ImageToListSampleAdaptorType::MeasurementVectorType  v2 = sample->GetMeasurementVector( 4 );

  if ( v1[0] != v2[0] )
    {
    std::cerr << "Accessing the measurement vector using the two method produced different \
                  result " << std::endl; 
    return EXIT_FAILURE;
    }

  std::cout << "Test passed." << std::endl;
  return EXIT_SUCCESS;
}
