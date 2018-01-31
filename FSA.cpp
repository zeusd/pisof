#include <iostream>
#include <string>
#include <stack>
#include <queue>

#include "FSA.hpp"
#include "RegIzr.hpp"

FSA::FSA(std::string expr)
{
    if(expr.length() <= 1)
    {
        State start;
        State fin;

        start.prehodi.push_back(std::make_pair(expr[0], 1));
        start.fin = false;
        fin.fin = true;

        states.push_back(start);
        states.push_back(fin);
    }
    else
    {
        thompson(expr);
    }
}

FSA& FSA::operator =(const FSA& other)
{
    states = other.states;

    return *this;
}

bool FSA::run(std::string str) const
{
    if(states.empty())
    {
        return false;
    }

    std::set<int> curr;
    std::set<int> next;

    curr.insert(0);

    for(int i=0; i<str.size(); ++i)
    {
        char sym = str[i];
        next.clear();

        for(std::set<int>::iterator it=curr.begin(); it!=curr.end(); ++it)
        {
            std::set<int> eps;
            epsilonClosure(*it, eps);
            curr.insert(eps.begin(), eps.end());
        }

        for(std::set<int>::iterator it=curr.begin(); it!=curr.end(); ++it)
        {
            for(int k=0; k<states[*it].prehodi.size(); ++k)
            {
                if(states[*it].prehodi[k].first == sym || states[*it].prehodi[k].first == '?')
                {
                    next.insert(states[*it].prehodi[k].second);
                }
            }
        }

        curr = next;
    }

    for(std::set<int>::iterator it=curr.begin(); it!=curr.end(); ++it)
    {
        std::set<int> eps;
        epsilonClosure(*it, eps);
        curr.insert(eps.begin(), eps.end());
    }

    for(std::set<int>::iterator it=curr.begin(); it!=curr.end(); ++it)
    {
        if(states[*it].fin)
        {
            return true;
        }
    }

    return false;
}

/*
std::string toRegIzr() const;
*/

FSA& FSA::iterate()
{
    State nuStart;
    nuStart.prehodi.insert(nuStart.prehodi.begin(), std::make_pair('@', 1));
    nuStart.fin = false;

    State nuEnd;
    nuEnd.prehodi.push_back(std::make_pair('@', 0));
    nuEnd.fin = true;

    for(int i=0; i<states.size(); ++i)
    {
        for(int j=0; j<states[i].prehodi.size(); ++j)
        {
            states[i].prehodi[j].second++;
        }

        if(states[i].fin)
        {
            states[i].prehodi.push_back(std::make_pair('@', 0));
            states[i].fin = false;
        }
    }

    states.insert(states.begin(), nuStart);
    states.push_back(nuEnd);

    return *this;
}

FSA& FSA::uniteWith(const FSA& other)
{
    FSA second = other;

    State nuStart;
    nuStart.prehodi.insert(nuStart.prehodi.begin(), std::make_pair('@', states.size() + 1));
    nuStart.prehodi.insert(nuStart.prehodi.begin(), std::make_pair('@', 1));
    nuStart.fin = false;

    State nuEnd;
    nuEnd.fin = true;

    for(int i=0; i<states.size(); ++i)
    {
        for(int j=0; j<states[i].prehodi.size(); ++j)
        {
            states[i].prehodi[j].second++;
        }

        if(states[i].fin)
        {
            states[i].prehodi.push_back(std::make_pair('@', states.size() + second.states.size() + 1));
            states[i].fin = false;
        }
    }

    for(int i=0; i<second.states.size(); ++i)
    {
        for(int j=0; j<second.states[i].prehodi.size(); ++j)
        {
            second.states[i].prehodi[j].second += states.size() + 1;
        }

        if(second.states[i].fin)
        {
            second.states[i].prehodi.push_back(std::make_pair('@', states.size() + second.states.size() + 1));
            second.states[i].fin = false;
        }
    }

    states.insert(states.begin(), nuStart);
    states.insert(states.end(), second.states.begin(), second.states.end());
    states.push_back(nuEnd);

    return *this;
}

FSA& FSA::concatWith(const FSA& other)
{
    FSA second = other;

    for(int i=0; i<states.size(); ++i)
    {
        if(states[i].fin)
        {
            states[i].prehodi.push_back(std::make_pair('@', states.size()));
            states[i].fin = false;
        }
    }

    for(int i=0; i<second.states.size(); ++i)
    {
        for(int j=0; j<second.states[i].prehodi.size(); ++j)
        {
            second.states[i].prehodi[j].second += states.size();
        }
    }

    states.insert(states.end(), second.states.begin(), second.states.end());

    return *this;
}

