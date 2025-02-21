/* =====================================
Aim: Convolving two signals
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


void fConvolve1D(torch::Tensor& inputMatrix, \
                 torch::Tensor& kernelMatrix){

    // calculating length of final result
    int final_length = inputMatrix.size(0) + kernelMatrix.size(0) - 1;
    if (MYDEBUGFLAG) std::cout << "\t\t\t fConvolve1D: line 25" << std::endl;

    // converting to float since FFT only works with float
    inputMatrix  = inputMatrix.to(torch::kFloat);
    kernelMatrix = kernelMatrix.to(torch::kFloat);

    // calculating FFT of the two matrices
    torch::Tensor inputMatrix_FFT   = torch::fft::fftn(inputMatrix,     {final_length}, {0});
    torch::Tensor kernelMatrix_FFT  = torch::fft::fftn(kernelMatrix,    {final_length}, {0});
    if (MYDEBUGFLAG) std::cout << "\t\t\t fConvolve1D: line 28" << std::endl;

    // element-wise multiplying the two matrices
    torch::Tensor MulProduct = torch::mul(inputMatrix_FFT, kernelMatrix_FFT);
    if (MYDEBUGFLAG) std::cout << "\t\t\t fConvolve1D: line 34" << std::endl;

    // finding the inverse FFT
    torch::Tensor convolvedResult = torch::fft::ifftn(MulProduct, \
                                                      {MulProduct.size(0)}, \
                                                      {0});
    if (MYDEBUGFLAG) std::cout << "\t\t\t fConvolve1D: line 40" << std::endl;

    // converting back to required data-type
    inputMatrix  = inputMatrix.to(DATATYPE);
    kernelMatrix = kernelMatrix.to(DATATYPE);

    // over-riding the result with the input so that we can save memory
    inputMatrix = convolvedResult;

}