#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


/*Input: a file named textlist.txt w the format
 "branch label (V1,R1)| source node label (0,1,2) | destination label (0,1,2) | value (5,10,20)"
 Output: a file named output.txt w the format potentials, voltages, currents with one space between them
 "e1 e2 ...en v1 v2... vn i1 i2...in" 
 These values are floating-point numbers w 3 decimal places. */

void readFile (string file){
       
    ifstream f(file);                // Open the text file 
    vector<string> labels;           //this creates an array that stores the branch names (to know voltage vs resistance)
    vector<vector<int>> data;              //this stores all other info (source and destination node and nueric component value)
    string label;
    int a,b,c;
   
    while (f >> label >> a >> b >> c) {    // Read each line of the file and extract the first value into a labels vector, and the next three values in an integer vector
      labels.push_back(label);  
      data.push_back({a, b, c});  
    } 

    f.close();
    
    int numBranches=labels.size();     //number of branches=number of labels 
    int s[numBranches];               //create int array for source nodes
    int d[numBranches];              //create int array for destination nodes
    int v[numBranches];            //create int array for voltage values
    int r[numBranches];            //create int array for resistance values
    for (int i=0; i<numBranches; i++){
      s[i]=data[i][0];                       //fill source array with info from first column of data
      d[i]=data[i][1];                        //fill destination array with info from second column of data
      if(labels[i].find("V")<=numBranches){
        v[i]=data[i][2];
        r[i]=0;
       }else {
        v[i]=0;
        r[i]=data[i][2];
      }
  
    }

    cout <<"labels array:";            //CHECK ARRAYS
    for (int j=0;j<numBranches;j++){
      cout<< labels[j] <<" ";
    }
    cout <<"\nsource node array:";            
    for (int j=0;j<numBranches;j++){
      cout<< s[j] <<" ";
    }
    cout <<"\ndestination node array:";
    for (int j=0;j<numBranches;j++){
      cout<< d[j] <<" ";
    }
    cout <<"\nVoltages array:";
    for (int j=0;j<numBranches;j++){
      cout<< v[j] <<" ";
    }
    cout <<"\nResistances array:";
    for (int j=0;j<numBranches;j++){
      cout<< r[j] <<" ";
    }

    
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

 int main() {
  readFile("netlist.txt");
  vector<int> s={3,3,4};
  resultsInTxt(s);
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