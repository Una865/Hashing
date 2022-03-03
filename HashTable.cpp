#include <iostream>
#include<string>
#include <cmath>
#include <fstream>
using namespace std;
int KORAK;
int MAX_VELICINA;

//HashTable.h
struct description
{
    std::string revenue;
    std::string runtime;
    std::string original_language;
    std::string release_date;
};

int object_num = 0;
int key_num = 0;

struct objekat
{
    std::string original_name;
    std::string revenue;
    std::string runtime;
    std::string original_language;
    std::string release_date;
};

class HashNode
{
public:
    HashNode(std::string k, description v)
    {
        key = k;
        value.original_language = v.original_language;
        value.runtime = v.runtime;
        value.revenue = v.revenue;
        value.release_date = v.release_date;
    }
public:
    std::string key;
    description value;
};

class HashTable
{
    HashNode** arr;
    HashNode* del;
    int korak;   // ako je HashNode ovakav znaci da je izbrisan objekat koji je bio na toj adresi
    int capacity;    //max-size tabele
    int size;        //velicina tabele
    int attempts;    //koliko puta smo puta hteli da pronadjemo kljuc
    int key_number;  //koliko kljuceva imamo
    int success;     //koliko puta je uspesno nadjen kljuc
    int unssecc;
    friend class LinearHashing;
public:
    HashTable();
    friend std::ostream& operator<<(std::ostream& os, const HashTable& h);
    void Test(HashTable& h, objekat o[], std::string t[]);
    int convert(const std::string& s);
    int hash_value(int key);
    bool insertKey(std::string key, description value);
    void  findKey(std::string key, description& p);
    bool deleteKey(std::string key);
    int avgAccessSuccess();
    int avgAccessUnsuccess();
    int avgAccessUnsuccessEst();
    void resetStatistic();
    void clear();
    double fillRatio();
    int tableSize();
    ~HashTable();
};
//HashTable.h


//AddressFunction.h
class AddressFunciton
{
protected:
    HashTable& hashTable;
    AddressFunciton(HashTable&);
public:
    virtual int getAddress(std::string key, int address, int attempt, int size) = 0;
};


class LinearHashing : public AddressFunciton
{
public:
    LinearHashing(HashTable& hashTable);
    virtual int getAddress(std::string key, int address, int attempt, int size) override;
};
//AddressFunction

//HashTable.cpp
HashTable::HashTable()
{
    capacity = MAX_VELICINA;
    size = 0;
    korak = KORAK;
    key_number = 0;
    success = 0;
    attempts = 0;
    unssecc = 0;

    arr = new HashNode * [capacity];

    for (int i = 0; i < capacity; i++)
        arr[i] = NULL;

    description des;
    del = new HashNode("-1", des);
}

std::ostream& operator<<(std::ostream& os, const HashTable& h)
{
    for (int i = 0; i < h.capacity; i++)
    {
        if (h.arr[i] == NULL)
            os << "EMPTY" << std::endl;

        else
            if (h.arr[i]->key == "-1")os << "DELETED" << std::endl;
            else
                os << "Ime filma: " << h.arr[i]->key << " Original_language: " << h.arr[i]->value.original_language << " Revenue: " << h.arr[i]->value.revenue << " Runtime: " << h.arr[i]->value.runtime << " Release-date: " << h.arr[i]->value.release_date << std::endl;
    }

    return os;
}

int HashTable::tableSize()
{
    return size;
}
int HashTable::convert(const std::string& s)
{
    int value = 0;
    int mul = 1;

    for (unsigned i = 0; i < s.size(); i++, mul *= 2)
    {
        char ch = s[i];
        int add = (int)ch;
        value += add * mul;
    }
    return value;
}

int HashTable::hash_value(int k)
{
    int m = capacity;
    double A = 0.6180339887;
    return (int)(floor(m * (k * A - floor(k * A))));
}

bool HashTable::insertKey(std::string k, description v)
{
    LinearHashing linearHashing(*this);
    int newAddress = linearHashing.getAddress(k, -1, 1, size);
    if (newAddress == -1) {
        cout << "Kljuc se ne moze ubaciti" << endl; return false;
    }
    else
        if (arr[newAddress] == NULL || arr[newAddress]->key.compare("-1") == 0)
        {
            arr[newAddress] = new HashNode(k, v);
            size++;
            return true;
        }

    return false;
}

