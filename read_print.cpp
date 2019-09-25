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

	hash.tabs[LEFT_PEREN].symbol = "(";
	hash.tabs[RIGHT_PEREN].symbol = ")";
	hash.tabs[PLUS].symbol = "+";
	hash.tabs[MINUS].symbol = "-";
	hash.tabs[TIMES].symbol = "*";
	hash.tabs[CONS].symbol = "cons";
	hash.tabs[COND].symbol = "cond";
	hash.tabs[CAR].symbol = "car";
	hash.tabs[CDR].symbol = "cdr";
	hash.tabs[DEFINE].symbol = "define";
	hash.tabs[QUOTE].symbol = "quote";
	hash.tabs[LEFT_LARGE].symbol = ">"; // >
	hash.tabs[RIGHT_LARGE].symbol = "<"; //<
	hash.tabs[LEFT_EQLARGE].symbol = ">="; //>=
	hash.tabs[RIGHT_EQLARGE].symbol = "<=";//<=
	hash.tabs[EQ].symbol = "=="; //==
	hash.tabs[NOT_EQ].symbol = "!=";
	hash.tabs[ELSE].symbol = "else";
	hash.tabs[LAMBDA].symbol = "lambda";
	hash.tabs[TRUE].symbol = "#t";
	hash.tabs[FALSE].symbol = "#f";

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
	
	//Printfree(Memory.free_list, Memory);

	cout << endl;

	cout << "Hash table =" << endl;
	cout << setw(12) << "Hash Index" << setw(12) << "symbol" << endl;
	for (int i = 0; i < HASH_SIZE; i++) {
		if(HashTable.tabs[i].symbol != "")
			cout << setw(12) << i << ' ' << setw(12) << HashTable.tabs[i].symbol <<setw(12) << HashTable.tabs[i].Linkvalue << endl;
	}

	cout << endl;
}