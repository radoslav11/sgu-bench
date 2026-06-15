#include <bits/stdc++.h>
using namespace std;
int main(){
    long long N,q;
    cin>>N>>q;
    long long offset=0;
    while(N>1){
        long long K=N/2;
        if(q<=K){
            offset += (N-K);
            // subproblem length K, new q = K-q+1
            q = K-q+1;
            N = K;
        } else {
            // subproblem length N-K, new q = N-q+1
            q = N-q+1;
            N = N-K;
        }
    }
    cout << offset+1 << endl;
}
