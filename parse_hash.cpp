#include "header.h"

/* calculate hash value and get parsed string*/


int calculate_hash(const string& str) {
	int sum = 0;
	for (char i : str) {
		sum += i;
	}
	return (sum % HASH_SIZE != 0 ? sum % HASH_SIZE : 1);
}//0�� ����־� �ؼ� �׷��ϴ�. 0�� �ǵ帮�� ����!!!

//���� HASHSIZE�� �Ⱥ��ϰ���..?
int GetHashValue(const string& str, HashTable& hash, int hash_len) {
	string str2 = Downgrade(str);
	if (str2 == "(")
		return LEFT_PEREN;
	else if (str2 == ")")
		return RIGHT_PEREN;
	else if (str2 == "+")
		return PLUS;
	else if (str2 == "-")
		return MINUS;
	else if (str2 == "*")
		return TIMES;
	else if (str2 == "cons")
		return CONS;
	else if (str2 == "cond")
		return COND;
	else if (str2 == "car")
		return CAR;
	else if (str2 == "cdr")
		return CDR;
	else if (str2 == "define")
		return DEFINE;
	else if (str2 == "quote")
		return QUOTE;
	else if (str2 == ">")
		return LEFT_LARGE;
	else if (str2 == "<")
		return RIGHT_LARGE;
	else if (str2 == ">=")
		return LEFT_EQLARGE;
	else if (str2 == "<=")
		return RIGHT_EQLARGE;
	else if (str2 == "==")
		return EQ;
	else if (str2 == "!=")
		return NOT_EQ;
	else if (str2 == "else")
		return ELSE;
	else if (str2 == "lambda")
		return LAMBDA;
	else if (str2 == "#t")
		return TRUE;
	else if (str2 == "#f")
		return FALSE;
	else if (str == "error")
		return ERROR;
	else if (str == "null?")
		return NULL_Q;
	else if (str == "NODE")
		return NODE_INDEX;

	//���� ���� Ư�����̽�

	int cal = calculate_hash(str2);
	//�ϴ� ����ִ��� Ȯ���ϰ� �� ��������� symbol�� ������ Ȯ��.
	//Ȥ�� ������ ������ ó������ ���ư��� 
	while (hash.tabs[cal].symbol != "") {
		if (hash.tabs[cal].symbol == str2) {
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
	if (str[0] == '(' || str[0] == ')' || str[0] =='\'') {
		return 0;
	}

	int len = str.length();
	//���� blank ���� �����Ѵ�.
	for (int i = 0; i < len; i++) {

		if (str[i] == '(' || str[i] == ')' || str[i] == ' ' || str[i] == '\'') {
			return i - 1;
		}

	}
	return str.length();

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