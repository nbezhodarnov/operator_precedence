#include "opm_recognizer.h"

OPM_recognizer::OPM_recognizer(OPM &opm_input): opm(opm_input) {
	
}

// Просмотр предыдущего элемента стека
char OPM_recognizer::stack_previous_element(std::stack<char> &stack_input) {
	if (stack_input.empty()) {
		return '\0';
	}
	char last = stack_input.top();
	stack_input.pop();
	if (stack_input.empty()) {
		stack_input.push(last);
		return '\0';
	}
	char symbol = stack_input.top();
	stack_input.push(last);
	return symbol;
}

// Распознавание
std::vector<int> OPM_recognizer::recognize(std::string str) {
	std::stack<char> str_stack;
	std::stack<int> result_temp;
	char symbol_left, symbol_right;
	str_stack.push(str[0]);
	symbol_left = str_stack.top();
	bool is_not_end = true, is_precedence_equal = false;
	int i = 1;
	
	while (is_not_end) {
		// Заполнение стека, пока статус предшествования - <
		while ((i < str.size()) && (opm.get_precedence(symbol_left, str[i]) == '<')) {
			str_stack.push(str[i]);
			symbol_left = str_stack.top();
			i++;
		}
		// Статус предшествования отсутсвует - ошибка
		if (opm.get_precedence(symbol_left, str[i]) == ' ') {
			std::cout << "Error in recognizing!\n";
			polysis = "";
			return std::vector<int>();
		}
		
		// Заполнение правила
		std::string rule;
		symbol_right = str[i];
		rule.insert(rule.begin(), str_stack.top());
		str_stack.pop();
		while (rule[0] != symbol_left) {
			rule.insert(rule.begin(), str_stack.top());
			str_stack.pop();
		}
		// Если статус предшествования - =, то вставка символа справа
		if (opm.get_precedence(symbol_left, symbol_right) == '=') {
			rule.push_back(symbol_right);
			is_precedence_equal = true;
		}
		if (!str_stack.empty()) {
			if (str_stack.top() != 'N') {
				symbol_left = str_stack.top();
			} else {
				symbol_left = stack_previous_element(str_stack);
			}
			if (rule[0] != 'N') {
				symbol_right = rule[0];
			}
			// Чтение стека, пока статус предшествования не равен <
			while (opm.get_precedence(symbol_left, symbol_right) != '<') {
				rule.insert(rule.begin(), symbol_left);
				if (rule[0] != 'N') {
					symbol_right = rule[0];
				}
				str_stack.pop();
				if (str_stack.top() != 'N') {
					symbol_left = str_stack.top();
				} else {
					symbol_left = stack_previous_element(str_stack);
				}
			}
			if (str_stack.top() == 'N') {
				rule.insert(rule.begin(), str_stack.top());
				str_stack.pop();
			}
		}
		
		// Поиск номера правила и последующее запоминание
		int rule_number = 0;
		while ((rule_number < opm.grammar.RR.size()) && (opm.grammar.RR[rule_number] != rule)) {
			rule_number++;
		}
		// Правило отсутствует - ошибка
		if (rule_number == opm.grammar.RR.size()) {
			std::cout << "Error in recognizing!\n";
			polysis = "";
			return std::vector<int>();
		}
		// Заполнение полиса
		if (opm.polysis_symbols_add[rule_number] != '\0') {
			polysis += opm.polysis_symbols_add[rule_number];
		}
		result_temp.push(rule_number + 1);
		
		// Заполнение временных переменных и проверка на конец
		if ((i < str.size() - 1) && (is_precedence_equal)) {
			i++;
			is_precedence_equal = false;
		}
		symbol_right = str[i];
		if (str_stack.empty()) {
			is_not_end = false;
		} else {
			str_stack.push('N');
		}
	}
	
	// Заполнение результата
	while (!result_temp.empty()) {
		result.push_back(result_temp.top());
		result_temp.pop();
	}
	return result;
}

// Чтение полиса
std::string OPM_recognizer::get_polysis() {
	return polysis;
}
