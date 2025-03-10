#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include<cmath>
using namespace std;

/*Input: a file named textlist.txt w the format
 "branch label (V1,R1)| source node label (0,1,2) | destination label (0,1,2) | value (5,10,20)"
 Output: a file named output.txt w the format potentials, voltages, currents with one space between them
 "e1 e2 ...en v1 v2... vn i1 i2...in" 
 These values are floating-point numbers w 3 decimal places. */
 
struct circuitData {
    vector<int> s;  // Source node array
    vector<int> d;  // Destination node array
    vector<vector<float>> v;  // New: 2D vector (column matrix)
    vector<float> r;  // Resistance array
    vector<string> labels;  // Array that stores the branch names (to know voltage vs resistance)
    vector<vector<float>> data;  // Stores all other info (source and destination node and numeric component value)
};

circuitData readFile(string file, circuitData& circuit) {
    ifstream f(file); // Open the text file 
    string label;
    float a, b, c;

    while (f >> label >> a >> b >> c) { // Read each line and extract the values
        circuit.labels.push_back(label);
        circuit.data.push_back({a, b, c});
    }

    f.close();
    return circuit;
}

circuitData processData(circuitData &circuit) {
    int numBranches = circuit.labels.size();
    circuit.s.resize(numBranches);
    circuit.d.resize(numBranches);
    circuit.r.resize(numBranches, 0.0f);  

    circuit.v.assign(numBranches, vector<float>(1, 0.0f)); // Column matrix (numBranches × 1)

    for (int i = 0; i < numBranches; i++) {
        circuit.s[i] = circuit.data[i][0];
        circuit.d[i] = circuit.data[i][1];

        if (circuit.labels[i].find("V") != string::npos) { // Check if it's a voltage source
            circuit.v[i][0] = circuit.data[i][2]; // Store voltage in column matrix
            circuit.r[i] = 0;
        } else {
            circuit.v[i][0] = 0;
            circuit.r[i] = circuit.data[i][2];
        }
    }
    return circuit;
}

void resultsInTxt(vector<int> data) {
    ofstream file("output.txt");
    for (size_t i = 0; i < data.size(); i++) {
        file << data[i];
        if (i != data.size() - 1) {
            file << " ";
        }
    }
    file << endl;
    file.close();
}

// Modify to return vector<vector<float>> instead of vector<vector<int>>
vector<vector<float>> transposeMatrix(vector<vector<float>>& matrix) {
    int rows = matrix.size();
    int col = matrix[0].size();
    vector<vector<float>> matrixT(col, vector<float>(rows));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < col; j++) {
            matrixT[j][i] = matrix[i][j];
        }
    }
    return matrixT;
}


// Keep printMatrix unchanged
void printMatrix(vector<vector<float>>& matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

vector<vector<float>> createAmatrix(const vector<int>& sourceNodes, const vector<int>& destinationNodes){
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
    vector<vector<float>> A(row_num, vector<float>(col_num, 0.0));

    //create matrix A
    for (int i = 0; i < col_num; i++){
        A[sourceNodes[i]][i] = 1.0; //is a source node
        A[destinationNodes[i]][i] = -1.0; // is a destination node
    }
    return A;
}

vector<vector<float>> deleteTHErow(const vector<int>& sourceNodes, const vector<vector<float>> &A){
    
    //get dimensions
    int row_num = A.size();
    int col_num = A[0].size();

    //create Aa (should have one less row that A)
    vector<vector<float>> Aa((row_num - 1), vector<float>((col_num), 0));

    //find what row we need to get rid of
    int deleteRow = -5;
    for (int i = 0; i < row_num; i++) {
        int count = 0;
        for (int j = 0; j < col_num; j++) {
            if (A[i][j] == -1.0){
                count += 1;
            }
            if (count == 2){
                deleteRow = i;
                break;
            }
        }
    }

    //check if there are no rows to get rid of
    if (deleteRow == -5) {                          //this means open circuit- if only one branch, remove the ground node
        //return A;
        for (int i = 0; i < row_num; i++) {
            if (A[i][0] == -1.0){
            deleteRow = i;
            }   
        }
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

// Function to transpose matrix A (A^T)
vector<vector<float>> transposeMatrix(const vector<vector<float>>& mat) {
    int rows = mat.size();
    int cols = mat[0].size();
    vector<vector<float>> transpose(cols, vector<float>(rows));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            transpose[j][i] = mat[i][j];
        }
    }
    return transpose;
}
//construct big matrixm with row block 1: KVL, ROW BLACK 2: KCL, row block 3: Ohm's law
vector<vector<float>> constructBigMatrix(
    const vector<vector<float>>& A, 
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
    int totalCols = aCols + nCols + aRows +1;

    // Initialize the big matrix with zeros
    vector<vector<float>> bigMatrix(totalRows, vector<float>(totalCols, 0.0f));
    //printMatrix(bigMatrix);
    cout << bigMatrix.size() <<endl;
    cout << bigMatrix[0].size() <<endl; //coloum should be rows+1

    // Fill Block 1: [0, 0, A, 0]
    for (int i = 0; i < aRows; i++) {
        for (int j = 0; j < aCols; j++) {
            bigMatrix[i][j + aRows + nCols] = A[i][j]; // Place A in the third block , shift by aRows+aCols
        }
    }

    // Fill Block 2: [-A^T, I, 0, 0]
    vector<vector<float>> A_T = transposeMatrix(A);  // Transpose of A
    for (int i = 0; i < aCols; i++) {
        for (int j = 0; j < aRows; j++) {
            bigMatrix[i + aRows][j] = -A_T[i][j]; // -A^T goes in the most left block
        }
    }
    // Identity matrix I goes in the middle block 
    vector<vector<float>> identity;
    identityMatrix(identity, aCols);
    for (int i = 0; i < aCols; i++) {
        for (int j = 0; j < aCols; j++) {
            bigMatrix[i + aRows][j + aRows] = identity[i][j]; // Identity matrix goes in the middle, shift right and down by aRows
        }
    }

    // Fill Block 3: [0, M, N, u]
    // M is an identity matrix (size aCols x aCols) or (nCols x nCols)
    vector<vector<float>> M;
    identityMatrix(M, nCols); 
    for (int i = 0; i < nCols; i++) {
        for (int j = 0; j < nCols; j++) {
            bigMatrix[i + aRows + aCols][j + aRows] = M[i][j]; // M goes in the third block, shifr right by aRows+aCols, shift down by aRows
        }
    }
    // N goes into the third block starting at (row = aRows + aCols) and column aCols to aRows + nCols - 1
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            bigMatrix[i + aRows + aCols][j + aRows + nCols] = N[i][j]; // N goes in the fourth block, shifting
        }
    }
    // u goes in the last column
    for (int i = 0; i < uRows; i++) {
        bigMatrix[i + aRows + aCols][totalCols - 1] = u[i][0]; // u goes in the last column, shifting
    }
    return bigMatrix;
}

