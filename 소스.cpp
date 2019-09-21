#include "header.h"


int main() {

	Memo Memory;
	HashTable HashTable;

	Initialize(Memory, HashTable);
	string A;


	while (cout << ">", getline(cin, A)) {
		if (A == "") break;
		while (no_front_blank(A) != "") {
			int root = Read(A, HashTable, MEM_SIZE, Memory);
			PrintMemHash(HashTable, Memory, root);
			Print(root, root, HashTable, Memory);
			Memreset(Memory, MEM_SIZE,root);
			cout << endl<<endl;
		}


	}

}