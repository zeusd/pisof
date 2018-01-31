#ifndef FSA_H
#define FSA_H

#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <set>

struct State
{
    std::vector<std::pair<char, int>> prehodi;
    bool fin;
};

class FSA
{
public:

    std::vector<State> states;

public:
    FSA() {};
    FSA(const FSA& other) : states(other.states) {}
    FSA(std::string expression);
    FSA& operator =(const FSA& other);

    bool run(std::string str) const;
    std::string toRegIzr() const;

    FSA& iterate();
    FSA& complement();
    FSA& uniteWith(const FSA& other);
    FSA& intersectWith(const FSA& other);
    FSA& concatWith(const FSA& other);

//private:
    void thompson(std::string expr);
    void epsilonClosure(const int& state, std::set<int>& eps) const;

public:
    static int priority(char op);
    static bool isWord(char token);
    static bool isOperator(char token);
    static bool isLBrack(char token);
    static bool isRBrack(char token);
    static std::string dot(std::string regIzr);
    static std::string toPostfix(std::string regIzr);

    friend std::ostream& operator <<(std::ostream& out, const FSA& fsa);
};

#endif // FSA_H
