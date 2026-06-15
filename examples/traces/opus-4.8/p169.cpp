#include <bits/stdc++.h>
using namespace std;
int main(){
    long long K;
    if(!(cin>>K)) return 0;
    long long L = K-1;
    // repunit of L ones mod m for m=1..9
    long long valR[10];
    for(int m=1;m<=9;m++){
        long long r=0;
        for(long long i=0;i<L;i++){
            r=(r*10+1)%m;
        }
        valR[m]=(r*10)%m;
    }
    int count=0;
    for(int d=1;d<=8;d++){
        if(valR[d]==0 && valR[d+1]==0) count++;
    }
    cout<<count<<endl;
    return 0;
}
