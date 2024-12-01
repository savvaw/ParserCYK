#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <string>
#include "../encoded_rules/encoded_rules.h"

class Grammar {
protected:
    int terminalsSize, nonTerminalsSize, rulesSize;
    char start;
    std::string nonTerminals, terminals;
    std::vector<std::pair<std::string, std::string>> rules;
protected:
    void readGrammar();

public:
    virtual ~Grammar() = default;
    Grammar();
    virtual void fit() = 0;
    virtual bool predict(const std::string&) = 0;

    friend struct GrammarGraph;
};

class ContextFreeGrammar : public Grammar {
private:
    EncodedRules* encodedRules = nullptr;

    void proccessAttainableNonTerms(std::vector<std::vector<std::vector<bool>>>&, int, int);
    void checkContextFree() const;

public:
    ContextFreeGrammar();
    void fit() override;
    bool predict(const std::string&) override;

    friend struct GrammarGraph;

    ~ContextFreeGrammar() override;
};

#endif
