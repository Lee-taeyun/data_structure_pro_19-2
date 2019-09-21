#include "header.h"



//참조로 받고, 포인터로 받아야한다.
void Initialize(Memo& memory, HashTable& hash) {

	memory.Nodes = new Node[MEM_SIZE];
	//일단은 노드의 rchild에는 전부
	for (int i = 0; i < MEM_SIZE; i++) {
		memory.Nodes[i].rchild = i + 1;
		if (i == MEM_SIZE - 1) {
			memory.Nodes[i].rchild = -1;
		}
		//맨끝 freelist에는 -1을 넣어주겠다 끝낸다는 의미로
	}

	memory.free_list = 1;
	//free_list는 1으로 해두자 일단은 ㅇㅇㅇㅇㅇ

	hash.tabs = new Hashtab[HASH_SIZE];
	//'(', ')'도 HASHTAB에 넣어줘야한다. 1에 '(', 2에 ')']

	hash.tabs[1].symbol = "(";
	hash.tabs[2].symbol = ")";


}

int calculate_hash(const string& str) {
	int sum = 0;
	for (char i : str) {
		sum += i;
	}
	return (sum % HASH_SIZE != 0 ? sum % HASH_SIZE : 1);
}//0을 비워둬야 해서 그러하다. 0은 건드리지 마라!!!

//설마 HASHSIZE는 안변하겠지..?
int GetHashValue(const string& str, HashTable& hash, int hash_len) {
	if (str == "(")
		return LEFT_PEREN;
	if (str == ")")
		return RIGHT_PEREN;
	//위의 경우는 특수케이스

	int cal = calculate_hash(str);
	//일단 비어있는지 확인하고 안 비어있으면 symbol이 같은지 확인.
	//혹시 끝까지 갔으면 처음으로 돌아가서 
	while (hash.tabs[cal].symbol != "") {
		if (hash.tabs[cal].symbol == str) {
			return cal;
		}

		if (cal == hash_len - 1) {
			cal = 1;
		}
		else
			cal++;
	}

	hash.tabs[cal].symbol = Downgrade(str);

	return cal;
	//str의 hash table 주소를 반환한다.
}
//getline으로 string을 받고 나면 그것의 첫번째 토큰만 잘라버리자. 첫번째 토큰의 마지막 index를 리턴 한글자면 0

int stringParser(const string& str) {
	//이순서가 맞나 싶다...

	//str ==""일때, 아니며 공백만 있을때
	if (str == "")
		return -1;
	if (str[0] == '(' || str[0] == ')') {
		return 0;
	}

	int len = str.length();
	//이제 blank 없이 시작한다.
	for (int i = 0; i < len; i++) {

		if (str[i] == '(' || str[i] == ')' || str[i] == ' ') {
			return i - 1;
		}

	}

}
//아무것도 없으면 -1을반환하는 stringParser



string no_front_blank(string& str) {
	int len = str.length();
	int i = 0;
	for (; i < len; i++) {
		if (str[i] != ' ') {
			break;
		}
	}
	return str.substr(i);
}


string GetNextToken(string& str) {

	string no_blank = no_front_blank(str);
	int tokenindex = stringParser(no_blank);
	if (tokenindex != -1) {

		string token = no_blank.substr(0, tokenindex + 1);

		if (tokenindex != no_blank.length())
			str = no_blank.substr(tokenindex + 1);
		else
			str = "";
		return token;
	}
	else {
		return (str = "");
	}

}

