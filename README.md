How to compile:
This code will compile with the g++ command. The only file that needs to be included in the working directory with the file is a text file named 
"netlist.txt". If there are any errors running the code, double check that the file name matches what is being called in main.

How to execute/run:
After compiling the code, the program should run in its entirety when called. The results of the program (voltages, potentials, and currents) will be found in a text file named "output.txt" which will be created in the same directory. We have included print statements throughout the code as well to showcase some of the inner workings of the algorithm as it is run. However, these can easily be commented out if desired.


Bonus: Yes, our group have attempted the bonus points. Our big matrix is a sparse matrix with full combination of each small matrix, with the 0 matrix included. We performed Gaussian Elimation and back substitution to achieve the final answer as mentioned by the TA. We also generated the big matrix to the sparse (row, col, value) for mat, saving memory, we also performed Gaussian Elimation and back substitution with the sparse triplets matrix.


Steps/Functions in the Code
- readFile will take in the provided text file and the circuitData struct. It will return circuitData with the nessesary data extracted from a text file in the same directory.

- processData will take in the circuitData struct containing information from the text file and sort it to be used within the system. It will return circuitData.

- createAmatrix will take in the source and destination nodes as vectors and output the initial matrix A prior to getting rid of the ground node branch.

- deleteTHErow will take in the source node vector and the matrix A from the createAmatrix function. It will get rid of the ground node branch and return a new matrix A.

- constructBigMatrix function combines multiple input matrices (A, N, and u) and (M) to generate a large matrix representing a system of equations. The matrix is constructed with three main row blocks: Block 1 corresponds to Kirchhoff's Voltage Law (KVL), Block 2 represents Kirchhoff's Current Law (KCL), and Block 3 is based on Ohm's Law. Size of the big matrix defined by the passing A, N, u. The function initializes a zero matrix of appropriate dimensions and fills each block with the corresponding values, creating a compact system representation by shifting each small matrix to it's corresponding location. The final big matrix is ready to use for rref.

- echelon_matrix: The echelon_matrix function transforms a given matrix into row echelon form using Gaussian elimination. It first applies partial pivoting by swapping rows to ensure numerical stability. Then, it performs forward elimination by making all elements below the pivot zero through row operations. The function processes the matrix iteratively, reducing it to an upper triangular form. However, it lacks a check for zero pivots, which may cause division errors.

- back_substitute: Solve for final answer called after echelon_matrix. this function iterates from the last row to the first, solving for each unknown by isolating it using the values already computed in previous iterations. The function starts with the right-hand side value (the augmented column) and subtracts contributions from already determined variables. It then divides by the diagonal element to obtain the final solution for each variable. This method assumes that the matrix is in upper triangular form, ensuring that each variable can be solved sequentially.

- sparseFormat: function converts a given dense matrix into a sparse matrix representation using the three-column format: row index, column index, and nonzero value. It first counts the number of nonzero elements in the input matrix to determine the required storage size. Then, it iterates through the matrix, storing the position and value of each nonzero element in a compact format. The function also prints the resulting sparse matrix for verification. This approach efficiently reduces memory usage for matrices with many zero entries.

- resultsInTxt will print the final results to a text file named output.txt.