void HashTable::findKey(std::string k, description& p)
{
    int index = hash_value(convert(k));
    attempts++;
    p.original_language = "-1";
    p.revenue = "-1";
    p.runtime = "-1";
    p.release_date = "-1";

    while (arr[index] != NULL)
    {
        if (index > capacity)
        {
            unssecc++;
            return;

        }
        if (arr[index]->key == k)
        {
            success++;
            p = arr[index]->value; return;

        }


        index = hash_value(index + korak);
    }


    unssecc++;
    return;



}

bool HashTable::deleteKey(std::string k)
{
    int index = convert(k);
    index = hash_value(index);

    while (arr[index] != NULL && index < capacity)
    {
        if (arr[index]->key == k)
        {
            arr[index]->key = "-1"; return true;
        }
        index = hash_value(index + korak);

    }
    return false;
}

int HashTable::avgAccessSuccess()
{
    if (attempts == 0)return 0;
    return  success / attempts;
}

int HashTable::avgAccessUnsuccess()
{
    if (attempts == 0)return 0;
    return unssecc / attempts;
}

void HashTable::resetStatistic()
{
    key_number = 0;
    attempts = 0;
    success = 0;
    unssecc = 0;
}

void HashTable::clear()
{
    for (int i = 0; i < capacity; i++)
        arr[i] = NULL;
}

double HashTable::fillRatio()
{
    int filled = 0;
    for (int i = 0; i < capacity; i++)
        if (arr[i] != NULL && arr[i]->key.compare("-1") != 0)filled++;
    return filled * 1.0 / capacity;
}

int HashTable::avgAccessUnsuccessEst()
{
    double d = fillRatio();
    return (int)d * 100;
}

HashTable::~HashTable()
{
    delete[] arr;
}

//HashTable.cpp

//AddressFunction.cpp
AddressFunciton::AddressFunciton(HashTable& hashTable)
    :
    hashTable(hashTable)
{
}

LinearHashing::LinearHashing(HashTable& hashTable)
    :
    AddressFunciton(hashTable)
{
}

int LinearHashing::getAddress(std::string key, int address, int attempt, int size)
{
    int value = hashTable.convert(key);
    int newAddress = hashTable.hash_value(value);

    while (hashTable.arr[newAddress] != NULL && newAddress < hashTable.capacity && hashTable.arr[newAddress]->key != key && hashTable.arr[newAddress]->key == "-1")
        newAddress = hashTable.hash_value(newAddress + hashTable.korak);

    if (!(hashTable.arr[newAddress] != NULL && newAddress >= hashTable.capacity))
        return newAddress;
    else return -1;
}
//AddressFunction.cpp

