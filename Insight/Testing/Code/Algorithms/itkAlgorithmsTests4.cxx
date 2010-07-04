/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAlgorithmsTests4.cxx,v $
  Language:  C++
  Date:      $Date: 2010-06-24 20:41:10 $
  Version:   $Revision: 1.18 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkConfigure.h"
#include "vnl/vnl_sample.h"
#include "itkTestMain.h"


void RegisterTests()
{
  vnl_sample_reseed(8775070);
  REGISTER_TEST(itkCollidingFrontsImageFilterTest );
  REGISTER_TEST(itkBayesianClassifierImageFilterTest );
  //REGISTER_TEST(itkBinaryMedialNodeMetricTest); Test disabled while underlying Blox code is tested for the source of failures
  REGISTER_TEST(itkCurvesLevelSetImageFilterZeroSigmaTest );
  REGISTER_TEST(itkFastMarchingUpwindGradientTest );
  REGISTER_TEST(itkGeodesicActiveContourLevelSetImageFilterZeroSigmaTest );
  REGISTER_TEST(itkLabelVotingImageFilterTest );
  REGISTER_TEST(itkNarrowBandCurvesLevelSetImageFilterTest );
  REGISTER_TEST(itkVectorThresholdSegmentationLevelSetImageFilterTest );
  REGISTER_TEST(itkWatershedImageFilterTest );
  REGISTER_TEST(itkVoronoiPartitioningImageFilterTest );
  REGISTER_TEST(itkVnlFFTTest);
#if defined(USE_FFTWF)
  REGISTER_TEST(itkFFTWF_FFTTest);
  REGISTER_TEST(itkVnlFFTWF_FFTTest);
#endif
#if defined(USE_FFTWD)
  REGISTER_TEST(itkFFTWD_FFTTest);
  REGISTER_TEST(itkVnlFFTWD_FFTTest);
#endif
#if defined(USE_FFTWD)
  REGISTER_TEST(itkCurvatureRegistrationFilterTest);
#endif
#if defined(USE_SCSL)
  REGISTER_TEST(itkSCSLFFTTest);
  REGISTER_TEST(itkVnlSCSL_FFTTest);
#endif
  REGISTER_TEST(itkMeanReciprocalSquareDifferencePointSetToImageMetricTest );
  REGISTER_TEST(itkMeanReciprocalSquareDifferencePointSetToImageMetricTest );
  REGISTER_TEST(itkThresholdMaximumConnectedComponentsImageFilterTest );
  REGISTER_TEST(itkScalarImageKmeansImageFilter3DTest );
  REGISTER_TEST(itkLevelSetMotionRegistrationFilterTest );
}

