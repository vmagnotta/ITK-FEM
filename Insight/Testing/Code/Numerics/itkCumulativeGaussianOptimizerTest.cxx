/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCumulativeGaussianOptimizerTest.cxx,v $
  Language:  C++
  Date:      $Date: 2009-06-24 12:03:51 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkCumulativeGaussianOptimizer.h"
#include "itkCumulativeGaussianCostFunction.h"

#include <iostream>
#include <cstdlib>

/**
 * Generate test data with the Cumulative Gaussian Cost Function
 * given parameter values for mean, standard deviation,
 * lower and upper asymptotes of a Cumulative Gaussian. 
 * Estimate the parameters of the test data with the
 * Cumulative Gaussian optimizer. The solution should
 * be within differenceTolerance of the fitError.
 */

int itkCumulativeGaussianOptimizerTest(int, char* [] ) 
{  
  double mean                = 3; // Mean of the Cumulative Gaussian. 
                                  // Ranges from 0 to N-1, where N is numberOfSamples.
  double standardDeviation   = 2; // Standard deviation of the Cumulative Gaussian.
  double lowerAsymptote      = -10; // Lower asymptotic value of the Cumulative Gaussian.
  int    numberOfSamples     = 9; // Number of data samples.
  double upperAsymptote      = 10; // Upper asymptotic value of the Cumulative Gaussian.
  double differenceTolerance = 1e-20; // Tolerance allowed for the difference between Gaussian iterations.

  // Typedef and initialization for the Cumulative Gaussian Optimizer.
  typedef itk::CumulativeGaussianOptimizer CumulativeGaussianOptimizerType;
  CumulativeGaussianOptimizerType::Pointer optimizer = CumulativeGaussianOptimizerType::New();

  // Typedef and initialization for the Cumulative Gaussian Cost Function.
  typedef itk::CumulativeGaussianCostFunction CostFunctionType;
  CostFunctionType::Pointer costFunction = CostFunctionType::New();

  // Declare and initialize the data array.
  //CostFunctionType::MeasureType * cumGaussianArray = new CostFunctionType::MeasureType();
  //cumGaussianArray->SetSize(numberOfSamples);
  
  // Set the parameters.
  CostFunctionType::ParametersType parameters;
  parameters.SetSize(4);
  parameters[0] = mean;
  parameters[1] = standardDeviation;
  parameters[2] = lowerAsymptote;
  parameters[3] = upperAsymptote;

  // Set the range of data sampled from a Cumulative Gaussian.
  costFunction->Initialize(numberOfSamples);

  // Generate data given a set of parameters.
  CostFunctionType::MeasureType * cumGaussianArray = costFunction->GetValue(parameters);

  // Set the data array.
  costFunction->SetOriginalDataArray(cumGaussianArray);

  // Set the cost function.
  optimizer->SetCostFunction(costFunction);

  // Set the tolerance for the Gaussian iteration error.
  optimizer->SetDifferenceTolerance(differenceTolerance);

  // Print results after each iteration.
  optimizer->SetVerbose(1);

  // Set the data array.
  optimizer->SetDataArray(cumGaussianArray);

  // Start optimization;
  optimizer->StartOptimization();
 
  std::cout << "StopConditionDescription: " << optimizer->GetStopConditionDescription() << std::endl;

  // The test passes if the difference between the given parameters and estimated parameters
  // is less than or equal to 0.1.
  if( vcl_fabs(optimizer->GetComputedMean() - mean) <= 0.1 &&
      vcl_fabs(optimizer->GetComputedStandardDeviation() - standardDeviation) <= 0.1 &&
      vcl_fabs(optimizer->GetUpperAsymptote() - upperAsymptote) <= 0.1 &&
      vcl_fabs(optimizer->GetLowerAsymptote() - lowerAsymptote) <= 0.1)
    {
    std::cerr << std::endl
      << "Test Passed with a Fit Error of " << optimizer->GetFitError() 
      << std::endl << std::endl;
    
    // Print out the resulting paramters.
    std::cerr << "Fitted mean = " << optimizer->GetComputedMean() << std::endl;
    std::cerr << "Fitted standard deviation = " << optimizer->GetComputedStandardDeviation() << std::endl;
    std::cerr << "Fitted upper intensity = " << optimizer->GetUpperAsymptote() << std::endl;
    std::cerr << "Fitted lower intensity = " << optimizer->GetLowerAsymptote() << std::endl; 
    std::cout << "[TEST DONE]" << std::endl;
    return EXIT_SUCCESS;
    }
  else
    {
    std::cerr << std::endl
      << "Test Failed with a Fit Error of " << optimizer->GetFitError() 
      << std::endl << std::endl;
    
    // Print out the resulting paramters.
    std::cerr << "Fitted mean = " << optimizer->GetComputedMean() << std::endl;
    std::cerr << "Fitted standard deviation = " << optimizer->GetComputedStandardDeviation() << std::endl;
    std::cerr << "Fitted upper asymptote = " << optimizer->GetUpperAsymptote() << std::endl;
    std::cerr << "Fitted lower asymptote = " << optimizer->GetLowerAsymptote() << std::endl;

    return EXIT_FAILURE;
    }

}
