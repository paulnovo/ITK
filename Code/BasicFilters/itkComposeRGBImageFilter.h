/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkComposeRGBImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkComposeRGBImageFilter_h
#define __itkComposeRGBImageFilter_h

#include "itkTernaryFunctorImageFilter.h"
#include "itkRGBPixel.h"

namespace itk
{
namespace Function
{
template< class TInput >
class ComposeRGB
{
public:
  typedef RGBPixel< TInput > OutputType;
  ComposeRGB() {}
  ~ComposeRGB() {}
  bool operator!=(const ComposeRGB &) const
  {
    return false;
  }

  bool operator==(const ComposeRGB & other) const
  {
    return !( *this != other );
  }

  inline OutputType operator()(const TInput & R,
                               const TInput & G,
                               const TInput & B) const
  {
    OutputType rgbPixel;

    rgbPixel.Set(R, G, B);
    return rgbPixel;
  }
};
}

/** \class ComposeRGBImageFilter
 * \brief Implements pixel-wise composition of an RGB pixel from three scalar images.
 *
 * This filter receives three scalar images as input. Each image containing
 * one of the RGB components of a color image. The filter produces as output an
 * RGB image in which the three components have been unified. The Component
 * type is preserved from the PixelType of the input images.
 *
 * \ingroup IntensityImageFilters
 */

template< typename TInputImage,
          typename TOutputImage =
            Image< RGBPixel< ITK_TYPENAME TInputImage::PixelType >,
                   ::itk::GetImageDimension< TInputImage >::ImageDimension > >
class ITK_EXPORT ComposeRGBImageFilter:
  public
  TernaryFunctorImageFilter< TInputImage, TInputImage,
                             TInputImage, TOutputImage,
                             Function::ComposeRGB< ITK_TYPENAME TInputImage::PixelType >   >
{
public:
  /** Standard class typedefs. */
  typedef ComposeRGBImageFilter Self;
  typedef TernaryFunctorImageFilter<
    TInputImage, TInputImage,
    TInputImage, TOutputImage,
    Function::ComposeRGB<
      ITK_TYPENAME TInputImage::PixelType > > Superclass;

  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  typedef typename Superclass::OutputImageType OutputImageType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(ComposeRGBImageFilter,
               TernaryFunctorImageFilter);
protected:
  ComposeRGBImageFilter() {}
  virtual ~ComposeRGBImageFilter() {}
private:
  ComposeRGBImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);        //purposely not implemented
};
} // end namespace itk

#endif