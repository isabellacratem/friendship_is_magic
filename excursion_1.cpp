#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


/*Input: a file named textlist.txt w the format
 "branch label (V1,R1)| source node label (0,1,2) | destination label (0,1,2) | value (5,10,20)"
 Output: a file named output.txt w the format potentials, voltages, currents with one space between them
 "e1 e2 ...en v1 v2... vn i1 i2...in" 
 These values are floating-point numbers w 3 decimal places. */
 
 
 struct circuitData{
    vector<int> s;  // Source node array
    vector<int> d;  // Destination node array
    vector<float> v;  // Voltage array
    vector<float> r;  // Resistance array
    vector<string> labels;           //this creates an array that stores the branch names (to know voltage vs resistance)
    vector<vector<float>> data;              //this stores all other info (source and destination node and numeric component value)

 };


circuitData readFile (string file, circuitData &circuit){
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
      if(circuit.labels[i].find("V")<=numBranches){
        circuit.v[i]=circuit.data[i][2];
       circuit. r[i]=0;
       }else {
        circuit.v[i]=0;
        circuit.r[i]=circuit.data[i][2];
      }
  
    }

    return circuit;
}

void resultsInTxt (vector<int> data){      //function to print results in a text file
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

vector<vector<int>> transposeMatrix (vector<vector<int>> &matrix){
  int rows=matrix.size();
  int col=matrix[0].size();
  vector<vector<int>> matrixT(col, vector<int>(rows));
  for(int i=0; i<rows; i++){
    for(int j=0; j<col; j++){
      matrixT[j][i]=matrix[i][j];
    }
  }
  return matrixT;
}

//identity matrix
void identityMatrix(vector<vector<float>>& mat, int size) {
  mat.assign(size, vector<float>(size, 0.0f));
  for (int i = 0; i < size; i++) {
      mat[i][i] = 1.0f;
  }
}

//identity matri

 int main() {
  circuitData circuit;
  readFile("netlist.txt",circuit);
  processData(circuit);
  cout <<"labels array: ";            //CHECK ARRAYS
    for (const string &j :circuit.labels){
      cout<< j <<" ";
    }
    cout <<"\nsource node array: ";            
    for (int j: circuit.s){
      cout<< j <<" ";
    }
    cout <<"\ndestination node array: ";
    for (int j: circuit.d){
      cout<< j <<" ";
    }
    cout <<"\nVoltages array: ";
    for (int j:circuit.v){
      cout<< j <<" ";
    }
    cout <<"\nResistances array: ";
    for (int j: circuit.r){
      cout<< j <<" ";
    }
  vector<int> s={3,3,4};
  resultsInTxt(s);
  vector<vector<int>> a ={{1,2,3},{4,5,6}};
  vector<vector<int>>aT;

  
  cout <<"\nMatrix A:\n";
    for (int i=0; i<a.size();i++){
      for (int j=0; j<a[0].size();j++){
        cout<< a[i][j] <<" ";
      }
      cout <<endl;
    }
  aT=transposeMatrix(a);
  cout <<"\nMatrix A Transposed:\n";
  for (int i=0; i<aT.size();i++){
    for (int j=0; j<aT[0].size();j++){
      cout<< aT[i][j] <<" ";
    }
    cout <<endl;
  }
  vector<vector<float>> N(circuit.r.size(), vector<float>(circuit.r.size(), 0.0f)); // Ensure proper size
  for (int i = 0; i < circuit.r.size(); i++) {
      N[i][i] = -circuit.r[i];  // Set diagonal elements
  }

  for (int i=0; i<N.size();i++){
    for (int j=0; j<N[0].size();j++){
      cout<< N[i][j] <<" ";
    }
    cout <<endl;
  }

  return 0;
} 


/*each row in the input is a branch
 we need to find the max number out of column 2 and 3 to find number of nodes
--extract all input data into separate arrays to use later
 incidence matrix: nodes on rows and branches on columns: 1 placed in source node and -1 in destination
 --THIS INFO IS GIVEN IN INPUT FILE so take column 2 info for 1 and column 3 info for destination
 
 remove any nodes that do not have both 1 and -1 for reduced incidence matrix

current coefficients: KCL can be expressed in matrix form as Aai=0 OR Ai=0 for reduced form
voltage coefficients: KVL can be expressed as difference between two node voltages
v=[reduced incidence]* nodes

combined coefficients: v=A^T*e & Ai=0

final thoughts: emily is 
 */