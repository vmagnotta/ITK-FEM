/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: testMetaLine.cxx,v $
  Language:  C++
  Date:      $Date: 2009-11-13 17:51:20 $
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

#include <stdio.h>
#include <ctype.h>
#include <cstdlib>
#include <metaLine.h>
#include <itksys/SystemTools.hxx>

int testMetaLine(int argc, char * argv[])
{
  if (argc > 1)
    {
    itksys::SystemTools::ChangeDirectory(argv[1]);
    }

  std::cout << "Creating test file ...";
  MetaLine line0;
  MetaLine* Line = new MetaLine(3);
  Line->ID(0);
  LinePnt* pnt;

  unsigned int i;
  for(i=0;i<10;i++)
  {
    pnt = new LinePnt(3);
    pnt->m_X[0]=(float)0.2;
    pnt->m_X[1]=static_cast<float>(i);
    pnt->m_X[2]=static_cast<float>(i);
    pnt->m_V[0][0]=(float)0.3;
    pnt->m_V[0][1]=static_cast<float>(i);
    pnt->m_V[0][2]=static_cast<float>(i);
    pnt->m_V[1][0]=(float)0.4;
    pnt->m_V[1][1]=static_cast<float>(i+1);
    pnt->m_V[1][2]=static_cast<float>(i+1);
    Line->GetPoints().push_back(pnt);
  }
  
  std::cout << "Writing test file ...";
   
  Line->BinaryData(true);

  Line->Write("myLine.meta");

  std::cout << "done" << std::endl;
  std::cout << "Reading test file ...";

  Line->Clear();
  Line->Read("myLine.meta");

  MetaLine LineRead("myLine.meta");
  MetaLine LineCopy(&LineRead);

  std::cout << "PointDim = " << LineCopy.PointDim() << std::endl;
  std::cout << "NPoints = " << LineCopy.NPoints() << std::endl;
  std::cout << "ElementType = " << LineCopy.ElementType() << std::endl;

  Line->PrintInfo();

  MetaLine::PointListType list =  Line->GetPoints();
  MetaLine::PointListType::const_iterator it = list.begin();
  
  i=0;
  while(it != list.end())
  {
    std::cout << "Point #" << i++ << ":" << std::endl;
    std::cout << "position = ";
    unsigned int d;
    for(d = 0; d < 3; d++)
    {
      std::cout << (*it)->m_X[d] << " ";
    }
    std::cout << std::endl;
    std::cout << "First normal = ";
    for(d = 0; d < 3; d++)
    {
      std::cout << (*it)->m_V[0][d] << " ";
    }
    std::cout << std::endl;
    std::cout << "Second normal = ";
    for(d = 0; d < 3; d++)
    {
      std::cout << (*it)->m_V[1][d] << " ";
    }
    std::cout << std::endl;
    it++;
  }

  delete Line;
  std::cout << "done" << std::endl;
  return EXIT_SUCCESS;
}
