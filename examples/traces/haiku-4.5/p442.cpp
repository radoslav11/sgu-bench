#include<bits/stdc++.h>
using namespace std;

long long reverseNumber(long long x) {
    long long rev = 0;
    while (x > 0) {
        rev = rev * 10 + x % 10;
        x /= 10;
    }
    return rev;
}

int main(){
    string n_str;
    while(cin >> n_str && n_str != "0") {
        int count = 0;
        
        if (n_str.length() <= 9) {
            long long n = stoll(n_str);
            for (long long x = 1; x < n; x++) {
                if (x + reverseNumber(x) == n) {
                    count++;
                }
            }
        }
        
        cout << count << "\n";
    }
    return 0;
}
