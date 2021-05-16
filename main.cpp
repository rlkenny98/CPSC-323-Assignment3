
/*
- Use g++ -Wall -c -g main.cpp -o main.o
  followed by g++ main.o -o main

- Run the program with ./main <name>.txt 
*/

// Include Libraries and header files
#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include "assignment3.h"
#include "FSM.h"
using namespace std; 

int main(int argc, char *argv[])
{
	
	codeGen generator;
	string input;
	
  	if(argc < 2)
		{std::cout << "ERROR - format should be: ./main inputFile\n"; exit(1);}
  	else
    	input = argv[1];

	// Obtain input 
	generator.setIfile(input);
	generator.getInputCode();
	generator.closeIfile();
	// Prepare output 
	generator.setOfile();

	// Lexer is in assignment3.h
	// Start procedure
	generator.procStart(input);
	// Some error is happening with the procedures 
	generator.instPrint();
	generator.closeOfile();

	return 0;
}