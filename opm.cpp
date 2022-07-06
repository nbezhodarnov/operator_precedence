#include "opm.h"

// Поиск элемента матрицы по терминалам
char& OPM::get_element(char &first, char &second) {
	int index_first = 0, index_second = 0;
	for (int i = 0; i < grammar.T.size(); i++) {
		if (grammar.T[i] == first) {
			index_first = i;
		}
		if (grammar.T[i] == second) {
			index_second = i;
		}
	}
	return matrix[index_first][index_second];
}

// Поиск индекса по терминалу
int OPM::get_index(char &symbol) {
	int line_index = 0;
	while (grammar.T[line_index] != symbol) {
		line_index++;
	}
	return line_index;
}

// Заполнение строки заданным символом
void OPM::fill_line(char &symbol, std::string &str, char fill) {
	for (int i = 0; i < str.size(); i++) {
		get_element(symbol, str[i]) = fill;
	}
}

// Заполнение столбца заданным символом
void OPM::fill_row(char &symbol, std::string &str, char fill) {
	for (int i = 0; i < str.size(); i++) {
		get_element(str[i], symbol) = fill;
	}
}

OPM::OPM(Grammar &gr, TableLRt &table) {
	grammar = gr;
	matrix.resize(grammar.T.size());
	for (int i = 0; i < grammar.T.size(); i++) {
		matrix[i] = std::vector<char>(grammar.T.size(), ' ');
	}
	
	// Подсчёт количества терминалов в правилах
	std::vector<int> terminals_count(grammar.RR.size(), 0);
	for (int i = 0; i < grammar.RR.size(); i++) {
		for (int j = 0; j < grammar.RR[i].size(); j++) {
			if (char_existance_check(grammar.T, grammar.RR[i][j])) {
				terminals_count[i]++;
			}
		}
	}
	
	std::vector<int> non_terminals_info_change;
	char non_terminal = '\0';
	for (int i = terminals_count.size() - 1; i >= 0; i--) {
		// Заполнение массива нетерминалов для восстановления данных после вырезания правил
		if (grammar.LR[i] != non_terminal) {
			non_terminal = grammar.LR[i];
			int non_terminal_index = 0;
			while(grammar.inf[non_terminal_index].Net != non_terminal) {
				non_terminal_index++;
			}
			non_terminals_info_change.push_back(non_terminal_index);
		}
		// Терминалы отсутсвуют, правило можно удалить
		if (terminals_count[i] == 0) {
			grammar.RR.erase(grammar.RR.begin() + i);
			grammar.LR.erase(grammar.LR.begin() + i);
			
			// Починка данных о правилах последующих нетерминалов
			for (int j = 0; j < non_terminals_info_change.size() - 1; j++) {
				grammar.inf[non_terminals_info_change[j]].firstN--;
			}
			grammar.inf[non_terminals_info_change[non_terminals_info_change.size() - 1]].countAlt--;
		// Терминалов больше одного, формируем статус предшествования - =
		} else if (terminals_count[i] > 1) {
			for (int j = 0; j < grammar.RR[i].size() - 2; j++) {
				if (char_existance_check(grammar.T, grammar.RR[i][j])) {
					int next_terminal = -1;
					if (char_existance_check(grammar.T, grammar.RR[i][j + 1])) {
						next_terminal = j + 1;
					} else if (char_existance_check(grammar.T, grammar.RR[i][j + 2])) {
						next_terminal = j + 2;
					}
					if (next_terminal != -1) {
						get_element(grammar.RR[i][j], grammar.RR[i][next_terminal]) = '=';
					}
				}
			}
			if ((char_existance_check(grammar.T, grammar.RR[i][grammar.RR[i].size() - 2])) && (char_existance_check(grammar.T, grammar.RR[i][grammar.RR[i].size() - 1]))) {
				get_element(grammar.RR[i][grammar.RR[i].size() - 2], grammar.RR[i][grammar.RR[i].size() - 1]) = '=';
			}
		}
	}
	
	// Заполнение матрицы статусами < и >
	for (int i = 0; i < grammar.T.size(); i++) {
		for (int j = 0; j < grammar.RR.size(); j++) {
			for (int k = 0; k < grammar.RR[j].size(); k++) {
				if (grammar.RR[j][k] == grammar.T[i]) {
					// Поиск предыдущих и последующих символов для терминалов
					char previous_symbol, next_symbol;
					if (k == 0) {
						previous_symbol = '\0';
					} else {
						previous_symbol = grammar.RR[j][k - 1];
					}
					if (k == grammar.RR[j].size() - 1) {
						next_symbol = '\0';
					} else {
						next_symbol = grammar.RR[j][k + 1];
					}
					
					// Статус > по tableLRt (R)
					if (previous_symbol != '\0') {
						int non_terminal_index = 0;
						while ((non_terminal_index < table.symbols.size()) && (table.symbols[non_terminal_index] != previous_symbol)) {
							non_terminal_index++;
						}
						if (non_terminal_index < table.symbols.size()) {
							fill_row(grammar.T[i], table.R[non_terminal_index], '>');
						}
					}
					
					// Статус < по tableLRt (L)
					if (next_symbol != '\0') {
						int non_terminal_index = 0;
						while ((non_terminal_index < table.symbols.size()) && (table.symbols[non_terminal_index] != next_symbol)) {
							non_terminal_index++;
						}
						if (non_terminal_index < table.symbols.size()) {
							fill_line(grammar.T[i], table.L[non_terminal_index], '<');
						}
					}
				}
			}
		}
	}
	
	// Замена нетерминалов на одинаковый символ в правилах
	for (int i = 0; i < grammar.RR.size(); i++) {
		for (int j = 0; j < grammar.RR[i].size(); j++) {
			if (char_non_existance_check(grammar.T, grammar.RR[i][j])) {
				grammar.RR[i][j] = 'N';
			}
		}
	}
	
	// Заполнение массива символов для добавления в полис
	polysis_symbols_add = std::vector<char>(grammar.RR.size(), '\0');
	for (int i = 0, k = 0; i < grammar.RR.size(); i++, k++) {
		while (terminals_count[k] == 0) {
			k++;
		}
		if (terminals_count[k] == 1) {
			int j = 0;
			while (char_non_existance_check(grammar.T, grammar.RR[i][j])) {
				j++;
			}
			polysis_symbols_add[i] = grammar.RR[i][j];
		}
	}
}

// Получение статуса предшествования
char OPM::get_precedence(char &first, char &second) {
	if ((char_existance_check(grammar.T, first)) && (char_existance_check(grammar.T, second))) {
		return get_element(first, second);
	}
	return ' ';
}

void OPM::output() {
	std::cout << "Grammar:\n";
	grammar.output();
	
	std::cout << "OPM:\n  ";
	for (int i = 0; i < grammar.T.size(); i++) {
		std::cout << grammar.T[i] << ' ';
	}
	std::cout << '\n';
	
	for (int i = 0; i < matrix.size(); i++) {
		std::cout << grammar.T[i] << ' ';
		for (int j = 0; j < matrix[i].size(); j++) {
			std::cout << matrix[i][j] << ' ';
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}
