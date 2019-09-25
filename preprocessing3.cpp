#include "header.h"


Stack stack;
Stab::Stab() {}
Stab::Stab(int a, int b) {
	argument = a;
	assigned = b;
}
Stack::Stack() {
	tabs = new Stab[capacity];
	top = -1;

}

Stack::~Stack() {
	delete[] tabs;
}
void Stack::push(Stab value) {
	if (top == capacity - 1) {
		Stab* new_tabs = new Stab[capacity * 2];

		for (int i = 0; i < capacity; i++) {
			new_tabs[i] = tabs[i];
		}
		capacity *= 2;
		delete[] tabs;
		tabs = new_tabs;

	}
	top++;
	tabs[top] = value;

}
Stab Stack::pop(HashTable& hash, Memo & memory) {
	hash.tabs[-tabs[top].argument].Linkvalue = tabs[top].assigned;
	return tabs[top--];
}
string Preprocessing(string& command) {
	string newcommand = "";
	string token;
	while ((token = GetNextToken(command)) != "") {
		if (token == "define") {
			newcommand = newcommand + " define";
			token = GetNextToken(command);
			if (token == "(") {
				token = GetNextToken(command);
				newcommand = newcommand + " " + token + " ( lambda(" + Preprocessing(command) + " )";
			}
			else {
				newcommand = newcommand + " " + token;
			}
		}
		else if (token == "'") {
			newcommand = newcommand + " (quote";
			int Number_of_left = 0;
			do {
				token = GetNextToken(command);
				newcommand = newcommand + " " + token;
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
			newcommand = newcommand + " " + token;
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
	else if (token_index == LEFT_LARGE) {
		return ((Getval(Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory), hash)
	> Getval(Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory), hash)) ? -TRUE : -FALSE);
	}
	else if (token_index == RIGHT_LARGE) {
		return ((Getval(Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory), hash)
			< Getval(Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory), hash)) ? -TRUE : -FALSE);
	}
	else if (token_index == LEFT_EQLARGE) {
		return ((Getval(Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory), hash)
			>= Getval(Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory), hash)) ? -TRUE : -FALSE);
	}
	else if (token_index == RIGHT_EQLARGE) {
		return ((Getval(Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory), hash)
			<= Getval(Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory), hash)) ? -TRUE : -FALSE);
	}
	else if (token_index == EQ) {
		return ((Getval(Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory), hash)
			== Getval(Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory), hash)) ? -TRUE : -FALSE);
	}
	else if (token_index == NOT_EQ) {
		return ((Getval(Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory), hash)
			!= Getval(Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory), hash)) ? -TRUE : -FALSE);
	}
	else if (token_index == CONS) {
		int new_memory = memory.free_list;//ALLOC()
		memory.free_list = memory.Nodes[memory.free_list].rchild;//freelist옮겨주고
		memory.Nodes[new_memory].lchild = Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory);
		memory.Nodes[new_memory].rchild = Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory);
		return new_memory;
	}

	else if (token_index == COND) {
		while (memory.Nodes[(memory.Nodes[root].rchild)].rchild != 0) {
			root = memory.Nodes[root].rchild;
			if (Eval(memory.Nodes[memory.Nodes[root].lchild].lchild, hash, memory) == TRUE) {//==??¶? !=??¶? < > <= >=?? ?Ø¾ß°?³×
				return Eval(memory.Nodes[memory.Nodes[root].lchild].rchild, hash, memory);
			}

		}
		if (-memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].lchild].lchild != ELSE) {
			cout << "error" << endl;
			return 0;//return뭐로해야하냐이거
		}
		return Eval(memory.Nodes[memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].lchild].rchild].lchild, hash, memory);
	}

	else if (token_index == CAR) {
		return memory.Nodes[Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory)].lchild;
	}
	else if (token_index == CDR) {
		return memory.Nodes[Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory)].rchild;
	}

	else if (token_index == QUOTE) {
		return memory.Nodes[memory.Nodes[root].rchild].lchild;
	}

	else if (token_index == DEFINE) {//여기가 논란의 여지가 좀 있을듯
		if (-memory.Nodes[memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild].lchild == LAMBDA) {
			hash.tabs[-memory.Nodes[memory.Nodes[root].rchild].lchild].Linkvalue = memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild;
			return root;
		}
		else {
			hash.tabs[-memory.Nodes[memory.Nodes[root].rchild].lchild].Linkvalue = Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory);
			return hash.tabs[-memory.Nodes[memory.Nodes[root].rchild].lchild].Linkvalue;
		}
	}

	else if (token_index > 0 && hash.tabs[token_index].Linkvalue != 0 && -memory.Nodes[hash.tabs[token_index].Linkvalue].lchild == LAMBDA) {//userdefined fucntion

		int function_arg = memory.Nodes[memory.Nodes[hash.tabs[token_index].Linkvalue].rchild].lchild;
		int temp = memory.Nodes[root].rchild; // 인자들이 나오겠지
		int arg_num = 0;
		while (temp != 0) {
			arg_num++;
			stack.push(Stab(memory.Nodes[function_arg].lchild, hash.tabs[-memory.Nodes[function_arg].lchild].Linkvalue));//(  ,hash value of assigned)
			hash.tabs[-memory.Nodes[function_arg].lchild].Linkvalue = Eval(memory.Nodes[temp].lchild, hash, memory);

			function_arg = memory.Nodes[function_arg].rchild;
			temp = memory.Nodes[temp].rchild;
		}//인수 갯수는 같은걸로 우리 생각하자 제발.... 살려주,,,,

		int result = Eval(memory.Nodes[memory.Nodes[memory.Nodes[hash.tabs[token_index].Linkvalue].rchild].rchild].lchild, hash, memory);
		
		for (int i = 0; i < arg_num; i++) {
			stack.pop(hash,memory);
		}

		return result;

	}

	else {
		return root;//이게 존나 애매하네.... getval을 해줘야하나 말아야하나
	}

}

bool is_digit(string str) {
	return atoi(str.c_str()) != 0 || str.compare("0") == 0;
}//숫자면 true, 암것도 아니면 false


