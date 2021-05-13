
/*
- Use g++ -Wall -c -g mainEC.cpp -o mainEC.o
  followed by g++ mainEC.o -o mainEC

- Run the program with ./main
*/

// Include Libraries and header files
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;

int memory = 5000; 

// Data structure for assembly productions
struct asmTuple{
	int index, address;
	std::string instruction;
};

// Data structure that will contain the information needed in our table
struct tableObject{
    string identifier, idType;
    int memAddress; 

};

class table {
    private:
        // Vector that will represent our Symbol table using the tableObject struct created above
        // Each vector element contains a tableObject, which has three attributes: Identifier, Type, and Memory Address
        std::vector<tableObject> tableVector;
       
    public:

        string addID(string id, int mem, string type){
        
            tableObject elem; 

            elem.identifier = id;
            elem.idType = type;
            elem.memAddress = mem;

            // Iterate through the vector and check if Identifier exists

            for (const auto item : tableVector)
                if (item.identifier == id)
                {return "Element already exists in the table!";}
          
            //if it doesnt exist, push to the vector 
            tableVector.push_back(elem);

            return "Success";
        }

        void printTable(ofstream &opFile){

             std::cout << "Symbol Table\n"
                    << "Identifier         MemoryLocation           Type" << endl;
                
             opFile << "Symbol Table\n"
                    << "Identifier         MemoryLocation           Type" << endl; 
                

             for (auto it : tableVector) {
                 std::cout << setw(20) << left << it.identifier << setw(25) << it.memAddress 
                           << setw(34) << it.idType << endl;
                
                 opFile  << setw(20) << left << it.identifier << setw(25) << it.memAddress 
                        << setw(34) << it.idType << endl;
                } 

        }

};

void assemble(ofstream &outfile){

	table symbolTable;
	std::vector<asmTuple> instructions;
	asmTuple elem;
	// Instruction 1 
	elem.index = 1;
	elem.address = 0;
	elem.instruction = "PUSHI";
	instructions.push_back(elem);

	// Adding first identifier 
	cout << symbolTable.addID("num",memory, "integer") << endl;
	elem.index = 2;
	elem.address = memory;
	elem.instruction = "POPM";
	instructions.push_back(elem);
	memory++;

	// Instruction 3
	elem.index = 3;
	elem.address = 15;
	elem.instruction = "PUSHI";
	instructions.push_back(elem);

	// Instruction 4 
	cout << symbolTable.addID("nu2m",memory, "integer") << endl;
	elem.index = 4;
	elem.address = memory;
	elem.instruction = "POPM";
	instructions.push_back(elem);
	memory++;

	// Instruction 5 
	elem.index = 5;
	elem.address = 1;
	elem.instruction = "PUSHI";
	instructions.push_back(elem);

	// Instruction 6 
	cout << symbolTable.addID("large$",memory, "integer") << endl;
	elem.index = 6;
	elem.address = memory;
	elem.instruction = "POPM";
	instructions.push_back(elem);
	memory++;

	// Instruction 7 
	elem.index = 7;
	elem.address = 0;
	elem.instruction = "PUSHI";
	instructions.push_back(elem);
	
	// Instruction 8 
	cout << symbolTable.addID("hey",memory, "boolean") << endl;
	elem.index = 8;
	elem.address = memory;
	elem.instruction = "POPM";
	instructions.push_back(elem);
	memory = 5000;

	// Instruction 9 
	elem.index = 9;
	elem.address = 0;
	elem.instruction = "PUSHI";
	instructions.push_back(elem);

	// Instruction 10 
	elem.index = 10;
	elem.address = 1;
	elem.instruction = "PUSHI";
	instructions.push_back(elem);

	// Instruction 11
	elem.index = 11;
	elem.address = -1;
	elem.instruction = "EQU";
	instructions.push_back(elem);

	// Instruction 12
	elem.index = 12;
	elem.address = 15;
	elem.instruction = "JUMPZ";
	instructions.push_back(elem);

	// Instruction 13 
	elem.index = 13;
	elem.address = memory;
	elem.instruction = "PUSHM";
	instructions.push_back(elem);

	// Instruction 14
	memory += 2;
	elem.index = 14;
	elem.address = memory;
	elem.instruction = "POPM";
	instructions.push_back(elem);
	memory--;

	// Instruction 15
	elem.index = 15;
	elem.address = memory;
	elem.instruction = "PUSHM";
	instructions.push_back(elem);
	memory++;

	// Instruction 16 
	elem.index = 16;
	elem.address = memory;
	elem.instruction = "POPM";
	instructions.push_back(elem);

	// Print everything 
	for (auto inst : instructions)
	{
		if (inst.address < 0) {outfile << setw(10) << left << inst.index << setw(15) << inst.instruction << endl;}

		else 
			outfile << setw(10) << left << inst.index << setw(15) << inst.instruction <<setw(15)<< inst.address <<endl;
	}

    outfile << endl;

	symbolTable.printTable(outfile);

}

// Testing table 
int main()
{
	ofstream fout;

	fout.open("output.txt");
	if(!fout.is_open()){ cout << "Output File Error\n"; exit(1);}
	// Print assembly code 
	assemble(fout);
	fout.close();

	return 0;
}
