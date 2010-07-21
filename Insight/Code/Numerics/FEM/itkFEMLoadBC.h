/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMLoadBC.h,v $
  Language:  C++
  Date:      $Date: 2009-01-30 21:10:12 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFEMLoadBC_h
#define __itkFEMLoadBC_h

#include "itkFEMLoadBase.h"

namespace itk {
namespace fem {

/**
 * \class LoadBC
 * \brief Generic essential (Dirichlet) boundary conditions.
 *
 * Objects of this class specify, which Degrees of Freedom in a system are fixed.
 */
class LoadBC : public Load
{
  FEM_CLASS(LoadBC,Load)
public:

  typedef Element::Float              LoadFloatType;
  typedef Element::ConstPointer       LoadElementConstPointerType;
  typedef vnl_vector<Element::Float>  LoadValueType;
  
  
  /** Default constructor */
  LoadBC() : m_Element(0), m_DegreeOfFreedom(0), m_Value() {}
  
  /** Set the Local degree of freedom for the specified element affected
   *  the boundary condition. This should be 0-1 for 2D problems and
   *  0-2 for 3D problems.
   */
  void SetDegreeOfFreedom( int dof );
  
  /** Get the Degrees of Freedom for the specified element affected
   *  the boundary condition
   */
  int GetDegreeOfFreedom( );
  
  /** Set the element affected by the boundary condition
   */
  void SetElement(LoadElementConstPointerType element);
  
  /** Get the element affected by the boundary condition
   */
  const LoadElementConstPointerType GetElement();
  
  /** Set the value for the Boundary Condition. 
   *  This is a vector so the application of the
   *  to isotropic element is easy. The specified BC value is 
   *  applied to the master force vector in AssembleF
   *  function in solver.
   */
  void SetValue(LoadValueType value);
  
  /** Get the value for the Boundary Condition. 
   *  This is a vector that should match the dimensionality of the
   *  problem.
   */
  const LoadValueType GetValue();

  /** Read a LoadBC object from input stream.*/
  virtual void Read( std::istream& f, void* info );

  /** Write a LoadBC object to the output stream*/
  virtual void Write( std::ostream& f ) const;

protected:  

  void PrintSelf(std::ostream& os, Indent indent) const;
  
  /**
   * Pointer to an element, which holds the DOF that is affected
   * by boundary condition.
   */
  Element::ConstPointer m_Element;

  /**
   * Local DOF number within the Element object.
   */
  unsigned int m_DegreeOfFreedom;

  /**
   * Value which the DOF is being fixed.
   *
   * \note This is a vector so that implementation of BC on isotropic elements
   *       is easy. Which value is applied to the master force vector is
   *       defined by optional dim parameter (defaults to 0) in AssembleF
   *       function in solver.
   */
  LoadValueType m_Value;

};

FEM_CLASS_INIT(LoadBC)

}} // end namespace itk::fem

#endif // #ifndef __itkFEMLoadBC_h
