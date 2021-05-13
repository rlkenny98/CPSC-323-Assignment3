/* 
 Assembly Code Instructions: 
 PUSHI {Integer Value} Pushes the {Integer Value} onto the Top of the Stack (TOS)

 PUSHM {ML - Memory Location} Pushes the value stored at {ML} onto TOS

 POPM {ML} Pops the value from the top of the stack and stores it at {ML}

 STDOUT Pops the value from TOS and outputs it to the standard output

 STDIN Get the value from the standard input and place in onto the TOS

 ADD Pop the first two items from stack and push the sum onto the TOS

 SUB Pop the first two items from stack and push the difference onto the TOS ( Second item - First item)

 MUL Pop the first two items from stack and push the product onto the TOS

 DIV Pop the first two items from stack and push the result onto the TOS ( Second item / First item and ignore the remainder)

 GRT Pops two items from the stack and pushes 1 onto TOS if second item is larger otherwise push 0

 LES Pops two items from the stack and pushes 1 onto TOS if the second item is smaller than first item otherwise push 0

 EQU Pops two items from the stack and pushes 1 onto TOS if they are equal otherwise push 0

 NEQ Pops two items from the stack and pushes 1 onto TOS if they are not equal otherwise push 0

 GEQ Pops two items from the stack and pushes 1 onto TOS if second item is larger or equal, otherwise push 0

 LEQ Pops two items from the stack and pushes 1 onto TOS if second item is less or equal, otherwise push 0

 JUMPZ {IL - Instruction Location} Pop the stack and if the value is 0 then jump to {IL}

 JUMP {IL} Unconditionally jump to {IL}

 LABEL Empty Instruction; Provides the instruction location to jump to.
*/

#ifndef ASSIGNMENT3_H
#define ASSIGNMENT3_H
#pragma once
#include <cassert>
#include <iomanip>
#include <string>
#include <vector>
#include <stack>
#include <iterator> 
#include <algorithm>
#include <fstream>
#include <iostream>
#include "FSM.h"

using namespace std;

// Global variable for the memory address
int mem_addrs = 5000;

// Global variable for the instruction address/ index 
int instr_addrs = 1;

// Global variable for accessing vector 
unsigned vectorPos = 0;

char operators[] = "*+-=/><%";

// Data structure for tables 
struct tableObject{
    string identifier, idType;
    int memAddress; 
};

string get_address(string tok){

    string add = to_string(mem_addrs);
    mem_addrs++;
    return add;

}

class codeGen{
    private:
         // Input file 
        ifstream input;
        // Output file 
        ofstream output; 
        // Token value 
        string token = ""; 

        // Vector that contains the code from input 
        vector<string> codeVector; 
        // Vector that contains tokens and lexemes 
	    vector<tokens> lexerStorage;
          
        // Instruction Table
        vector<tableObject> instrTable; 
        // Symbol Table 
        vector<tableObject> symbolTable;

	     
    public: 
        // Put input into vector 
        void getInputCode(){
            string line;
            while (getline(input, line)) {codeVector.push_back(line);}
        }

        // Access lexer storage and store element in token
        void callLexer(){
            // Obtain new value for our token 
            cout << lexerStorage[vectorPos].token << endl;

            if(lexerStorage[vectorPos].token == "IDENTIFIER")
              token = "id";
            
            if(lexerStorage[vectorPos].lexeme == "=")
              token = "=";
            
            if(lexerStorage[vectorPos].lexeme == "+")
              token = "+";
            
            if(lexerStorage[vectorPos].lexeme == "*")
              token = "*";

            //if ((vectorPos + 1) < lexerStorage.size())
                vectorPos++;

            cout << token << endl;

        }

        // Set input 
        void setIfile(string in){
            input.open(in);
	        if (!input.is_open()){cerr << "File Opening Error\n"; exit(-1);}
        }
        
        // Close output
        void closeIfile(){
            input.close();
        }

        // Set output 
        void setOfile(string name){

            output.open(name); 
            if(!output.is_open()){ std::cout << "Output File Error\n"; exit(1);}
        }

        // Close output 
        void closeOfile(){

            output.close();
        }

        // Add an identifier to the symbol table 
       string addID(string id, int mem, string type){
        
            tableObject elem; 

            elem.identifier = id;
            elem.idType = type;
            elem.memAddress = mem;

            // Iterate through the vector and check if Identifier exists
            for (const auto &item : symbolTable)
                if (item.identifier == id)
                {return "Element already exists in the table!";}
          
            //if it doesnt exist, push to the vector 
            symbolTable.push_back(elem);

            return "Success";
        }

        // Function that generates an instruction within the instruction table  
        void gen_instr(string op, string oprnd){
                instrTable[instr_addrs].memAddress = instr_addrs;
                instrTable[instr_addrs].identifier = op;
                instrTable[instr_addrs].idType = oprnd;

                instr_addrs++;

            }
        // Found under the While Statement Pseudocode 
        void back_patch(int jump_add){
            /* TO-DO 
            Procedure back_patch (jump_addr)
            {
                addr = pop_jumpstack();
                Instr_table[addr].oprn = jump_addr;
                }
            */
        }

