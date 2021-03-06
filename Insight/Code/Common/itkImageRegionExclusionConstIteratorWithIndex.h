/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageRegionExclusionConstIteratorWithIndex.h,v $
  Language:  C++
  Date:      $Date: 2008-10-18 21:13:25 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageRegionExclusionConstIteratorWithIndex_h
#define __itkImageRegionExclusionConstIteratorWithIndex_h

#include "itkImageConstIteratorWithIndex.h"

namespace itk
{

/** \class ImageRegionExclusionConstIteratorWithIndex
 *
 *  \brief Multi-dimensional image iterator that walks an image region,
 *  excluding a second region contained within the first.
 * 
 * ImageRegionExclusionConstIteratorWithIndex is a templated class to represent
 * a multi-dimensional iterator. ImageRegionExclusionConstIteratorWithIndex is
 * templated over the image type.  ImageRegionExclusionConstIteratorWithIndex
 * is constrained to walk only within the specified region. The exclusion
 * region is set after construction. By default the exclusion region is empty
 * and the iterator will behave as the itk::ImageRegionIteratorWithIndex with a
 * penalty in performance due to internal bounds checking.
 *
 * As with other ITK image iterators,
 * ImageRegionExclusionConstIteratorWithIndex requires more information be
 * specified before the iterator can be used than conventional
 * iterators. Whereas the std::vector::iterator from the STL only needs to be
 * passed a pointer to establish the iterator, the multi-dimensional image
 * iterator needs a pointer, the size of the buffer, the size of the region,
 * the start index of the buffer, and the start index of the region. To gain
 * access to this information, ImageRegionExclusionConstIteratorWithIndex holds
 * a reference to the image over which it is traversing.
 *
 * ImageRegionExclusionConstIteratorWithIndex assumes a particular layout of
 * the image data. The is arranged in a 1D array as if it were
 * [][][][slice][row][col] with Index[0] = col, Index[1] = row, Index[2] =
 * slice, etc.
 *
 * The operator++ method provides a simple syntax for walking around a region
 * of a multidimensional image. operator++ iterates across a row, constraining
 * the movement to within a region of image. When the iterator reaches the
 * boundary of the region along a row, the iterator automatically wraps to the
 * next row, starting at the first pixel in the row that is part of the
 * region. This allows for simple processing loops of the form:
 *
 * \example itkImageRegionExclusionConstIteratorWithIndex
 *
 * \code
 * 
 *  IteratorType it( image, image->GetRequestedRegion() );
 *
 *  it.SetExclusionRegion( exclusionRegion );
 *
 *  it.GoToBegin();
 *
 *  while( ! it.IsAtEnd() ) 
 *  {  
 *    it.Set( 100.0 + it.Get() );
 *    ++it;
 *  }
 *
 * \endcode
 *
 *  It also can be used for walking in the reverse direction like
 *
 * \code
 * 
 *  IteratorType it( image, image->GetRequestedRegion() );
 *
 *  it.SetExclusionRegion( exclusionRegion );
 *  it.GoToEnd();
 *
 *  while( !it.IsAtBegin() ) 
 *  {  
 *    it.Set( 100.0 );
 *    --it;
 *  }
 *
 * \endcode
 *
 * \par MORE INFORMATION
 * For a complete description of the ITK Image Iterators and their API, please
 * see the Iterators chapter in the ITK Software Guide.  The ITK Software Guide
 * is available in print and as a free .pdf download from http://www.itk.org.
 *
 * \ingroup ImageIterators
 *
 * \sa ImageConstIterator \sa ConditionalConstIterator
 * \sa ConstNeighborhoodIterator \sa ConstShapedNeighborhoodIterator
 * \sa ConstSliceIterator  \sa CorrespondenceDataStructureIterator 
 * \sa FloodFilledFunctionConditionalConstIterator 
 * \sa FloodFilledImageFunctionConditionalConstIterator 
 * \sa FloodFilledImageFunctionConditionalIterator 
 * \sa FloodFilledSpatialFunctionConditionalConstIterator 
 * \sa FloodFilledSpatialFunctionConditionalIterator 
 * \sa ImageConstIterator \sa ImageConstIteratorWithIndex 
 * \sa ImageIterator \sa ImageIteratorWithIndex
 * \sa ImageLinearConstIteratorWithIndex  \sa ImageLinearIteratorWithIndex 
 * \sa ImageRandomConstIteratorWithIndex  \sa ImageRandomIteratorWithIndex 
 * \sa ImageRegionConstIterator \sa ImageRegionConstIteratorWithIndex 
 * \sa ImageRegionExclusionConstIteratorWithIndex 
 * \sa ImageRegionExclusionIteratorWithIndex 
 * \sa ImageRegionIterator  \sa ImageRegionIteratorWithIndex 
 * \sa ImageRegionReverseConstIterator  \sa ImageRegionReverseIterator 
 * \sa ImageReverseConstIterator  \sa ImageReverseIterator 
 * \sa ImageSliceConstIteratorWithIndex  \sa ImageSliceIteratorWithIndex 
 * \sa NeighborhoodIterator \sa PathConstIterator  \sa PathIterator 
 * \sa ShapedNeighborhoodIterator  \sa SliceIterator 
 * \sa ImageConstIteratorWithIndex */
template<typename TImage>
class ITK_EXPORT ImageRegionExclusionConstIteratorWithIndex : public ImageConstIteratorWithIndex<TImage>
{
public:
  /** Standard class typedefs. */
  typedef ImageRegionExclusionConstIteratorWithIndex    Self;
  typedef ImageConstIteratorWithIndex<TImage>           Superclass;
  
