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

	hash.tabs[1].symbol = "(";
	hash.tabs[2].symbol = ")";


}

int calculate_hash(const string& str) {
	int sum = 0;
	for (char i : str) {
		sum += i;
	}
	return (sum % HASH_SIZE != 0 ? sum % HASH_SIZE : 1);
}//0�� ����־� �ؼ� �׷��ϴ�. 0�� �ǵ帮�� ����!!!

//���� HASHSIZE�� �Ⱥ��ϰ���..?
int GetHashValue(const string& str, HashTable& hash, int hash_len) {
	if (str == "(")
		return LEFT_PEREN;
	if (str == ")")
		return RIGHT_PEREN;
	//���� ���� Ư�����̽�

	int cal = calculate_hash(str);
	//�ϴ� ����ִ��� Ȯ���ϰ� �� ��������� symbol�� ������ Ȯ��.
	//Ȥ�� ������ ������ ó������ ���ư��� 
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
	//str�� hash table �ּҸ� ��ȯ�Ѵ�.
}
//getline���� string�� �ް� ���� �װ��� ù��° ��ū�� �߶������. ù��° ��ū�� ������ index�� ���� �ѱ��ڸ� 0

int stringParser(const string& str) {
	//�̼����� �³� �ʹ�...

	//str ==""�϶�, �ƴϸ� ���鸸 ������
	if (str == "")
		return -1;
	if (str[0] == '(' || str[0] == ')') {
		return 0;
	}

	int len = str.length();
	//���� blank ���� �����Ѵ�.
	for (int i = 0; i < len; i++) {

		if (str[i] == '(' || str[i] == ')' || str[i] == ' ') {
			return i - 1;
		}

	}

}
//�ƹ��͵� ������ -1����ȯ�ϴ� stringParser



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