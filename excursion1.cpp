#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

 struct circuitData{
    vector<int> s;  // Source node array
    vector<int> d;  // Destination node array
    vector<vector<float>> v;  // Voltage array
    vector<vector<float>> r;  // Resistance array
    vector<string> labels;           //this creates an array that stores the branch names (to know voltage vs resistance)
    vector<vector<float>> data;              //this stores all other info (source and destination node and numeric component value)

 };


circuitData readFile (const string &file, circuitData &circuit){
    ifstream f(file);                // Open the text file 
    string label;
    float a,b,c;
   
    while (f >> label >> a >> b >> c) {    // Read each line of the file and extract the first value into a labels vector, and the next three values in an integer vector
      circuit.labels.push_back(label);  
      circuit.data.push_back({a, b, c});  
    } 

    f.close();
    return circuit;
  }
  
  circuitData processData(circuitData &circuit){
    int numBranches=circuit.labels.size();     //number of branches=number of labels 
    circuit.s.resize(numBranches);
    circuit.d.resize(numBranches);
    circuit.v.resize(numBranches);
    circuit.r.resize(numBranches);
    
    for (int i=0; i<numBranches; i++){
      circuit.s[i]=circuit.data[i][0];                       //fill source array with info from first column of data
      circuit.d[i]=circuit.data[i][1];                        //fill destination array with info from second column of data
      if(circuit.labels[i].find("V")!=string::npos){
        circuit.v[i][0]=circuit.data[i][2];
       circuit. r[i][0]=0;
       }else {
        circuit.v[i][0]=0;
        circuit.r[i][0]=circuit.data[i][2];
      }
  
    }

    return circuit;
}

void resultsInTxt (const vector<float> &data){      //function to print results in a text file
  ofstream file("output.txt");
  for ( size_t i = 0; i < data.size(); i++) {
      file << data[i];
      if (i != data.size() - 1) {
        file << " ";  // Add space between numbers
      }
    }
    file << endl;
    file.close();
}

  vector<vector<int>> createAmatrix(const vector<int>& sourceNodes, const vector<int>& destinationNodes){
    //find length of array
    int length = sourceNodes.size();

    //find the largest Node value between source AND destination
    int max_source = sourceNodes[0];
    for (int i = 1; i < sourceNodes.size(); i++) {
        if (sourceNodes[i] > max_source) {
            max_source = sourceNodes[i];
        }
    }

    int max_destination = destinationNodes[0];
    for (int i = 1; i < destinationNodes.size(); i++) {
        if (destinationNodes[i] > max_source) {
            max_source = destinationNodes[i];
        }
    }
   //Determine if the max is in the source or destination nodes
    int max;
        if (max_source > max_destination) {
             max = max_source;
        } 
        else {
            max = max_destination;
        }

    //create dimensions for matrix A
    int row_num = max + 1;
    int col_num = length;

    //create matrix A
    vector<vector<int>> A(row_num, vector<int>(col_num, 0));

    for (int i = 0; i < col_num; i++){ // col_num used to be row_num
        A[sourceNodes[i]][i] = 1; //is a source node
        A[destinationNodes[i]][i] = -1; // is a destination node
    }
    return A;
}

vector<vector<int>> deleteTHErow(const vector<int>& sourceNodes, const vector<vector<int>> &A){
    
    //get dimensions
    int row_num = A.size();
    int col_num = A[0].size();

    //create Aa (should have one less row that A)
    vector<vector<int>> Aa((row_num - 1), vector<int>((col_num), 0));

    // find what row we need to get rid of (assume we only need to get rid of one row)
    int deleteRow = -5;
    for (int i = 0; i < row_num; i++) {
        int count = 0;
        for (int j = 0; j < col_num; j++) {
            if (A[i][j] == -1){
                count += 1;
            }
            if (count == 2){
                deleteRow = i;
                break;
            }
        }
    }

    //check if there are no rows to get rid of
    if (deleteRow == -5) {
        return A;
    }

    //delete the row by adding into new matrix
    int newrow = 0;
    for(int row = 0; row < row_num; row++){
        if(row == deleteRow) continue;
        for(int col = 0; col < col_num; col++){
            if(row != deleteRow){
                Aa[newrow][col] = A[row][col];
            }
        }
        newrow++;
    }
    return Aa;
}


//zero matrix
void zeroMatrix(vector<vector<float>>& mat, int rows, int cols) {
    mat.assign(rows, vector<float>(cols, 0.0f));
}

//identity matrix
void identityMatrix(vector<vector<float>>& mat, int size) {
    mat.assign(size, vector<float>(size, 0.0f));
    for (int i = 0; i < size; i++) {
        mat[i][i] = 1.0f;
    }
}

void printMatrix(vector<vector<float>>& matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}
// Function to transpose matrix A (A^T)
vector<vector<int>> transposeMatrix(const vector<vector<int>>& mat) {
    int rows = mat.size();
    int cols = mat[0].size();
    vector<vector<int>> transpose(cols, vector<int>(rows));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            transpose[j][i] = mat[i][j];
        }
    }
    return transpose;
}

