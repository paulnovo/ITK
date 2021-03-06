/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkForwardFFTImageFilter_hxx
#define __itkForwardFFTImageFilter_hxx
#include "itkMetaDataObject.h"

#include "itkVnlForwardFFTImageFilter.h"

#if defined( USE_FFTWD ) || defined( USE_FFTWF )
#include "itkFFTWForwardFFTImageFilter.h"
#endif

namespace itk
{

template< typename TSelfPointer, class TInputImage, class TOutputImage, typename TPixel >
struct DispatchFFTW_Forward_New
{
  static TSelfPointer Apply()
    {
      return VnlForwardFFTImageFilter< TInputImage, TOutputImage >
        ::New().GetPointer();
    }
};

#ifdef USE_FFTWD
template< typename TSelfPointer, class TInputImage, class TOutputImage >
struct DispatchFFTW_Forward_New< TSelfPointer, TInputImage, TOutputImage, double >
{
  static TSelfPointer Apply()
    {
      return FFTWForwardFFTImageFilter< TInputImage, TOutputImage >
        ::New().GetPointer();
    }
};
#endif

#ifdef USE_FFTWF
template< typename TSelfPointer, class TInputImage, class TOutputImage >
struct DispatchFFTW_Forward_New< TSelfPointer, TInputImage, TOutputImage, float >
{
  static TSelfPointer Apply()
    {
      return FFTWForwardFFTImageFilter< TInputImage, TOutputImage >
        ::New().GetPointer();
    }
};
#endif

template< class TInputImage, class TOutputImage >
typename ForwardFFTImageFilter< TInputImage, TOutputImage >::Pointer
ForwardFFTImageFilter< TInputImage, TOutputImage >
::New(void)
{
  Pointer smartPtr = ::itk::ObjectFactory< Self >::Create();

  if ( smartPtr.IsNull() )
    {
    smartPtr = DispatchFFTW_Forward_New< Pointer, TInputImage, TOutputImage,
                                         typename NumericTraits< OutputPixelType >::ValueType >
      ::Apply();
    }

  return smartPtr;
}

template< class TInputImage, class TOutputImage >
void
ForwardFFTImageFilter< TInputImage, TOutputImage >
::GenerateInputRequestedRegion()
{
  // Call the superclass implementation of this method.
  Superclass::GenerateInputRequestedRegion();

  // Get pointer to the input.
  typename InputImageType::Pointer input  =
    const_cast< InputImageType * >( this->GetInput() );

  if ( !input )
    {
    return;
    }

  input->SetRequestedRegionToLargestPossibleRegion();
}

template< class TInputImage, class TOutputImage >
void
ForwardFFTImageFilter< TInputImage, TOutputImage >
::EnlargeOutputRequestedRegion(DataObject *output)
{
  Superclass::EnlargeOutputRequestedRegion(output);
  output->SetRequestedRegionToLargestPossibleRegion();
}
}
#endif
