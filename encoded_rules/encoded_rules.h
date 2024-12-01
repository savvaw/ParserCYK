#ifndef ENCODED_RULES_H
#define ENCODED_RULES_H

#include <vector>
#include <string>
#include <map>

struct EncodedRules {
    std::vector<std::pair<int, std::vector<int>>> encodedRules;
    std::map<int, char> terminalsCodes;
    std::vector<int*> attainable;
    int codeCount;

    EncodedRules(const std::vector<std::pair<std::string, std::string>>&, char);

private:
    void calcAttainable();
};



#endif
