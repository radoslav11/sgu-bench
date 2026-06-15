#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,K; 
    if(!(cin>>N>>K))return 0;
    int M=N*N;
    vector<string> g(M, string(M,'.'));
    long long m=N+1;
    for(int r=0;r<M;r++){
        for(int j=0;j<K;j++){
            long long c=((m*(long long)r)%M + j)%M;
            g[r][c]='*';
        }
    }
    bool ok=true;
    for(int r=0;r<M&&ok;r++){int cnt=0;for(int c=0;c<M;c++)if(g[r][c]=='*')cnt++;if(cnt!=K)ok=false;}
    for(int c=0;c<M&&ok;c++){int cnt=0;for(int r=0;r<M;r++)if(g[r][c]=='*')cnt++;if(cnt!=K)ok=false;}
    for(int br=0;br<N&&ok;br++)for(int bc=0;bc<N&&ok;bc++){
        int cnt=0;
        for(int ir=0;ir<N;ir++)for(int ic=0;ic<N;ic++)if(g[br*N+ir][bc*N+ic]=='*')cnt++;
        if(cnt!=K)ok=false;
    }
    if(!ok){cout<<"NO SOLUTION\n";return 0;}
    for(int r=0;r<M;r++)cout<<g[r]<<"\n";
    return 0;
}
