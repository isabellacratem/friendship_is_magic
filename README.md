How to compile:
- a simple g++ command in the terminal line will successfully run our code. However, if there are any compilation errors, ensure that the input
   text file matches what is being called in main().
what files need to be included...
- to run this program, a netlist.txt file must be present in the same directory as the .cpp file.
How to execute/run:
expected outputs

Bonus:


Steps/Functions in the Code
- readFile will take in the provided text file named netlist.txt and the circuitData struct. It will return circuitData with the nessesary data extracted from the text file.
- processData will take in the circuitData struct containing information from the text file and sort it to be used within the system. It will return circuitData.
- createAmatrix will take in the source and destination nodes as vectors and output the initial matrix A prior to getting rid of the ground node branch.
- deleteTHErow will take in the source node vector and the matrix A from the createAmatrix function. It will get rid of the ground node branch and return a new matrix A.
- constructBigMatrix used the three matrices, A, N, and u and returns the one large matrix that now needs to be solved.
- echelon_matrix
- back_substitute
- resultsInTxt will print the final results to a text file named output.txt
