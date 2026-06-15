#include <bits/stdc++.h>
using namespace std;

int main(){
    long long N, K;
    if(!(cin >> N >> K)) return 0;
    for(long long i = 0; i < N; i++){
        long long val = ((i+1)*K)/N - (i*K)/N;
        cout << val;
        if(i+1 < N) cout << ' ';
    }
    cout << '\n';
    return 0;
}
