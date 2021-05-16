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

// Data structure for generating instructions
struct tokenObject{
    string tokenName, tokenType;
};

// Data structure for tables 
struct tableObject{
    string identifier, idType;
    int memAddress; 
};

// Data structure used in the lexer 
struct tokens {
	string token;
	string lexeme;
	tokens(string tok, string lex) {
		token = tok, lexeme = lex;
	}
};

// Class that contains attributes and functions used in this assignment 
class codeGen{
    private:

        // Tracks the current vector position whenever callLexer() is invoked 
        int vectorPos = 0;

         // Input file 
        ifstream input;
        // Output file 
        ofstream output; 
        // Token value 
        tokenObject token;

        // Vector that contains the code from input 
        vector<string> codeVector; 
        // Vector that contains tokens and lexemes 
	    vector<tokens> lexerStorage;
          
        // Instruction Table
        tableObject instrTable[1000];
        // Symbol Table 
        vector<tableObject> symbolTable;

    public: 
         // Print an error message lol 
        void error_message(string msg){
            output << msg << endl;
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
        void setOfile(string name = "output.txt"){

            output.open(name); 
            if(!output.is_open()){ std::cout << "Output File Error\n"; exit(1);}
        }

        // Close output 
        void closeOfile(){

            output.close();
        }

        // Put input into vector 
        void getInputCode(){
            string line;
            while (getline(input, line)) {codeVector.push_back(line);}
        }

        // This function can be modified for testing purposes 
        void testVector(){
            cout << "Testing vector" << endl; 
            /*for (auto it : lexerStorage){
                cout << it.token << ": " << it.lexeme <<  endl;
            }*/
            cout << "Vector Size: " << lexerStorage.size() << endl;
        }

        // Get address based off of the token?
        // Maybe check symbol table 
        string get_address(tokenObject tok){

            // Check symbol table given the token

            for (auto item : symbolTable)
            {
                if (tok.tokenName == item.identifier){
                    return to_string(item.memAddress);
                }
            } 
            return "nil"; 
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
         // Function that generates an instruction within the instruction table  
        void gen_instr(string op, string oprnd){
                instrTable[instr_addrs].memAddress = instr_addrs;
                instrTable[instr_addrs].identifier = op;       
                instrTable[instr_addrs].idType = oprnd;              
                instr_addrs++;
            }

        // Print the instruction table while igoring 'nil'
        void instPrint(){
                output << "Instruction Table: " << endl;
                output << setw(10) << left <<"Address"<< setw(15) << "Op" <<setw(15)<< "Oprnd" <<endl;
	            for (int i = 1; i < instr_addrs; i++)
	            {
		            if (instrTable[i].idType == "nil") {output << setw(10) << left << instrTable[i].memAddress 
                        << setw(15) << instrTable[i].identifier<< endl;}

		            else 
			            output << setw(10) << left << instrTable[i].memAddress<< setw(15) << instrTable[i].identifier 
                        << setw(15)<< instrTable[i].idType <<endl;
	            }
                output << "\n===================================================";
	            printSymbol();
            }

         // Add an identifier to the symbol table 
        string addID(string id, int mem, string type){
        
            tableObject elem; 

            elem.identifier = id;
            elem.idType = type;
            elem.memAddress = mem;

            // Iterate through the vector and check if Identifier exists
            for (auto item : symbolTable){
                if (item.identifier == id)
                {return "Element already exists in the table!";}
            }
          
            //if it doesnt exist, push to the vector 
            symbolTable.push_back(elem);

            return "Success";
        }

          // Access lexer storage and store element in token
        void callLexer(){
            // Obtain new value for our token 
            // If the current value is a keyword, move forward
            if(lexerStorage[vectorPos].token == "KEYWORD"){
                vectorPos++;
                if(lexerStorage[vectorPos].token == "IDENTIFIER"){
                    token.tokenName = lexerStorage[vectorPos].lexeme;
                    token.tokenType = "id";
                    }
            }

            if(lexerStorage[vectorPos].token == "IDENTIFIER"){
                    token.tokenName = lexerStorage[vectorPos].lexeme;
                    token.tokenType = "id";
                    }

            if(lexerStorage[vectorPos].lexeme == "="){
               token.tokenName = "=";
               token.tokenType = "Operator";
            }
            
            if(lexerStorage[vectorPos].lexeme == "+"){
               token.tokenName = "+";
               token.tokenType = "Operator";
            }
            
            if(lexerStorage[vectorPos].lexeme == "*"){
               token.tokenName = "*";
               token.tokenType = "Operator";
            }

            vectorPos++;

            cout << "Token value: " << token.tokenName <<  " Token type: " << token.tokenType << endl;

        }

        // Moved lexer from main
        void lexer()
        {   
            FSM machine;
            int state = 0;
            int lexStart = 0;
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
                                if(machine.getTokenName(state, lex) == "IDENTIFIER"){
                                    // Add to the symbol table 
                                    addID(lex, mem_addrs, "null");
                                    mem_addrs++;
                                }
                                
                            }
                        }
                        state = 0;
                    }
                }
            }
        }

