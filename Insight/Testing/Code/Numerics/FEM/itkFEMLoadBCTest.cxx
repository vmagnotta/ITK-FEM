/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMLandmarkLoadImplementationTest.cxx,v $
  Language:  C++
  Date: $Date: 2006-11-07 23:23:16 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// disable debug warnings in MS compiler
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "itkFEMLoadImplementationGenericLandmarkLoad.h"
#include "itkFEMElement2DC0LinearQuadrilateralMembrane.h"
#include "itkFEM.h"
#include "itkFEMLinearSystemWrapperItpack.h"


//
int itkFEMLoadBCTest(int, char*[])
{
    itk::fem::Solver S;
    S.InitializeLinearSystemWrapper();


    itk::fem::Node::Pointer n1 = itk::fem::Node::New();
    itk::fem::Element::VectorType pt(2);

    pt[0]=0.;
    pt[1]=0.;
    n1->SetCoordinates(pt);
    S.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n1));

    n1=itk::fem::Node::New();
    pt[0]=1.;
    pt[1]=1.;
    n1->SetCoordinates(pt);
    S.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n1));

    n1=itk::fem::Node::New();
    pt[0]=3.;
    pt[1]=2.;
    n1->SetCoordinates(pt);
    S.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n1));

    n1=itk::fem::Node::New();
    pt[0]=0.;
    pt[1]=3.;
    n1->SetCoordinates(pt);
    S.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n1));

    S.node.Renumber();

    std::cout << "Nodes\n";

    itk::fem::MaterialLinearElasticity::Pointer m;
    m=itk::fem::MaterialLinearElasticity::New();
    m->SetGlobalNumber(0);
    m->E=30000.0;
    m->A=0.02;
    m->I=0.004;
    S.mat.push_back( itk::fem::FEMP<itk::fem::Material>(&*m) );

    std::cout << "Material\n";

    itk::fem::Element2DC0LinearQuadrilateralMembrane::Pointer e0 = 
      itk::fem::Element2DC0LinearQuadrilateralMembrane::New();

    e0->SetGlobalNumber(0);
    e0->SetNode(0, &*S.node.Find(0));
    e0->SetNode(1, &*S.node.Find(1));
    e0->SetNode(2, &*S.node.Find(2));
    e0->SetNode(3, &*S.node.Find(3));
    e0->m_mat=dynamic_cast<itk::fem::MaterialLinearElasticity*>(&*S.mat.Find(0));

    std::cout << "Element\n";

    itk::fem::LoadBC::Pointer l1 = itk::fem::LoadBC::New();
    vnl_vector<double> loadValue;
    loadValue.set_size(2);
    loadValue[0] = 0.0;
    loadValue[1] = 1.0;
    l1->SetElement( &*e0 );
    l1->SetDegreeOfFreedom( 1 );
    l1->SetValue( loadValue );
    l1->Print( std::cout );
    
    
    vnl_vector<double> resultValue = l1->GetValue( );
    int dof = l1->GetDegreeOfFreedom( );
    itk::fem::Element::ConstPointer e1 = l1->GetElement( );
    
    if (dof != 1)
    {
      std::cout << "LoadBC->GetDegreesOfFreedom() failed to return the proper value" << std::endl;
      std::cout << "\tExpected 1 but got " << dof << std::endl;
      return EXIT_FAILURE;
    }
    
    if (resultValue.size() != loadValue.size())
    {
      std::cout << "LoadBC->GetValue() failed to return the proper value" << std::endl;
      std::cout << "\tExpected value to have size  " << loadValue.size() << std::endl;
      return EXIT_FAILURE;
    }
    
    for (int i=0;i<2;i++)
    {
      if (resultValue[i] != loadValue[i])
      {
        std::cout << "LoadBC->GetValue() failed to return the proper value" << std::endl;
        std::cout << "\tIndex " << i << ": Expected " << loadValue[i] << " but got " << resultValue[i] << std::endl;
        return EXIT_FAILURE;
      }
    }
    
    for (int i=0;i<4;i++)
    {
      if (e0->GetNode(i) != e1->GetNode(i))
      {
        std::cout << "LoadBC->GetElement() failed to return the proper value" << std::endl;
        std::cout << "\tIndex " << i << ": Expected " << e0->GetNode(0) << " but got " << e1->GetNode(0) << std::endl;
        return EXIT_FAILURE;
      }
    }
    
    std::cout << "Completed 2D LoadBC\n";
    
    
    
    itk::fem::Solver S3d;
    S3d.InitializeLinearSystemWrapper();


    itk::fem::Node::Pointer n3d = itk::fem::Node::New();
    itk::fem::Element::VectorType pt3d(3);

    pt3d[0]=0.;
    pt3d[1]=0.;
    pt3d[2]=0.;
    n3d->SetCoordinates(pt3d);
    S3d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n3d));

    n3d=itk::fem::Node::New();
    pt3d[0]=1.;
    pt3d[1]=0.;
    pt3d[2]=0.;
    n3d->SetCoordinates(pt3d);
    S3d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n3d));

    n3d=itk::fem::Node::New();
    pt3d[0]=0.;
    pt3d[1]=2.;
    pt3d[1]=0.;
    n3d->SetCoordinates(pt3d);
    S3d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n3d));
    
    n3d=itk::fem::Node::New();
    pt3d[0]=1.;
    pt3d[1]=2.;
    pt3d[1]=0.;
    n3d->SetCoordinates(pt3d);
    S3d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n3d));
    

    n3d=itk::fem::Node::New();
    pt3d[0]=0.;
    pt3d[1]=0.;
    pt3d[1]=5.;
    n3d->SetCoordinates(pt3d);
    S3d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n3d));
    
    n3d=itk::fem::Node::New();
    pt3d[0]=1.;
    pt3d[1]=0.;
    pt3d[1]=5.;
    n3d->SetCoordinates(pt3d);
    S3d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n3d));
    
    n3d=itk::fem::Node::New();
    pt3d[0]=0.;
    pt3d[1]=2.;
    pt3d[1]=5.;
    n3d->SetCoordinates(pt3d);
    S3d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n3d));
    
    n3d=itk::fem::Node::New();
    pt3d[0]=2.;
    pt3d[1]=2.;
    pt3d[1]=5.;
    n3d->SetCoordinates(pt3d);
    S3d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n3d));
    S3d.node.Renumber();

    std::cout << "Nodes\n";

    itk::fem::MaterialLinearElasticity::Pointer m3d;
    m3d=itk::fem::MaterialLinearElasticity::New();
    m3d->SetGlobalNumber(0);
    m3d->E=30000.0;
    m3d->A=0.02;
    m3d->I=0.004;
    S3d.mat.push_back( itk::fem::FEMP<itk::fem::Material>(&*m3d) );

    std::cout << "Material\n";

    itk::fem::Element3DC0LinearHexahedronMembrane::Pointer e3d = 
      itk::fem::Element3DC0LinearHexahedronMembrane::New();

    e3d->SetGlobalNumber(0);
    e3d->SetNode(0, &*S3d.node.Find(0));
    e3d->SetNode(1, &*S3d.node.Find(1));
    e3d->SetNode(2, &*S3d.node.Find(2));
    e3d->SetNode(3, &*S3d.node.Find(3));
    e3d->SetNode(4, &*S3d.node.Find(4));
    e3d->SetNode(5, &*S3d.node.Find(5));
    e3d->SetNode(6, &*S3d.node.Find(6));
    e3d->SetNode(7, &*S3d.node.Find(7));
    e3d->m_mat=dynamic_cast<itk::fem::MaterialLinearElasticity*>(&*S3d.mat.Find(0));

    std::cout << "Element\n";

    itk::fem::LoadBC::Pointer l3d = itk::fem::LoadBC::New();
    vnl_vector<double> load3dValue;
    load3dValue.set_size(3);
    load3dValue[0] = 0.0;
    load3dValue[1] = 0.0;
    load3dValue[2] = 1.0;
    l3d->SetElement( &*e3d );
    l3d->SetDegreeOfFreedom( 2 );
    l3d->SetValue( load3dValue );
    l3d->Print( std::cout );
    
    vnl_vector<double> result3dValue = l3d->GetValue( );
    int dof3d = l3d->GetDegreeOfFreedom( );
    itk::fem::Element::ConstPointer e3d1 = l3d->GetElement( );
    
    if (dof3d != 2)
    {
      std::cout << "LoadBC->GetDegreesOfFreedom() failed to return the proper value" << std::endl;
      std::cout << "\tExpected 2 but got " << dof3d << std::endl;
      return EXIT_FAILURE;
    }
    
    if (result3dValue.size() != load3dValue.size())
    {
      std::cout << "LoadBC->GetValue() failed to return the proper value" << std::endl;
      std::cout << "\tExpected value to have size  " << load3dValue.size() << std::endl;
      return EXIT_FAILURE;
    }
    
    for (int i=0;i<2;i++)
    {
      if (result3dValue[i] != load3dValue[i])
      {
        std::cout << "LoadBC->GetValue() failed to return the proper value" << std::endl;
        std::cout << "\tIndex " << i << ": Expected " << load3dValue[i] << " but got " << result3dValue[i] << std::endl;
        return EXIT_FAILURE;
      }
    }
    
    for (int i=0;i<4;i++)
    {
      if (e3d->GetNode(i) != e3d1->GetNode(i))
      {
        std::cout << "LoadBC->GetElement() failed to return the proper value" << std::endl;
        std::cout << "\tIndex " << i << ": Expected " << e0->GetNode(0) << " but got " << e1->GetNode(0) << std::endl;
        return EXIT_FAILURE;
      }
    }
    
    std::cout << "Completed 3D LoadBC\n";
    

    std::cout << "Test PASSED!\n";
    return EXIT_SUCCESS;
}


