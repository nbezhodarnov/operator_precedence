#ifndef OPM_H
#define OPM_H

#include <iostream>
#include <vector>
#include <string>

#include "grammar.h"
#include "tableLRt.h"

class OPM {
private:
	std::vector<std::vector<char>> matrix; // Матрица операторного предшествования
	
	char& get_element(char&, char&);
	int get_index(char&);
	void fill_line(char&, std::string&, char);
	void fill_row(char&, std::string&, char);
	
public:
	Grammar grammar; // Грамматика
	std::vector<char> polysis_symbols_add; // Массив символов для добавления в полис
	
	OPM(Grammar&, TableLRt&);
	char get_precedence(char&, char&);
	void output();
};

#endif
