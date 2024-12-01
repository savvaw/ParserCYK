#include "encoded_rules.h"
#include <iostream>

bool isCap(char c) {
    return 'A' <= c && c <= 'Z';
}

void dfs(int v, int* used, const std::vector<std::vector<int>>& graph) {
    used[v] = 1;
    for (int u : graph[v]) {
        if (!used[u]) {
            dfs(u, used, graph);
        }
    }
}

void EncodedRules::calcAttainable() {
    std::vector<std::vector<int>> graph(codeCount);
    for (const auto& rule : encodedRules) {
        if (rule.second.size() == 1) {
            if (terminalsCodes[rule.second[0]] == 0 && rule.first != rule.second[0]) {
                graph[rule.first].push_back(rule.second[0]);
            }
        }
    }
    int* used = new int[codeCount];
    attainable.resize(codeCount);
    for (int i = 0; i < codeCount; ++i) {
        std::fill_n(used, codeCount, 0);
        dfs(i, used, graph);
        attainable[i] = used;
    }
    for (int i = 0; i < attainable.size(); ++i) {
        for (int j = 0; j < codeCount; ++j) {
            std::cerr << attainable[i][j] << " ";
        }
        std::cerr << std::endl;
    }
}

EncodedRules::EncodedRules(const std::vector<std::pair<std::string, std::string>>& rules, char startNonTerm) {
    std::map<char, int> symbolCode;
    int currCode = 0;
    for (const auto& rule : rules) {
        if (!symbolCode[rule.first[0]]) {
            symbolCode[rule.first[0]] = ++currCode;
        }
    }
    for (const auto& rule : rules) {
        for (char c : rule.second) {
            if (isCap(c)) {
                continue;
            }
            if (!symbolCode[c]) {
                symbolCode[c] = ++currCode;
                terminalsCodes[currCode] = c;
            }
        }
    }
    int start = symbolCode[startNonTerm];
    encodedRules.push_back({0, {start}});
    for (const auto& rule : rules) {
        bool block = false;
        for (char c : rule.second) {
            block |= (isCap(c) && symbolCode[c] == 0);
        }
        if (block) {
            continue;
        }
        int vertex = symbolCode[rule.first[0]];
        int lastSymbolCode = symbolCode[rule.second.back()];

        if (rule.second.empty()) {
            encodedRules.push_back({vertex, {}});
            continue;
        }
        if (rule.second.size() == 1) {
            encodedRules.push_back({vertex, {symbolCode[rule.second[0]]}});
            continue;
        }

        for (int i = static_cast<int>(rule.second.size()) - 2; i > 0; --i) {
            encodedRules.push_back({++currCode, {symbolCode[rule.second[i]], lastSymbolCode}});
            lastSymbolCode = currCode;
        }
        encodedRules.push_back({vertex, {symbolCode[rule.second[0]], lastSymbolCode}});
    }
    for (auto p : terminalsCodes) {
        std::cerr << p.first << " " << p.second << std::endl;
    }

    codeCount = currCode + 1;
    calcAttainable();
    std::cerr << "bop bop bop" << std::endl;
    for (const auto& rule : encodedRules) {
        std::cerr << rule.first << " -> ";
        for (auto c : rule.second) {
            std::cerr << c << " ";
        }
        std::cerr << std::endl;
    }
}
