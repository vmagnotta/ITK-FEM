/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDynamicQuadEdgeMeshTest.cxx,v $
  Language:  C++
  Date:      $Date: 2007-07-31 05:37:12 $
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

#include "itkQuadEdgeMesh.h"
#include "itkTetrahedronCell.h"
#include "itkHexahedronCell.h"
#include "itkBoundingBox.h"
#include "itkDefaultDynamicMeshTraits.h"

#include <iostream>
#include <string>

/**
 * Some typedefs to make things easier.
 */

                                 
typedef itk::QuadEdgeMesh< double, 3 >  MeshType;
                   

typedef MeshType::CellTraits  CellTraits;


/**
 * The type of point stored in the mesh. Because mesh was instantiated
 * with defaults (itkDefaultDynamicMeshTraits), the point dimension is 3 and
 * the coordinate representation is float.
 */
typedef MeshType::PointType             PointType;
typedef PointType::VectorType           VectorType;

typedef MeshType::Pointer               MeshPointer;
typedef MeshType::ConstPointer          MeshConstPointer;

typedef MeshType::PointType             PointType;

typedef MeshType::PointsContainer       PointsContainer;
typedef MeshType::PointDataContainer    PointDataContainer;

typedef PointsContainer::Iterator       PointsIterator;
typedef PointDataContainer::Iterator    CellsIterator;

int itkDynamicQuadEdgeMeshTest(int, char* [] )
{
  
  /**
   * Create the mesh through its object factory.
   */
  MeshType::Pointer mesh(MeshType::New());  

  PointType pointA;
  PointType pointB;
  PointType pointC;
  PointType pointD;

  VectorType displacement;

  displacement[0] = 2;
  displacement[1] = 5;
  displacement[2] = 0;

  pointA.Fill( 0.0 );
  pointB = pointA + displacement;
  pointC = pointB + displacement;
  pointD = pointC + displacement;
  
  PointsContainer::Pointer pointsContainter = mesh->GetPoints();

  pointsContainter->SetElement( 0, pointA );
  pointsContainter->SetElement( 1, pointB );
  pointsContainter->SetElement( 2, pointC );
  pointsContainter->SetElement( 3, pointD );


  std::cout << "Number of Points = " << mesh->GetNumberOfPoints() << std::endl;

  PointsIterator point    = pointsContainter->Begin();
  PointsIterator endpoint = pointsContainter->End();

  while( point != endpoint )
    {
    std::cout << point.Index() << " = " << point.Value() << std::endl;
    point++;
    }


  return 0;  

}