vector<vector<float>> constructBigMatrix(
    const vector<vector<int>>& A, 
    const vector<vector<float>>& N, 
    const vector<vector<float>>& u
) {
    int aRows = A.size();
    int aCols = A[0].size();
    int nRows = N.size();
    int nCols = N[0].size();
    int uRows = u.size(); // Number of rows in u
    
    // Total rows and columns for the big matrix
    int totalRows = aRows + aCols + uRows;
    int totalCols = aCols + nCols + nCols;

    // Initialize the big matrix with zeros
    vector<vector<float>> bigMatrix(totalRows, vector<float>(totalCols, 0.0f));
   
    cout << bigMatrix.size() <<endl;
    cout << bigMatrix[0].size() <<endl;

    // Fill Block 1: [0, 0, A, 0]
    for (int i = 0; i < aRows; i++) {
        for (int j = 0; j < aCols; j++) {
            bigMatrix[i][j + aRows + nCols] = A[i][j]; // Place A in the third block 
        }
    }

    // Fill Block 2: [-A^T, I, 0, 0]
    vector<vector<int>> A_T = transposeMatrix(A);  // Transpose of A
    for (int i = 0; i < aCols; i++) {
        for (int j = 0; j < aRows; j++) {
            bigMatrix[i + aRows][j] = -A_T[i][j]; // -A^T goes in the top-left block
        }
    }

    // Identity matrix I goes in the middle block 
    vector<vector<float>> identity;
    identityMatrix(identity, aCols);
    for (int i = 0; i < aCols; i++) {
        for (int j = 0; j < aCols; j++) {
            bigMatrix[i + aRows][j + aRows] = identity[i][j]; // Identity matrix goes in the middle
        }
    }

    // Fill Block 3: [0, M, N, u]
    // M is an identity matrix (size nCols x nCols)
    vector<vector<float>> M;
    identityMatrix(M, nCols); // Identity matrix M (size nCols x nCols)
    for (int i = 0; i < nCols; i++) {
        for (int j = 0; j < nCols; j++) {
            bigMatrix[i + aRows + aCols][j + aRows] = M[i][j]; // M goes in the third block
        }
    }

    // N goes into the third block starting at (row = aRows + aCols) and column aCols to aRows + nCols - 1
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            bigMatrix[i + aRows + aCols][j + aRows + nCols] = N[i][j]; // N goes in the fourth block
        }
    }

    // u goes in the last column
    for (int i = 0; i < uRows; i++) {
        bigMatrix[i + aRows + aCols][totalCols - 1] = u[i][0]; // u goes in the last column
    }

    return bigMatrix;
}

// swap function
void swap_rows(vector<float>& row1, vector<float>& row2) {
    vector<float> temp = row1;
    row1 = row2;
    row2 = temp;
}
//row echelon matrix
void echelon_matrix(vector<vector<float>>& A, int n) {
    for (int i = 0; i < n; i++) {
        int pivot_row = i;
        for (int j = i + 1; j < n; j++) {
            if (abs(A[j][i]) > abs(A[pivot_row][i])) {
                pivot_row = j;
            }
        }
        if (pivot_row != i) {
            swap_rows(A[i], A[pivot_row]);
        }
        for (int j = i + 1; j < n; j++) {
            float factor = A[j][i] / A[i][i];
            for (int k = i; k <= n; k++) {
                A[j][k] -= factor * A[i][k];
            }
        }
    }
}

void back_substitute(const vector<vector<float>>& mat, int N, vector<float>& x) {
    for (int i = N - 1; i >= 0; i--) {
        x[i] = mat[i][N];
        for (int j = i + 1; j < N; j++) {
            x[i] -= mat[i][j] * x[j];
        }
        x[i] = x[i] / mat[i][i];
    }
}
vector <vector<float>> multiplyMatrices (vector<vector<float>> & mat_1,vector<vector<float>> & mat_2){
   
    int numRows1= mat_1.size();   // rows in mat_1   (determines output size)
    int numRows2= mat_2.size();   // rows in mat_2
    int numCol1=mat_1[0].size();  //columns in mat_1 (must match rows in mat_2)
    int numCol2=mat_2[0].size();  //columns in mat_2 (determines output size)
    
    if (numCol1!=numRows2){       //if invalid dimensions, return -1 matrix
        return {{-1}};
    }

    vector<vector<float>> result(numRows1,vector<float>(numCol2,0));    //initialize the resulting vector with the right dimensions and fill it with 0s

    for (int i=0;i< numRows1;i++){       //iterates through rows in mat_1
        for(int j=0; j<numCol2;j++){     //iterates through columns in mat_2 
            for(int k=0; k<numCol1; k++){     //iterates through columns in mat_1 (which is also rows in mat_2!)
                result[i][j]+=mat_1[i][k]*mat_2[k][j];     //the [row][column] of the result is the sum of the multiplications of the items in the column in mat_2 and the row in mat_1
            }
        }
    }
    return result;
}

 int main() {
  circuitData circuit;
  readFile("netlist.txt",circuit);
  processData(circuit);
  vector<vector<int>> A = createAmatrix(circuit.s, circuit.d);
  vector<vector<int>> Aa = deleteTHErow(circuit.s, A);
  vector<vector<float>> identity;
  identityMatrix(identity, A[0].size());
  vector<vector<float>> N(circuit.r.size(), vector<float>(circuit.r.size(), 0.0f)); // Ensure proper size
  for (size_t i = 0; i < circuit.r.size(); i++) {
      N[i][i] = -circuit.r[i][0];  // Set diagonal elements
  }
  printMatrix(circuit.r);
  printMatrix(circuit.v);

/*
  vector<vector<float>> bigMatrix = constructBigMatrix(A, N , circuit.v);
  vector<vector<int>> A_T = transposeMatrix(A);
  
  // Gaussian Elimination (RREF)
  int size = bigMatrix.size();
  //partial pivoting and forward elimination
  echelon_matrix(bigMatrix, size);
  // Back substitution to get the solution
  vector<float> x(size);
  back_substitute(bigMatrix, size, x);
  resultsInTxt(x);
  */
  return 0;
} 


