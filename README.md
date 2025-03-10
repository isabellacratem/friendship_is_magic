How to compile:
what files need to be included...

How to execute/run:
expected outputs

Bonus: yes


Steps/Functions in the Code
- readFile will take in the provided text file and the circuitData struct. It will return circuitData with the nessesary data extracted from the text file.
- processData will take in the circuitData struct containing information from the text file and sort it to be used within the system. It will return circuitData.
- createAmatrix will take in the source and destination nodes as vectors and output the initial matrix A prior to getting rid of the ground node branch.
- deleteTHErow will take in the source node vector and the matrix A from the createAmatrix function. It will get rid of the ground node branch and return a new matrix A.
- constructBigMatrix used the three matrices, A, N, and u and returns the one large matrix that now needs to be solved.
- echelon_matrix
- back_substitute
- resultsInTxt will print the final results to a text file.
