#include <bits/stdc++.h>
using namespace std;
int main(){
    int T; cin>>T;
    for(int t=0;t<T;t++){
        int H,W,N; cin>>H>>W>>N;
        if(t>0) cout<<"\n";
        int ih=H-2, iw=W-2;
        int maxN=0;
        if(ih>0&&iw>0) maxN=(ih*iw+1)/2;
        if(N>maxN){cout<<"Impossible"<<"\n";continue;}
        vector<string> g(H,string(W,'.'));
        int cnt=0;
        for(int i=1;i<H-1&&cnt<N;i++)
            for(int j=1;j<W-1&&cnt<N;j++)
                if((i+j)%2==0){g[i][j]='#';cnt++;}
        for(int i=0;i<H;i++) cout<<g[i]<<"\n";
    }
}
