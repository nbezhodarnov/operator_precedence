#ifndef OPM_RECOGNIZER_H
#define OPM_RECOGNIZER_H

#include <iostream>
#include <vector>
#include <string>
#include <stack>

#include "opm.h"

class OPM_recognizer {
private:
	OPM opm; // Матрица операторного предшествования
	std::string polysis; // Полис
	std::vector<int> result; // Результат распознавания
	
	char stack_previous_element(std::stack<char>&);
	
public:
	OPM_recognizer(OPM&);
	std::vector<int> recognize(std::string);
	std::string get_polysis();
};

#endif
