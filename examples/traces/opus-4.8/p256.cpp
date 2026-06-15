#include <bits/stdc++.h>
using namespace std;
int M,N,A[10],B[10];
int main(){
    cin>>M>>N;
    for(int i=0;i<N;i++)cin>>A[i]>>B[i];
    for(int T=0;;T++){
        map<vector<int>,int> cur;
        cur[vector<int>(N,0)]=0;
        for(int t=0;t<T;t++){
            map<vector<int>,int> nxt;
            for(auto&pr:cur){
                vector<int> s=pr.first; int v=pr.second;
                vector<int> ns=s; for(int&x:ns)if(x>0)x--;
                {auto it=nxt.find(ns); if(it==nxt.end()||it->second<v)nxt[ns]=v;}
                for(int i=0;i<N;i++)if(s[i]==0){
                    vector<int> ns2=s; for(int j=0;j<N;j++)if(ns2[j]>0)ns2[j]--;
                    ns2[i]=B[i];
                    int nv=v+A[i];
                    auto it2=nxt.find(ns2); if(it2==nxt.end()||it2->second<nv)nxt[ns2]=nv;
                }
            }
            cur=nxt;
        }
        int best=0; for(auto&pr:cur)best=max(best,pr.second);
        if(best>=M){cout<<T<<endl;return 0;}
    }
}
