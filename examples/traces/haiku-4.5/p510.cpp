#include<bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;
    
    string result = "";
    
    for (char c = 'a'; c <= 'z'; c++) {
        while (true) {
            string test = result + c;
            set<string> subs;
            for (int i = 0; i < (int)test.length(); i++) {
                for (int j = i; j < (int)test.length(); j++) {
                    subs.insert(test.substr(i, j - i + 1));
                }
            }
            int count = subs.size();
            
            if (count == n) {
                cout << test << endl;
                return 0;
            }
            if (count > n) {
                break;
            }
            result = test;
        }
    }
    
    return 0;
}
