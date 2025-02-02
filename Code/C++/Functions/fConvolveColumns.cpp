/* =====================================
Aim: Convolving the columns of two input matrices
======================================*/ 
#include <ratio>
#include <stdexcept>
#include <torch/torch.h>

#pragma once

// hash-defines
#define PI          3.14159265
#define MYDEBUGFLAG false

#ifndef DEVICE
    // #define DEVICE          torch::kMPS
    #define DEVICE          torch::kCPU
#endif


void fConvolveColumns(torch::Tensor& inputMatrix, \
                      torch::Tensor& kernelMatrix){


    // printing shape
    if(MYDEBUGFLAG) std::cout<<"inputMatrix.shape = ["<<inputMatrix.size(0)<<","<<inputMatrix.size(1)<<std::endl;
    if(MYDEBUGFLAG) std::cout<<"kernelMatrix.shape = ["<<kernelMatrix.size(0)<<","<<kernelMatrix.size(1)<<std::endl;

    // ensuring the two have the same number of columns
    if (inputMatrix.size(1) != kernelMatrix.size(1)){
        throw std::runtime_error("fConvolveColumns: arguments cannot have different number of columns");
    }
    

    // calculating length of final result
    int final_length = inputMatrix.size(0) + kernelMatrix.size(0) - 1; if(MYDEBUGFLAG) std::cout<<"\t\t\t fConvolveColumns: 27"<<std::endl;

    // calculating FFT of the two matrices
    torch::Tensor inputMatrix_FFT = torch::fft::fftn(inputMatrix, \
                                                     {final_length}, \
                                                     {0}); if(MYDEBUGFLAG) std::cout<<"\t\t\t fConvolveColumns: 32"<<std::endl;
    torch::Tensor kernelMatrix_FFT = torch::fft::fftn(kernelMatrix, \
                                                      {final_length}, \
                                                      {0}); if(MYDEBUGFLAG) std::cout<<"\t\t\t fConvolveColumns: 35"<<std::endl;

    // element-wise multiplying the two matrices
    torch::Tensor MulProduct = torch::mul(inputMatrix_FFT, kernelMatrix_FFT); if(MYDEBUGFLAG) std::cout<<"\t\t\t fConvolveColumns: 38"<<std::endl;

    // finding the inverse FFT
    torch::Tensor convolvedResult = torch::fft::ifftn(MulProduct, \
                                                      {MulProduct.size(0)}, \
                                                      {0}); if(MYDEBUGFLAG) std::cout<<"\t\t\t fConvolveColumns: 43"<<std::endl;

    // over-riding the result with the input so that we can save memory
    inputMatrix = convolvedResult; if(MYDEBUGFLAG) std::cout<<"\t\t\t fConvolveColumns: 46"<<std::endl;

}