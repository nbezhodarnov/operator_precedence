#include "tableLRt.h"

TableLRt::TableLRt (Grammar &gr, TableLR &table) {
	L = std::vector<std::string>(gr.inf.size());
	R = std::vector<std::string>(gr.inf.size());
	symbols = std::vector<char>(gr.inf.size());
	
	for (int i = 0; i < gr.inf.size(); i++) {
		symbols[i] = gr.inf[i].Net;
		for (int j = 0; j < gr.inf[i].countAlt; j++) {
			char Term = '\0';
			int k = 0;
			
			// Поиск терминалов рядом с требуемым нетерминалом с начала строки (L)
			while (gr.RR[gr.inf[i].firstN + j][k] != Term) {
				int l = 0;
				while ((l < gr.T.size()) && (gr.T[l] != gr.RR[gr.inf[i].firstN + j][k])) {
					l++;
				}
				if (l == gr.T.size()) {
					k++;
				} else {
					Term = gr.T[l];
				}
			}
			if ((Term != '\0') && (char_non_existance_check(L[i], Term))) {
				L[i] += Term;
			}
			
			Term = '\0';
			k = gr.RR[gr.inf[i].firstN + j].size() - 1;
			
			// Поиск терминалов рядом с требуемым нетерминалом с конца строки (R)
			while ((k >= 0) && (gr.RR[gr.inf[i].firstN + j][k] != Term)) {
				int l = 0;
				while ((l < gr.T.size()) && (gr.T[l] != gr.RR[gr.inf[i].firstN + j][k])) {
					l++;
				}
				if (l == gr.T.size()) {
					k--;
				} else {
					Term = gr.T[l];
				}
			}
			if ((Term != '\0') && (char_non_existance_check(R[i], Term))) {
				R[i] += Term;
			}
		}
	}
	
	std::vector<std::string> *ptr[2], *ptr_old[2];
	ptr[0] = &L;
	ptr[1] = &R;
	ptr_old[0] = &(table.L);
	ptr_old[1] = &(table.R);
	for (int i = 0; i < gr.inf.size(); i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < (*ptr_old[j])[i].size(); k++) {
				int indexNet = 0;
				while ((indexNet < symbols.size()) && ((*ptr_old[j])[i][k] != symbols[indexNet])) {
					indexNet++;
				}
				
				// Заполнение из других строк таблицы по tableLR
				if ((indexNet < symbols.size()) && (indexNet != i)) {
					for (int l = 0; l < (*ptr[j])[indexNet].size(); l++) {
						bool new_symbol = true;
						for (int m = 0; m < (*ptr[j])[i].size(); m++) {
							if ((*ptr[j])[indexNet][l] == (*ptr[j])[i][m]) {
								new_symbol = false;
								break;
							}
						}
						if (new_symbol) {
							(*ptr[j])[i] += (*ptr[j])[indexNet][l];
						}
					}
				}
			}
		}
	}
}

void TableLRt::output() {
	std::vector<std::string>* ptr[2];
	ptr[0] = &L;
	ptr[1] = &R;
	char letter[2];
	letter[0] = 'L';
	letter[1] = 'R';
	for (int i = 0; i < 2; i++) {
		std::cout << letter[i] << "t(U):\n";
		for (int j = 0; j < (*ptr[i]).size(); j++) {
			std::cout << symbols[j] << ": " << (*ptr[i])[j] << '\n';
		}
		std::cout << '\n';
	}
}
