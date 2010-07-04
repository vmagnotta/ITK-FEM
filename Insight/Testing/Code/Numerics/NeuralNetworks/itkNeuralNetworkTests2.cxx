/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNeuralNetworkTests2.cxx,v $
  Language:  C++
  Date:      $Date: 2006-09-25 21:28:56 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

// some compilers have trouble with the size of this test
#define ITK_LEAN_AND_MEAN

#include <iostream>
#include "itkTestMain.h" 


void
RegisterTests()
{
  REGISTER_TEST(RBFTest1);
  REGISTER_TEST(itkNeuralNetworksPrintTest);
}


