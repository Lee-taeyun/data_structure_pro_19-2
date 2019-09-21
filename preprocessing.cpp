#include "header.h"

string Preprocessing(string& command) {
	string newcommand = "";
	string token;
	while ((token = GetNextToken(command)) != "") {
		if (token == "define") {
			newcommand = newcommand + " define";
			token = GetNextToken(command);
			if (token == "(") {
				token = GetNextToken(command);
				newcommand = newcommand + " "+token + " ( lambda(" + Preprocessing(command) + " ) )";
			}
			else {
				newcommand = newcommand +" "+ token;
			}
		}
		else if (token == "'") {
			newcommand = newcommand + " (quote";
			int Number_of_left = 0;
			do {
				token = GetNextToken(command);
				newcommand = newcommand +" "+ token;
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
			newcommand = newcommand + " "+ token;
		}
	}

	return newcommand;
}
