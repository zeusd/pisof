#include <iostream>
#include <set>

#include "FSA.hpp"

using namespace std;

int main()
{
    FSA fsaA("b*");

    cout << fsaA << endl << endl;

    cout << fsaA.run("b");

    return 0;
}
