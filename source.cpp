#include "header.h"



int main() {

	Memo Memory;
	HashTable HashTable;

	Initialize(Memory, HashTable);
	string command;
	int a = 1;
	cout << (a == 1) << endl;

	while (cout << ">", getline(cin, command)) {
		if (command == "") break;
		while (no_front_blank(command) != "") {

			command = Preprocessing(command);//this is newcommamd
			cout << command << endl;
			int root = Read(command, HashTable, MEM_SIZE, Memory);
			int result = Eval(root, HashTable, Memory);
			PrintMemHash(HashTable, Memory, result);
			Print(result, result, HashTable, Memory);
			
			//Memreset(Memory, MEM_SIZE,root);
			cout << endl << endl;
		}


	}
	
	


	/*
	
	while (true) {
		command = Getcommand();
		string newcommand = Preprocessing(command);
		int root = Read(command, HashTable, MEM_SIZE, Memory);
		int result = Eval(root,HashTable,Memory);

		PrintMemHash(HashTable, Memory, root);
		Print(root, root, HashTable, Memory);
		cout << endl << endl;
	}
	*/
}