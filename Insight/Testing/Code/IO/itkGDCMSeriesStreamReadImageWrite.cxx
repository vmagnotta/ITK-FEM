/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGDCMSeriesStreamReadImageWrite.cxx,v $
  Language:  C++
  Date:      $Date: 2009-04-05 23:09:36 $
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

#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkPipelineMonitorImageFilter.h"
#include "itkStreamingImageFilter.h"

/// \brief is comparison with a percentage tollerance
///
/// returns true of  the current value is with in tol percentage of m);
///
///  | u - v | <= e * |u| or |  u - v | <= e * |v|
/// defines a "close with tolerance e" relationship between u and v
/// this is a symetric comparison
///
/// Also it is check to see if the u an v are both less then
/// epsilon for the type
static bool IsEqualTolerant(const float lm, const float rm, double tol) {
  tol = vcl_fabs(tol);
  float temp = vcl_fabs(lm - rm);    
  return  temp <= tol*vcl_fabs(lm) ||
    temp <= tol*vcl_fabs(rm) || 
    (vcl_fabs(lm) < vcl_numeric_limits<float>::epsilon() &&
     vcl_fabs(rm) < vcl_numeric_limits<float>::epsilon());
 }

int main( int argc, char* argv[] )
{
  if( argc < 6 )
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " DicomDirectory  outputFile ";
    std::cerr << " spacingX spacingY spacingZ [ force-no-streaming 1|0]" << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::Image<unsigned short,3>            ImageType;
  typedef itk::ImageSeriesReader< ImageType >     ReaderType;
  typedef itk::GDCMImageIO                        ImageIOType;
  typedef itk::GDCMSeriesFileNames                SeriesFileNames;


  unsigned int numberOfDataPieces = 4;

  ImageType::SpacingType expectedSpacing;

  expectedSpacing[0] = atof(argv[3]);
  expectedSpacing[1] = atof(argv[4]);
  expectedSpacing[2] = atof(argv[5]);
  

  bool forceNoStreaming = true;
  if( argc > 6 )
    {
    if( atoi(argv[6]) != 1 )
      {
      forceNoStreaming = false;
      }
    }

  bool expectedToStream = !forceNoStreaming;

  ImageIOType::Pointer gdcmIO = ImageIOType::New();
  SeriesFileNames::Pointer filenameGenerator = SeriesFileNames::New();
  filenameGenerator->SetInputDirectory( argv[1] );

  ReaderType::Pointer reader = ReaderType::New();

  const ReaderType::FileNamesContainer & filenames =
    filenameGenerator->GetInputFileNames();

  reader->SetFileNames( filenames );
  reader->SetImageIO( gdcmIO );


  typedef itk::PipelineMonitorImageFilter<ImageType> MonitorFilter;
  MonitorFilter::Pointer monitor = MonitorFilter::New();
  monitor->SetInput( reader->GetOutput() );

  typedef itk::StreamingImageFilter<ImageType, ImageType> StreamingFilter;
  StreamingFilter::Pointer streamer = StreamingFilter::New();
  streamer->SetInput( monitor->GetOutput() );
  streamer->SetNumberOfStreamDivisions( numberOfDataPieces );

  try
    {
    if( forceNoStreaming )
      {
      // no streaming
      reader->UseStreamingOff();      
      streamer->Update();
      }
    else
      {
      // stream based on the number of z-slices
      reader->UseStreamingOn();
      reader->UpdateOutputInformation();
      numberOfDataPieces = reader->GetOutput()->GetLargestPossibleRegion().GetSize()[2];
      streamer->SetNumberOfStreamDivisions( numberOfDataPieces );
      streamer->Update();
      }
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    std::cerr << monitor;
    return EXIT_FAILURE;
    }

  // verify things executed as expected
  bool passed = true;
  if( expectedToStream )
    {
    if( !monitor->VerifyAllInputCanStream(numberOfDataPieces) )
      {
      passed = false;
      }
    }
  else
    {
    if( !monitor->VerifyAllIputCanNotStream() )
      {
      passed = false;
      }
    }

  if( !passed )
    {
    std::cerr << monitor << std::endl;
    std::cerr << "pipeline did not execute as expected!" << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Origin: " << reader->GetOutput()->GetOrigin() << std::endl;
  std::cout << "direction: " << reader->GetOutput()->GetDirection() << std::endl;  
  std::cout << "Spacing: " << reader->GetOutput()->GetSpacing() << std::endl;
  std::cout << "Expected Spacing: " << expectedSpacing << std::endl;


  ImageType::SpacingType spacing = reader->GetOutput()->GetSpacing();

  // we only give 4 bits of tolerence, IEEE float a 24-bit mantissa 
  const double percentTolerance = 1.0 / double ( (unsigned int)(1) << 18);  

  if (!IsEqualTolerant(spacing[0], expectedSpacing[0], percentTolerance) ||
      !IsEqualTolerant(spacing[1], expectedSpacing[1], percentTolerance) ||
      !IsEqualTolerant(spacing[2], expectedSpacing[2], percentTolerance)) 
    {
    std::cerr << "Spacing does not match expected" << std::endl;
    return EXIT_FAILURE;
    }
  

  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName( argv[2] );
  writer->SetInput( reader->GetOutput() );

  try
    {
    writer->Update();
    }
  catch (itk::ExceptionObject &excp)
    {
    std::cerr << "Exception thrown while writing the image" << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
