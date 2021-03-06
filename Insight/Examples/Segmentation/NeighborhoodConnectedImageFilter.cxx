/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: NeighborhoodConnectedImageFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2009-03-17 21:44:42 $
  Version:   $Revision: 1.29 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

// Software Guide : BeginLatex
//
// The following example illustrates the use of the
// \doxygen{NeighborhoodConnectedImageFilter}.  This filter is a close variant
// of the \doxygen{ConnectedThresholdImageFilter}. On one hand, the
// ConnectedThresholdImageFilter accepts a pixel in the region if its intensity
// is in the interval defined by two user-provided threshold values.  The
// NeighborhoodConnectedImageFilter, on the other hand, will only accept a
// pixel if \textbf{all} its neighbors have intensities that fit in the
// interval. The size of the neighborhood to be considered around each pixel is
// defined by a user-provided integer radius. 
//
// The reason for considering the neighborhood intensities instead of only the
// current pixel intensity is that small structures are less likely to be
// accepted in the region. The operation of this filter is equivalent to
// applying the ConnectedThresholdImageFilter followed by mathematical
// morphology erosion using a structuring element of the same shape as
// the neighborhood provided to the NeighborhoodConnectedImageFilter.
//
// Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
#include "itkNeighborhoodConnectedImageFilter.h"
// Software Guide : EndCodeSnippet


#include "itkImage.h"
#include "itkCastImageFilter.h"


//  Software Guide : BeginLatex
//
//  The \doxygen{CurvatureFlowImageFilter} is used here to smooth the image
//  while preserving edges.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "itkCurvatureFlowImageFilter.h"
// Software Guide : EndCodeSnippet


#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"


