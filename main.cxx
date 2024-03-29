#include <iostream>
#include <cstring>

#include <itkImage.h>
#include <itkImageFileReader.h> 
#include <itkImageFileWriter.h>
#include <itkImageRegionIterator.h>
#include <itkImageRegionConstIterator.h>

const int Dimension = 3;
typedef unsigned short ImagePixelType;
typedef itk::Image<ImagePixelType,Dimension> ImageType;
typedef itk::ImageFileReader<ImageType> VolumeReaderType;
typedef itk::ImageFileWriter<ImageType> VolumeWriterType;
typedef itk::ImageRegionIterator<ImageType> IteratorType;
typedef itk::ImageRegionConstIterator<ImageType> ConstIteratorType;


int main (int argc, char *argv[])
{
  if (argc != 6)
    {
      std::cout<<"Test <rest> <white> <gray> <csf> <outputLabelImage>"<<std::endl;
      exit(EXIT_FAILURE);
    }
  else
    {
      ImageType::Pointer m_restImage, m_whiteImage, m_grayImage, m_csfImage, m_outputImage;

      std::cout<<"Reading images..."<<std::endl;
  
      // Read rest proba map
      std::cout<<"\tReading rest image..."<<std::endl;
      VolumeReaderType::Pointer imageReader1 = VolumeReaderType::New();
      imageReader1->SetFileName(argv[1]);
      try
	{
	  imageReader1->Update() ;
	}
      catch (itk::ExceptionObject err)
	{
	  std::cerr<<"Exception object caught!"<<std::endl;
	  std::cerr<<err<<std::endl;
	  exit(EXIT_FAILURE);
	}
      m_restImage = imageReader1->GetOutput();

      // Read white proba map
      std::cout<<"\tReading white image..."<<std::endl;
      VolumeReaderType::Pointer imageReader2 = VolumeReaderType::New();
      imageReader2->SetFileName(argv[2]);
      try
	{
	  imageReader2->Update() ;
	}
      catch (itk::ExceptionObject err)
	{
	  std::cerr<<"Exception object caught!"<<std::endl;
	  std::cerr<<err<<std::endl;
	  exit(EXIT_FAILURE);
	}
      m_whiteImage = imageReader2->GetOutput();

      // Read gray proba map
      std::cout<<"\tReading gray image..."<<std::endl;
      VolumeReaderType::Pointer imageReader3 = VolumeReaderType::New();
      imageReader3->SetFileName(argv[3]);
      try
	{
	  imageReader3->Update() ;
	}
      catch (itk::ExceptionObject err)
	{
	  std::cerr<<"Exception object caught!"<<std::endl;
	  std::cerr<<err<<std::endl;
	  exit(EXIT_FAILURE);
	}
      m_grayImage = imageReader3->GetOutput();

      // Read csf proba map
      std::cout<<"\tReading csf image..."<<std::endl;
      VolumeReaderType::Pointer imageReader4 = VolumeReaderType::New();
      imageReader4->SetFileName(argv[4]);
      try
	{
	  imageReader4->Update() ;
	}
      catch (itk::ExceptionObject err)
	{
	  std::cerr<<"Exception object caught!"<<std::endl;
	  std::cerr<<err<<std::endl;
	  exit(EXIT_FAILURE);
	}
      m_csfImage = imageReader4->GetOutput();
      
      // Allocating output image
      m_outputImage = ImageType::New();
      m_outputImage->SetRegions(m_whiteImage->GetRequestedRegion());
      m_outputImage->SetSpacing(m_whiteImage->GetSpacing());
      m_outputImage->Allocate();
    
      // Computing Label Map
      ConstIteratorType ConstrestInputIterator(m_restImage,m_restImage->GetRequestedRegion());
      ConstIteratorType ConstwhiteInputIterator(m_whiteImage,m_whiteImage->GetRequestedRegion());
      ConstIteratorType ConstgrayInputIterator(m_grayImage,m_grayImage->GetRequestedRegion());
      ConstIteratorType ConstcsfInputIterator(m_csfImage,m_csfImage->GetRequestedRegion());
      IteratorType OutputIterator(m_outputImage, m_whiteImage->GetRequestedRegion());
    
      ConstrestInputIterator.GoToBegin();
      ConstwhiteInputIterator.GoToBegin();
      ConstgrayInputIterator.GoToBegin();
      ConstcsfInputIterator.GoToBegin();
      OutputIterator.GoToBegin();      
      while (!ConstrestInputIterator.IsAtEnd())
	{
	  ImagePixelType Max = ConstrestInputIterator.Get();
	  ImagePixelType Label = 0;

	  if (ConstwhiteInputIterator.Get() > Max)
	    {
	      Max = ConstwhiteInputIterator.Get();
	      Label = 1;
	    }
	  if (ConstgrayInputIterator.Get() > Max)
	    {
	      Max = ConstgrayInputIterator.Get();
	      Label = 2;
	    }
	  if (ConstcsfInputIterator.Get() > Max)
	    {
	      Max = ConstcsfInputIterator.Get();
	      Label = 3;
	    } 
	  
	  
	  OutputIterator.Set(Label);
	  ++ConstrestInputIterator;
	  ++ConstwhiteInputIterator;
	  ++ConstgrayInputIterator;
	  ++ConstcsfInputIterator;
	  ++OutputIterator;
	}    

      std::cout<<"Writing image..."<<std::endl;  
      VolumeWriterType::Pointer imageWriter = VolumeWriterType::New();
      imageWriter->SetFileName(argv[5]);
      imageWriter->SetInput(m_outputImage);
      try
	{
	  imageWriter->Update();
	}
      catch (itk::ExceptionObject & err)
	{
	  std::cerr<<"Exception object caught!"<<std::endl;
	  std::cerr<<err<<std::endl;
	  exit(EXIT_FAILURE);
	}      
    }  
  return EXIT_SUCCESS;
}
