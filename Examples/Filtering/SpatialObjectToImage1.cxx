/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    SpatialObjectToImage1.cxx
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

//  Software Guide : BeginLatex
//
//  This example illustrates the use of the
//  \doxygen{SpatialObjectToImageFilter}. This filter expect a
//  \doxygen{SpatialObject} as input, and rasterize it in order to generate an
//  output image. This is particularly useful for generating synthetic images,
//  in particular binary images containing a mask.
//  
//  \index{itk::SpatialObjectToImageFilter|textbf}
//
//  Software Guide : EndLatex 

//  Software Guide : BeginLatex
//
//  The first step required for using this filter is to include its header file
//
//  \index{itk::SpatialObjectToImageFilter!header}
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
#include "itkSpatialObjectToImageFilter.h"
// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  This filter takes as input a SpatialObject. However, SpatialObject can be
//  grouped together in a hierarchical structure in order to produce more
//  complex shapes. In this case, we illustrate how to aggregate multiple basic
//  shapes. We should, therefore, include the headers of the individual elementary
//  SpatialObjects.
//
//  \index{itk::EllipseSpatialObject!header}
//  \index{itk::CylinderSpatialObject!header}
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
#include "itkEllipseSpatialObject.h"
#include "itkCylinderSpatialObject.h"
// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  Then we include the header of the \doxygen{GroupSpatialObject} that will
//  group together these instances of SpatialObjects.
//
//  \index{itk::GroupSpatialObject!header}
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "itkGroupSpatialObject.h"
// Software Guide : EndCodeSnippet


#include "itkImageFileWriter.h"


int main( int argc, char *argv[] )
{
  if( argc != 2 )
    {
    std::cerr << "Usage: " << argv[0] << " outputimagefile " << std::endl;
    return EXIT_FAILURE;
    }


  //  Software Guide : BeginLatex
  //
  //  We declare the pixel type and dimension of the image to be produced as
  //  output.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef signed short  PixelType;
  const unsigned int    Dimension = 3;

  typedef itk::Image< PixelType, Dimension >       ImageType;
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  Using the same dimension, we instantiate the types of the elementary
  //  SpatialObjects that we plan to group, and we instantiate as well the type
  //  of the SpatialObject that will hold the group together.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::EllipseSpatialObject< Dimension >   EllipseType;
  typedef itk::CylinderSpatialObject               CylinderType;
  typedef itk::GroupSpatialObject< Dimension >     GroupType;
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  We instantiate the SpatialObjectToImageFilter type by using as template
  //  arguments the input SpatialObject and the output image types.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::SpatialObjectToImageFilter< 
    GroupType, ImageType >   SpatialObjectToImageFilterType;

  SpatialObjectToImageFilterType::Pointer imageFilter =
    SpatialObjectToImageFilterType::New();
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  The SpatialObjectToImageFilter requires that the user defines the grid
  //  parameters of the output image. This includes the number of pixels along
  //  each dimension, the pixel spacing, image direction and 
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ImageType::SizeType size;
  size[ 0 ] = 31;
  size[ 1 ] = 31;
  size[ 2 ] = 100;

  imageFilter->SetSize( size );
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginCodeSnippet
  ImageType::SpacingType spacing;
  spacing[0] =  100.0 / size[0];
  spacing[1] =  150.0 / size[1];
  spacing[2] = 1000.0 / size[2];

  imageFilter->SetSpacing( spacing );
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  We create the elementary shapes that are going to be composed into the
  //  group spatial objects.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  EllipseType::Pointer ellipse    = EllipseType::New();
  CylinderType::Pointer cylinder1 = CylinderType::New();
  CylinderType::Pointer cylinder2 = CylinderType::New();
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  The Elementary shapes have internal parameters of their own. These
  //  parameters define the geometrical characteristics of the basic shapes.
  //  For example, a cylinder is defined by its radius and height.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ellipse->SetRadius(  size[0] * 0.2 * spacing[0] );

  cylinder1->SetRadius(  size[0] * 0.2 * spacing[0] );
  cylinder2->SetRadius(  size[0] * 0.2 * spacing[0] );

  cylinder1->SetHeight( size[2] * 0.45 * spacing[2]);
  cylinder2->SetHeight( size[2] * 0.45 * spacing[2]);
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  Each one of these components will be placed in a different position and
  //  orientation. We define transforms in order to specify those relative
  //  positions and orientations. 
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef GroupType::TransformType                 TransformType;

  TransformType::Pointer transform1 = TransformType::New();
  TransformType::Pointer transform2 = TransformType::New();
  TransformType::Pointer transform3 = TransformType::New();

  transform1->SetIdentity();
  transform2->SetIdentity();
  transform3->SetIdentity();
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  Then we set the specific values of the transform parameters, and we
  //  assign the transforms to the elementary shapes.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  TransformType::OutputVectorType  translation;
  TransformType::CenterType        center;

  translation[ 0 ] =  size[0] * spacing[0] / 2.0;
  translation[ 1 ] =  size[1] * spacing[1] / 4.0;
  translation[ 2 ] =  size[2] * spacing[2] / 2.0;
  transform1->Translate( translation, false );
 
  translation[ 1 ] =  size[1] * spacing[1] / 2.0;
  translation[ 2 ] =  size[2] * spacing[2] * 0.22;
  transform2->Rotate( 1, 2, vnl_math::pi / 2.0 );
  transform2->Translate( translation, false );

  translation[ 2 ] = size[2] * spacing[2] * 0.78;
  transform3->Rotate( 1, 2, vnl_math::pi / 2.0 );
  transform3->Translate( translation, false );

  ellipse->SetObjectToParentTransform( transform1 );
  cylinder1->SetObjectToParentTransform( transform2 );
  cylinder2->SetObjectToParentTransform( transform3 );
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  The elementary shapes are aggregated in a parent group, that in turn is
  //  passed as input to the filter.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  GroupType::Pointer group = GroupType::New();
  group->AddSpatialObject( ellipse );
  group->AddSpatialObject( cylinder1 );
  group->AddSpatialObject( cylinder2 );

  imageFilter->SetInput(  group  );
  // Software Guide : EndCodeSnippet



  ellipse->SetDefaultInsideValue(   800.0 );
  cylinder1->SetDefaultInsideValue( 800.0 );
  cylinder2->SetDefaultInsideValue( 800.0 );

  ellipse->SetDefaultOutsideValue(   -1000.0 );
  cylinder1->SetDefaultOutsideValue( -1000.0 );
  cylinder2->SetDefaultOutsideValue( -1000.0 );

  imageFilter->SetUseObjectValue( true );

  imageFilter->SetOutsideValue( -1000.0 );

  typedef itk::ImageFileWriter< ImageType >     WriterType;
  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName( argv[1] );
  writer->SetInput( imageFilter->GetOutput() );

  try
    {
    imageFilter->Update();
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}