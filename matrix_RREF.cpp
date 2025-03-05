#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

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
    int totalCols = aCols + nCols + nCols;

    // Initialize the big matrix with zeros
    vector<vector<float>> bigMatrix(totalRows, vector<float>(totalCols, 0.0f));
    printMatrix(bigMatrix);
    cout << bigMatrix.size() <<endl;
    cout << bigMatrix[0].size() <<endl;

    // Fill Block 1: [0, 0, A, 0]
    for (int i = 0; i < aRows; i++) {
        for (int j = 0; j < aCols; j++) {
            bigMatrix[i][j + aRows + nCols] = A[i][j]; // Place A in the third block 
        }
    }

    // Fill Block 2: [-A^T, I, 0, 0]
    vector<vector<float>> A_T = transposeMatrix(A);  // Transpose of A
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

int main() {
    vector<vector<float>> A = {{1, 1, 0}, {0, -1, 1}};
    vector<vector<float>> N = {{0, 0, 0}, {0, -10, 0}, {0, 0, -20}};
    vector<vector<float>> u = {{5}, {0}, {0}};  // column matrix

    // Construct the big matrix
    vector<vector<float>> bigMatrix = constructBigMatrix(A, N, u);

    // constructed matrix
    cout << "Big Matrix:\n";
    printMatrix(bigMatrix);
    printMatrix(A);
    vector<vector<float>> A_T = transposeMatrix(A);
    printMatrix(A_T);

    // Gaussian Elimination (RREF)
    int size = bigMatrix.size();
    //partial pivoting and forward elimination
    echelon_matrix(bigMatrix, size);

    printMatrix(bigMatrix);

    // Back substitution to get the solution
    vector<float> x(size);
    back_substitute(bigMatrix, size, x);

    for (int i = 0; i < size; i++) {
        cout << x[i] << " ";
    }
    cout << endl;    
    return 0;
}