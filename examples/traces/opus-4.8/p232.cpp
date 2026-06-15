#include <bits/stdc++.h>
using namespace std;
int main(){
    int N; long long K;
    scanf("%d %lld",&N,&K);
    char* buf=new char[200005];
    scanf("%s",buf);
    string D=buf;
    long long k = K % N;
    int kk=(int)k;
    vector<int> cand(N), nc;
    vector<int> cur(N);
    for(int i=0;i<N;i++){cand[i]=i;cur[i]=i;}
    string out; out.reserve(N);
    for(int step=0; step<N; step++){
        char best=0;
        for(int idx: cand){ char c=D[cur[idx]]; if(c>best) best=c; }
        out.push_back(best);
        if((int)cand.size()>1){
            nc.clear();
            for(int idx: cand) if(D[cur[idx]]==best) nc.push_back(idx);
            cand.swap(nc);
        }
        for(int idx: cand){ cur[idx]+=kk; if(cur[idx]>=N) cur[idx]-=N; }
        if(cand.size()==1){
            int idx=cand[0];
            for(int s=step+1; s<N; s++){
                out.push_back(D[cur[idx]]);
                cur[idx]+=kk; if(cur[idx]>=N) cur[idx]-=N;
            }
            break;
        }
    }
    printf("%s\n", out.c_str());
    return 0;
}
