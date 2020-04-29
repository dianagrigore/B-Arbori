#include <iostream>
#include <fstream>
std :: ifstream f("abce.in");
std :: ofstream g("abce.out");
#define nullptr NULL

class Nod {
    int n; //numarul de chei
    int *chei; // vectorul de chei
    Nod **C; //vector de pointeri la descendentii directi ai nodului
    bool frunza; //1 daca nodul e frunza
    int t; //grad minim al arborelui
public:
    explicit Nod (int t = 2, bool frunza = true);
    virtual ~Nod();
    void insereazaposibil(int k); //insereaza o noua cheie
    void desparte(int i, Nod* y); //functie care desparte fiul y al nodului, unde i e indexul lui y in vectorul de fii C
    void parcurge(); //traverseaza subarborele care are acest nod ca radacina
    Nod* cautare(int k); //cauta o cheie in subarbore, returneaza null daca nu o gaseste
    int gasestecheie(int k); //functie care returneaza indexul primei chei >=k
    void eliminadinfrunza(int poz);
    void eliminanodintern(int poz);
    void elimina(int k);
    int predecesor(int k);
    int succesor(int k);
    void fill(int k);
    void anterior(int k);
    void urmator(int k);
    void merge(int k);
    int getpred(int k);
    int getsucc(int k);
    void afisarexy(int x, int y);
    friend class BArbore;
};

Nod :: Nod (int t1, bool frunza1){
    t = t1;
    frunza = frunza1;
    chei = new int [2 * t - 1];
    C = new Nod *[2 * t];
    for (int i = 0; i < 2 * t; i++)
        C[i] = nullptr;
    n = 0;
}

Nod :: ~Nod (){
    delete [] chei;
    delete [] C;
}
void Nod :: desparte(int i, Nod* y){
    Nod* z;
    z = new Nod(y -> t, y -> frunza);
    z -> n = t - 1;
    for (int j = 0; j < t - 1; j++)
        z -> chei[j] = y -> chei[j + t];
    if (y -> frunza == false)
    {
        for (int j = 0; j < t; j++)
            z -> C[j] = y -> C[j + t];
    }
    y -> n = t - 1;
    for (int j = n; j >= i + 1; j--)
        C[j + 1] = C[j];
    C[i + 1] = z;

    for (int j = n - 1; j >= i; j--)
        chei[j + 1] = chei[j];

    chei[i] = y -> chei[t - 1];

    n = n + 1;

}

void Nod :: parcurge(){

    for (int i = 0; i < n; i++){
        if (frunza == false)
            C[i] -> parcurge();
    g << " " << chei[i]; //cheile se opresc la n
    }
    if (frunza == false) //parcurge si cel de-al n+1-lea copil, daca exista
        C[n] -> parcurge();
}

Nod *Nod :: cautare(int k) {
    int i = 0;
    while (i < n && k > chei[i])
        i++;
    if (chei[i] == k && i < n)    //am gasit cheia k
        return this;
    if (frunza == true) //nu am gasit si nodul e frunza
        return nullptr;
    return C[i] -> cautare(k); //cauta in alta parte
}

void Nod :: insereazaposibil(int k) {
    int i = n - 1;
    if (frunza == true){ //daca nodul e frunza
        while (i >= 0 && chei[i] > k ) //cauta pozitia + muta cheile mai mari
        {
            chei[i + 1] = chei[i];
            i--;
        }
        chei[i + 1] = k; //insereaza cheile
        n = n + 1;
    }
    else //daca nodul nu e frunza
    {
        while (i >= 0 && chei[i] > k) //cauta fiul in care trebuie pusa noua cheie
            i--;
        if (C[i + 1] -> n == 2 * t - 1)  //daca fiul gasit e plin
        {
            desparte(i+1, C[i+1]); //desparte
            if (chei[i + 1] < k) //vezi in care jumatate din cele doua pui cheia
                i++;
        }
        C[i + 1] -> insereazaposibil(k);
    }
}

