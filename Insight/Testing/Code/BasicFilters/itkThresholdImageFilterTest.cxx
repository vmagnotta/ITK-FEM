/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkThresholdImageFilterTest.cxx,v $
  Language:  C++
  Date:      $Date: 2009-07-12 10:53:00 $
  Version:   $Revision: 1.25 $

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
#include "itkRandomImageSource.h"
#include "itkThresholdImageFilter.h"
#include "itkCommand.h"
#include "itkTextOutput.h"

#include <itksys/ios/sstream>


int itkThresholdImageFilterTest(int, char* [] )
{
  // Comment the following if you want to use the itk text output window
  itk::OutputWindow::SetInstance(itk::TextOutput::New());

  // Uncomment the following if you want to see each message independently
  // itk::OutputWindow::GetInstance()->PromptUserOn();

  typedef itk::Image<float,2> FloatImage2DType;

  itk::RandomImageSource<FloatImage2DType>::Pointer random;
  random = itk::RandomImageSource<FloatImage2DType>::New();
  random->SetMin(0.0);
  random->SetMax(1000.0);
  random->ReleaseDataFlagOn();
  
  FloatImage2DType::SpacingValueType spacing[2] = {0.7, 2.1};
  random->SetSpacing( spacing );

  FloatImage2DType::PointValueType origin[2] = {15, 400};
  random->SetOrigin( origin );
    
  itksys_ios::ostringstream *os;

  // Test #1, filter goes out of scope
  itk::OutputWindow::GetInstance()->DisplayText( "Test #1: Filter goes out of scope -----------------" );
    {
    itk::ThresholdImageFilter<FloatImage2DType>::Pointer threshold;
    threshold = itk::ThresholdImageFilter<FloatImage2DType>::New();
    threshold->SetInput(random->GetOutput());

    // Exercise threshold setting functions
    threshold->ThresholdAbove( 10.0 );
    threshold->ThresholdBelow( 900.0 );
    threshold->ThresholdOutside( 5.0, 40.0 );

    // Call update multiple times to make sure that the RandomImageSource
    // is releasing and regenerating its data
    threshold->Update();
    threshold->Modified();
    threshold->Update();

    std::cout << "Input spacing: " << random->GetOutput()->GetSpacing()[0]
      << ", "
      << random->GetOutput()->GetSpacing()[1] << std::endl;
    std::cout << "Output spacing: " << threshold->GetOutput()->GetSpacing()[0]
      << ", "
      << threshold->GetOutput()->GetSpacing()[1] << std::endl;

    os = new itksys_ios::ostringstream();
    *os << "Filter: " << threshold.GetPointer();
    itk::OutputWindow::GetInstance()->DisplayText( os->str().c_str() );
    delete os;
    os = new itksys_ios::ostringstream();
    *os << "Output #0: " << threshold->GetOutput(0);
    itk::OutputWindow::GetInstance()->DisplayText( os->str().c_str() );
    delete os;


    itk::OutputWindow::GetInstance()->DisplayText( "Ending Test #1: filter goes out of scope" );
    itk::OutputWindow::GetInstance()->DisplayText( "End of Test #1 -----------------------------------" );
    }

  // Test #2, user keeps an extra handle to an output
  itk::OutputWindow::GetInstance()->DisplayText( "Test #2: User keeps an extra hold on an output  -----------------" );
    {
    FloatImage2DType::Pointer keep;

    itk::ThresholdImageFilter<FloatImage2DType>::Pointer threshold;
    threshold = itk::ThresholdImageFilter<FloatImage2DType>::New();
    threshold->SetInput(random->GetOutput());
    threshold->Update();

    os = new itksys_ios::ostringstream();
    *os << "Filter: " << threshold.GetPointer();
    itk::OutputWindow::GetInstance()->DisplayText( os->str().c_str() );
    delete os;
    os = new itksys_ios::ostringstream();
    *os << "Output #0: " << threshold->GetOutput(0);
    itk::OutputWindow::GetInstance()->DisplayText( os->str().c_str() );
    delete os;

    keep = threshold->GetOutput(0);

    itk::OutputWindow::GetInstance()->DisplayText( "End of Test #2: last handle on output 0 should go out of scope");
    }
  itk::OutputWindow::GetInstance()->DisplayText( "End of Test #2 -----------------------------------");

  // Test #3, user disconnects a data object from the pipeline
  itk::OutputWindow::GetInstance()->DisplayText( "Test #3: user disconnects a data object from the pipeline  -----------------" );
    {
    FloatImage2DType::Pointer keep;

    itk::ThresholdImageFilter<FloatImage2DType>::Pointer threshold;
    threshold = itk::ThresholdImageFilter<FloatImage2DType>::New();
    threshold->SetInput(random->GetOutput());
    threshold->Update();

    os = new itksys_ios::ostringstream;
    *os << "Filter: " << threshold.GetPointer();
    itk::OutputWindow::GetInstance()->DisplayText( os->str().c_str() );
    delete os;
    os = new itksys_ios::ostringstream();
    *os << "Output #0: " << threshold->GetOutput(0);
    itk::OutputWindow::GetInstance()->DisplayText( os->str().c_str() );
    delete os;

    keep = threshold->GetOutput(0);
    keep->DisconnectPipeline();

    itk::OutputWindow::GetInstance()->DisplayText( "End of Test #3: last handle on output 0 should go out of scope");
    }
  itk::OutputWindow::GetInstance()->DisplayText( "End of Test #3 -----------------------------------");
  
  return EXIT_SUCCESS;
}



