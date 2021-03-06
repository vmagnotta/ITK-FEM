/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: NNetClassifierTest1.cxx,v $
  Language:  C++
  Date:      $Date: 2009-04-05 23:09:36 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
//#define USE_REVIEW_NETIO
#ifdef USE_REVIEW_NETIO
#include "itkNeuralNetworkFileReader.h"
#include "itkNeuralNetworkFileWriter.h"
#endif

#include "itkOneHiddenLayerBackPropagationNeuralNetwork.h"
#include "itkIterativeSupervisedTrainingFunction.h"
#include "itkBatchSupervisedTrainingFunction.h"
#include "itkVector.h"
#include "itkArray.h"
#include "itkListSample.h"
#include <vector>
#include <fstream>

#define ROUND(x) (floor(x+0.5))

typedef itk::Array<double> MeasurementVectorType;
typedef itk::Array<double> TargetVectorType;
typedef itk::Statistics::ListSample<TargetVectorType> TargetType;
typedef itk::Statistics::ListSample<MeasurementVectorType> SampleType;
typedef itk::Statistics::OneHiddenLayerBackPropagationNeuralNetwork<MeasurementVectorType, TargetVectorType> OneHiddenLayerBackPropagationNeuralNetworkType;


static int TestNetwork(SampleType::Pointer TestSample, TargetType::Pointer TestTargets,
  OneHiddenLayerBackPropagationNeuralNetworkType::Pointer OneHiddenLayerNetwork)
{
  //Network Simulation
  std::cout << TestSample->Size() << std::endl;
  std::cout << "Network Simulation" << std::endl;
  SampleType::ConstIterator iter1 = TestSample->Begin();
  TargetType::ConstIterator iter2 = TestTargets->Begin();
  unsigned int error1 = 0 ;
  unsigned int error2 = 0 ;
  int flag;
  std::ofstream outfile;
  outfile.open("out1.txt",std::ios::out);
  while (iter1 != TestSample->End())
    {
    MeasurementVectorType mv = iter1.GetMeasurementVector();
    TargetVectorType tv = iter2.GetMeasurementVector();
    TargetVectorType ov = OneHiddenLayerNetwork->GenerateOutput(mv);
    flag = 0;
    if (vcl_fabs(tv[0]-ov[0])>0.2)
      {
      outfile<<vcl_fabs(tv[0]-ov[0])<<std::endl;
      flag = 1;
      }
    if (flag == 1 && ROUND(tv[0]) == 1)
      {
      ++error1;
      }
    else if (flag == 1 && ROUND(tv[0]) == -1)
      {
      ++error2;
      }
    std::cout << "Network Input = " << mv << std::endl;
    std::cout << "Network Output = " << ov << std::endl;
    std::cout << "Target = " << tv << std::endl;
    ++iter1;
    ++iter2;
    }

  std::cout << "Among "<<TestSample->Size()<<" measurement vectors, " << error1 + error2
    << " vectors are misclassified." << std::endl ;
  std::cout<<"Network Weights and Biases after Training= "<<std::endl;
  std::cout << OneHiddenLayerNetwork << std::endl;
  if (double(error1 / 10) > 2 || double(error2 / 10) > 2)
    {
    std::cout << "Test failed." << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}

int
NNetClassifierTest1(int argc, char* argv[])
{
  if (argc < 2)
    {
    std::cout << "ERROR: data file name argument missing." << std::endl ;
    return EXIT_FAILURE;
    }

  int num_train=800;
  int num_test=200;

  char* trainFileName = argv[1]; //"train.txt"; //argv[1];
  char* testFileName = argv[2]; //"test.txt"; //argv[2];

  int num_input_nodes = 2;
  int num_hidden_nodes = 5;
  int num_output_nodes = 1;

  typedef itk::Statistics::BatchSupervisedTrainingFunction<SampleType, TargetType, double> TrainingFcnType;

  MeasurementVectorType mv;
  TargetVectorType tv;
  TargetVectorType ov;
  mv.SetSize(num_input_nodes);
  ov.SetSize(num_output_nodes);
  tv.SetSize(num_output_nodes);

  SampleType::Pointer trainsample = SampleType::New();
  SampleType::Pointer testsample = SampleType::New();
  TargetType::Pointer traintargets = TargetType::New();
  TargetType::Pointer testtargets = TargetType::New();
  trainsample->SetMeasurementVectorSize( num_input_nodes);
  traintargets->SetMeasurementVectorSize( num_output_nodes);
  testsample->SetMeasurementVectorSize( num_input_nodes);
  testtargets->SetMeasurementVectorSize( num_output_nodes);

  std::ifstream infile1;
  infile1.open(trainFileName, std::ios::in);

  for (int a = 0; a < num_train; a++)
    {
    for (int i = 0; i < num_input_nodes; i++)
      {
      infile1 >> mv[i];
      }
    infile1 >> tv[0];
    trainsample->PushBack(mv);
    traintargets->PushBack(tv);
    std::cout << "Input =" << mv << std::endl;
    std::cout << "target =" << tv << std::endl;
    }
  infile1.close();

  std::ifstream infile2;
  infile2.open(testFileName, std::ios::in);
  for (int a = 0; a < num_test; a++)
    {
    for (int i = 0; i < num_input_nodes; i++)
      {
      infile2 >> mv[i];
      }
    infile2 >> tv[0];
    testsample->PushBack(mv);
    testtargets->PushBack(tv);
    std::cout << "Input =" << mv << std::endl;
    std::cout << "target =" << tv << std::endl;
    }
  infile2.close();

  OneHiddenLayerBackPropagationNeuralNetworkType::Pointer OneHiddenLayerNet = OneHiddenLayerBackPropagationNeuralNetworkType::New();
  OneHiddenLayerNet->SetNumOfInputNodes(num_input_nodes);
  OneHiddenLayerNet->SetNumOfFirstHiddenNodes(num_hidden_nodes);
  OneHiddenLayerNet->SetNumOfOutputNodes(num_output_nodes);

  OneHiddenLayerNet->Initialize();
  OneHiddenLayerNet->InitializeWeights();
  OneHiddenLayerNet->SetLearningRate(0.001);

  TrainingFcnType::Pointer trainingfcn = TrainingFcnType::New();
  trainingfcn->SetIterations(200);
  trainingfcn->Train(OneHiddenLayerNet, trainsample, traintargets);
  int return_value1=TestNetwork(testsample,testtargets,OneHiddenLayerNet);
  int return_value2=EXIT_SUCCESS;

#ifdef USE_REVIEW_NETIO
    {//Test Reading and writing.
    typedef itk::Statistics::OneHiddenLayerBackPropagationNeuralNetwork<MeasurementVectorType, TargetVectorType> OneHiddenLayerBackPropagationNeuralNetworkType;
    std::string TestOneHiddenLayerNetFileName("/tmp/OneLayer.net");
    {
    typedef itk::NeuralNetworkFileWriter<OneHiddenLayerBackPropagationNeuralNetworkType> OHLWriterType;
    OHLWriterType::Pointer writerOneHiddenLayerBackPropagation=OHLWriterType::New();
    writerOneHiddenLayerBackPropagation->SetWriteWeightValuesType(OHLWriterType::ASCII);
    writerOneHiddenLayerBackPropagation->SetFileName(TestOneHiddenLayerNetFileName);
    writerOneHiddenLayerBackPropagation->SetInput(OneHiddenLayerNet);
    writerOneHiddenLayerBackPropagation->Update();
    }
    {
    typedef itk::NeuralNetworkFileReader<OneHiddenLayerBackPropagationNeuralNetworkType> OHLReaderType;
    OHLReaderType::Pointer readerOneHiddenLayerBackPropagation=OHLReaderType::New();
    readerOneHiddenLayerBackPropagation->SetFileName(TestOneHiddenLayerNetFileName);
    readerOneHiddenLayerBackPropagation->SetReadWeightValuesType( OHLReaderType::ASCII );
    readerOneHiddenLayerBackPropagation->Update();
    //The following line gives a compiler error
    OneHiddenLayerBackPropagationNeuralNetworkType::Pointer OneHiddenLayerNet_ReadIn = readerOneHiddenLayerBackPropagation->GetOutput();
    return_value2=TestNetwork(testsample,testtargets,OneHiddenLayerNet_ReadIn);
    }
    }
#endif
  if(return_value1 == EXIT_FAILURE || return_value2 == EXIT_FAILURE)
    {
    return EXIT_FAILURE;
    }
  std::cout << "Test passed." << std::endl;
  return EXIT_SUCCESS;
}
