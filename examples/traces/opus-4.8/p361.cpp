#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,M;
    cin>>N>>M;
    long long cnt[3]={0,0,0};
    for(int i=0;i<N;i++)for(int j=0;j<M;j++)cnt[(i+j)%3]++;
    int best=0;
    for(int r=1;r<3;r++)if(cnt[r]<cnt[best])best=r;
    string out;
    for(int i=0;i<N;i++){
        string line;
        for(int j=0;j<M;j++)line+=((i+j)%3==best)?'#':'0';
        out+=line; out+='\n';
    }
    cout<<out;
    return 0;
}