int Nod :: gasestecheie(int k) {

    int i = 0;
    while (i < n && chei[i] < k)
        i++;
    return i;
}

void Nod :: eliminadinfrunza(int poz) {
    for (int i = poz+1; i < n; i++) //elimina o cheie aducand cheile de dupa poz peste poz
        chei[i - 1] = chei[i];
    n--;
    return;
}

void Nod :: eliminanodintern(int poz) {
    int eliminat = chei[poz];
    if (C[poz] -> n >= t)
    {
        int pred = predecesor(poz);
        chei[poz] = pred;
        C[poz] -> elimina(pred);
    }
    else if (C[poz + 1] -> n >= t)
    {
        int suc = succesor(poz);
        chei[poz]= suc;
        C[poz + 1] -> elimina(suc);
    }
    else
    {
        merge(poz);
        C[poz] -> elimina(eliminat);
    }
    return;
}

void Nod ::elimina(int k) {
    int i = gasestecheie(k);
    if (i < n && chei[i] == k)
    {
        if (frunza == true)
            eliminadinfrunza(i);
        else
            eliminanodintern(i);
    } else {
        if (frunza == true)
        {
           return;
        }
        int ok = 0;
        if (i == n)
            ok = 1;
        if (C[i] -> n < t)
            fill(i);
        if (ok && i > n)
            C[i - 1] -> elimina(k);
        else
            C[i] -> elimina(k);
    }
    return;
}

int Nod :: predecesor(int k){
    Nod* actual = C[k];
    while (actual -> frunza == false)
        actual = actual -> C[actual -> n];
    return actual -> chei[actual -> n-1];
}

int Nod :: succesor(int k){
    Nod* actual = C[k+1];
    while (actual -> frunza == false)
        actual = actual -> C[0];
    return actual -> chei[0];
}

void Nod :: fill(int k) {
    if (k != 0 && C[k - 1] -> n >= t)
        anterior(k);
    else if (k != n && C[k + 1]->n >= t)
        urmator(k);
    else {
        if (k != n)
            merge(k);
        else
            merge(k - 1);
    }
    return;
}

void Nod::anterior(int k)
{

    Nod *fiu = C[k];
    Nod *frate = C[k-1];

    for (int i = fiu->n-1; i >= 0; --i)
        fiu -> chei[i + 1] = fiu -> chei[i];

    if (!fiu->frunza)
    {
        for(int i = fiu->n; i >= 0; i--)
            fiu -> C[i+1] = fiu -> C[i];
    }

    fiu -> chei[0] = chei[k - 1];

    if(!fiu -> frunza)
        fiu -> C[0] = frate->C[frate->n];

    chei[k - 1]  = frate -> chei[frate -> n-1];

    fiu -> n += 1;
    frate -> n -= 1;
    return;
}

void Nod ::urmator(int k) {
    Nod *fiu = C[k];
    Nod *frate = C[k + 1];
    fiu -> chei[fiu -> n] = chei[k];
    if(!fiu -> frunza)
        fiu -> C[(fiu -> n) + 1] = frate -> C[0];
    chei[k] = frate -> chei[0];
    for (int i = 1; i < frate ->n; i++)
        frate -> chei[i - 1] = frate -> chei[i];
    if (!frate -> frunza)
    {
        for (int i = 1; i <= frate->n; i++)
            frate -> C[i - 1] = frate -> C[i];
    }

    fiu -> n ++;
    frate -> n --;
    return;
}

void Nod :: merge(int k) {
    Nod *fiu = C[k];
    Nod *frate = C[k + 1];

    fiu -> chei[t - 1] = chei[k];
    for (int i = 0; i  < frate -> n; i++)
        fiu -> chei[i + t] = frate -> chei[i];
    if (!fiu->frunza)
    {
        for (int i = 0; i <= frate->n; i++)
            fiu -> C[i + t] = frate -> C[i];
    }
    for (int i = k + 1; i <n; i++)
        chei[i-1] = chei[i];
    for (int i = k + 2; i <=n; i++)
        C[i - 1] = C[i];
    fiu -> n += frate -> n+1;
    n--;
    delete frate;
    return;
}

