#include <iostream>
#include <assert.h>

using namespace std;

template <typename T>
struct Chaka6t
{
    int predimstvo;
    T tqlo;

    Chaka6t* lqv;
    Chaka6t* desen;
};

template <typename T>
void kopirai_kup4ina(Chaka6t<T>* vTuk, Chaka6t<T>* otTuk)
{
    if(otTuk != nullptr)
    {
        vTuk = new Chaka6t<T>;
        vTuk->predimstvo = otTuk->predimstvo;
        vTuk->tqlo = otTuk->tqlo;

        kopirai_kup4ina(vTuk->lqv, otTuk->lqv);
        kopirai_kup4ina(vTuk->desen, otTuk->desen);
    }
}

template <typename T>
void trii_kup4ina(Chaka6t<T>* koren)
{
    if(koren != nullptr)
    {
        trii_kup4ina(koren->lqv);
        trii_kup4ina(koren->desen);

        delete koren;
    }
}

template <typename T>
void vkarai_v_opa6ka(Chaka6t<T>* purvi, Chaka6t<T> nov)
{
    if(purvi == nullptr)
    {
        return;
    }

    if(purvi->predimstvo < nov.predimstvo)
    {
        Chaka6t<T> ne_ba6_nov;
        ne_ba6_nov.predimstvo = purvi->predimstvo;
        ne_ba6_nov.tqlo = purvi->tqlo;

        purvi->predimstvo = nov.predimstvo;
        purvi->tqlo = nov.tqlo;

        vkarai_v_opa6ka(purvi, ne_ba6_nov);
    }
    else
    {
        if(purvi->desen == nullptr)
        {
            purvi->desen = new Chaka6t<T>;

            purvi->desen->predimstvo = nov.predimstvo;
            purvi->desen->tqlo = nov.tqlo;
        }
        else if(purvi->lqv == nullptr)
        {
            purvi->lqv = new Chaka6t<T>;

            purvi->lqv->predimstvo = nov.predimstvo;
            purvi->lqv->tqlo = nov.tqlo;
        }
        else
        {
            vkarai_v_opa6ka(purvi->lqv, nov);
        }
    }
}

template <typename T>
void vkarai_opa6ka_v_opa6ka(Chaka6t<T>* tazi, Chaka6t<T>* vTazi)
{
    if(vTazi != nullptr && tazi != nullptr)
    {
        if(vTazi->predimstvo < tazi->predimstvo)
        {
            vkarai_opa6ka_v_opa6ka(vTazi, tazi);
        }
        else
        {
            if(vTazi->desen == nullptr)
            {
                vTazi->desen = tazi;
            }
            else if(vTazi->lqv == nullptr)
            {
                vTazi->lqv = tazi;
            }
            else
            {
                vkarai_opa6ka_v_opa6ka(tazi, vTazi->lqv);
            }
        }
    }
}

template <typename T>
Chaka6t<T>* izkarai_ot_opa6ka(Chaka6t<T>* purvi)
{
    if(purvi == nullptr)
    {
        return nullptr;
    }
    else
    {
        Chaka6t<T>* paza4 = purvi;
        purvi = purvi->lqv;
        vkarai_opa6ka_v_opa6ka(purvi, paza4->desen);

        return paza4;
    }
}

template <typename T>
class Opa6ka
{
    Chaka6t<T>* purvi;

public:
    Opa6ka() : purvi(nullptr) {}

    Opa6ka(const Opa6ka& druga)
    {
        kopirai_kup4ina(purvi, druga.purvi);
    }

    Opa6ka& operator =(const Opa6ka& druga)
    {
        iz4isti();
        kopirai_kup4ina(purvi, druga.purvi);
    }

    void vkarai(Chaka6t<T> chaka4)
    {
        if(prazna())
        {
            purvi = new Chaka6t<T>;
            purvi->predimstvo = chaka4.predimstvo;
            purvi->tqlo = chaka4.tqlo;
        }
        else
        {
            vkarai_v_opa6ka(purvi, chaka4);
        }
    }

    Chaka6t<T> dai_purvi()
    {
        Chaka6t<T> rezultat;
        if(!prazna())
        {
            Chaka6t<T>* paza4 = izkarai_ot_opa6ka(purvi);
            rezultat.predimstvo = paza4->predimstvo;
            rezultat.tqlo = paza4->tqlo;
        }

        return rezultat;
    }

    bool prazna()
    {
        return purvi == nullptr;
    }

    ~Opa6ka()
    {
        iz4isti();
    }

private:
    void iz4isti()
    {
        trii_kup4ina(purvi);
    }
};

int main()
{
    Chaka6t<char> ivan;
    ivan.predimstvo = 3;
    ivan.tqlo = 'i';

    Chaka6t<char> dragan;
    dragan.predimstvo = 2;
    dragan.tqlo = 'd';

    Chaka6t<char> petkan;
    petkan.predimstvo = 1;
    petkan.tqlo = 'p';

    Opa6ka<char> pred_bani4arnicata;

    pred_bani4arnicata.vkarai(petkan);
    pred_bani4arnicata.vkarai(dragan);
    pred_bani4arnicata.vkarai(ivan);

    assert(pred_bani4arnicata.dai_purvi().tqlo == 'i');
}