string Downgrade(string str) {
	for (char& i : str) {
		if (i >= 'A' && i <= 'Z') {
			i += 'a' - 'A';
		}
	}
	return str;
}
//GetNextToken에서 str을 바꿔준다.
int Read(string& str, HashTable& hash, int hash_len, Memo& memory) {
	int root = 0;//아직 루트가 할당받은게 없다.
	int first = 1;//first == true;
	
	if (str == "") {
		return -1;
	}
	//빈 스트링이 들어오면 -1을 출력
	int tokenHash = GetHashValue(GetNextToken(str), hash, hash_len);
	int temp; // temp는 현재 노드
	if (tokenHash == LEFT_PEREN) {
		while ((tokenHash = GetHashValue(GetNextToken(str), hash, hash_len)) != RIGHT_PEREN) {
			if (first == 1) {
				root = memory.free_list;// 처음시작할때는 nodearray ID =1<- 이거 굉장히 중요
				temp = root;
				memory.free_list = memory.Nodes[temp].rchild;//freelist에서는 그 다음 node를 가르켜줘야지

				// 처음이자 마지막 노드니까 rchild에는 0이 들어가줘야한다.
				first = 0;
			}
			else {
				memory.Nodes[temp].rchild = memory.free_list;
				temp = memory.Nodes[temp].rchild;
				memory.free_list = memory.Nodes[temp].rchild;

			}
			if (tokenHash == LEFT_PEREN) {
				str = '(' + str;// 안 읽은 것처럼 앞에 스리슬쩍 '('을 붙여준다.
				memory.Nodes[temp].lchild = Read(str, hash, hash_len, memory);
			}
			else {
				memory.Nodes[temp].lchild = -tokenHash;
			}
			memory.Nodes[temp].rchild = 0;//현재 노드엔 연결된게 없으니까 rchild=0로 해두는거지

		}
		return root;

	}
	else return -tokenHash;

	/*아직 미완성인 코드입니다!!!!!!!! */

	;
}

Memo::~Memo() {
	delete[] Nodes;
}

HashTable::~HashTable() {
	delete[] tabs;
}

//delete[]랑 delete랑 차이가 뭐드라
void Memreset(Memo& memo, int size,int index) {
	if (index <= 0)//root가 0보다 작거나 같으면 list자체가 생기질 않음
		return;

	int finish = 0;
		
	if (memo.Nodes[index].rchild > 0){

		Memreset(memo, size, memo.Nodes[index].rchild);
	}
	if (memo.Nodes[index].lchild > 0) {
		Memreset(memo, size, memo.Nodes[index].lchild);
	}

	memo.Nodes[index].rchild = memo.free_list;
	memo.free_list = index;
	
	
}
void Print(int root, int index, HashTable& hash,  Memo& memory) {
	if (root == 0) {
		cout << "()";
		return;
	}
	else if (root < 0) {
		cout << hash.tabs[-root].symbol << ' ';
	}
	else {
		if (index == root) {
			cout << "( ";
		}
		if (memory.Nodes[index].lchild >= 0) {
			Print(memory.Nodes[index].lchild, memory.Nodes[index].lchild, hash,  memory);
		}
		else {
			cout << hash.tabs[-memory.Nodes[index].lchild].symbol << ' ';
		}

		if (memory.Nodes[index].rchild != 0) {
			Print(root, memory.Nodes[index].rchild, hash, memory);
		}
		else
			cout << ") ";
	}

}
void Printtree(int root, int index, const Memo& memory) {
	if (root <= 0)
		return;
	cout << setw(12) << index << setw(12) << memory.Nodes[index].lchild << setw(12) << memory.Nodes[index].rchild << endl;

	if (memory.Nodes[index].lchild > 0)
		Printtree(memory.Nodes[index].lchild, memory.Nodes[index].lchild, memory);
	if(memory.Nodes[index].rchild > 0)
		Printtree(root, memory.Nodes[index].rchild, memory);

}

void Printfree(int free_list, const Memo& memory) {
	if (free_list > 0) {
		cout << setw(12) << free_list << setw(12) << memory.Nodes[free_list].lchild << setw(12) << memory.Nodes[free_list].rchild << endl;
		Printfree(memory.Nodes[free_list].rchild, memory);
	}
	
}

void PrintMemHash(const HashTable& HashTable, const Memo& Memory,int root) {

	cout << "] Free list's root = " << Memory.free_list<<endl;

	cout << "List's root = " << root << endl;

	cout << "Memory table =" << endl;
	cout << setw(12) << "Index" << setw(12) << "lchild" << setw(12) << "rchild" << endl;
	cout << " Tree=" << endl;
	
	Printtree(root, root,  Memory);
	cout << endl;
	
	cout << " free list=" << endl;
	//Printtree(Memory.free_list, Memory.free_list,  Memory);
	Printfree(Memory.free_list, Memory);

	cout << endl;

	cout << "Hash table =" << endl;
	cout << setw(12) << "Hash Index" << setw(12) << "symbol" << endl;
	for (int i = 0; i < HASH_SIZE; i++) {
		if(HashTable.tabs[i].symbol != "")
			cout << setw(12) << i << ' ' << setw(12) << HashTable.tabs[i].symbol << endl;
	}

	cout << endl;
}