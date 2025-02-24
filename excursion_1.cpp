#include <iostream>
#include <fstream>
using namespace std;


/*Input: a file named textlist.txt w the format
 "branch label (V1,R1)| source node label (0,1,2) | destination label (0,1,2) | value (5,10,20)"
 Output: a file named output.txt w the format potentials, voltages, currents with one space between them
 "e1 e2 ...en v1 v2... vn i1 i2...in" 
 These values are floating-point numbers w 3 decimal places. */

string readFile (string file){
     // Open the text file named "netlist.txt"
     ifstream f(file);
     
     //this is rly basic code that needs to be changed- my idea is read each line as a string and add it to an array
    // Read each line of the file and print it to the
    // standard output stream till the whole file is
  	// completely read
    string s, line;
    while (getline(f, line)) {
        s += line + "\n";
    } 
    // Close the file
    f.close();
  return s;
}

 int main() {
  string s=readFile("netlist.txt");
  cout <<s;
} 