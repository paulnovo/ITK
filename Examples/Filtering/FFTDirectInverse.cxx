/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    FFTDirectInverse.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

//
// This example was originally contributed by Stephan in the users list
//
//     http://public.kitware.com/pipermail/insight-users/2005-June/013482.html
//
//


// Software Guide : BeginLatex
//
// This example illustrates how to compute the direct Fourier transform
// followed by the inverse Fourier transform in order to recover the original
// data.
//
// Software Guide : EndLatex 

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "itkVnlFFTRealToComplexConjugateImageFilter.h"
#include "itkVnlFFTComplexConjugateToRealImageFilter.h"

int main( int argc, char * argv[] )
{
  if( argc != 3 ) 
    {
    std::cerr << "Usage: " << argv[0] << " input output" << std::endl;
    return EXIT_FAILURE;
    }
  
// Software Guide : BeginLatex
//
// First we set up the types of the input and output images.
//
// Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
  const   unsigned int      Dimension = 2;
  typedef unsigned short    IOPixelType;
  typedef float             WorkPixelType; 
  
  typedef itk::Image< IOPixelType,  Dimension >  IOImageType;
  typedef itk::Image< WorkPixelType, Dimension > WorkImageType;
  // Software Guide : EndCodeSnippet

  
  // File handling
  typedef itk::ImageFileReader< IOImageType > ReaderType;
  typedef itk::ImageFileWriter< IOImageType > WriterType;
  
  ReaderType::Pointer inputreader = ReaderType::New();
  ReaderType::Pointer kernelreader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  inputreader->SetFileName( argv[1] );
  writer->SetFileName( argv[2] );

  // Handle padding of the image with resampling
  typedef itk::ResampleImageFilter< 
                              IOImageType, 
                              WorkImageType >  ResamplerType;

  ResamplerType::Pointer inputresampler = ResamplerType::New();
  inputresampler->SetDefaultPixelValue(0);

  // Read the image and get its size
  inputreader->Update();

  IOImageType::SizeType     inputsize;
  IOImageType::SizeType     worksize;

  inputsize = inputreader->GetOutput()->GetLargestPossibleRegion().GetSize();

  // worksize is the nearest multiple of 2 larger than the input 
  for( unsigned int i=0; i < 2; i++ ) 
    {
    int n=0;
    worksize[i] = inputsize[i];
    while( worksize[i] >>= 1 ) 
      { 
      n++; 
      }
    worksize[i] = 1 << (n+1);

    std::cout << "inputsize[" << i << "]=" << inputsize[i] << std::endl;
    std::cout << "worksize[" << i << "]=" << worksize[i] << std::endl;
    }

  inputresampler->SetSize( worksize );
  inputresampler->SetInput( inputreader->GetOutput() );

  // Forward FFT filter
  typedef itk::VnlFFTRealToComplexConjugateImageFilter < 
                                              WorkPixelType, 
                                              Dimension > FFTFilterType;
  FFTFilterType::Pointer fftinput = FFTFilterType::New();
  fftinput->SetInput( inputresampler->GetOutput() );

  // This is the output type from the FFT filters
  typedef FFTFilterType::OutputImageType ComplexImageType;

  // Do the inverse transform = forward transform / num voxels
  typedef itk::VnlFFTComplexConjugateToRealImageFilter < 
                                              WorkPixelType, 
                                              Dimension > invFFTFilterType;
  invFFTFilterType::Pointer fftoutput = invFFTFilterType::New();
  fftoutput->SetInput(fftinput->GetOutput()); // try to recover the input image

  // undo the padding
  typedef itk::ResampleImageFilter<WorkImageType, IOImageType> ResampleOutType;
  ResampleOutType::Pointer outputResampler = ResampleOutType::New();
  outputResampler->SetDefaultPixelValue( 0 );
  outputResampler->SetSize( inputsize );
  outputResampler->SetInput( fftoutput->GetOutput() );

  // Write the output
  writer->SetInput(outputResampler->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}