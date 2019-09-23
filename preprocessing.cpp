#include "header.h"

string Preprocessing(string& command) {
	string newcommand = "";
	string token;
	while ((token = GetNextToken(command)) != "") {
		if (token == "define") {
			newcommand = newcommand + " define";
			token = GetNextToken(command);
			if (token == "(") {
				token = GetNextToken(command);
				newcommand = newcommand + " "+token + " ( lambda(" + Preprocessing(command) + " ) )";
			}
			else {
				newcommand = newcommand +" "+ token;
			}
		}
		else if (token == "'") {
			newcommand = newcommand + " (quote";
			int Number_of_left = 0;
			do {
				token = GetNextToken(command);
				newcommand = newcommand +" "+ token;
				if (token == "(") {
					Number_of_left++;
				}
				else if (token == ")") {
					Number_of_left--;
				}
			} while (Number_of_left > 0);
			newcommand = newcommand + " )";

		}
		else {
			newcommand = newcommand + " "+ token;
		}
	}

	return newcommand;
}

int Getval(int hash_value, HashTable& hash) {
	//���� symbol�� ���ڸ� ���ڰ��� return
	//���� symbol�� �ٸ� ���̶�� ex)x �� ���� ����, linkvalue�� Ÿ�� ����.
	//x�� link value�� ������ error�� ����.
	if (is_digit(hash.tabs[-hash_value].symbol)) {
		return atoi(hash.tabs[-hash_value].symbol.c_str());
	}
	else if (hash.tabs[-hash_value].Linkvalue < 0) {
		Getval(hash.tabs[-hash_value].Linkvalue, hash);
	}
	else {
		cout << "symbol binded to no value" << endl;
		return -1;//-1�� ��ȯ�ϴ°��� ������
	}
}

//Eval�� return���� index�ϼ��� �ְ�, Ȥ�� hashvalue�ϼ��� �ִ�. Hashvalue�ϰ�쿡�� ������ �����Ѵ�.
int Eval(int root, HashTable& hash, Memo& memory) {
	//one symbol(hash_value) or empty list
	
	if (root <= 0) // ��ȣ�� �������� �ȵ������� ������ �� �𸣰ڴ�
		return root;
	int token_index = -memory.Nodes[root].lchild;
	
	if (token_index == PLUS) {
		return -GetHashValue(to_string(Getval(Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory), hash)
			+ Getval(Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory), hash)), hash, HASH_SIZE);
	}
	else if (token_index == MINUS) {
		return -GetHashValue(to_string(Getval(Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory), hash)
			- Getval(Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory), hash)), hash, HASH_SIZE);
	}
	else if (token_index == TIMES) {
		return -GetHashValue(to_string(Getval(Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory), hash)
			* Getval(Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory), hash)), hash, HASH_SIZE);
	}
	else if (token_index == CONS) {
		int new_memory = memory.free_list;//ALLOC()
		memory.free_list = memory.Nodes[memory.free_list].rchild;//freelist�Ű��ְ�
		memory.Nodes[new_memory].lchild = Eval(memory.Nodes[memory.Nodes[root].rchild].lchild,hash, memory);
		memory.Nodes[new_memory].rchild = Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory);
		return new_memory;
	}
	else if (token_index == COND) {
		while (memory.Nodes[(memory.Nodes[root].rchild)].rchild != 0) {
			root = memory.Nodes[root].rchild;
			if (Eval(memory.Nodes[memory.Nodes[root].lchild].lchild,hash,memory) == true) {//==�̶� !=�̶� < > <= >=�� �ؾ߰ڳ�
				return Eval(memory)
























			}

		}
	}
	
	
}

bool is_digit(string str) {
	return atoi(str.c_str()) != 0 || str.compare("0") == 0;
}//���ڸ� true, �ϰ͵� �ƴϸ� false


