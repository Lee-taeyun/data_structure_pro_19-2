#pragma once

#include<iomanip>
#include <string>
#include <iostream>

#define MEM_SIZE 31
#define HASH_SIZE 100

#define LEFT_PEREN 1//왼쪽 괄호의 hashvalue=1, 오른쪽 괄호의 hashvalue =2
#define RIGHT_PEREN 2


using namespace std;

class Node {
public:
	int lchild = 0;
	int rchild = 0;
};
/* 왼쪽은 car한 거 예를 들어 다음 노드의 값 or hashtable의 hashvalue; hashvalue이면 음의 값으로 들어가고, 다음노드의 id이면 양수로 들어감
오른쪽에는 cdr한거 보통 다음 노드이거나, 끝이면 NULL
*/
class Memo {
public:
	Node* Nodes;
	int free_list;
	~Memo();//destructor꼭 만들어줘라
};


class Hashtab {
public:
	string symbol = "";
	int Linkvalue = 0;//이번 프로젝트에서는 null로 고정

};

class HashTable {
public:
	Hashtab* tabs;
	~HashTable();
};
// symbol이랑 linkvalue 탭만 있을면 되겠지

void Initialize(Memo& memory, HashTable& hash);
int Read(string& str, HashTable& hash, int hash_len, Memo& memory);
void Print(int root, int index, HashTable& hash, Memo& memory);
void Memreset(Memo& memo, int size,int index);
void PrintMemHash(const HashTable& HashTable, const Memo& Memory, int root);

int calculate_hash(const string& str);
int GetHashValue(const string& str, HashTable& hash, int hash_len);
int stringParser(const string& str);
string no_front_blank(string& str);
string GetNextToken(string& str);
string Downgrade(string str);
string Preprocessing(string& str);
int Eval(int root, HashTable& hash, Memo& memory);