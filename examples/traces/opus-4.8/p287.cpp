#include <bits/stdc++.h>
using namespace std;
int main(){
    long long q, c;
    cin >> q >> c;
    long long m = c + 1;
    vector<long long> buf(m, 0);
    long long prev = 0; // f(0)
    long long n = 0;
    long long ans = -1;
    while(true){
        n++;
        long long idx = n % m;
        long long old = buf[idx];      // f(n-1-c)
        long long cur = prev + old + 1;
        buf[idx] = cur;
        prev = cur;
        if(cur >= q){ ans = n; break; }
    }
    cout << (ans + c) << "\n";
    return 0;
}
