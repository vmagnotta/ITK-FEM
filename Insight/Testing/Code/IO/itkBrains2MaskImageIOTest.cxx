/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBrains2MaskImageIOTest.cxx,v $
  Language:  C++
  Date:      $Date: 2008-05-21 18:59:29 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <string>
#include "itkImage.h"
#include "itkExceptionObject.h"
#include "itkNumericTraits.h"
#include "itkImageRegionIterator.h"
#include "itkImageIOFactory.h"
#include "itkBrains2MaskImageIO.h"
#include "itkBrains2MaskImageIOFactory.h"
#include "stdlib.h"
#include <itksys/SystemTools.hxx>
#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"
//#include "itkFlipImageFilter.h"
#include "itkSpatialOrientationAdapter.h"
#include "vnl/vnl_sample.h"

int itkBrains2MaskTest(int ac, char *av[])
{
  typedef itk::Image<unsigned int,3> ImageType;
  typedef itk::ImageFileReader<ImageType> ImageReaderType;
  typedef itk::ImageFileWriter<ImageType> ImageWriterType;

  //
  // create a random image to write out.
  const ImageType::SizeType imageSize = {{4,4,4}};
  const ImageType::IndexType imageIndex = {{0,0,0}};

  ImageType::RegionType region;
  region.SetSize(imageSize);
  region.SetIndex(imageIndex);
  ImageType::Pointer img = ImageType::New();
  img->SetLargestPossibleRegion(region);
  img->SetBufferedRegion(region);
  img->SetRequestedRegion(region);
  img->Allocate();
  itk::SpatialOrientationAdapter::DirectionType CORdir=itk::SpatialOrientationAdapter().ToDirectionCosines(itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RIP);
  img->SetDirection(CORdir);
  ImageType::PointType origin;
  origin[0] = 10;
  origin[1] = 20;
  origin[2] = 30;
  img->SetOrigin(origin);
  vnl_sample_reseed(8775070);
  itk::ImageRegionIterator<ImageType> ri(img,region);
  try
    {
    unsigned int counter = 0;
    while(!ri.IsAtEnd())
      {
      unsigned int val
        = static_cast<unsigned int>(vnl_sample_uniform(0.0, 16384.0));
      val = val > 8192 ? 255 : 0;
      if(counter && counter % 8 == 0)
        std::cerr << val << std::endl;
      else
        std::cerr << val << " ";
      counter++;
      ri.Set(val);
      ++ri;
      }
    std::cerr << std::endl << std::endl;
    }
  catch(itk::ExceptionObject & ex)
    {
    ex.Print(std::cerr);
    return EXIT_FAILURE;
    }
  itk::ImageIOBase::Pointer io;
  io = itk::Brains2MaskImageIO::New();

  if(ac < 2)
    {
    std::cout << "Must specify directory containing Brains2Test.mask"
              << std::endl;
    return EXIT_FAILURE;
    }
  std::string fileName(av[1]);
  fileName = fileName + "/Brains2Test.mask";

  ImageWriterType::Pointer imageWriter = ImageWriterType::New();
  imageWriter->SetImageIO(io);
  imageWriter->SetFileName(fileName.c_str());
  imageWriter->SetInput(img);
  try 
    {
    imageWriter->Write();
    }
  catch (itk::ExceptionObject &ex)
    {
    std::string message;
    message = "Problem found while writing image ";
    message += fileName;
    message += "\n";
    message += ex.GetLocation();
    message += "\n";
    message += ex.GetDescription();
    std::cerr << message << std::endl;
    itksys::SystemTools::RemoveFile(fileName.c_str());
    return EXIT_FAILURE;
    }
  ImageType::Pointer readImage;
  try
    {
    ImageReaderType::Pointer imageReader = ImageReaderType::New();
    imageReader->SetImageIO(io);
    imageReader->SetFileName(fileName.c_str());
    imageReader->Update();
    readImage = imageReader->GetOutput();
    }
  catch (itk::ExceptionObject& ex)
    {
    std::string message;
    message = "Problem found while reading image ";
    message += fileName;
    message += "\n";
    message += ex.GetLocation();
    message += "\n";
    message += ex.GetDescription();
    std::cerr << message << std::endl;
    itksys::SystemTools::RemoveFile(fileName.c_str());
    return EXIT_FAILURE;
    }
  itk::SpatialOrientationAdapter::DirectionType readDirection(readImage->GetDirection());
  if(readDirection != CORdir)
    {
    std::string message("Direction Cosines read do not match direction cosines written");
    return EXIT_FAILURE;
    }
  ImageType::PointType readOrigin(readImage->GetOrigin());
  if(readOrigin != origin)
    {
    std::string message("Origin read does not match origin written");
    return EXIT_FAILURE;
    }
  ri.GoToBegin();
  itk::ImageRegionIterator<ImageType> ri2(readImage,region);
  try
    {
    unsigned int counter = 0;
    while(!ri.IsAtEnd() && !ri2.IsAtEnd())
      {
      unsigned int x = ri.Get();
      unsigned int y = ri2.Get();
      if(counter && counter % 8 == 0)
        std::cerr << y << std::endl;
      else
        std::cerr << y << " "; 
      if(x != y)
        {
        std::cerr << 
          "Error comparing Input Image and File Image of Brains2 Mask" << 
          std::endl;
        return EXIT_FAILURE;
        }
      counter++;
      ++ri;
      ++ri2;
      }
    if(!ri.IsAtEnd() || !ri2.IsAtEnd())
      {
      std::cerr << "Error, inconsistent image sizes " << std::endl;
      return EXIT_FAILURE;
      }
    }
  catch(itk::ExceptionObject & ex)
    {
    ex.Print(std::cerr);
    return EXIT_FAILURE;
    }
  //
  // test the factory interface. This ImageIO class doesn't get
  // added to the list of Builtin factories, so add it explicitly, and
  // then try and open the mask file.
  itk::ObjectFactoryBase::RegisterFactory(itk::Brains2MaskImageIOFactory::New() );
  try
    {
    ImageReaderType::Pointer imageReader = ImageReaderType::New();
    imageReader->SetFileName(fileName.c_str());
    imageReader->Update();
    readImage = imageReader->GetOutput();
    }  
  catch(itk::ExceptionObject & ex)
    {
    ex.Print(std::cerr);
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
