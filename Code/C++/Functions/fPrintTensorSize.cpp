/* =====================================
Aim: Setup sea floor
======================================*/ 

#pragma once

#include <torch/torch.h>

// function to print tensor size
// void fPrintTensorSize(const torch::Tensor& inputTensor) {
//     // Printing size
//     std::cout << "[";
//     for (const auto& size : inputTensor.sizes()) {
//         std::cout << size << ",";
//     }
//     std::cout << "\b]" <<std::endl;
// }

void fPrintTensorSize(const torch::Tensor& inputTensor) {
    // Printing size
    std::vector<int64_t> shapevector = inputTensor.sizes().vec();
    std::cout<<"["<<shapevector<<"]"<<std::endl;
}