  /** Index typedef support. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc.
   * Note that we have to rescope Index back to itk::Index to that is it not
   * confused with ImageIterator::Index. */
  typedef typename TImage::IndexType            IndexType;
  typedef typename TImage::SizeType             SizeType;

  /** Image typedef support. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc.
   * Note that we have to rescope Image back to itk::Image to that is it not
   * confused with ImageIterator::Image. */
  typedef TImage                                ImageType;

  /** PixelContainer typedef support. Used to refer to the container for
   * the pixel data. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc. */
  typedef typename TImage::PixelContainer       PixelContainer;
  typedef typename PixelContainer::Pointer      PixelContainerPointer;
  
  /** Region typedef support. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc.
   * Note that we have to rescope Region back to itk::ImageRegion so that is
   * it not confused with ImageIterator::Index. */
  typedef typename TImage::RegionType RegionType;

  /** Default constructor. Needed since we provide a cast constructor. */
  ImageRegionExclusionConstIteratorWithIndex() : ImageConstIteratorWithIndex<TImage>() {}
  
  /** Constructor establishes an iterator to walk a particular image and a
   * particular region of that image. */
  ImageRegionExclusionConstIteratorWithIndex(const TImage *ptr,
                            const RegionType& region)
    : ImageConstIteratorWithIndex<TImage>(ptr, region) {}

  /** Constructor that can be used to cast from an ImageIterator to an
   * ImageRegionExclusionConstIteratorWithIndex. Many routines return an ImageIterator but for a
   * particular task, you may want an ImageRegionExclusionConstIteratorWithIndex.  Rather than
   * provide overloaded APIs that return different types of Iterators, itk
   * returns ImageIterators and uses constructors to cast from an
   * ImageIterator to a ImageRegionExclusionConstIteratorWithIndex. */
  ImageRegionExclusionConstIteratorWithIndex( const ImageConstIteratorWithIndex<TImage> &it)
    { this->ImageConstIteratorWithIndex<TImage>::operator=(it); }

  /** Increment (prefix) the fastest moving dimension of the iterator's index.
   * This operator will constrain the iterator within the region (i.e. the
   * iterator will automatically wrap from the end of the row of the region
   * to the beginning of the next row of the region) up until the iterator
   * tries to moves past the last pixel of the region.  Here, the iterator
   * will be set to be one pixel past the end of the region.
   * \sa operator++(int) */
  Self & operator++();

  /** Decrement (prefix) the fastest moving dimension of the iterator's index.
   * This operator will constrain the iterator within the region (i.e. the
   * iterator will automatically wrap from the beginning of the row of the region
   * to the end of the next row of the region) up until the iterator
   * tries to moves past the first pixel of the region.  Here, the iterator
   * will be set to be one pixel past the beginning of the region.
   * \sa operator--(int) */
  Self & operator--();


  /** Method to define the Exclusion region. The iterator will skip pixels 
   * inside this region. 
   * \warning The exclusion region must be completly containe inside the
   * normal region used to construct the iterator. A border of at least on
   * pixels should exist between the normal region and the exclusion region.
   */
  void SetExclusionRegion( const RegionType & region );

  /** Set the exclusion region to be inset one pixel in from the
   * region the iterator walks. This configures the iterator to only
   * walk the pixels on the boundary of the region.
   */
  void SetExclusionRegionToInsetRegion();
  
private:

  RegionType      m_ExclusionRegion;

  IndexType       m_ExclusionBegin;
  IndexType       m_ExclusionEnd;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageRegionExclusionConstIteratorWithIndex.txx"
#endif

#endif 