int Nod ::getpred(int k) {
    int pred = -1000000001;
    int i  = 0;
    while (i < n)
    {
        if (chei[i] > k)
            break;
        if (pred <= chei[i])
            pred = chei[i];
        i++;
    }
    if (frunza == false) //n+1 fii
        pred = std :: max(pred, C[i]->getpred(k));
    return pred;
}

int Nod :: getsucc(int k) {
    int succ = 1000000001;
    int i = n - 1;
    while (i >= 0) {
        if (chei[i] < k)
            break;
        if (succ >= chei[i])
            succ = chei[i];
        i--;
    }
    if (frunza == false) //n+1 fii
        succ = std :: min(succ, C[i + 1]->getsucc(k));

    return succ;
}

void Nod :: afisarexy(int x, int y) {

    for (int i = 0; i < n; i++)
    {   if (frunza == false)
            C[i]->afisarexy(x,y);
        if (x <= chei[i] && chei[i] <=y)
            g << chei[i] << " ";

    }
    if (frunza == false)
        C[n] -> afisarexy(x, y);

}
class BArbore {
    Nod *radacina;
    int t;
public:
    explicit BArbore(int t=2);
    virtual ~BArbore();
    void parcurgere();
    Nod* cautare(int k);
    void insert(int k);
    void remove(int k);
    int predecesor (int k);
    int succesor(int k);
    void afisare(int x, int y);
};

BArbore :: BArbore(int t1) {
    t = t1;
    radacina = nullptr;
}

BArbore :: ~BArbore() {
    delete radacina;
}

void BArbore :: parcurgere(){
    if (radacina != nullptr)
        radacina -> parcurge();
}

Nod* BArbore :: cautare(int k){
    if (radacina == nullptr)
        return nullptr;
    else
        return radacina -> cautare(k);

}

void BArbore ::insert(int k) {
    if (radacina == nullptr) //daca arborele e gol
    {
        radacina = new Nod(t, true); //aloc memorie pentru radacina
        radacina->chei[0] = k;  //insereaza cheie
        radacina->n = 1;  //schimba nr de chei in radacina
    }
    else
    {
        if (radacina->n == 2 * t - 1)
        {
            Nod *s = new Nod(t, false);
            s->C[0] = radacina;
            s->desparte(0, radacina);
            int i = 0;
            if (s->chei[0] < k)
                i++;
            s->C[i]->insereazaposibil(k);
            radacina = s;
        }
        else
            radacina->insereazaposibil(k);
    }
}

void BArbore :: remove(int k) {
    if (!radacina){
        //std :: cout << "Arborele este gol\n";
        return;
    }

    radacina -> elimina(k);
    if (radacina -> n == 0)
    {
        Nod *x = radacina;
        if (radacina -> frunza)
            radacina = nullptr;
        else
            radacina = radacina -> C[0];
        delete x;
    }
    return;
}

int BArbore :: predecesor(int k) {

    return radacina -> getpred(k);

}

int BArbore :: succesor(int k){

    return radacina -> getsucc(k);
}

void BArbore :: afisare(int x, int y) {

    radacina -> afisarexy(x, y);
}

int main() {
    BArbore t(3);

    int op, x, y, q;
    f >> q;
    for (int i = 0; i < q; i++){
        f >> op;
        if (op == 1) {
            f >> x;
            t.insert(x);
        }
        else if (op == 2){
            f >> x;
            t.remove(x);
        }
        else if (op == 3){
            f >> x;
            if (t.cautare(x) != nullptr)
                g << 1 << '\n';
            else
                g << 0 << '\n';
        }
        else if (op == 4){
            f >> x;
            g << t.predecesor(x) <<'\n';
        }
        else if (op == 5){
            f >> x;
            g << t.succesor(x) << '\n';
        }
        else if (op == 6){
            f >> x >> y;
            t.afisare(x, y);
            g << '\n';
        }
    }
    f.close();
    g.close();
    return 0;
}
