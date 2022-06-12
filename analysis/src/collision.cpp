#include <iostream>
#include <fstream> 
#include <cmath>
#include <set>
#include <random>
#include <algorithm>
#include <unistd.h>
#include "uwp.h"
using namespace std;

#define testNum 100000
#define secondPreimageNum 10000000
#define rainbowNum 1
#define rainbowLinks 100000
#define generateNum 1250000

struct Pass_Hash
{
    string frstpss;
    string lsthsh;
};

string randstr(int length) {
    char tmp;
    string buffer;
    
    random_device rd;
    default_random_engine random(rd());
    
    for (int i = 0; i < length; i++) {
        tmp = random() % 36;
        if (tmp < 10) {
            tmp += '0';
        } 
        else if (tmp < 36) {
            tmp -= 10;
            tmp += 'a';
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
        buffer = randstr((rand() % 100)+56);
        while (find(input_arr.begin(), input_arr.end(), buffer) != input_arr.end()) {
            buffer = randstr((rand() % 100)+56);
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
    OutFile << "total uuid = "<< testNum << endl;
    OutFile << "total hash = " << hash_set.size() << endl;
    OutFile << "total collision = " << testNum - hash_set.size() << endl;
    OutFile << "P(collision) = " << (double)(testNum - hash_set.size()) / testNum << endl;
    OutFile.close();
    cout << "All collisions have been saved to txt file." << endl;
}

void second_preimage_attack(string x) {
    UWP1 uwp;
    vector<string> input_arr;
    string x_hash = uwp.hash(x);
    string hash;
    int succeed_flag = 0;
    for(int i = 0; i < secondPreimageNum; i++) {
        string buffer;
        buffer = randstr(x.length());
        // buffer = randstr(rand() % 100+56);
        while (find(input_arr.begin(), input_arr.end(), buffer) != input_arr.end() | buffer == x) {
            buffer = randstr(x.length());
        }
        hash = uwp.hash(buffer);
        if(hash == x_hash) {
            cout << "Second-preimage attack succeed with " << i+1 << " attempts!" << endl;
            cout << "Input: " << x << " HASH(Input): 0x" << x_hash << endl;
            cout << "Output: " << buffer << " HASH(Output): 0x" << hash << endl;
            succeed_flag = 1;
            i = secondPreimageNum;
        }
    }
    if(succeed_flag == 0) {
        cout << "Second-preimage attack failed with " << secondPreimageNum << " attempts!" << endl;
    }
}

string reduce(string hashed){
    string reduced_hash = hashed.substr(0, 10) + hashed.substr(5, 15) + hashed.substr(10, 20) + hashed.substr(3, 13) + hashed.substr(8, 18) + hashed.substr(7, 14);
    return reduced_hash;
}

void create_rainbow_table(){
    UWP1 uwp;
    int links = rainbowLinks;
    int plains_num = 1000;
    vector<string> plains_to_hash;
    ofstream OutFile("table.txt");
    for(int i = 0; i < plains_num; i++) {
        string buffer;
        buffer = randstr((rand() % 100)+56);
        while (find(plains_to_hash.begin(), plains_to_hash.end(), buffer) != plains_to_hash.end()) {
            buffer = randstr((rand() % 100)+56);
        }
        plains_to_hash.push_back(buffer);
        string H;
        string R = buffer;
        OutFile << R << ",";
        for(int j = 0; j <= links; j++){
            H = uwp.hash(R);
            R = reduce(H);
        }
        OutFile << H << endl;
        
    }
    OutFile.close();
}

void string_split(const string& str, const string& splits, vector<string>& res)
{
	if(str == "") {
        return;
    }
	string strs = str + splits;
	size_t pos = strs.find(splits);
	int step = splits.size();
	while(pos != strs.npos)
	{
		string temp = strs.substr(0, pos);
		res.push_back(temp);
		strs = strs.substr(pos + step, strs.size());
		pos = strs.find(splits);
	}
}

Pass_Hash read_final_hashes(string check_hash) {
    string table_line;
    string placer_1;
    string placer_2;
    int found = 0;
    ifstream InFile("table.txt");
    while (getline(InFile, table_line)) {
        vector<string> split_part;
        string_split(table_line, ",", split_part);
        placer_1 = split_part[0];
        placer_2 = split_part[1];
        if (placer_2 == check_hash){
            found = 1;
            break;
        }
    }
    InFile.close();
    Pass_Hash ph;
    if(found == 1) {
        ph.frstpss = placer_1;
        ph.lsthsh = check_hash;
    }
    else {
        ph.frstpss = "No Match!";
        ph.lsthsh = "No Match!";
    }
    return ph;
}

string rainbow_attack(string attack_hash, int mode) {
    UWP1 uwp;
    int found = 0;
    string H = attack_hash;
    string R, found_first_plain, found_end_hash;
    int n;
    for(int i=0;i<rainbowLinks;i++) {
        Pass_Hash res = read_final_hashes(H);
        string check_pass = res.frstpss;
        string check_hash = res.lsthsh;
        if(check_hash != "No Match!") {
            found = 1;
            n = rainbowLinks - i;
            found_first_plain = check_pass;
            found_end_hash = check_hash;
            break;
        }
        R = reduce(H);
        H = uwp.hash(R);
    }
    if(found == 1) {
        cout << "Attack Hash: " << attack_hash << endl;
        cout << "First plaintext: " << found_first_plain << endl;
        cout << "Last hash in link: " << found_end_hash << endl;
        string next_reduce = found_first_plain;
        string next_hash, prev_reduce;
        for(int j = 0; j <= n; j++){
            next_hash = uwp.hash(next_reduce);
            prev_reduce = next_reduce;
            next_reduce = reduce(next_hash);
        }
        // cout << "Attack Hash: " << attack_hash << endl;
        // cout << "Next Hash: " << next_hash << endl;
        // cout << "The original plaintext is: " << prev_reduce << endl;
        return prev_reduce;
    }
    else {
        cout << "No match found after " << rainbowNum << " attempts!" << endl;
    }
    return "No Match!";
}

void test() {
    UWP1 uwp;
    string x = "RjndubmrrNgo1s0dPbIJJ9adb0cgkp35j1njB63p1OIB7F28ORjndum1qwbdhjqwdhbhjbx";
    string y = "Rjndqwegjqikuehi23hy83hdhq3h1o9823120eu3j948uyr89y7y58tyhf82wdqewcecqcx";
    cout << "X: " << x << " HASH(X): 0x" << uwp.hash(x) << endl;
    cout << "Y: " << y << " HASH(Y): 0x" << uwp.hash(y) << endl;
}

string outputBinary(string hv){
    string ret;
    string buf[16] = {"0000","0001","0010","0011",\
                      "0100","0101","0110","0111",\
                      "1000","1001","1010","1011",\
                      "1100","1101","1110","1111"};
    char item[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    for(int i=0;i<hv.length();i++) {
        for(int j=0;j<16;j++) {
            if(hv[i] == item[j]) {
                ret += buf[j];
                j = 16;
            }
        }
    }
    return ret;
}

int main(int argc, char **argv) {
    // 0: collision attack; 1: second preimage attack; 2: test
    if (strcmp(argv[1], "0") == 0) {
        collision_attack();
    }
    else if (strcmp(argv[1], "1") == 0) {
        string x = "RjndubmrrNgo1s0dPbIJJ9adb0cgkpu3j948uyr89y7y58tyhf82o982";
        second_preimage_attack(x);
    }
    else if (strcmp(argv[1], "2") == 0) {
        test();
    }
    else if (strcmp(argv[1], "3") == 0) {
        if (access("table.txt", F_OK) != 0) {
            create_rainbow_table();
        }
        UWP1 uwp;
        string buffer, buffer_hash;
        vector<string> tests;
        int count_found = 0;
        string x = "ec56caeed4aeed4136097c0c5136097c0c56caeed4136097d4136097c0c5ed4136097c0c5";
        string x_hash = uwp.hash(x);
        string x_rc = rainbow_attack(x_hash, 1);
        cout << "Original Plaintext:  " << x << endl;
        cout << "Recovered Plaintext: " << x_rc << endl;
        // for(int i=0;i<rainbowNum;i++) {
        //     buffer = randstr(73);
        //     while (find(tests.begin(), tests.end(), buffer) != tests.end()) {
        //         buffer = randstr(73);
        //     }
        //     buffer_hash = uwp.hash(buffer);
        //     count_found += rainbow_attack(buffer_hash, 1);
        // }
        // cout << "All: " << rainbowNum << " Found: " << count_found << endl;
        // cout << "P = " << (double)(count_found / rainbowNum) << endl;
    }
    else if (strcmp(argv[1], "4") == 0) {
        UWP1 uwp;
        string buffer;
        string hash_value;
        string hash_binary;
        ofstream OutFile("hash.txt");
        for(int i=0;i<generateNum;i++) {
            buffer = randstr((rand() % 100)+56);
            hash_value = uwp.hash(buffer);
            OutFile << outputBinary(hash_value);
        }
        OutFile.close();
    }
    return 0;
}