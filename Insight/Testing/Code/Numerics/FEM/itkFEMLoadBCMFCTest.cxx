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
#include "itkFEMLoadBCMFC.h"


int Test2D();
int Test3D();

 

//
int itkFEMLoadBCMFCTest(int, char*[])
{
 
  int result2d = Test2D();
  if (result2d != EXIT_SUCCESS)
    {
    return result2d;
    }

#if 0    
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
#endif
    return EXIT_SUCCESS;
}

int Test2D()
{

  itk::fem::Solver S2d;
  S2d.InitializeLinearSystemWrapper();
    
  itk::fem::Node::Pointer n2d;
  itk::fem::Element::VectorType pt2d(2);

  /* Quad 0 */
  n2d=itk::fem::Node::New();
  pt2d[0]=0.;
  pt2d[1]=0.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));

  n2d=itk::fem::Node::New();
  pt2d[0]=1.;
  pt2d[1]=0.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));
  
  n2d=itk::fem::Node::New();
  pt2d[0]=1.;
  pt2d[1]=1.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));

  n2d=itk::fem::Node::New();
  pt2d[0]=0.;
  pt2d[1]=1.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));
  
  /* Quad 1 */
  n2d=itk::fem::Node::New();
  pt2d[0]=1.;
  pt2d[1]=0.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));

  n2d=itk::fem::Node::New();
  pt2d[0]=2.;
  pt2d[1]=0.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));
  
  n2d=itk::fem::Node::New();
  pt2d[0]=2.;
  pt2d[1]=1.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));

  n2d=itk::fem::Node::New();
  pt2d[0]=1.;
  pt2d[1]=1.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));
  
  /* Quad 2 */
  n2d=itk::fem::Node::New();
  pt2d[0]=2.;
  pt2d[1]=0.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));

  n2d=itk::fem::Node::New();
  pt2d[0]=3.;
  pt2d[1]=0.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));
  
  n2d=itk::fem::Node::New();
  pt2d[0]=3.;
  pt2d[1]=1.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));

  n2d=itk::fem::Node::New();
  pt2d[0]=2.;
  pt2d[1]=1.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));
  
  /* Quad 3 */
  n2d=itk::fem::Node::New();
  pt2d[0]=3.;
  pt2d[1]=0.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));

  n2d=itk::fem::Node::New();
  pt2d[0]=4.;
  pt2d[1]=0.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));
  
  n2d=itk::fem::Node::New();
  pt2d[0]=4.;
  pt2d[1]=1.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));

  n2d=itk::fem::Node::New();
  pt2d[0]=3.;
  pt2d[1]=1.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));
  
  /* Quad 4 */
  n2d=itk::fem::Node::New();
  pt2d[0]=4.;
  pt2d[1]=0.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));

  n2d=itk::fem::Node::New();
  pt2d[0]=5.;
  pt2d[1]=0.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));
  
  n2d=itk::fem::Node::New();
  pt2d[0]=5.;
  pt2d[1]=1.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));

  n2d=itk::fem::Node::New();
  pt2d[0]=4.;
  pt2d[1]=1.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));
  
  /* Quad 5 */
  n2d=itk::fem::Node::New();
  pt2d[0]=5.;
  pt2d[1]=0.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));

  n2d=itk::fem::Node::New();
  pt2d[0]=6.;
  pt2d[1]=0.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));
  
  n2d=itk::fem::Node::New();
  pt2d[0]=6.;
  pt2d[1]=1.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));

  n2d=itk::fem::Node::New();
  pt2d[0]=5.;
  pt2d[1]=1.;
  n2d->SetCoordinates(pt2d);
  S2d.node.push_back(itk::fem::FEMP<itk::fem::Node>(&*n2d));
  
  S2d.node.Renumber();

  std::cout << "Nodes\n";

  itk::fem::MaterialLinearElasticity::Pointer m;
  m=itk::fem::MaterialLinearElasticity::New();
  m->SetGlobalNumber(0);
  m->E=30000.0;
  m->A=0.02;
  m->I=0.004;
  S2d.mat.push_back( itk::fem::FEMP<itk::fem::Material>(&*m) );

  std::cout << "Material\n";

  itk::fem::Element2DC0LinearQuadrilateralMembrane::Pointer e0 = 
    itk::fem::Element2DC0LinearQuadrilateralMembrane::New();
      
  e0->SetGlobalNumber(0);
  e0->SetNode(0, &*S2d.node.Find(0));
  e0->SetNode(1, &*S2d.node.Find(1));
  e0->SetNode(2, &*S2d.node.Find(2));
  e0->SetNode(3, &*S2d.node.Find(3));
  e0->m_mat=dynamic_cast<itk::fem::MaterialLinearElasticity*>(&*S2d.mat.Find(0));
  S2d.el.push_back(itk::fem::FEMP<itk::fem::Element>(&*e0));
  
  e0 = itk::fem::Element2DC0LinearQuadrilateralMembrane::New(); 
  e0->SetGlobalNumber(1);
  e0->SetNode(4, &*S2d.node.Find(4));
  e0->SetNode(5, &*S2d.node.Find(5));
  e0->SetNode(6, &*S2d.node.Find(6));
  e0->SetNode(7, &*S2d.node.Find(7));
  e0->m_mat=dynamic_cast<itk::fem::MaterialLinearElasticity*>(&*S2d.mat.Find(0));
  S2d.el.push_back(itk::fem::FEMP<itk::fem::Element>(&*e0));
  
  e0 = itk::fem::Element2DC0LinearQuadrilateralMembrane::New(); 
  e0->SetGlobalNumber(2);
  e0->SetNode(8, &*S2d.node.Find(8));
  e0->SetNode(9, &*S2d.node.Find(9));
  e0->SetNode(10, &*S2d.node.Find(10));
  e0->SetNode(11, &*S2d.node.Find(11));
  e0->m_mat=dynamic_cast<itk::fem::MaterialLinearElasticity*>(&*S2d.mat.Find(0));
  S2d.el.push_back(itk::fem::FEMP<itk::fem::Element>(&*e0));
  
  e0 = itk::fem::Element2DC0LinearQuadrilateralMembrane::New(); 
  e0->SetGlobalNumber(3);
  e0->SetNode(12, &*S2d.node.Find(12));
  e0->SetNode(13, &*S2d.node.Find(13));
  e0->SetNode(14, &*S2d.node.Find(14));
  e0->SetNode(15, &*S2d.node.Find(15));
  e0->m_mat=dynamic_cast<itk::fem::MaterialLinearElasticity*>(&*S2d.mat.Find(0));
  S2d.el.push_back(itk::fem::FEMP<itk::fem::Element>(&*e0));
  
  e0 = itk::fem::Element2DC0LinearQuadrilateralMembrane::New(); 
  e0->SetGlobalNumber(4);
  e0->SetNode(16, &*S2d.node.Find(16));
  e0->SetNode(17, &*S2d.node.Find(17));
  e0->SetNode(18, &*S2d.node.Find(18));
  e0->SetNode(19, &*S2d.node.Find(19));
  e0->m_mat=dynamic_cast<itk::fem::MaterialLinearElasticity*>(&*S2d.mat.Find(0));
  S2d.el.push_back(itk::fem::FEMP<itk::fem::Element>(&*e0));
  
  e0 = itk::fem::Element2DC0LinearQuadrilateralMembrane::New(); 
  e0->SetGlobalNumber(5);
  e0->SetNode(20, &*S2d.node.Find(20));
  e0->SetNode(21, &*S2d.node.Find(21));
  e0->SetNode(22, &*S2d.node.Find(22));
  e0->SetNode(23, &*S2d.node.Find(23));
  e0->m_mat=dynamic_cast<itk::fem::MaterialLinearElasticity*>(&*S2d.mat.Find(0));
  S2d.el.push_back(itk::fem::FEMP<itk::fem::Element>(&*e0));
  
  
  itk::fem::LoadBCMFC::LhsType lhs;
  lhs.push_back( itk::fem::LoadBCMFC::MFCTerm( S2d.el.Find(1), 0, 0.5 ) );
  lhs.push_back( itk::fem::LoadBCMFC::MFCTerm( S2d.el.Find(5), 1, 2.1 ) );
  
  itk::fem::LoadBCMFC::RhsType rhs(10.0);
  
  itk::fem::LoadBCMFC::Pointer loadBCMFC = itk::fem::LoadBCMFC::New();
  loadBCMFC->SetLeftHandSide(lhs);
  loadBCMFC->SetRightHandSide(rhs);
  
  return EXIT_SUCCESS;
  
}


