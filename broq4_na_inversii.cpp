#include <iostream>
#include <assert.h>
#include <vector>

using namespace std;

unsigned long long slei_s_broi_inversii(int masiv[], int na4alo, int sreda, int krai)
{
    int razmer1 = sreda - na4alo + 1;
    int razmer2 =  krai - sreda;

    int* lqv = new int[razmer1];
    int* desen = new int[razmer2];

    unsigned long long broi = 0;

    for(int i=0; i<razmer1; ++i)
    {
        lqv[i] = masiv[na4alo+i];
    }

    for(int i=0; i<razmer2; ++i)
    {
        desen[i] = masiv[sreda+i+1];
    }

    int i=0;
    int j=0;
    int k=na4alo;

    while(i<razmer1 && j<razmer2)
    {
        if(lqv[i] <= desen[j])
        {
            masiv[k] = lqv[i];
            ++i;

            broi += j;
        }
        else
        {
            masiv[k] = desen[j];
            ++j;

            broi += i;
        }
        ++k;
    }

    broi -= j;

    while (i < razmer1)
    {
        masiv[k] = lqv[i];
        i++;
        k++;

        broi += j;
    }

    while (j < razmer2)
    {
        masiv[k] = desen[j];
        j++;
        k++;
    }

    delete[] lqv;
    delete[] desen;

    return broi;
}

unsigned long long sliva6to_redene_s_broi_inversii(int masiv[], int na4alo, int krai)
{
    if(na4alo < krai)
    {
        int sreda = (krai+na4alo)/2;
        unsigned long long inversii = 0;

        inversii += sliva6to_redene_s_broi_inversii(masiv, na4alo, sreda);
        inversii += sliva6to_redene_s_broi_inversii(masiv, sreda+1, krai);
        inversii += slei_s_broi_inversii(masiv, na4alo, sreda, krai);

        return inversii;
    }
    else
    {
        return 0;
    }
}

int main()
{
    int mas[] = {4, 2, 1, 3, 5, 8, 0, 43, 10};

	unsigned long long broi = sliva6to_redene_s_broi_inversii(mas, 0, 8);

	cout << "Broi inversii: " << broi;

	return 0;
}
