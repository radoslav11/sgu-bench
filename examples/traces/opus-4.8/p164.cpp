#include<bits/stdc++.h>
using namespace std;
int N,M;vector<vector<int>> col;
bool ok(set<int>&S){
    for(int s=0;s<N;s++){
        vector<int> d(N,-1);queue<int>q;d[s]=0;q.push(s);
        while(!q.empty()){int u=q.front();q.pop();if(d[u]>=3)continue;for(int v=0;v<N;v++)if(v!=u&&d[v]<0&&S.count(col[u][v])){d[v]=d[u]+1;q.push(v);}}
        for(int v=0;v<N;v++)if(d[v]<0)return false;
    }
    return true;
}
int main(){
    scanf("%d %d",&N,&M);col.assign(N,vector<int>(N));
    for(int i=0;i<N;i++)for(int j=0;j<N;j++)scanf("%d",&col[i][j]);
    int lim=(M+1)/2;
    if(N==1){printf("0\n\n");return 0;}
    vector<int> cnt(M+1,0);
    for(int v=1;v<N;v++)cnt[col[0][v]]++;
    vector<int> cols;for(int c=1;c<=M;c++)if(cnt[c])cols.push_back(c);
    sort(cols.begin(),cols.end(),[&](int a,int b){return cnt[a]>cnt[b];});
    for(int k=0;k<=(int)cols.size();k++){
        if(k>lim)break;
        set<int> S(cols.begin(),cols.begin()+k);
        if(ok(S)){printf("%d\n",(int)S.size());bool f=1;for(int x:S){printf("%s%d",f?"":" ",x);f=0;}printf("\n");return 0;}
    }
    set<int> all;for(int c=1;c<=M&&(int)all.size()<lim;c++)all.insert(c);
    if((int)all.size()<=lim&&ok(all)){printf("%d\n",(int)all.size());bool f=1;for(int x:all){printf("%s%d",f?"":" ",x);f=0;}printf("\n");return 0;}
    printf("-1\n");
    return 0;
}