        // Print the symbol table 
        void printSymbol(){
             output << "\nSymbol Table:\n"
                    << "Identifier         MemoryLocation           Type" << endl; 
             for (auto it : symbolTable) {
                 output << setw(20) << left << it.identifier << setw(25) << it.memAddress 
                           << setw(34) << it.idType << endl;
                } 

        }

        // Print the instruction table while igoring 'nil'
        void instPrint(){
	            for (auto inst : instrTable)
	            {
		            if (inst.idType == "nil") {output << setw(10) << left << inst.memAddress << setw(15) << inst.identifier<< endl;}

		            else 
			            output << setw(10) << left << inst.memAddress<< setw(15) << inst.identifier <<setw(15)<< inst.idType <<endl;
	            }
	            printSymbol();

            }
      
        // Print an error message lol 
        void error_message(string msg){
            output << msg << endl;
        }

        // Moved lexer from main
        void lexer()
        {   
            cout << "Time for lexer to do its thing...." << endl;

            FSM machine;
            int state = 0;
            int lexStart = 0;

            // These for loops need the integer type because of the return type of .size() and .length()
            for (long long unsigned int vecString = 0; vecString < codeVector.size(); vecString++) {
                for (long long unsigned int vecChar = 0; vecChar <= codeVector[vecString].length(); vecChar++) {
                    if (state == 0) {
                        lexStart = vecChar;
                    }

                    state = machine.check_input(state, machine.char_to_input(codeVector[vecString][vecChar]));
                    if (machine.is_final_state(state)) {
                        if (machine.should_back_up(state)) {
                            vecChar--;
                        }
                        if (state != 7) {
                            string lex = "";
                            for (long long unsigned int i = lexStart; i <= vecChar; i++) {
                                lex += codeVector[vecString][i];
                            }
                            
                            if (machine.getTokenName(state, lex) != "OTHER") {
                                lexerStorage.push_back(tokens(machine.getTokenName(state, lex), lex));
                                
                            }
                        }
                        state = 0;
                    }
                }
            }
        }

// ####################################################################################################################################
// ##################################### Intermediate code generation below ###########################################################
// ####################################################################################################################################
        
        // For testing purposes 
        void procStart(string type){

            // Get token from lexer 
            lexer();
            callLexer();
            if (type == "assignment.txt"){
            // Reinitialize vector position
                output << "Assignment Statement" << endl;
                //vectorPos = 0;
                proc_A();
            }
            // Incomplete procedures below 
            if (type == "while")
                procWhile();
            if (type == "if")
                procI();
        } 
       
        // 1. ASSIGNMENT STATEMENT
        /*
            A1) A -> id = E { gen_instr (POPM, get_address(id)) }
            A2) E -> T E’ 
            A3) E’ -> + T { gen_intsr (ADD, nil) } E’ 
            A4) E’ -> ε 
            A5) T -> F T’ 
            A6) T’ -> *F { gen_instr (MUL, nil) } T’ 
            A7) T’ -> ε 
            A8) F -> id { gen_instr (PUSHM, get_address(id) )
        */ 

        void proc_A(){
            /* TO-DO */
            if(token == "id")
                {
                string save = token;
                callLexer();
                if(token == "=")
                    {
                    callLexer();
                    proc_E();
                    gen_instr ("POPM",get_address(save));
                    }
                else error_message("= expected");
                }
            else error_message("id expected");
        }

        void proc_E(){
            proc_T();
            proc_Eprime();
        }

        void proc_Eprime(){
            if (token == "+")
            {
                callLexer(); 
                proc_T();
                gen_instr("ADD", "nil");
                proc_Eprime();
            }
        }

        void proc_T(){
            proc_F();
            proc_Tprime();
        }

        void proc_Tprime(){
            /* TO-DO */
            if (token == "*")
            {
                callLexer();
                proc_F();
                gen_instr("MUL","nil");
                proc_Tprime();
            }
        }

        void proc_F(){
            /* TO-DO */
            if(token == "id"){
                gen_instr("PUSHM", get_address(token));
                callLexer();
            }
            else error_message("id expected");
        }
    // ##############################################################################################

        // 2. WHILE STATEMENT 
        /*
            W1) W -> while ( C ) S whileend
            W2) C - > E R E 
            W3) R -> == | ^= | > | < | => | =<
        */
        
        void procWhile(){
            /* TO-DO */
        }

        void procC(){
            /* TO-DO */
        }

        // 3. IF STATEMENT
        void procI(){
            /* TO-DO */
        }

        // 4. Compound statement?
        // 5. Scan statement?
        // 6. Print statement?
};

#endif