//##########################################################################################################################
//###################################### Intermediate code generation below ################################################
//##########################################################################################################################
        
        // For testing purposes 
        void procStart(string type){
            cout << "Starting the procedure " << endl;
            // Get token from lexer 
            lexer();
            //testVector();
            if (type == "assignment.txt"){
                proc_A();
            }
            // Incomplete procedures below 
            if (type == "while")
                procWhile();
            if (type == "if")
                procI();
        } 

        // Declarative Statement!
        /*
            D1) D -> Y id
            D2) Y -> bool | float | int
        */
       
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
            cout << "Procedure A" << endl;
            callLexer();
            if(token.tokenType == "id")
                {
                tokenObject save = token;
                callLexer();
                if(token.tokenName == "=")
                    {
                    callLexer();
                    proc_E();
                    gen_instr("POPM",get_address(save));
                    }
                else error_message("= expected");
                }
            else error_message("id expected");
        }

        void proc_E(){
            cout << "Procedure E" << endl;
            proc_T();
            proc_Eprime();
        }

        void proc_Eprime(){
            cout << "Procedure E Prime" << endl;
            if (token.tokenName == "+")
            {   
                cout << " '+' detected\n";
                callLexer(); 
                proc_T();
                gen_instr("ADD", "nil");
                proc_Eprime();
            }
        }

        void proc_T(){
            cout << "Procedure T" << endl;
            proc_F();
            proc_Tprime();
        }

        void proc_Tprime(){
            cout << "Procedure T Prime" << endl;
            if (token.tokenName == "*")
            {
                callLexer();
                proc_F();
                gen_instr("MUL","nil");
                proc_Tprime();
            }
        }

        void proc_F(){
            cout << "Procedure F" << endl;
            if(token.tokenType == "id"){
                gen_instr("PUSHM", get_address(token));
                callLexer();
            }
            else error_message("id expected");
        }
    //###############################################################################################

        // 2. WHILE STATEMENT 
        /*
            W1) W -> while ( C ) S whileend
            W2) C - > E R E 
            W3) R -> == | ^= | > | < | => | =<
        */
        
        void procWhile(){
            // messy af 
            if (token.tokenName == "while"){
                int addr = instr_addrs;
                gen_instr("LABEL", "nil");
                callLexer();
                if (token.tokenName == "("){
                    callLexer();
                    procC();
                    if (token.tokenName == ")"){
                        callLexer();
                        // Procedure for separator??
                        gen_instr("JUMP", to_string(addr));
                        back_patch(instr_addrs);
                        if (token.tokenName == "whileend")
                        {
                            callLexer();
                        }
                        else error_message("whileend expected");
                    }
                   else error_message(") expected"); 
                }
                else error_message("( expected");
            }
            else error_message("while expected");
        }

        void procC(){
            proc_E();
            
            /* 
            if token in R then
                {
                    op = token;
                    lexer();
                    E();
                    case op of
                        < : gen instr(LES,nil);
                            push_jumpstack(instr_address);
                            gen_instr(JUMPZ, nil);
                        >:
                        ==:
                        ^=:
                    case
                }
                else error_message("R token expected");
             */
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