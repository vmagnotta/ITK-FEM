/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMLoadBCMFC.h,v $
  Language:  C++
  Date:      $Date: 2009-01-30 21:10:13 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFEMLoadBCMFC_h
#define __itkFEMLoadBCMFC_h

#include "itkFEMLoadBase.h"

namespace itk {
namespace fem {

/**
 * \class LoadBCMFC
 * \brief Generic linear multi freedom displacement constraint in global coordinate system.
 *
 * These constraints are implemented using the Lagrange multiplier method.
 * We treat displacement constraints (or essential boundary conditions, which are
 * exactly the same but with less functionality) as a special kind of load on the system.
 * 
 * How to store data in a LoadBCMFC object... Suppose you want to impose the following
 * constraint to the system:
 *
 *     0.5*u1x + 2.1*u5y = 10.0
 *
 * u1x is the first DOF in the element with global number 1, and u5y is the second DOF
 * in an element with GN=5.
 *
 * ... then use the following lines of code
 *        itk::LoadBCMFC m;
 *        m.lhs.push_back( LoadBCMFC::MFCTerm( elements.Find(1), 0, 0.5 ) );
 *        m.lhs.push_back( LoadBCMFC::MFCTerm( elements.Find(5), 1, 2.1 ) );
 *        m.rhs=10.0;
 */

// forward declaratons...
//class Solver;

class LoadBCMFC : public Load
{
  FEM_CLASS(LoadBCMFC,Load)
public:

  /**
   * \class MFCTerm
   * \brief Class that holds information about one term in MFC constraint equation.
   * \sa LoadBCMFC
   */
  class MFCTerm 
    {
    public:
    
      void SetDegreeOfFreedom( int dof ) { m_DegreeOfFreedom = dof;};
      int GetDegreeOfFreedom( ) const { return m_DegreeOfFreedom;};
      
      void SetElement(Element::ConstPointer element) {m_Element = element;};
      const Element::ConstPointer GetElement() const {return m_Element;};
      
      void SetValue(Element::Float cvalue) {m_Value = cvalue;};
      Element::Float GetValue() const { return m_Value;};

    /**
     * Constructor for easy object creation.
     */
      MFCTerm(Element::ConstPointer element_, int dof_, Element::Float value_) : m_Element(element_), m_DegreeOfFreedom(dof_), m_Value(value_) {}
    
    protected:
      /** Pointer to element, which holds the DOF that is affected by MFC*/
      Element::ConstPointer m_Element;
      /** DOF number within the Element object*/
      unsigned int m_DegreeOfFreedom;  
      /** Value with which this displacement is multiplied on the lhs of MFC equation */
      Element::Float m_Value;
    };
  
  /**
   * Left hand side of the MFC constraint equation
   */
  typedef std::vector<MFCTerm>        LhsType;
  typedef vnl_vector<Element::Float>  RhsType;

  

  /**
   * Right hand side of the linear equation that defines the constraints.
   * It is a vector so that implementation of BC on isotropic elements is easy.
   * Which value is applied to the master force vector is defined by optional
   * dim parameter (defaults to 0) in AssembleF function in solver.
   */
  //vnl_vector<Element::Float> rhs;

  /** Default constructor */
  LoadBCMFC() {}

  /**
   * With this constructor, we can easy fix the global
   * displacement dof given by node to a value val.
   *
   * \param element Pointer to an element, which holds a displacements that
   *                needs to be fixed.
   * \param dof Local DOF number in an element.
   * \param val The fixed value of a DOF.
   */
  LoadBCMFC(Element::ConstPointer element, int dof, vnl_vector<Element::Float> val);

  /** read a LoadBCMFC object from input stream. */
  virtual void Read( std::istream& f, void* info );

  /** write a LoadBCMFC object to the output stream. */
  virtual void Write( std::ostream& f ) const;
  
  void SetIndex(int value) {m_Index = value;};
  int GetIndex() {return m_Index;};
  
  void SetLeftHandSide(LhsType value) {m_LeftHandSide=value;};
  LhsType GetLeftHandSide() {return m_LeftHandSide;};
  
  void SetRightHandSide(vnl_vector<Element::Float> value) {m_RightHandSide = value;};
  vnl_vector<Element::Float> GetRightHandSide() {return m_RightHandSide;};
  

protected:
  void PrintSelf(std::ostream& os, Indent indent) const;

  int     m_Index;
  LhsType m_LeftHandSide;
  RhsType m_RightHandSide;
};

FEM_CLASS_INIT(LoadBCMFC)

}} // end namespace itk::fem

#endif // #ifndef __itkFEMLoadBCMFC_h
