/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBioGeneNetworkTest.cxx,v $
  Language:  C++
  Date:      $Date: 2008-05-26 11:39:43 $
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

#include <iostream>
#include <cstdlib>

#include "itkBioGeneNetwork.h"


int itkBioGeneNetworkTest( int, char * [] )
{
   itk::bio::GeneNetwork net;


   std::cout << "Test Passed !" << std::endl;
   return EXIT_SUCCESS;
}











