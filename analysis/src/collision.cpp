#include <iostream>
#include <fstream> 
#include <cmath>
#include <set>
#include <random>
#include <algorithm>
#include "uwp.h"
using namespace std;

#define testNum 100000

string randstr(int length) {
    char tmp;
    string buffer;
    
    random_device rd;
    default_random_engine random(rd());
    
    for (int i = 0; i < length; i++) {
        tmp = random() % 62;
        if (tmp < 10) {
            tmp += '0';
        } 
        else if (tmp < 36) {
            tmp -= 10;
            tmp += 'a';
        }    
        else {
            tmp -= 36;
            tmp += 'A';
        }
        buffer += tmp;
    }
    return buffer;
}

void collision_attack() {
    set<string> hash_set;
    vector<string> hash_arr;
    vector<string> input_arr;
    UWP1 uwp;
    string hash;
    vector<string> collision_x;
    vector<string> collision_y;
    for(int i = 0; i < testNum; i++) {
        string buffer;
        buffer = randstr(rand() % 100);
        while (find(input_arr.begin(), input_arr.end(), buffer) != input_arr.end()) {
            buffer = randstr(rand() % 100);
        }
        hash = uwp.hash(buffer);
        set<string>::iterator hash_search=hash_set.find(hash);
        if(hash_search == hash_set.end()){
            hash_set.insert(hash);
            hash_arr.push_back(hash);
            input_arr.push_back(buffer);
        }
        else {
            string collision_hash = *hash_search;
            vector<string>::iterator hash_in_vector = find(hash_arr.begin(), hash_arr.end(), collision_hash);
            collision_x.push_back(input_arr[hash_in_vector - hash_arr.begin()]);
            collision_y.push_back(buffer);
        }
    }

    ofstream OutFile("collision.txt");
    for (int i = 0; i < collision_x.size(); i++) {
        OutFile << "Collision " << i << ":" << endl;
        OutFile << "X: " << collision_x[i] << " HASH(X): 0x";
        OutFile << uwp.hash(collision_x[i]) << endl;
        OutFile << "Y: " << collision_y[i] << " HASH(Y): 0x";
        OutFile << uwp.hash(collision_y[i]) << endl;
    }
    OutFile.close();
    cout << "total uuid = "<< testNum << endl;
    cout << "total hash = " << hash_set.size() << endl;
    cout << "total collision = " << testNum - hash_set.size() << endl;
    cout << "P(collision) = " << (double)(testNum - hash_set.size()) / testNum << endl;
    cout << "All collisions have been saved to collision.txt." << endl;
}

void test() {
    UWP1 uwp;
    string x = "tsAOabCR24EKOg2HNpQqjwhwaycPH9Xgldbsfg3I";
    string y = "kZPqqaB2cKCHzOZefvTmrsBrXbqKKdBOBlKuRFHP";
    cout << "X: " << x << " HASH(X): 0x" << uwp.hash(x) << endl;
    cout << "Y: " << y << " HASH(Y): 0x" << uwp.hash(y) << endl;
}

int main(){
    collision_attack();
    // test();
    return 0;
}