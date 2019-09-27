#include "header.h"


Stack stack;
Stack arg_stack;
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
	hash.tabs[tabs[top].argument].Linkvalue = tabs[top].assigned;
	return tabs[top--];
}
Stab Stack::pop() {
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

int Getval(int hash_value, HashTable& hash, Memo& memory) {
	//만약 symbol이 숫자면 숫자값을 return
	//만약 symbol이 다른 것이라면 ex)x 와 같은 변수, linkvalue를 타고 간다.
	//x에 link value가 없으면 error를 띄운다.
	if (hash_value > 0) {
		return Eval(hash_value, hash, memory);
	}

	if (is_digit(hash.tabs[-hash_value].symbol)) {
		return atoi(hash.tabs[-hash_value].symbol.c_str());
	}
	else if (hash.tabs[-hash_value].Linkvalue < 0) {
		Getval(hash.tabs[-hash_value].Linkvalue, hash,memory);
	}
	else {
		if (hash_value == 0) {
			cout << "nothing to implement" << endl;
			return -ERROR;
		}
		if (hash.tabs[-hash_value].Linkvalue == 0) {
			//cout << "symbol binded to no value or empty List" << endl;
			return -ERROR;//-1을 반환하는것은 에러다
		}
		if (hash.tabs[-hash_value].Linkvalue > 0) {
			return -NODE_INDEX;
		}
	}
}
int GetNode(int hash_value, HashTable& hash, Memo& memory) {

	
	if (hash.tabs[-hash_value].Linkvalue < 0) {
		GetNode(hash.tabs[-hash_value].Linkvalue, hash, memory);
	}
	else {
		return hash.tabs[-hash_value].Linkvalue;
	}
}

//Eval의 return형은 index일수도 있고, 혹은 hashvalue일수도 있다. Hashvalue일경우에는 음수를 리턴한다.
int Eval(int root, HashTable& hash, Memo& memory) {
	//one symbol(hash_value) or empty list

	if (root <= 0) { // 등호가 들어가야할지 안들어가야할지 솔직히 잘 모르겠다
		if (Getval(root, hash, memory) == -ERROR)
			return 0;
		else if (Getval(root, hash, memory) == -NODE_INDEX) {
			return GetNode(root, hash, memory);
		}
		return -GetHashValue(to_string(Getval(root, hash, memory)), hash, HASH_SIZE); //
	}
	
	int token_index = -memory.Nodes[root].lchild;



	if (token_index == PLUS) {
		return -GetHashValue(to_string(Getval(Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory), hash,memory)
			+ Getval(Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory), hash, memory)), hash, HASH_SIZE);
	}
	else if (token_index == MINUS) {
		return -GetHashValue(to_string(Getval(Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory), hash, memory)
			- Getval(Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory), hash, memory)), hash, HASH_SIZE);
	}
	else if (token_index == TIMES) {
		return -GetHashValue(to_string(Getval(Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory), hash, memory)
			* Getval(Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory), hash, memory)), hash, HASH_SIZE);
	}
	else if (token_index == LEFT_LARGE) {
		return ((Getval(Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory), hash, memory)
	> Getval(Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory), hash, memory)) ? -TRUE : -FALSE);
	}
	else if (token_index == RIGHT_LARGE) {
		return ((Getval(Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory), hash, memory)
			< Getval(Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory), hash, memory)) ? -TRUE : -FALSE);
	}
	else if (token_index == LEFT_EQLARGE) {
		return ((Getval(Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory), hash, memory)
			>= Getval(Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory), hash, memory)) ? -TRUE : -FALSE);
	}
	else if (token_index == RIGHT_EQLARGE) {
		return ((Getval(Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory), hash, memory)
			<= Getval(Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory), hash, memory)) ? -TRUE : -FALSE);
	}
	else if (token_index == EQ) {
		return ((Getval(Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory), hash, memory)
			== Getval(Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory), hash, memory)) ? -TRUE : -FALSE);
	}
	else if (token_index == NOT_EQ) {
		return ((Getval(Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory), hash, memory)
			!= Getval(Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].rchild].rchild].lchild, hash, memory), hash, memory)) ? -TRUE : -FALSE);
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
			if (-Eval(memory.Nodes[memory.Nodes[root].lchild].lchild, hash, memory) == TRUE) {//==??¶? !=??¶? < > <= >=?? ?Ø¾ß°?³×
				return Eval(memory.Nodes[memory.Nodes[memory.Nodes[root].lchild].rchild].lchild, hash, memory);
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
	else if (token_index == NULL_Q) {//뒤에 리스트가 나온다고 가정.
		return (memory.Nodes[Eval(memory.Nodes[memory.Nodes[root].rchild].lchild, hash, memory)].lchild ==0? -TRUE: -FALSE);
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
			int linkv = Eval(memory.Nodes[temp].lchild, hash, memory);//인자들을 먼저 eval을 해준다... 그게 먼저지
			arg_stack.push(Stab(-memory.Nodes[function_arg].lchild, linkv));

			function_arg = memory.Nodes[function_arg].rchild;
			temp = memory.Nodes[temp].rchild;
		}
		
		function_arg = memory.Nodes[memory.Nodes[hash.tabs[token_index].Linkvalue].rchild].lchild;
		for(int i=0; i< arg_num; i++){
			Stab temp = arg_stack.pop();
			stack.push(Stab(temp.argument, hash.tabs[temp.argument].Linkvalue));
			hash.tabs[temp.argument].Linkvalue = temp.assigned;
			function_arg = memory.Nodes[function_arg].rchild;

		}

		int result = Eval(memory.Nodes[memory.Nodes[memory.Nodes[hash.tabs[token_index].Linkvalue].rchild].rchild].lchild, hash, memory);
		
		for (int i = 0; i < arg_num; i++) {
			stack.pop(hash,memory);
		}

		return result;

	}

	else {
		return Eval(memory.Nodes[-token_index].lchild,hash, memory);//이게 존나 애매하네.... getval을 해줘야하나 말아야하나
	}

}

bool is_digit(string str) {
	return atoi(str.c_str()) != 0 || str.compare("0") == 0;
}//숫자면 true, 암것도 아니면 false