bool isPrime(int n)
{
    // Corner cases
    if (n <= 1)  return false;
    if (n <= 3)  return true;

    // This is checked so that we can skip
    // middle five numbers in below loop
    if (n % 2 == 0 || n % 3 == 0) return false;

    for (int i = 5; i * i <= n; i = i + 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;

    return true;
}

// Function to return the smallest
// prime number greater than N
int nextPrime(int N)
{

    // Base case
    if (N <= 1)
        return 2;

    int prime = N;
    bool found = false;

    // Loop continuously until isPrime returns
    // true for a number greater than n
    while (!found) {
        prime++;

        if (isPrime(prime))
            found = true;
    }

    return prime;
}






void Test(HashTable& h, objekat o[], std::string t[])
{
    description find;
    for (int i = 0; i < object_num; i++)
    {
        description value;
        value.runtime = o[i].runtime;
        value.release_date = o[i].release_date;
        value.original_language = o[i].original_language;
        value.revenue = o[i].revenue;
        if (h.insertKey(o[i].original_name, value))cout << "Kljuc: " << o[i].original_name << "je uspesno ubacen u tabelu";
        else
            cout << "Kljuc: " << o[i].original_name << "je uspesno ubacen u tabelu" << endl;

    }

    for (int i = 0; i < key_num; i++)
    {

        h.findKey(t[i], find);
        if (find.original_language == "-1" && find.revenue == "-1" && find.release_date == "-1" && find.runtime == "-1")
            std::cout << "Trazenje kljuca: " << t[i] << " je neuspesno" << std::endl;
        else
            std::cout << "Trazenje kljuca: " << t[i] << " je uspesno" << std::endl;
    }

    std::cout << "Prosecan broj pristupa pri uspesnoj pretrazi: " << h.avgAccessSuccess() << std::endl;
    std::cout << "Proseccan broj pristupa pri neuspesnoj pretrazi: " << h.avgAccessUnsuccess() << std::endl;

}

int main()
{


    int c;



    std::string key;
    description newItem;
    int flag = 1;

    cout << "Uneiste velicinu tabele: " << endl;
    cin >> MAX_VELICINA;
    if (!isPrime(MAX_VELICINA)) {
        MAX_VELICINA = nextPrime(MAX_VELICINA);
    }
    cout << endl;
    cout << "Unesite korak pri linearnom pretrazivanju: " << endl;
    cin >> KORAK;
    cout << endl;

    HashTable hash;
    std::string film;
    std::string dl;
    objekat* o = new objekat[MAX_VELICINA];
    std::string* test = new std::string[MAX_VELICINA];

    while (flag)
    {
        std::cout << "1) Unos iz fajla: " << std::endl;
        std::cout << "2) Unos  sa standardnog ulaza: " << std::endl;
        std::cout << "3)Broj popunjenih mesta u hes tabeli: " << std::endl;
        std::cout << "4)Popunjenost tabele: " << std::endl;
        std::cout << "5)Ispis tabele:" << std::endl;
        std::cout << "6)Nadji zapis po kljucu: " << std::endl;
        std::cout << "7)Brisanje kljuca iz tabele: " << std::endl;
        std::cout << "8)Brisanje cele tabele: " << std::endl;
        std::cout << "9)Citanje fajla: " << std::endl;
        std::cout << "10)Exit: " << std::endl;
        cout << "Izbor: " << endl;
        std::cin >> c;
        std::string ime;
        std::string ime_f;
        std::ifstream outfile; //ucitavanje iz fajla
        std::ifstream ffile;


        int i = 0;

        switch (c)
        {
        case 1:
            std::cout << std::endl << "Unesite ime fajla: ";
            std::cin >> ime;
            outfile.open(ime);

            if (outfile.is_open())
            {
                while (outfile.good())
                {
                    getline(outfile, o[i].original_name);
                    getline(outfile, o[i].original_language);
                    getline(outfile, o[i].revenue);
                    getline(outfile, o[i].runtime);
                    getline(outfile, o[i].release_date);
                    i++;
                    object_num++;
                }

                outfile.close();

            }
            else
                std::cout << std::endl << "Neuspesno otvaranje fajla" << std::endl;

            break;

        case 2:

            std::cout << "Unesi naziv filma: ";
            std::cin >> o[i].original_name;
            std::cout << std::endl << "Unesi original_language: ";
            std::cin >> o[i].original_language;
            std::cout << std::endl << "Unesi revenue: ";
            std::cin >> o[i].revenue;
            std::cout << std::endl << "Unesi runtime: ";
            std::cin >> o[i].runtime;
            std::cout << std::endl << "Unesi release_date: ";
            std::cin >> o[i].release_date;
            i++;
            object_num++;



            break;

        case 3:
            cout << hash.tableSize() << endl;
            break;
        case 4:
            cout << hash.fillRatio() << endl;
            break;
        case 5:
            cout << hash;
            break;
        case 6:


            std::cout << std::endl << "Unesite broj kljuceva za pretragu :" << std::endl;
            std::cin >> key_num;
            std::cout << "Unesite kljuceve: " << std::endl;

            for (int i = 0; i < key_num; i++)
                std::cin >> test[i];

            Test(hash, o, test);
            break;
        case 7:

            cout << "Unesite kljuc za brisanje: " << endl;
            cin >> dl;
            hash.deleteKey(dl);
            break;
        case 8:
            hash.~HashTable();
            break;
        case 9:
            cout << "Unesite naziv fajla:" << endl;
            cin >> ime_f;
             
            ffile.open(ime_f);
            if (ffile.is_open())
            {
                while (ffile.good())
                {
                    getline(ffile, film);
                    cout << film << endl;
                }
                ffile.close();
            }
            else
                cout << "Neuspesno otvoren fajl." << endl;
            break;

        case 10:


            flag = 0;
            break;
        default:
            std::cout << "Pogresno izabran broj" << endl;
            break;



        }
    }
    delete[] o;
    delete[] test;



}
