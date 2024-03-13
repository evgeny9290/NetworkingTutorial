#pragma once

#include "Common.h"
#include <string>

inline std::string getLineFromUser() {
	std::string inp;
	std::getline(std::cin, inp);
	return inp;
}

inline bool checkIfMsgTooLong(const std::string& a_inp) {
	if (a_inp.size() >= MAXLINE - 1) {
		std::cin.clear();
		std::cout << "your request is too long, retard" << std::endl;
		return false;
	}
	return true;
}

inline bool checkIfMsgEmpty(const std::string& a_inp) {
	if (strcmp(a_inp.c_str(), "\0") == 0) {
		return false;
	}
	return true;
}

inline bool checkAppropriateCommand(const std::string& a_inp) {
	bool msgIsGood = true;
	msgIsGood &= checkIfMsgTooLong(a_inp);
	msgIsGood &= checkIfMsgEmpty(a_inp);
	return msgIsGood;

}