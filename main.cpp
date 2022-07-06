#include <iostream>
#include <string>
#include <vector>

#include "grammar.h"
#include "tableLR.h"
#include "tableLRt.h"
#include "opm.h"
#include "opm_recognizer.h"

int main() {
	Grammar grammar("ABBTTMMM", {"!B!","B+T","T","M","T*M","a","b","(B)"});
	TableLR tableLR(grammar);
	TableLRt tableLRt(grammar, tableLR);
	OPM opm(grammar, tableLRt);
	OPM_recognizer recognizer(opm);
	
	std::cout << "Grammar:\n";
	grammar.output();
	tableLR.output();
	tableLRt.output();
	opm.output();
	
	std::string line;
	std::cout << "Input line for recognize: ";
	std::cin >> line;
	
	std::vector<int> result = recognizer.recognize(line);
	std::cout << "Result: ";
	for (int i = 0; i < result.size(); i++) {
		std::cout << result[i] << ' ';
	}
	std::cout << "\nPolysis: " << recognizer.get_polysis() << '\n';
	
	return 0;
}
