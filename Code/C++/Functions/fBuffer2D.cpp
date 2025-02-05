/* =====================================
Aim: Convolving the columns of two input matrices
======================================*/ 
#include <stdexcept>
#include <torch/torch.h>

#pragma once

// hash-defines
#ifndef DEVICE
    // #define DEVICE          torch::kMPS
    #define DEVICE          torch::kCPU
#endif

// #define DEBUG_Buffer2D  true
#define DEBUG_Buffer2D  false


void fBuffer2D(torch::Tensor&  inputMatrix, 
                int frame_size){
    
    // ensuring the first dimension is 1.
    if(inputMatrix.size(0) != 1){
        throw std::runtime_error("fBuffer2D: The first-dimension must be 1 \n");
    }

    // padding with zeros in case it is not a perfect multiple 
    if(inputMatrix.size(1)%frame_size != 0){
        // padding with zeros
        int numberofzeroestoadd = frame_size - (inputMatrix.size(1) % frame_size);
        if(DEBUG_Buffer2D) {
            std::cout   << "\t\t\t fBuffer2D: frame_size = "                     << frame_size                << std::endl;
            std::cout   << "\t\t\t fBuffer2D: inputMatrix.sizes().vec() = "   << inputMatrix.sizes().vec() << std::endl;
            std::cout   << "\t\t\t fBuffer2D: numberofzeroestoadd = "         << numberofzeroestoadd       << std::endl;
        }

        // creating zero matrix
        torch::Tensor zeroMatrix = torch::zeros({inputMatrix.size(0),   \
                                                 numberofzeroestoadd,   \
                                                 inputMatrix.size(2)});
        if(DEBUG_Buffer2D) std::cout<<"\t\t\t fBuffer2D: zeroMatrix.sizes() = "<<zeroMatrix.sizes().vec()<<std::endl;

        // adding the zero matrix 
        inputMatrix = torch::cat({inputMatrix, zeroMatrix}, 1);
        if(DEBUG_Buffer2D) std::cout<<"\t\t\t fBuffer2D: inputMatrix.sizes().vec() = "<<inputMatrix.sizes().vec()<<std::endl;
    }

    // calculating some parameters
    // int num_frames = inputMatrix.size(1)/frame_size;
    int num_frames = std::ceil(inputMatrix.size(1)/frame_size);
    if(DEBUG_Buffer2D) std::cout << "\t\t\t fBuffer2D: inputMatrix.sizes = "<< inputMatrix.sizes().vec()<< std::endl;
    if(DEBUG_Buffer2D) std::cout << "\t\t\t fBuffer2D: framesize = "        << frame_size               << std::endl;
    if(DEBUG_Buffer2D) std::cout << "\t\t\t fBuffer2D: num_frames = "       << num_frames               << std::endl;

    // defining target shape and size
    std::vector<int64_t> target_shape   = {num_frames,                          \
                                           frame_size,                          \
                                           inputMatrix.size(2)};
    std::vector<int64_t> target_strides = {frame_size * inputMatrix.size(2),    \
                                           inputMatrix.size(2),                 \
                                           1};
    if(DEBUG_Buffer2D) std::cout << "\t\t\t fBuffer2D: STATUS: created shape and strides"<< std::endl;

    // creating the transformation
    inputMatrix = inputMatrix.as_strided(target_shape, target_strides);
    
}