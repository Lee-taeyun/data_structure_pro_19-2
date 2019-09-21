#pragma once

#include<iomanip>
#include <string>
#include <iostream>

#define MEM_SIZE 31
#define HASH_SIZE 100

#define LEFT_PEREN 1//���� ��ȣ�� hashvalue=1, ������ ��ȣ�� hashvalue =2
#define RIGHT_PEREN 2


using namespace std;

class Node {
public:
	int lchild = 0;
	int rchild = 0;
};
/* ������ car�� �� ���� ��� ���� ����� �� or hashtable�� hashvalue; hashvalue�̸� ���� ������ ����, ��������� id�̸� ����� ��
�����ʿ��� cdr�Ѱ� ���� ���� ����̰ų�, ���̸� NULL
*/
class Memo {
public:
	Node* Nodes;
	int free_list;
	~Memo();//destructor�� ��������
};


class Hashtab {
public:
	string symbol = "";
	int Linkvalue = 0;//�̹� ������Ʈ������ null�� ����

};

class HashTable {
public:
	Hashtab* tabs;
	~HashTable();
};
// symbol�̶� linkvalue �Ǹ� ������ �ǰ���

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