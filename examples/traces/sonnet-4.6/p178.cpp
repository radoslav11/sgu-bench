#include<bits/stdc++.h>
using namespace std;
int main(){
    long long N;
    cin>>N;
    for(long long k=0;;k++){
        // max N with k breaks
        // k + (k+1)*(2^(k+1)-1)
        // be careful of overflow
        long long pw;
        if(k+1>=60) pw = (long long)9e18;
        else pw = (1LL<<(k+1));
        long long maxN;
        // k + (k+1)*(pw-1)
        if(pw > (long long)2e16) maxN = (long long)9e18;
        else maxN = k + (k+1)*(pw-1);
        if(maxN >= N){
            cout<<k<<endl;
            return 0;
        }
    }
}
