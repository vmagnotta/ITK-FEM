/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMLoadBCMFC.cxx,v $
  Language:  C++
  Date:      $Date: 2009-01-30 21:10:12 $
  Version:   $Revision: 1.16 $

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

#include "itkFEMLoadBCMFC.h"

namespace itk {
namespace fem {

/**
 * Fix a DOF to a prescribed value
 */
LoadBCMFC::LoadBCMFC(Element::ConstPointer element, int dof, vnl_vector<Element::Float> val)
{
  m_LeftHandSide.clear();

  /** Set the correct weight */
  m_LeftHandSide.push_back( MFCTerm(element, dof, 1.0) );
  m_RightHandSide=val;
}

/** Read the LoadBCMFC object from input stream */
void LoadBCMFC::Read( std::istream& f, void* info )
{
  int nlhs, n;
  Node::Float d;
  /**
   * Convert the info pointer to a usable objects
   */
  ReadInfoType::ElementArrayPointer elements=static_cast<ReadInfoType*>(info)->m_el;


  /** first call the parent's Read function */
  Superclass::Read(f,info);

  /** read number of terms in lhs of MFC equation */
  this->SkipWhiteSpace(f); f>>nlhs; if(!f) goto out;
  
  m_LeftHandSide.clear();
  for(int i=0; i<nlhs; i++) 
    {
    /** read and set pointer to element that we're applying the load to */
    this->SkipWhiteSpace(f); f>>n; if(!f) goto out;
    Element::ConstPointer element;
    try
      {
      element=dynamic_cast<const Element*>( &*elements->Find(n));
      }
    catch ( FEMExceptionObjectNotFound e )
      {
      throw FEMExceptionObjectNotFound(__FILE__,__LINE__,"LoadBCMFC::Read()",e.m_baseClassName,e.m_GN);
      }

    /** read the number of dof within that element */
    this->SkipWhiteSpace(f); f>>n; if(!f) goto out;

    /** read weight */
    this->SkipWhiteSpace(f); f>>d; if(!f) goto out;

    /** add a new MFCTerm to the lhs */
    m_LeftHandSide.push_back( MFCTerm(element, n, d) );
    }

  /** read the rhs */
  this->SkipWhiteSpace(f); f>>n; if(!f) goto out;
  m_RightHandSide.set_size(n);
  this->SkipWhiteSpace(f); f>>m_RightHandSide; if(!f) goto out;

out:

  if( !f )
    {
    throw FEMExceptionIO(__FILE__,__LINE__,"LoadBCMFC::Read()","Error reading FEM load!");
    }

}


/**
 * Write the LoadBCMFC object to the output stream
 */
void LoadBCMFC::Write( std::ostream& f ) const 
{
  /** first call the parent's write function */
  Superclass::Write(f);

  /**
   * Write the actual Load data
   */

  /** write the number of DOFs affected by this MFC */
  f << "\t" << static_cast<int>( m_LeftHandSide.size() ) << "\t% Number of DOFs in this MFC" << std::endl;

  /** write each term */
  f << "\t  %==>\n";
  for(LhsType::const_iterator q=m_LeftHandSide.begin(); q != m_LeftHandSide.end(); q++) 
    {
    f << "\t  "<<q->GetElement()->GetGlobalNumber()<<"\t% GN of element" << std::endl;
    f << "\t  "<<q->GetDegreeOfFreedom()<<"\t% DOF# in element" << std::endl;
    f << "\t  "<<q->GetValue()<<"\t% weight" << std::endl;
    f << "\t  %==>\n";
    }

  /** write the rhs */
  f << "\t" << static_cast<int>( m_RightHandSide.size() );
  f << " "  << m_RightHandSide <<"\t% rhs of MFC" << std::endl;

  /** check for errors */
  if (!f)
    {
    throw FEMExceptionIO(__FILE__,__LINE__,"LoadBCMFC::Write()","Error writing FEM load!");
    }

}

/**
 * PrintSelf 
 */
void LoadBCMFC::PrintSelf( std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Index: " << m_Index << std::endl;
  os << indent << "Left Hand Side: " << std::endl;
  os << indent.GetNextIndent() << "Index, DOF, Element, Value" << std::endl; 
  int i=0;
  for(LoadBCMFC::LhsType::const_iterator q=m_LeftHandSide.begin();q != m_LeftHandSide.end();q++) 
    {
    os << indent.GetNextIndent() << i << ", "; 
    os << q->GetDegreeOfFreedom() << ", ";
    os << q->GetElement() << ", ";
    os << q->GetValue() << std::endl;
    i++;
    }  
  os << indent << "Right Hand Side:" << m_RightHandSide << std::endl;
}


FEM_CLASS_REGISTER(LoadBCMFC)

}} // end namespace itk::fem