// swap function
void swap_rows(vector<float>& row1, vector<float>& row2) {
    vector<float> temp = row1;
    row1 = row2;
    row2 = temp;
}
//row echelon matrix https://overbye.engr.tamu.edu/wp-content/uploads/sites/146/2018/09/ECE615Fall2018_Lect10.pdf
void echelon_matrix(vector<vector<float>>& A, int n) { 
    for (int i = 0; i < n; i++) { // iterate over each column
        int pivot_row = i;
        // Find the row with the largest absolute value in column i (partial pivoting)
        for (int j = i + 1; j < n; j++) {
            if (abs(A[j][i]) > abs(A[pivot_row][i])) {
                pivot_row = j;
            }
        }
        if (pivot_row != i) {
            swap_rows(A[i], A[pivot_row]); // swap row
        }
        // perform row elimination to make all elements below the pivot zero
        for (int j = i + 1; j < n; j++) {
            float factor = A[j][i] / A[i][i];  // factor to eliminate
            // Subtract the factor times the pivot row from the current row
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

void resultsInTxt (const vector<float> &data){      //function to print results in a text file
    ofstream file("output.txt");
   file << fixed << setprecision(3);
    for ( size_t i = 0; i < data.size(); i++) {
        file << data[i];
        if (i != data.size() - 1) {
          file << " ";  // Add space between numbers
        }
      }
      file << endl;
      file.close();
  }
//extra credits
// Function to print the sparse matrix in triplet form (row, column, value)
// Sparse format function to convert matrix to sparse format
vector<vector<float>> sparseFormat(vector<vector<float>>& matrix) {
    int size = 0;
    for (int i = 0; i < matrix.size(); i++)
        for (int j = 0; j < matrix[0].size() - 1; j++)  // Don't include the last column
            if (matrix[i][j] != 0)
                size++;

    vector<vector<float>> compactMatrix(3, vector<float>(size)); // Dependent on number of non-zero values

    int k = 0;
    for (int i = 0; i < matrix.size(); i++)  // Row 
        for (int j = 0; j < matrix[0].size() - 1; j++) // Column, skipping the last column (b values)
            if (matrix[i][j] != 0) // Only take non-zero
            {
                compactMatrix[0][k] = i;       // Row index
                compactMatrix[1][k] = j;       // Column index
                compactMatrix[2][k] = matrix[i][j]; // Value
                k++;
            }

    cout << "Sparse Matrix (Row Col Value) " << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < size; j++)
            cout << " " << compactMatrix[i][j];

        cout << "\n";
    }
    return compactMatrix;
}

// EXTRA CREDIT Sparse Gaussian Elimination
void sparseGaussianElimination(vector<vector<float>>& sparse, int n) {
    for (int k = 0; k < n; k++) {
        // Find pivot element
        float pivot = 0;
        int pivotIndex = -1;

        // Locate the pivot (diagonal element)
        for (int i = 0; i < sparse[0].size(); ++i) {
            if (sparse[0][i] == k && sparse[1][i] == k) {
                pivot = sparse[2][i];
                pivotIndex = i;
                break;
            }
        }

        if (pivot == 0) {
            cout << "Matrix is singular" << endl;
            return;
        }

        // Normalize pivot row
        for (int i = 0; i < sparse[0].size(); ++i) {
            if (sparse[0][i] == k) {
                sparse[2][i] /= pivot;
            }
        }

        // Eliminate entries below the pivot element
        for (int i = 0; i < sparse[0].size(); ++i) {
            if (sparse[0][i] > k && sparse[1][i] == k) {
                float factor = sparse[2][i];
                int row = sparse[0][i];

                // Update matrix
                for (int j = 0; j < sparse[0].size(); ++j) {
                    if (sparse[0][j] == row && sparse[1][j] == sparse[1][i]) {
                        sparse[2][j] -= factor * sparse[2][pivotIndex];
                    }
                }

                // Modify the right-hand side (last column)
                for (int j = 0; j < sparse[0].size(); ++j) {
                    if (sparse[0][j] == row && sparse[1][j] == n) {
                        sparse[2][j] -= factor * sparse[2][pivotIndex];
                    }
                }
            }
        }
    }
}

// EXTRA CREDIT Sparse Back Substitution
vector<float> sparseBackSubstitute(const vector<vector<float>>& sparse, int n) {
    vector<float> x(n, 0);  // Solution vector initialized to zero

    // Backward substitution
    for (int i = n - 1; i >= 0; --i) {
        float sum = 0;

        // Loop through all non-zero elements in the current row
        for (int j = 0; j < sparse[0].size(); ++j) {
            if (sparse[0][j] == i) {
                if (sparse[1][j] > i) {
                    sum -= sparse[2][j] * x[sparse[1][j]];  // For entries in the row after diagonal
                }
                if (sparse[1][j] == i) {
                    x[i] = (sparse[2][j] - sum) / sparse[2][j];  // Solve for diagonal element
                    break;
                }
            }
        }
    }
    return x;
}

int main() {
    circuitData circuit;
    readFile("netlist_1.txt", circuit);
    processData(circuit);
    vector<vector<float>> u = {circuit.v}; // Convert to 2D vector
    vector<vector<float>> r_matrix = {circuit.r}; // Convert to 2D vector
    cout <<"\nsource node array: ";            
    for (int j: circuit.s){
      cout<< j <<" "; }
    cout <<"\ndestination node array: ";
    for (int j: circuit.d){
      cout<< j <<" "; }

    vector<vector<float>> N(circuit.r.size(), vector<float>(circuit.r.size(), 0.0f)); // Ensure proper size
    for (int i = 0; i < circuit.r.size(); i++) {
        N[i][i] = -circuit.r[i]; // Set diagonal elements
    }
    cout << "N:"<<endl;
    printMatrix(N); // Ensure N has float values
    cout << "u:"<<endl;
    printMatrix(u);

    vector<int> sourceNodesV = {circuit.s}; // Convert to vector
    vector<int> destinationNodesV = {circuit.d}; // Convert to vector

    vector<vector<float>> A = createAmatrix(sourceNodesV, destinationNodesV);

    cout << "Matrix A:" << endl;
    printMatrix(A); 

    vector<vector<float>> Aa = deleteTHErow(sourceNodesV, A);
    cout << "Matrix Aa (after deleting row):" << endl;
    printMatrix(Aa);

    // Construct the big matrix
    vector<vector<float>> bigMatrix = constructBigMatrix(Aa, N, u);
    vector<vector<float>> bigMatrix_2 = bigMatrix;
    cout << "Big Matrix:\n";
    printMatrix(bigMatrix);
    cout << "------------------------\n";

    // Gaussian Elimination (RREF)
    int size = bigMatrix.size();
    //partial pivoting and forward elimination
    echelon_matrix(bigMatrix, size);
    printMatrix(bigMatrix);

    // Back substitution to get the solution
    vector<float> x(size);
    back_substitute(bigMatrix, size, x);
    cout <<"answer: " <<endl;
    for (int i = 0; i < size; i++) {
        cout << x[i] << " ";
    }
    cout << endl;    
    resultsInTxt(x);  //result to output file

    //Extra credits
    cout << "Extra credit output:" << endl;
    int n = bigMatrix_2.size();

    // Convert the matrix to sparse format
    vector<vector<float>> sparseMatrix = sparseFormat(bigMatrix_2);

    // Perform sparse Gaussian elimination
    sparseGaussianElimination(sparseMatrix, n);

    // Perform sparse back substitution to find the solution
    vector<float> solution = sparseBackSubstitute(sparseMatrix, n);

    // Output the solution
    cout << "Solution: ";
    for (const float& val : solution) {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}
