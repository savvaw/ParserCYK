#include <iostream>

#include "grammar/grammar.h"

int main() {
    ContextFreeGrammar grammar{};
    grammar.fit();
    int m;
    std::cin >> m;
    while (m--) {
        std::string string;
        std::cin >> string;
        std::cout << ((grammar.predict(string)) ? "Yes" : "No") << std::endl;
    }
}
