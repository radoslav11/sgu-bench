#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; scanf("%d",&n);
    vector<int> c(n);
    for(auto&x:c)scanf("%d",&x);
    vector<int> dp(n), par(n);
    unordered_map<int,int> bestColor;
    vector<array<int,3>> top;
    auto query=[&](int ca,int cb)->pair<int,int>{
        for(auto&t:top){
            if(t[1]!=ca && t[1]!=cb) return {t[0],t[2]};
        }
        return {0,-1};
    };
    int bestAns=0, bestEnd=-1;
    for(int i=0;i<n;i++){
        auto pr=query(c[i]-1,c[i]+1);
        dp[i]=pr.first+1;
        par[i]=pr.second;
        if(dp[i]>bestAns){bestAns=dp[i];bestEnd=i;}
        int col=c[i];
        auto it=bestColor.find(col);
        if(it==bestColor.end()||dp[i]>it->second){
            bestColor[col]=dp[i];
            bool found=false;
            for(auto&t:top){if(t[1]==col){t[0]=dp[i];t[2]=i;found=true;break;}}
            if(!found) top.push_back({dp[i],col,i});
            sort(top.begin(),top.end(),[](const array<int,3>&a,const array<int,3>&b){return a[0]>b[0];});
            if((int)top.size()>3) top.resize(3);
        }
    }
    vector<int> res;
    int cur=bestEnd;
    while(cur!=-1){res.push_back(c[cur]);cur=par[cur];}
    reverse(res.begin(),res.end());
    printf("%d\n",n-(int)res.size());
    for(int i=0;i<(int)res.size();i++){
        printf("%d",res[i]);
        if(i+1<(int)res.size())printf(" ");
    }
    printf("\n");
    return 0;
}
