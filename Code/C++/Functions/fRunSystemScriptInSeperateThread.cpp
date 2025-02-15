/* =========================================================
Aim: Script to run python scripts on a separate thread
----------------------------------------------------------*/ 
#include <torch/torch.h>
#include <iostream>
#include <cstdlib>          // For terminal access
#include <stdlib.h>

#pragma once

void fRunSystemScriptInSeperateThread(std::string inputArgument){

    // running the script
    system(inputArgument.c_str());

    // returning
    return;
}