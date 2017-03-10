#include<iostream>
#include<string>
#include<fstream>
using namespace std;

int main(){
    ifstream file;
    file.open("input0", ios::in);
    string sub;
    while (file >> sub) {
        cout << sub << endl;
    }
    int a = 10;
    return 0;
}