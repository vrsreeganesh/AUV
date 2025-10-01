#pragma once
/*==============================================================================
------------------------------------------------------------------------------*/ 
template<typename T>
void fPrintVector(const vector<T> input){
    for(auto x: input) cout << x << ",";
    cout << endl;
}

template<typename T>
void fpv(vector<T> input){
    for(auto x: input) cout << x << ",";
    cout << endl;
}
/*==============================================================================
------------------------------------------------------------------------------*/ 
template<typename T>
void fPrintMatrix(const std::vector<std::vector<T>> input_matrix){
    for(const auto& row: input_matrix)
        cout << format("{}\n", row);
}
/*==============================================================================
------------------------------------------------------------------------------*/ 
template <typename T>
void fPrintMatrix(const string&                     input_string,
                  const std::vector<std::vector<T>> input_matrix){
    cout << format("{} = \n", input_string);
    for(const auto& row: input_matrix)
        cout << format("{}\n", row);
}
/*==============================================================================
------------------------------------------------------------------------------*/ 
template<typename T, typename T1>
void fPrintHashmap(unordered_map<T, T1> input){
    for(auto x: input){
        cout << format("[{},{}] | ", x.first, x.second);
    }
    cout <<endl;
}
/*==============================================================================
------------------------------------------------------------------------------*/ 
void fPrintBinaryTree(TreeNode* root){
    // sending it back
    if (root == nullptr) return;
    
    // printing
    PRINTLINE
    cout << "root->val = " << root->val << endl;
    
    // calling the children
    fPrintBinaryTree(root->left);
    fPrintBinaryTree(root->right);
    
    // returning
    return;
    
}
/*==============================================================================
------------------------------------------------------------------------------*/ 
void fPrintLinkedList(ListNode* root){
    if (root == nullptr) return;
    cout << root->val << " -> ";
    fPrintLinkedList(root->next);
    return;
}
/*==============================================================================
------------------------------------------------------------------------------*/ 
template<typename T>
void fPrintContainer(T input){
    for(auto x: input) cout << x << ", ";
    cout << endl;
    return;
}
/*==============================================================================
------------------------------------------------------------------------------*/ 
template <typename T>
auto size(std::vector<std::vector<T>> inputMatrix){
    cout << format("[{}, {}]\n", 
                   inputMatrix.size(), 
                   inputMatrix[0].size());
}
/*==============================================================================
------------------------------------------------------------------------------*/ 
template <typename T>
auto size(const     std::string&                    inputstring, 
          const     std::vector<std::vector<T>>&    inputMatrix){
    cout << format("{} = [{}, {}]\n", 
                   inputstring, 
                   inputMatrix.size(), 
                   inputMatrix[0].size());
}