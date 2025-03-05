#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

vector<vector<int>> createAmatrix(const vector<int>& sourceNodes, const vector<int>& destinationNodes);

vector<vector<int>> deleteTHErow(const vector<int>& sourceNodes, const vector<vector<int>> A);

int main(){
    //GIVEN ARRAYS
    int sourceNodes[] = {1, 1, 2, 2, 3, 3, 4, 4, 5};
    int destinationNodes[] = {0, 2, 0, 3, 0, 4, 0, 5, 0};

    //CONVERT TO VECTORS (MUST BE DONE IN MAIN)
    vector<int> sourceNodesV(begin(sourceNodes), end(sourceNodes));
    vector<int> destinationNodesV(begin(destinationNodes), end(destinationNodes));

    vector<vector<int>> A = createAmatrix(sourceNodesV, destinationNodesV);

    vector<vector<int>> Aa = deleteTHErow(sourceNodesV, A);


    return 0;
}

vector<vector<int>> createAmatrix(const vector<int>& sourceNodes, const vector<int>& destinationNodes){
    //find length of array
    int length = sourceNodes.size();

    //find the largest Node value
    int max;
    for(int i = 0; i < length; i++){
        if(sourceNodes[i + 1] > sourceNodes[sourceNodes[i]]){
            max = sourceNodes[i];
        }
    }
    //cout << "max: " << max << endl;

    //create dimensions for matrix A
    int row_num = max + 1;
    int col_num = length;

    //create matrix A
    vector<vector<int>> A(row_num, vector<int>(col_num, 0));

    //print matrix of zeros (mainly for debugging)
    cout << "zeros matrix:" << endl;
    for (int i = 0; i < row_num; i++) {
        for (int j = 0; j < col_num; j++) {
            std::cout << A[i][j] << " ";
        }
        std::cout << std::endl;
    }

    //create matrix A
    for (int i = 0; i < col_num; i++){ // col_num used to be row_num
        A[sourceNodes[i]][i] = 1; //is a source node
        A[destinationNodes[i]][i] = -1; // is a destination node
    }
    
    //print matrix A
    cout << "Matrix A:" << endl;
    for (int i = 0; i < row_num; i++) {
        for (int j = 0; j < col_num; j++) {
            std::cout << A[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return A;
}

vector<vector<int>> deleteTHErow(const vector<int>& sourceNodes, const vector<vector<int>> A){
    
    //similar to the last function
    int count = 0;
    int length = sourceNodes.size();

    int max;
    for(int i = 0; i < length; i++){
        if(sourceNodes[i + 1] > sourceNodes[sourceNodes[i]]){
            max = sourceNodes[i];
        }
    }
    int row_num = max + 1;
    int col_num = length;

    //create Aa
    vector<vector<int>> Aa((row_num - 1), vector<int>((col_num - 1), 0));

    // find what row we need to get rid of (assume we only need to get rid of one row)
    int deleteRow;
    for (int i = 0; i < row_num; i++) {
        count = 0;
        for (int j = 0; j < col_num; j++) {
            if (A[i][j] == -1){
                count += 1;
            }
            if (count == 2){
                deleteRow = i;
            }
        }
    }


    cout << endl << "row to delete: " << deleteRow << endl;
    //delete the row by adding into new matrix
    int index = 0;
    for(int row = 0; row < row_num; row++){
        for(int col = 0; col < col_num; col++){
            if(row == deleteRow){
                index = 1;
            }
            else if(row != deleteRow){
                Aa[row - index][col] = A[row][col];
            }
        }
    }

    //print matrix Aa
    cout << "Matrix Aa:" << endl;
    for (int i = 0; i < row_num - 1; i++) {
        for (int j = 0; j < col_num; j++) {
            std::cout << Aa[i][j] << " ";
            }
            std::cout << std::endl;
        }

    return Aa;
}
