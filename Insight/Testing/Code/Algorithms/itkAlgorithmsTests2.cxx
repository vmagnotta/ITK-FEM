/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAlgorithmsTests2.cxx,v $
  Language:  C++
  Date:      $Date: 2005-05-05 20:27:55 $
  Version:   $Revision: 1.36 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "vnl/vnl_sample.h"
#include "itkTestMain.h" 


void RegisterTests()
{
  vnl_sample_reseed(8775070);
  REGISTER_TEST(itkNarrowBandThresholdSegmentationLevelSetImageFilterTest );
  REGISTER_TEST(itkThresholdSegmentationLevelSetImageFilterTest );
  REGISTER_TEST(itkCompareHistogramImageToImageMetricTest   );
  REGISTER_TEST(itkDeformableSimplexMesh3DFilterTest   );
  REGISTER_TEST(itkDeformableSimplexMesh3DBalloonForceFilterTest  );
  REGISTER_TEST(itkDeformableSimplexMesh3DGradientConstraintForceFilterTest  );
  REGISTER_TEST(itkGradientDifferenceImageToImageMetricTest );
  REGISTER_TEST(itkKappaStatisticImageToImageMetricTest );
  REGISTER_TEST(itkKullbackLeiblerCompareHistogramImageToImageMetricTest );
  REGISTER_TEST(itkImagePCAShapeModelEstimatorTest);
  REGISTER_TEST(itkImagePCADecompositionCalculatorTest);
  REGISTER_TEST(itkIsolatedWatershedImageFilterTest );
  REGISTER_TEST(itkMIRegistrationFunctionTest );
  REGISTER_TEST(itkMatchCardinalityImageToImageMetricTest );
  REGISTER_TEST(itkOtsuMultipleThresholdsCalculatorTest );
  REGISTER_TEST(itkOtsuMultipleThresholdsImageFilterTest );
  REGISTER_TEST(itkPointSetToPointSetRegistrationTest );
  REGISTER_TEST(itkPointSetToSpatialObjectDemonsRegistrationTest );
  REGISTER_TEST(itkRegularSphereMeshSourceTest );
  REGISTER_TEST(itkRayCastInterpolateImageFunctionTest );
  REGISTER_TEST(itkSimplexMeshVolumeCalculatorTest );
  REGISTER_TEST(itkSTAPLEImageFilterTest );
  REGISTER_TEST(itkSymmetricForcesDemonsRegistrationFilterTest );
  REGISTER_TEST(itkVoronoiDiagram2DTest );
  REGISTER_TEST(itkVoronoiSegmentationImageFilterTest );
}

