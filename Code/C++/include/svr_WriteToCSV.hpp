#pragma once
/*==============================================================================
writing the contents of a vector a csv-file
------------------------------------------------------------------------------*/ 
template <typename T>
void fWriteVector(const vector<T>&              inputvector, 
                  const string&                 filename){
    
    spdlog::warn(
        "File-write is taking place. Latency will be affected. Ignore if intentional"
    );

    // opening a file
    std::ofstream fileobj(filename);
    if (!fileobj)   {return;}

    // writing the real parts in the first column and the imaginary parts int he second column
    if constexpr(std::is_same_v<T, std::complex<double>>    || 
                 std::is_same_v<T, std::complex<float>>     || 
                 std::is_same_v<T, std::complex<long double>>){
        for(int i = 0;  i<inputvector.size(); ++i){
            // adding entry
            fileobj << inputvector[i].real() << "+" << inputvector[i].imag() << "i";

            // adding delimiter
            if(i!=inputvector.size()-1) {fileobj << ",";}
            else                        {fileobj << "\n";}
        }
    }
    else{
        for(int i = 0; i<inputvector.size(); ++i){
            fileobj << inputvector[i];
            if(i!=inputvector.size()-1) {fileobj << ",";}
            else                        {fileobj << "\n";}
        }
    }

    // return
    return;
}
/*==============================================================================
writing the contents of a matrix to a csv-file
------------------------------------------------------------------------------*/ 
template <typename T>
auto fWriteMatrix(const std::vector<std::vector<T>> inputMatrix, 
                  const string                      filename){

    // opening a file 
    std::ofstream fileobj(filename);

    // writing
    if (fileobj){
        for(int i = 0; i<inputMatrix.size(); ++i){
            for(int j = 0; j<inputMatrix[0].size(); ++j){
                fileobj << inputMatrix[i][j];
                if (j!=inputMatrix[0].size()-1)     {fileobj << ",";}
                else                                {fileobj << "\n";}
            }
        }
    }
    else{
        cout << format("File-write to {} failed\n", filename);
    }

}
/*==============================================================================
writing complex-matrix to a csv-file
------------------------------------------------------------------------------*/ 
template <>
auto fWriteMatrix(const std::vector<std::vector<std::complex<double>>>  inputMatrix, 
                  const string                                          filename){

    // opening a file 
    std::ofstream fileobj(filename);

    // writing
    if (fileobj){
        for(int i = 0; i<inputMatrix.size(); ++i){
            for(int j = 0; j<inputMatrix[0].size(); ++j){
                fileobj << inputMatrix[i][j].real() << "+" << inputMatrix[i][j].imag() << "i";
                if (j!=inputMatrix[0].size()-1)     {fileobj << ",";}
                else                                {fileobj << "\n";}
            }
        }
    }
    else{
        cout << format("File-write to {} failed\n", filename);
    }
}