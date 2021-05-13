
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
#include "syntax.h" 
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

	// Lexer is in assignment3.h
	// Prepare output 
	generator.setOfile("output.txt");
	// Start procedure
	generator.procStart(input);

	generator.instPrint();
	generator.closeOfile();

	return 0;
}