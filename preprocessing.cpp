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
	//만약 symbol이 숫자면 숫자값을 return
	//만약 symbol이 다른 것이라면 ex)x 와 같은 변수, linkvalue를 타고 간다.
	//x에 link value가 없으면 error를 띄운다.
	if (is_digit(hash.tabs[-hash_value].symbol)) {
		return atoi(hash.tabs[-hash_value].symbol.c_str());
	}
	else if (hash.tabs[-hash_value].Linkvalue < 0) {
		Getval(hash.tabs[-hash_value].Linkvalue, hash);
	}
	else {
		cout << "symbol binded to no value" << endl;
		return -1;//-1을 반환하는것은 에러다
	}
}

//Eval의 return형은 index일수도 있고, 혹은 hashvalue일수도 있다. Hashvalue일경우에는 음수를 리턴한다.
int Eval(int root, HashTable& hash, Memo& memory) {
	//one symbol(hash_value) or empty list
	
	if (root <= 0) // 등호가 들어가야할지 안들어가야할지 솔직히 잘 모르겠다
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
		memory.free_list = memory.Nodes[memory.free_list].rchild;//freelist옮겨주고
		memory.Nodes[new_memory].lchild = Eval(memory.Nodes[memory.Nodes[root].rchild].lchild,hash, memory);
		memory.Nodes[new_memory].rchild = Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory);
		return new_memory;
	}
	else if (token_index == COND) {
		while (memory.Nodes[(memory.Nodes[root].rchild)].rchild != 0) {
			root = memory.Nodes[root].rchild;
			if (Eval(memory.Nodes[memory.Nodes[root].lchild].lchild,hash,memory) == true) {//==이랑 !=이랑 < > <= >=도 해야겠네
				return Eval(memory)
























			}

		}
	}
	
	
}

bool is_digit(string str) {
	return atoi(str.c_str()) != 0 || str.compare("0") == 0;
}//숫자면 true, 암것도 아니면 false


