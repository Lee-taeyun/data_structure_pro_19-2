#include "header.h"

//������ �ް�, �����ͷ� �޾ƾ��Ѵ�.
void Initialize(Memo& memory, HashTable& hash) {

	memory.Nodes = new Node[MEM_SIZE];
	//�ϴ��� ����� rchild���� ����
	for (int i = 0; i < MEM_SIZE; i++) {
		memory.Nodes[i].rchild = i + 1;
		if (i == MEM_SIZE - 1) {
			memory.Nodes[i].rchild = -1;
		}
		//�ǳ� freelist���� -1�� �־��ְڴ� �����ٴ� �ǹ̷�
	}

	memory.free_list = 1;
	//free_list�� 1���� �ص��� �ϴ��� ����������

	hash.tabs = new Hashtab[HASH_SIZE];
	//'(', ')'�� HASHTAB�� �־�����Ѵ�. 1�� '(', 2�� ')']

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


//GetNextToken���� str�� �ٲ��ش�.
int Read(string& str, HashTable& hash, int hash_len, Memo& memory) {
	int root = 0;//���� ��Ʈ�� �Ҵ������ ����.
	int first = 1;//first == true;
	
	if (str == "") {
		return -1;
	}
	//�� ��Ʈ���� ������ -1�� ���
	int tokenHash = GetHashValue(GetNextToken(str), hash, hash_len);
	int temp; // temp�� ���� ���
	if (tokenHash == LEFT_PEREN) {
		while ((tokenHash = GetHashValue(GetNextToken(str), hash, hash_len)) != RIGHT_PEREN) {
			if (first == 1) {
				root = memory.free_list;// ó�������Ҷ��� nodearray ID =1<- �̰� ������ �߿�
				temp = root;
				memory.free_list = memory.Nodes[temp].rchild;//freelist������ �� ���� node�� �����������

				// ó������ ������ ���ϱ� rchild���� 0�� ������Ѵ�.
				first = 0;
			}
			else {
				memory.Nodes[temp].rchild = memory.free_list;
				temp = memory.Nodes[temp].rchild;
				memory.free_list = memory.Nodes[temp].rchild;

			}
			if (tokenHash == LEFT_PEREN) {
				str = '(' + str;// �� ���� ��ó�� �տ� ������½ '('�� �ٿ��ش�.
				memory.Nodes[temp].lchild = Read(str, hash, hash_len, memory);
			}
			else {
				memory.Nodes[temp].lchild = -tokenHash;
			}
			memory.Nodes[temp].rchild = 0;//���� ��忣 ����Ȱ� �����ϱ� rchild=0�� �صδ°���

		}
		return root;

	}
	else return -tokenHash;

	/*���� �̿ϼ��� �ڵ��Դϴ�!!!!!!!! */

	;
}

Memo::~Memo() {
	delete[] Nodes;
}

HashTable::~HashTable() {
	delete[] tabs;
}

//delete[]�� delete�� ���̰� �����
void Memreset(Memo& memo, int size,int index) {
	if (index <= 0)//root�� 0���� �۰ų� ������ list��ü�� ������ ����
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