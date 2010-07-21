/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMLoadBC.cxx,v $
  Language:  C++
  Date:      $Date: 2009-01-30 21:10:12 $
  Version:   $Revision: 1.9 $

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

#include "itkFEMLoadBC.h"

namespace itk {
namespace fem {


/****************** SetDegreeOfFreedom ******************/ 
void LoadBC::SetDegreeOfFreedom( int dof ) 
{ 
  m_DegreeOfFreedom= dof;
}

/****************** GetDegreeOfFreedom ******************/   
int LoadBC::GetDegreeOfFreedom( ) 
{ 
  return m_DegreeOfFreedom;
}
  

/****************** SetElement ******************/   
void LoadBC::SetElement(LoadElementConstPointerType element) 
{
  m_Element = element;
}
  
/****************** GetElement ******************/   
const LoadBC::LoadElementConstPointerType LoadBC::GetElement() 
{
  return m_Element;
}
  
/****************** SetValue ******************/   
void LoadBC::SetValue(LoadValueType value) 
{ 
  m_Value = value;
}
  
/****************** GetValue ******************/     
const LoadBC::LoadValueType LoadBC::GetValue() 
{ 
  return m_Value;
}
  
  
/** Read the LoadBC object from input stream */
void LoadBC::Read( std::istream& f, void* info )
{
  unsigned int n;
  /**
   * Convert the info pointer to a usable objects
   */
  ReadInfoType::ElementArrayPointer elements=static_cast<ReadInfoType*>(info)->m_el;


  /* first call the parent's Read function */
  Superclass::Read(f,info);

  /* read and set pointer to element that we're applying the load to */
  this->SkipWhiteSpace(f); f>>n; if(!f) goto out;
  try
    {
    this->m_Element=dynamic_cast<const Element*>( &*elements->Find(n) );
    }
  catch ( FEMExceptionObjectNotFound e )
    {
    throw FEMExceptionObjectNotFound(__FILE__,__LINE__,"LoadBC::Read()",e.m_baseClassName,e.m_GN);
    }
  
  /* read the local DOF number within that element */
  this->SkipWhiteSpace(f); f>>this->m_DegreeOfFreedom; if(!f) goto out;

  /* read the value to which the DOF is fixed */
  this->SkipWhiteSpace(f); f>>n; if(!f) goto out;
  this->m_Value.set_size(n);
  this->SkipWhiteSpace(f); f>>this->m_Value; if(!f) goto out;

  out:

  if( !f )
    {
    throw FEMExceptionIO(__FILE__,__LINE__,"LoadBC::Read()","Error reading FEM load!");
    }

}


/**
 * Write the LoadBC object to the output stream
 */
void LoadBC::Write( std::ostream& f ) const 
{
  /* first call the parent's write function */
  Superclass::Write(f);

  /*
   * Write the actual Load data
   */
  f<<"\t"<<this->m_Element->GetGlobalNumber()<<"\t% GN of element"<<"\n";
  f<<"\t"<<this->m_DegreeOfFreedom<<"\t% DOF# in element"<<"\n";

  /* write the value of dof */
  f<<"\t"<<this->m_Value.size();
  f<<" "<<this->m_Value<<"\t% value of the fixed DOF"<<"\n";

  /* check for errors */
  if (!f)
    {
    throw FEMExceptionIO(__FILE__,__LINE__,"LoadBC::Write()","Error writing FEM load!");
    }

}

/**
 * PrintSelf 
 */
void LoadBC::PrintSelf( std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Element: " << m_Element << std::endl;
  m_Element->Print(os, indent.GetNextIndent());
  os << indent << "Degree Of Freedom: " << m_DegreeOfFreedom << std::endl;
  os << indent << "Value: [";
  for (unsigned int i=0;i<m_Value.size();i++)
  {
    os << " " << m_Value[i];
  }
  os << "]" << std::endl;
}

FEM_CLASS_REGISTER(LoadBC)

}} // end namespace itk::fem
