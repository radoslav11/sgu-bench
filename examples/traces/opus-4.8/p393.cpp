#include <bits/stdc++.h>
using namespace std;

int N, M, K;
int color[13];
vector<array<int,4>> constraintsByMax[13];

bool valid(int idx){
    for(auto &q : constraintsByMax[idx]){
        if(color[q[0]]==color[q[2]] && color[q[1]]==color[q[3]]) return false;
    }
    return true;
}

bool dfs(int idx, int maxColor){
    if(idx==N) return true;
    int lim = min(maxColor+1, K-1);
    for(int c=0;c<=lim;c++){
        color[idx]=c;
        if(valid(idx)){
            if(dfs(idx+1, max(maxColor,c))) return true;
        }
    }
    color[idx]=-1;
    return false;
}

int main(){
    scanf("%d %d",&N,&M);
    vector<array<int,2>> words(M);
    for(int i=0;i<M;i++){
        char buf[10];
        scanf("%s", buf);
        words[i][0]=buf[0]-'a';
        words[i][1]=buf[1]-'a';
    }
    for(int i=0;i<M;i++)for(int j=i+1;j<M;j++){
        int a=words[i][0],b=words[i][1],c=words[j][0],d=words[j][1];
        int mx=max(max(a,b),max(c,d));
        constraintsByMax[mx].push_back({a,b,c,d});
    }
    for(K=1;K<=N;K++){
        for(int i=0;i<N;i++) color[i]=-1;
        if(dfs(0,-1)) break;
    }
    int mc=0;
    for(int i=0;i<N;i++) mc=max(mc,color[i]);
    int cnt=mc+1;
    vector<string> groups(cnt);
    for(int i=0;i<N;i++) groups[color[i]] += char('a'+i);
    vector<string> res;
    for(auto &g:groups) if(!g.empty()) res.push_back(g);
    printf("%d\n",(int)res.size());
    for(auto &g:res) printf("%s\n", g.c_str());
    return 0;
}
