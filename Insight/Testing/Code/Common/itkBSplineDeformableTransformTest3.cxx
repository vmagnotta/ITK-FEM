/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBSplineDeformableTransformTest3.cxx,v $
  Language:  C++
  Date:      $Date: 2010-06-15 14:49:27 $
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


#include "itkImageFileReader.h" 
#include "itkImageFileWriter.h" 

#include "itkImage.h"
#include "itkResampleImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"

#include "itkBSplineDeformableTransform.h"
#include "itkSimilarity2DTransform.h"

#include <fstream>

//  The following section of code implements a Command observer
//  used to monitor the evolution of the registration process.
//
#include "itkCommand.h"
class CommandProgressUpdate : public itk::Command 
{
public:
  typedef  CommandProgressUpdate      Self;
  typedef  itk::Command               Superclass;
  typedef itk::SmartPointer<Self>     Pointer;
  itkNewMacro( Self );
protected:
  CommandProgressUpdate() {};
public:
  void Execute(itk::Object *caller, const itk::EventObject & event)
    {
    Execute( (const itk::Object *)caller, event);
    }

  void Execute(const itk::Object * object, const itk::EventObject & event)
    {
    const itk::ProcessObject * filter = dynamic_cast< const itk::ProcessObject * >( object );
    if( ! itk::ProgressEvent().CheckEvent( &event ) )
      {
      return;
      }
    std::cout << filter->GetProgress() << std::endl;
    }
};


