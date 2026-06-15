#include <bits/stdc++.h>
using namespace std;

long long power(long long a, long long b) {
    long long res = 1;
    for(long long i = 0; i < b; i++) res *= a;
    return res;
}

int main(){
    long long N, M;
    cin >> N >> M;
    cout << power(6, (N/2) * (M/2)) << endl;
    return 0;
}
