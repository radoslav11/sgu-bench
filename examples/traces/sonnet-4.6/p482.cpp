#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; scanf("%d",&n);
    vector<int>h(n);
    for(auto&x:h)scanf("%d",&x);
    int P=2*n+h[0]+h[n-1];
    for(int i=0;i+1<n;i++)P+=abs(h[i]-h[i+1]);
    const int MAXS=5001;
    vector<vector<vector<int>>>dp(n,vector<vector<int>>(n,vector<int>(MAXS,-1)));
    vector<vector<vector<array<int,3>>>>par(n,vector<vector<array<int,3>>>(n,vector<array<int,3>>(MAXS,{-1,-1,-1})));
    for(int i=0;i<n;i++) dp[i][i][h[i]]=2;
    for(int l=0;l<n;l++)
        for(int f=0;f<=l;f++)
            for(int s=0;s<MAXS;s++){
                if(dp[f][l][s]<0)continue;
                int val=dp[f][l][s];
                for(int m=l+1;m<n;m++){
                    int ns=s+h[m];
                    if(ns>=MAXS)continue;
                    int nval=val+2+abs(h[l]-h[m]);
                    if(nval>dp[f][m][ns]){
                        dp[f][m][ns]=nval;
                        par[f][m][ns]={f,l,s};
                    }
                }
            }
    int total_h=0;for(int x:h)total_h+=x;
    int best_stolen=-1,best_f=0,best_l=0,best_s=0;
    for(int f=0;f<n;f++)
        for(int l=f;l<n;l++)
            for(int s=1;s<MAXS;s++){
                if(dp[f][l][s]<0)continue;
                int perim=dp[f][l][s]+h[f]+h[l];
                if(2*perim>=P){
                    int stolen=total_h-s;
                    if(stolen>best_stolen){
                        best_stolen=stolen;
                        best_f=f;best_l=l;best_s=s;
                    }
                }
            }
    vector<int>kept;
    int cl=best_l,cs=best_s;
    while(par[best_f][cl][cs][1]!=-1){
        kept.push_back(cl+1);
        auto p=par[best_f][cl][cs];
        cl=p[1];cs=p[2];
    }
    kept.push_back(cl+1);
    sort(kept.begin(),kept.end());
    vector<bool>in_kept(n,false);
    for(int x:kept)in_kept[x-1]=true;
    vector<int>stolen_boards;
    for(int i=0;i<n;i++)if(!in_kept[i])stolen_boards.push_back(i+1);
    printf("%d\n%d\n",best_stolen,(int)stolen_boards.size());
    for(int x:stolen_boards)printf("%d ",x);
    if(!stolen_boards.empty())printf("\n");
    return 0;
}