int main( int argc, char *argv[] )
{
  if( argc < 7 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage  outputImage seedX seedY lowerThreshold upperThreshold" << std::endl;
    return 1;
    }


  //  Software Guide : BeginLatex
  //  
  //  We now define the image type using a particular pixel type and image
  //  dimension. In this case the \code{float} type is used for the pixels due
  //  to the requirements of the smoothing filter. 
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef   float           InternalPixelType;
  const     unsigned int    Dimension = 2;
  typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;
  // Software Guide : EndCodeSnippet


  typedef unsigned char                            OutputPixelType;
  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

  typedef itk::CastImageFilter< InternalImageType, OutputImageType >
    CastingFilterType;
  CastingFilterType::Pointer caster = CastingFilterType::New();
                        

  // We instantiate reader and writer types
  //
  typedef  itk::ImageFileReader< InternalImageType > ReaderType;
  typedef  itk::ImageFileWriter<  OutputImageType  > WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( argv[1] );
  writer->SetFileName( argv[2] );


  //  Software Guide : BeginLatex
  //  
  //  The smoothing filter type is instantiated using the image type as
  //  a template parameter.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef   itk::CurvatureFlowImageFilter<InternalImageType, InternalImageType>
    CurvatureFlowImageFilterType;
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  //  Then, the filter is created by invoking the \code{New()} method and
  //  assigning the result to a \doxygen{SmartPointer}.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  CurvatureFlowImageFilterType::Pointer smoothing = 
                         CurvatureFlowImageFilterType::New();
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  //  We now declare the type of the region growing filter. In this case it is
  //  the NeighborhoodConnectedImageFilter. 
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef itk::NeighborhoodConnectedImageFilter<InternalImageType,
                                    InternalImageType > ConnectedFilterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //  
  //  One filter of this class is constructed using the \code{New()} method. 
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  ConnectedFilterType::Pointer neighborhoodConnected = ConnectedFilterType::New();
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //  
  //  Now it is time to create a simple, linear data processing pipeline. A
  //  file reader is added at the beginning of the pipeline and a cast
  //  filter and writer are added at the end. The cast filter is required
  //  to convert \code{float} pixel types to integer types since only a
  //  few image file formats support \code{float} types.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  smoothing->SetInput( reader->GetOutput() );
  neighborhoodConnected->SetInput( smoothing->GetOutput() );
  caster->SetInput( neighborhoodConnected->GetOutput() );
  writer->SetInput( caster->GetOutput() );
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  The CurvatureFlowImageFilter requires a couple of parameters to
  //  be defined. The following are typical values for $2D$ images. However
  //  they may have to be adjusted depending on the amount of noise present in
  //  the input image.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  smoothing->SetNumberOfIterations( 5 );
  smoothing->SetTimeStep( 0.125 );
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  The NeighborhoodConnectedImageFilter requires that two main parameters
  //  are specified. They are the lower and upper thresholds of the interval
  //  in which intensity values must fall to be included in the
  //  region. Setting these two values too close will not allow enough
  //  flexibility for the region to grow. Setting them too far apart will
  //  result in a region that engulfs the image.
  //
  //  \index{itk::NeighborhoodConnectedImageFilter!SetLower()}
  //  \index{itk::NeighborhoodConnectedImageFilter!SetUppder()}
  //
  //  Software Guide : EndLatex 

  const InternalPixelType lowerThreshold = atof( argv[5] );
  const InternalPixelType upperThreshold = atof( argv[6] );

  // Software Guide : BeginCodeSnippet
  neighborhoodConnected->SetLower(  lowerThreshold  );
  neighborhoodConnected->SetUpper(  upperThreshold  );
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //  
  //  Here, we add the crucial parameter that defines the neighborhood size
  //  used to determine whether a pixel lies in the region. The larger the
  //  neighborhood, the more stable this filter will be against noise in the
  //  input image, but also the longer the computing time will be.  Here we
  //  select a filter of radius $2$ along each dimension. This results in a
  //  neighborhood of $5 \times 5$ pixels.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  InternalImageType::SizeType   radius;

  radius[0] = 2;   // two pixels along X 
  radius[1] = 2;   // two pixels along Y 

  neighborhoodConnected->SetRadius( radius );
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  As in the ConnectedThresholdImageFilter we must now provide the
  //  intensity value to be used for the output pixels accepted in the region
  //  and at least one seed point to define the initial region.
  //
  //  \index{itk::NeighborhoodConnectedImageFilter!SetSeed()}
  //  \index{itk::NeighborhoodConnectedImageFilter!SetReplaceValue()}
  //
  //  Software Guide : EndLatex 

  InternalImageType::IndexType  index;
  
  index[0] = atoi( argv[3] );
  index[1] = atoi( argv[4] );


  // Software Guide : BeginCodeSnippet
  neighborhoodConnected->SetSeed( index );
  neighborhoodConnected->SetReplaceValue( 255 );
  // Software Guide : EndCodeSnippet

  
  //  Software Guide : BeginLatex
  //  
  //  The invocation of the \code{Update()} method on the writer triggers the
  //  execution of the pipeline.  It is usually wise to put update calls in a
  //  \code{try/catch} block in case errors occur and exceptions are thrown.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    }
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  Now we'll run this example using the image
  //  \code{BrainProtonDensitySlice.png} as input available from the
  //  directory \code{Examples/Data}. We can easily segment the major
  //  anatomical structures by providing seeds in the appropriate locations
  //  and defining values for the lower and upper thresholds. For example
  //
  //  \begin{center}
  //  \begin{tabular}{|l|c|c|c|c|}
  //  \hline
  //  Structure & Seed Index & Lower & Upper & Output Image \\ \hline
  //  White matter & $(60,116)$ & 150 & 180 & Second from left in Figure \ref{fig:NeighborhoodConnectedImageFilterOutput} \\  \hline
  //  Ventricle    & $(81,112)$ & 210 & 250 & Third  from left in Figure \ref{fig:NeighborhoodConnectedImageFilterOutput} \\  \hline
  //  Gray matter  & $(107,69)$ & 180 & 210 & Fourth from left in Figure \ref{fig:NeighborhoodConnectedImageFilterOutput} \\  \hline
  //  \end{tabular}
  //  \end{center}
  //
  // \begin{figure} \center
  // \includegraphics[width=0.24\textwidth]{BrainProtonDensitySlice.eps}
  // \includegraphics[width=0.24\textwidth]{NeighborhoodConnectedImageFilterOutput1.eps}
  // \includegraphics[width=0.24\textwidth]{NeighborhoodConnectedImageFilterOutput2.eps}
  // \includegraphics[width=0.24\textwidth]{NeighborhoodConnectedImageFilterOutput3.eps}
  // \itkcaption[NeighborhoodConnected segmentation results ]{Segmentation results
  // of the NeighborhoodConnectedImageFilter for various seed points.}
  // \label{fig:NeighborhoodConnectedImageFilterOutput}
  // \end{figure}
  //
  //  As with the ConnectedThresholdImageFilter, several seeds could
  //  be provided to the filter by using the \code{AddSeed()} method.
  //  Compare the output of Figure
  //  \ref{fig:NeighborhoodConnectedImageFilterOutput} with those of Figure
  //  \ref{fig:ConnectedThresholdOutput} produced by the
  //  ConnectedThresholdImageFilter. You may want to play with the
  //  value of the neighborhood radius and see how it affect the smoothness of
  //  the segmented object borders, the size of the segmented region and how
  //  much that costs in computing time. 
  //
  //  Software Guide : EndLatex 

  return 0;
}
