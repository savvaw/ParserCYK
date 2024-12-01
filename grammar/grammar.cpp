#include "grammar.h"
#include <iostream>
#include <ext/concurrence.h>

void Grammar::readGrammar() {
    std::cin >> nonTerminalsSize >> terminalsSize >> rulesSize;
    std::cin >> nonTerminals;
    std::cin >> terminals;
    rules.resize(rulesSize);
    std::string ruleScan;
    std::getline(std::cin, ruleScan);
    std::cerr << "first " << ruleScan << std::endl;
    for (int i = 0; i < rulesSize; ++i) {
        std::getline(std::cin, ruleScan);
        std::string rule;
        for (int j = 0; j < ruleScan.size(); ++j) {
            if (ruleScan[j] != ' ') {
                rule += ruleScan[j];
            }
        }
        std::cerr << rule << std::endl;
        int sepInd = 0;
        for (int j = 0; j + 1 < rule.size(); ++j) {
            if (rule[j] == '-' && rule[j + 1] == '>') {
                sepInd = j;
            }
        }
        rules[i].first = rule.substr(0, sepInd);
        rules[i].second = rule.substr(sepInd + 2,
                                      static_cast<int>(rule.size()) - 2 - static_cast<int>(rules[i].first.size()));
    }
    for (int i = 0; i < rulesSize; ++i) {
        std::cerr << i << " " << rules[i].first << " " << rules[i].second << std::endl;
    }
    std::cin >> start;
}

Grammar::Grammar() {
    readGrammar();
}

void ContextFreeGrammar::checkContextFree() const {
    for (int i = 0; i < rulesSize; ++i) {
        if (rules[i].first.size() != 1) {
            throw std::runtime_error("Not Context-free grammar");
        }
        int isNonTerm = false;
        for (int j = 0; j < nonTerminalsSize; ++j) {
            isNonTerm |= (nonTerminals[j] == rules[i].first[0]);
        }
        if (!isNonTerm) {
            throw std::runtime_error("Not Context-free grammar");
        }
    }
}

ContextFreeGrammar::ContextFreeGrammar() {
    checkContextFree();
    std::cerr << "context free Grammar created" << std::endl;
}

void ContextFreeGrammar::proccessAttainableNonTerms( std::vector<std::vector<std::vector<bool>>>& dp, int len, int strSize) {
    for (int i = 0; i < encodedRules->codeCount; ++i) {
        for (int j = 0; j < encodedRules->codeCount; ++j) {
            if (!encodedRules->attainable[i][j]) {
                continue;
            }
            for (int l = 0; l + len <= strSize; ++l) {
                dp[i][l][l + len] = dp[i][l][l + len] || dp[j][l][l + len];
            }
        }
    }
}


bool ContextFreeGrammar::predict(const std::string& string) {
    std::cerr << string << std::endl;
    std::vector<std::vector<std::vector<bool>>> dp(encodedRules->codeCount,
        std::vector<std::vector<bool>>(string.size() + 1,
            std::vector<bool>(string.size() + 1)));
    for (const auto& rule : encodedRules->encodedRules) {
        if (!rule.second.empty()) {
            continue;
        }
        std::cerr << rule.first << " ffewwfew" << std::endl;
        for (int i = 0; i <= string.size(); ++i) {
            for (int j = 0; j <= i; ++j) {
                dp[rule.first][i][j] = true;
            }
        }
    }
    for (auto& p : encodedRules->terminalsCodes) {
        for (int i = 0; i < string.size(); ++i) {
            if (string[i] == p.second) {
                dp[p.first][i][i + 1] = true;
                std::cerr << i << std::endl;
            }
        }
    }
    proccessAttainableNonTerms(dp, 0, string.size());
    for (int len = 1; len <= string.size(); ++len) {
        for (int l = 0; l + len <= string.size(); ++l) {
            int r = l + len;
            for (const auto& rule : encodedRules->encodedRules) {
                if (rule.second.empty()) {
                    continue;
                }
                if (rule.second.size() == 1) {
                    if (dp[rule.second[0]][l][r]) {
                        dp[rule.first][l][r] = true;
                    }
                    continue;
                }
                for (int mid = l; mid <= r; ++mid) {
                    if (dp[rule.second[0]][l][mid] && dp[rule.second[1]][mid][r]) {
                        dp[rule.first][l][r] = true;
                    }
                }
            }
        }
        proccessAttainableNonTerms(dp, len, string.size());
    }
    for (int i = 0; i < dp.size(); ++i) {
        for (auto a : dp[i]) {
            for (auto b : a) {
                std::cerr << b << " ";
            }
            std::cerr << std::endl;
        }

        std::cerr << "________________________\n";
    }
    return dp[0][0][string.size()];
}


void ContextFreeGrammar::fit() {
    std::cerr << "start transition" << std::endl;
    encodedRules = new EncodedRules(rules, start);
    std::cerr << "successsssss" << std::endl;
}

ContextFreeGrammar::~ContextFreeGrammar() {
    delete encodedRules;
}
