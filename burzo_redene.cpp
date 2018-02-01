#include <iostream>
#include <assert.h>
#include <cstdlib>
#include <ctime>

using namespace std;

void menkai(int& tui, int& onui)
{
    int paza4 = tui;
    tui = onui;
    onui = paza4;
}

int cepene(int masiv[], int na4alo, int krai)
{
    int os = masiv[krai];
    int i = na4alo - 1;

    for(int j = na4alo; j <= krai-1; ++j)
    {
        if(masiv[j] <= os)
        {
            ++i;
            menkai(masiv[i], masiv[j]);
        }
    }

    ++i;

    menkai(masiv[i], masiv[krai]);

    return i;
}

void burzo_redene(int masiv[], int na4alo, int krai)
{
    if(na4alo < krai)
    {
        int cepka = cepene(masiv, na4alo, krai);

        burzo_redene(masiv, na4alo, cepka-1);
        burzo_redene(masiv, cepka+1, krai);
    }
}

int main()
{
    int mas[10];
    srand(time(0));

    for(int i=0; i<10; ++i)
    {
        mas[i] = rand()%100 + 1;
    }

    burzo_redene(mas, 0, 9);

    int ad = mas[0];
    bool nqma_gre6ka = true;

    for(int i=1; i<10; ++i)
    {
        cout<<mas[i]<<" ";
        ad = mas[i];

        if(ad > mas[i])
        {
            nqma_gre6ka = false;
        }
    }

    assert(nqma_gre6ka);

    return 0;
}
