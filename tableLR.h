#ifndef TABLELR_H
#define TABLELR_H

#include <iostream>
#include <string>
#include <vector>

#include "grammar.h"

// Функции поиска символа в строке
bool char_non_existance_check(std::string&, char);
bool char_existance_check(std::string&, char);

struct TableLR {
	std::vector<std::string> L; // Левые части
	std::vector<std::string> R; // Правые части
	std::vector<char> symbols; // Нетерминалы
	
	TableLR(Grammar&);
	
	void output();
};

#endif
