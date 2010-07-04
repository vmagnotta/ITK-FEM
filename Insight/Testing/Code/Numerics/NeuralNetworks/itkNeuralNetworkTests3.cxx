/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNeuralNetworkTests3.cxx,v $
  Language:  C++
  Date:      $Date: 2006-09-25 21:28:56 $
  Version:   $Revision: 1.2 $

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
  REGISTER_TEST(NNetClassifierTest1);
  REGISTER_TEST(NNetClassifierTest2);
  REGISTER_TEST(NNetClassifierTest3);
  REGISTER_TEST(NNetClassifierTest4);
}


