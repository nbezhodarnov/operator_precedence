#include "tableLR.h"

bool char_non_existance_check(std::string &str, char symbol) {
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == symbol) {
			return false;
		}
	}
	return true;
}

bool char_existance_check(std::string &str, char symbol) {
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == symbol) {
			return true;
		}
	}
	return false;
}

TableLR::TableLR (Grammar &gr) {
	L = std::vector<std::string>(gr.inf.size());
	R = std::vector<std::string>(gr.inf.size());
	symbols = std::vector<char>(gr.inf.size());
	
	for (int i = 0; i < gr.inf.size(); i++) {
		symbols[i] = gr.inf[i].Net; // Заполнение нетерминалов
		for (int j = 0; j < gr.inf[i].countAlt; j++) {
			// Запоминание первых символов правил
			if (char_non_existance_check(L[i], gr.RR[gr.inf[i].firstN + j][0])) {
				L[i] += gr.RR[gr.inf[i].firstN + j][0];
			}
			// Запоминание последних символов правил
			if (char_non_existance_check(R[i], gr.RR[gr.inf[i].firstN + j][gr.RR[gr.inf[i].firstN + j].size() - 1])) {
				R[i] += gr.RR[gr.inf[i].firstN + j][gr.RR[gr.inf[i].firstN + j].size() - 1];
			}
		}
	}
	
	std::vector<std::string>* ptr[2];
	ptr[0] = &L;
	ptr[1] = &R;
	for (int i = 0; i < gr.inf.size(); i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < (*ptr[j])[i].size(); k++) {
				int indexNet = 0;
				while ((indexNet < symbols.size()) && ((*ptr[j])[i][k] != symbols[indexNet])) {
					indexNet++;
				}
				
				// Добавление элементов из других строчек таблицы
				if ((indexNet < symbols.size()) && (indexNet != i)) {
					for (int l = 0; l < (*ptr[j])[indexNet].size(); l++) {
						if (char_non_existance_check((*ptr[j])[i], (*ptr[j])[indexNet][l])) {
							(*ptr[j])[i] += (*ptr[j])[indexNet][l];
						}
					}
				}
			}
		}
	}
}

void TableLR::output() {
	std::vector<std::string>* ptr[2];
	ptr[0] = &L;
	ptr[1] = &R;
	char letter[2];
	letter[0] = 'L';
	letter[1] = 'R';
	for (int i = 0; i < 2; i++) {
		std::cout << letter[i] << "(U):\n";
		for (int j = 0; j < (*ptr[i]).size(); j++) {
			std::cout << symbols[j] << ": " << (*ptr[i])[j] << '\n';
		}
		std::cout << '\n';
	}
}
