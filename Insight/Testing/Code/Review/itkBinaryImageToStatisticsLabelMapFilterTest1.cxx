/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryImageToStatisticsLabelMapFilterTest1.cxx,v $
  Language:  C++
  Date:      $Date: 2009-08-07 17:44:34 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkLabelObject.h"
#include "itkLabelMap.h"
#include "itkBinaryImageToStatisticsLabelMapFilter.h"
#include "itkLabelMapToLabelImageFilter.h"

#include "itkTestingMacros.h"

int itkBinaryImageToStatisticsLabelMapFilterTest1(int argc, char * argv[])
{

  if( argc != 11 )
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputBinaryImage inputGrayscaleImage outputStatisticsLabelMap";
    std::cerr << " fullyConnected(0/1) foregroundValue backgroundValue";
    std::cerr << " feretDiameter, perimeter, histogram, numberOfBins";
    std::cerr << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int dim = 2;
  
  typedef itk::Image< unsigned char, dim > ImageType;

  //reading image to file 
  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
 
  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName( argv[2] );
 
  //converting binary image to Statistics label map 
  // don't set the output type to test the default value of the template parameter
  typedef itk::BinaryImageToStatisticsLabelMapFilter< ImageType, ImageType > I2LType;
  I2LType::Pointer i2l = I2LType::New();
  itk::SimpleFilterWatcher watcher1( i2l );

  i2l->SetInput( reader->GetOutput() );

  // test all the possible ways to set the feature image. Be sure they can work with const images.
  ImageType::ConstPointer constOutput = reader2->GetOutput();
  i2l->SetInput2( constOutput );
  i2l->SetFeatureImage( constOutput );
  i2l->SetInput( 1, constOutput );

  //testing get/set FullyConnected macro
  bool fullyConnected =  atoi(argv[4]);
  i2l->SetFullyConnected( fullyConnected );
  TEST_SET_GET_VALUE( fullyConnected, i2l->GetFullyConnected() );

  //testing boolean FullyConnected macro
  i2l->FullyConnectedOff();
  TEST_SET_GET_VALUE( false, i2l->GetFullyConnected() );

  i2l->FullyConnectedOn();
  TEST_SET_GET_VALUE( true, i2l->GetFullyConnected() );

  //testing get/set InputForegroundValue macro
  int inputForegroundValue = ( atoi(argv[5]) );
  i2l->SetInputForegroundValue( inputForegroundValue );
  TEST_SET_GET_VALUE( inputForegroundValue, i2l->GetInputForegroundValue() );

  //testing get/set OutputBackgroundValue macro
  unsigned int outputBackgroundValue = ( atoi(argv[6]) );
  i2l->SetOutputBackgroundValue( outputBackgroundValue );
  TEST_SET_GET_VALUE( outputBackgroundValue, i2l->GetOutputBackgroundValue() );
 
  //testing get/set ComputeFeretDiameter macro
  bool computeFeretDiameter =  ( atoi(argv[7]) );
  i2l->SetComputeFeretDiameter( computeFeretDiameter );
  TEST_SET_GET_VALUE( computeFeretDiameter, i2l->GetComputeFeretDiameter() );
  
  //testing boolean ComputeFeretDiameter macro
  i2l->ComputeFeretDiameterOff();
  TEST_SET_GET_VALUE( false, i2l->GetComputeFeretDiameter() );

  i2l->ComputeFeretDiameterOn();
  TEST_SET_GET_VALUE( true, i2l->GetComputeFeretDiameter() );
    
  //testing get/set ComputePerimeter macro
  bool computePerimeter =  atoi(argv[8]);
  i2l->SetComputePerimeter( computePerimeter );
  TEST_SET_GET_VALUE( computePerimeter, i2l->GetComputePerimeter() );

  //testing boolean ComputePerimeter macro
  i2l->ComputePerimeterOff();
  TEST_SET_GET_VALUE( false, i2l->GetComputePerimeter() );

  i2l->ComputePerimeterOn();
  TEST_SET_GET_VALUE( true, i2l->GetComputePerimeter() );
  
  //testing get/set ComputeHistogram macro
  bool computeHistogram =  ( atoi(argv[9]) );
  i2l->SetComputeHistogram( computeHistogram );
  TEST_SET_GET_VALUE( computeHistogram, i2l->GetComputeHistogram() );
  
  //testing boolean ComputeHistogram macro
  i2l->ComputeHistogramOff();
  TEST_SET_GET_VALUE( false, i2l->GetComputeHistogram() );

  i2l->ComputeHistogramOn();
  TEST_SET_GET_VALUE( true, i2l->GetComputeHistogram() );
    
  //testing get/set NumberOfBins macro
  unsigned int numberOfBins = ( atoi(argv[10]) );
  i2l->SetNumberOfBins( numberOfBins );
  TEST_SET_GET_VALUE( numberOfBins, i2l->GetNumberOfBins() );

  typedef itk::LabelMapToLabelImageFilter< I2LType::OutputImageType, ImageType> L2IType;
  L2IType::Pointer l2i = L2IType::New();
  itk::SimpleFilterWatcher watcher2( l2i );

  l2i->SetInput( i2l->GetOutput() );

  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( l2i->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->UseCompressionOn();

  TRY_EXPECT_NO_EXCEPTION( writer->Update() );

  return EXIT_SUCCESS;
}