/*
FSA& intersectWith(const FSA& other);
FSA& complement();
*/

void FSA::thompson(std::string expr)
{
    expr = toPostfix(expr);

    std::stack<char> syms;
    std::stack<FSA> fsas;

    for(std::string::iterator it = expr.begin(); it!=expr.end(); ++it)
    {
        char token = *it;

        if(isOperator(token))
        {
            switch(token)
            {
                case '*':
                    {
                        FSA toIterate = fsas.top();
                        fsas.pop();
                        fsas.push(toIterate.iterate());
                    }
                break;

                case '.':
                    {
                        FSA second = fsas.top();
                        fsas.pop();
                        FSA first = fsas.top();
                        fsas.pop();

                        fsas.push(first.concatWith(second));
                    }
                break;

                case '+':
                    {
                        FSA second = fsas.top();
                        fsas.pop();
                        FSA first = fsas.top();
                        fsas.pop();

                        fsas.push(first.uniteWith(second));
                    }
                break;

                case '&':
                    {
                        throw "not yet";
                        /*
                        FSA second = fsas.top();
                        fsas.pop();
                        FSA first = fsas.top();
                        fsas.pop();

                        fsas.push(first.intersectWith(second));
                        */
                    }
                break;

                default: throw "hell";
            }
        }
        else
        {
            fsas.push(FSA(std::string() + token));
        }
    }

    *this = fsas.top();
}

int FSA::priority(char op)
{
    switch(op)
    {
        case '*' : return 3;
        case '.' : return 2;
        case '+' : return 1;
        case '&' : return 1;
        default : return 0;
    }
}

bool FSA::isWord(char token)
{
    return !priority(token) && !isLBrack(token) && !isRBrack(token);
}

bool FSA::isOperator(char token)
{
    return (bool)priority(token);
}

bool FSA::isLBrack(char token)
{
    return token == '(';
}

bool FSA::isRBrack(char token)
{
    return token == ')';
}

std::string FSA::dot(std::string regIzr)
{
    for(int i=0; i<regIzr.size()-1; ++i)
    {
        if(isWord(regIzr[i]) && isWord(regIzr[i+1]))
        {
            regIzr.insert(i+1, ".");
        }
    }

    return regIzr;
}

std::string FSA::toPostfix(std::string regIzr)
{
    regIzr = dot(regIzr);
    std::string result = "";
    std::stack<char> ops;
    std::queue<char> words;

    for(std::string::iterator it=regIzr.begin(); it!=regIzr.end(); ++it)
    {
        char token = *it;
        if(isWord(token))
        {
            words.push(token);
        }
        else if(isOperator(token))
        {
            while(!ops.empty() && !isLBrack(ops.top()) && priority(ops.top()) > priority(token))
            {
                words.push(ops.top());
                ops.pop();
            }
            ops.push(token);
        }
        else if(isLBrack(token))
        {
            ops.push(token);
        }
        else if(isRBrack(token))
        {
            while(!isLBrack(ops.top()))
            {
                words.push(ops.top());
                ops.pop();
            }

            ops.pop();
        }
    }

    while(!ops.empty())
    {
        words.push(ops.top());
        ops.pop();
    }

    while(!words.empty())
    {
        result += words.front();
        words.pop();
    }

    return result;
}

void FSA::epsilonClosure(const int& state, std::set<int>& eps) const
{
    eps.insert(state);

    for(int i=0; i<states[state].prehodi.size(); ++i)
    {
        if(states[state].prehodi[i].first == '@')
        {
            eps.insert(states[state].prehodi[i].second);
        }
    }

    for(std::set<int>::iterator it=eps.begin(); it!=eps.end(); ++it)
    {
        for(int i=0; i<states[*it].prehodi.size(); ++i)
        {
            if(eps.find(states[*it].prehodi[i].second) == eps.end())
            {
                epsilonClosure(states[*it].prehodi[i].second, eps);
            }
        }
    }
}

std::ostream& operator <<(std::ostream& out, const FSA& fsa)
{
    for(int i=0; i<fsa.states.size(); ++i)
    {
        if(fsa.states[i].prehodi.empty())
        {
            out << (fsa.states[i].fin ? "(/" : "( ") << i << (fsa.states[i].fin ? "/)" : " )") << '\n';
        }

        for(int j=0; j<fsa.states[i].prehodi.size(); ++j)
        {
            out << (fsa.states[i].fin ? "(/" : "( ") << i << (fsa.states[i].fin ? "/)" : " )") << "-" << fsa.states[i].prehodi[j].first << "> " << fsa.states[i].prehodi[j].second << '\n';
        }
    }

    return out;
}
