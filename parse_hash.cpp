#include "header.h"

/* calculate hash value and get parsed string*/


int calculate_hash(const string& str) {
	int sum = 0;
	for (char i : str) {
		sum += i;
	}
	return (sum % HASH_SIZE != 0 ? sum % HASH_SIZE : 1);
}//0을 비워둬야 해서 그러하다. 0은 건드리지 마라!!!

//설마 HASHSIZE는 안변하겠지..?
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

	//위의 경우는 특수케이스

	int cal = calculate_hash(str2);
	//일단 비어있는지 확인하고 안 비어있으면 symbol이 같은지 확인.
	//혹시 끝까지 갔으면 처음으로 돌아가서 
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
	//str의 hash table 주소를 반환한다.
}
//getline으로 string을 받고 나면 그것의 첫번째 토큰만 잘라버리자. 첫번째 토큰의 마지막 index를 리턴 한글자면 0

int stringParser(const string& str) {
	//이순서가 맞나 싶다...

	//str ==""일때, 아니며 공백만 있을때
	if (str == "")
		return -1;
	if (str[0] == '(' || str[0] == ')' || str[0] =='\'') {
		return 0;
	}

	int len = str.length();
	//이제 blank 없이 시작한다.
	for (int i = 0; i < len; i++) {

		if (str[i] == '(' || str[i] == ')' || str[i] == ' ' || str[i] == '\'') {
			return i - 1;
		}

	}
	return str.length();

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