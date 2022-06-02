#include <iostream>
#include <cmath>
#include <set>
#include <random>
#include "uwp.h"
using namespace std;

#define testNum 1000000

string randstr(int length) {
    char tmp;
    string buffer;
    
    random_device rd;
    default_random_engine random(rd());
    
    for (int i = 0; i < length; i++) {
        tmp = random() % 36;
        if (tmp < 10) {
            tmp += '0';
        } else {
            tmp -= 10;
            tmp += 'A';
        }
        buffer += tmp;
    }
    return buffer;
}

int main(){

    set<string> hash_set;
    UWP1 uwp;
    string hash;
    for(int i = 0; i < testNum; i++) {
        hash = uwp.hash(randstr(rand() % 100));
        if(hash_set.find(hash) == hash_set.end()){
            hash_set.insert(hash);
        }
    }

    cout << "total uuid = "<< testNum << endl;
    cout << "total hash = " << hash_set.size() << endl;
    cout << "total collision = " << testNum - hash_set.size() << endl;
    cout << "P(collision) = " << (double)(testNum - hash_set.size()) / testNum << endl;
    
    return 0;
}