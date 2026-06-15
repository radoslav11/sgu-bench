#include<bits/stdc++.h>
using namespace std;

int main(){
    long long N, K;
    cin >> N >> K;
    
    for(int i = 0; i < N; i++){
        long long val = (K * (long long)(i + 1)) / N - (K * (long long)i) / N;
        if(i > 0) cout << " ";
        cout << val;
    }
    cout << endl;
    
    return 0;
}
