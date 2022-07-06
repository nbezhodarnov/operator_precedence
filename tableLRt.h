#ifndef TABLELRT_H
#define TABLELRT_H

#include <iostream>
#include <string>
#include <vector>

#include "grammar.h"
#include "tableLR.h"

struct TableLRt {
	std::vector<std::string> L; // Левые части правил
	std::vector<std::string> R; // Правые части правил
	std::vector<char> symbols; // Нетерминалы
	
	TableLRt(Grammar&, TableLR&);
	
	void output();
};

#endif