template <unsigned int VSplineOrder>
class BSplineDeformableTransformTest3Helper
{
public:
static int RunTest(int argc, char * argv [] )
{
  const     unsigned int   ImageDimension = 2;

  typedef   unsigned char                             PixelType;
  typedef   itk::Image< PixelType, ImageDimension >   FixedImageType;
  typedef   itk::Image< PixelType, ImageDimension >   MovingImageType;

  typedef   itk::ImageFileReader< FixedImageType  >   FixedReaderType;
  typedef   itk::ImageFileReader< MovingImageType >   MovingReaderType;

  typedef   itk::ImageFileWriter< MovingImageType >   MovingWriterType;


  typename FixedReaderType::Pointer fixedReader = FixedReaderType::New();
  fixedReader->SetFileName( argv[2] );

  try
    {
    fixedReader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception thrown " << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }


  typename MovingReaderType::Pointer movingReader = MovingReaderType::New();
  typename MovingWriterType::Pointer movingWriter = MovingWriterType::New();

  movingReader->SetFileName( argv[3] );
  movingWriter->SetFileName( argv[4] );


  typename FixedImageType::ConstPointer fixedImage = fixedReader->GetOutput();


  typedef itk::ResampleImageFilter< MovingImageType, 
                                    FixedImageType  >  FilterType;

  typename FilterType::Pointer resampler = FilterType::New();

  typedef itk::LinearInterpolateImageFunction< 
                       MovingImageType, double >  InterpolatorType;

  typename InterpolatorType::Pointer interpolator = InterpolatorType::New();

  resampler->SetInterpolator( interpolator );

  typename FixedImageType::SpacingType   fixedSpacing    = fixedImage->GetSpacing();
  typename FixedImageType::PointType     fixedOrigin     = fixedImage->GetOrigin();
  typename FixedImageType::DirectionType fixedDirection  = fixedImage->GetDirection();

  resampler->SetOutputSpacing( fixedSpacing );
  resampler->SetOutputOrigin(  fixedOrigin  );
  resampler->SetOutputDirection(  fixedDirection  );

  
  typename FixedImageType::RegionType fixedRegion = fixedImage->GetBufferedRegion();
  typename FixedImageType::SizeType   fixedSize =  fixedRegion.GetSize();
  resampler->SetSize( fixedSize );
  resampler->SetOutputStartIndex(  fixedRegion.GetIndex() );


  resampler->SetInput( movingReader->GetOutput() );
  
  movingWriter->SetInput( resampler->GetOutput() );


  const unsigned int SpaceDimension = ImageDimension;
  typedef double CoordinateRepType;

  typedef itk::BSplineDeformableTransform<
                            CoordinateRepType,
                            SpaceDimension,
                            VSplineOrder >     TransformType;
  
  typename TransformType::Pointer bsplineTransform = TransformType::New();


  typedef typename TransformType::RegionType RegionType;
  RegionType bsplineRegion;
  typename RegionType::SizeType   size;

  const unsigned int numberOfGridNodesOutsideTheImageSupport = VSplineOrder;

  const unsigned int numberOfGridNodesInsideTheImageSupport = 5;

  const unsigned int numberOfGridNodes = 
                        numberOfGridNodesInsideTheImageSupport +
                        numberOfGridNodesOutsideTheImageSupport;

  const unsigned int numberOfGridCells = 
                        numberOfGridNodesInsideTheImageSupport - 1;
                        
  size.Fill( numberOfGridNodes );
  bsplineRegion.SetSize( size );

  typedef typename TransformType::SpacingType SpacingType;
  SpacingType spacing;

  typedef typename TransformType::OriginType OriginType;
  OriginType origin;

#ifdef ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
  spacing[0] = fixedSpacing[0] * fixedSize[0]  / numberOfGridCells;
  spacing[1] = fixedSpacing[1] * fixedSize[1]  / numberOfGridCells;

  const unsigned int orderShift = VSplineOrder / 2;

  origin[0] = fixedOrigin[0] - orderShift * spacing[0] - fixedSpacing[0] / 2.0;
  origin[1] = fixedOrigin[1] - orderShift * spacing[1] - fixedSpacing[1] / 2.0;
#else
  spacing[0] = vcl_floor( fixedSpacing[0] * (fixedSize[0] - 1) / numberOfGridCells );
  spacing[1] = vcl_floor( fixedSpacing[1] * (fixedSize[1] - 1) / numberOfGridCells );

  origin[0] = fixedOrigin[0] - spacing[0];
  origin[1] = fixedOrigin[1] - spacing[1];
#endif
  
  bsplineTransform->SetGridSpacing( spacing );
  bsplineTransform->SetGridOrigin( origin );
  bsplineTransform->SetGridRegion( bsplineRegion );
  bsplineTransform->SetGridDirection( fixedImage->GetDirection() );
 
  typedef itk::Similarity2DTransform<CoordinateRepType> BulkTransformType;
  BulkTransformType::Pointer bulkTransform = BulkTransformType::New();
  bulkTransform->SetIdentity();

  BulkTransformType::ParametersType bulkParameters = bulkTransform->GetParameters();
  bulkParameters[0]= 0.5; //half the scale. 
  bulkTransform->SetParameters( bulkParameters ); 
  std::cout << " parameters " << bulkTransform->GetParameters() << "\n";  
  bsplineTransform->SetBulkTransform( bulkTransform );
  
  typedef typename TransformType::ParametersType     ParametersType;

  const unsigned int numberOfParameters = bsplineTransform->GetNumberOfParameters();

  const unsigned int numberOfNodes = numberOfParameters / SpaceDimension;

  ParametersType parameters( numberOfParameters );


  std::ifstream infile;

  infile.open( argv[1] );

  for( unsigned int n=0; n < numberOfNodes; n++ )
    {
    infile >>  parameters[n]; 
    infile >>  parameters[n+numberOfNodes]; 
    } 

  infile.close();


  bsplineTransform->SetParameters( parameters );


  typename CommandProgressUpdate::Pointer observer = CommandProgressUpdate::New();

  resampler->AddObserver( itk::ProgressEvent(), observer );
  

  resampler->SetTransform( bsplineTransform );
  
  try
    {
    movingWriter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception thrown " << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }


  typedef itk::Point<  float, ImageDimension >        PointType;
  typedef itk::Vector< float, ImageDimension >        VectorType;
  typedef itk::Image< VectorType, ImageDimension >    DeformationFieldType;

  typename DeformationFieldType::Pointer field = DeformationFieldType::New();
  field->SetRegions( fixedRegion );
  field->SetOrigin( fixedOrigin );
  field->SetSpacing( fixedSpacing );
  field->Allocate();

  typedef itk::ImageRegionIterator< DeformationFieldType > FieldIterator;
  FieldIterator fi( field, fixedRegion );

  fi.GoToBegin();

  typename TransformType::InputPointType  fixedPoint;
  typename TransformType::OutputPointType movingPoint;
  typename DeformationFieldType::IndexType index;

  VectorType displacement;

  while( ! fi.IsAtEnd() )
    {
    index = fi.GetIndex();
    field->TransformIndexToPhysicalPoint( index, fixedPoint );
    movingPoint = bsplineTransform->TransformPoint( fixedPoint );
    displacement[0] = movingPoint[0] - fixedPoint[0];
    displacement[1] = movingPoint[1] - fixedPoint[1];
    fi.Set( displacement );
    ++fi;
    }


  typedef itk::ImageFileWriter< DeformationFieldType >  FieldWriterType;
  typename FieldWriterType::Pointer fieldWriter = FieldWriterType::New();

  fieldWriter->SetInput( field );

  if( argc >= 6 )
    {
    fieldWriter->SetFileName( argv[5] );
    try
      {
      fieldWriter->Update();
      }
    catch( itk::ExceptionObject & excp )
      {
      std::cerr << "Exception thrown " << std::endl;
      std::cerr << excp << std::endl;
      return EXIT_FAILURE;
      }
    }
  return EXIT_SUCCESS;
}
};


int itkBSplineDeformableTransformTest3( int argc, char * argv[] )
{

  if( argc < 7 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " coefficientsFile fixedImage ";
    std::cerr << "movingImage deformedMovingImage" << std::endl;
    std::cerr << "[deformationField][multithreader use #threads]" << std::endl;
    return EXIT_FAILURE;
    }

 const int numberOfThreads = atoi( argv[6] );
   
 int status = 0;
 switch( numberOfThreads )
   {
   case 0:
     {
     //Don't invoke MultiThreader at all. 
     status |= BSplineDeformableTransformTest3Helper< 3 >::RunTest( argc, argv ); 
     break;
     }
   default:
     {
     //Use MultiThreader with argv[6] threads
     itk::MultiThreader::SetGlobalDefaultNumberOfThreads(numberOfThreads);
     itk::MultiThreader::SetGlobalMaximumNumberOfThreads(numberOfThreads);
     status |= BSplineDeformableTransformTest3Helper< 3 >::RunTest( argc, argv ); 
     break;
     }
   }
 
 if( status )
   {
   return EXIT_FAILURE;
   }

  return EXIT_SUCCESS;
}
