#include <bits/stdc++.h>
using namespace std;
int M,N;
vector<string> grid;
const int INF=1e9;

void rec(int row,int col,int covered,int down,int uncov,int cost,
         int covAbove,int prevUncov, map<pair<int,int>,int>&res){
    if(col==N){
        if(uncov & prevUncov) return;
        auto key=make_pair(down,uncov);
        auto it=res.find(key);
        if(it==res.end()||it->second>cost) res[key]=cost;
        return;
    }
    bool candle=(grid[row][col]=='*');
    bool covFromAbove=(covAbove>>col)&1;
    bool curCov=(covered>>col)&1;
    if(candle){
        rec(row,col+1,covered,down,uncov,cost,covAbove,prevUncov,res);
        return;
    }
    if(curCov||covFromAbove){
        rec(row,col+1,covered,down,uncov,cost,covAbove,prevUncov,res);
        return;
    }
    rec(row,col+1,covered,down,uncov|(1<<col),cost,covAbove,prevUncov,res);
    if(col+1<N && grid[row][col+1]!='*' && !((covered>>(col+1))&1) && !((covAbove>>(col+1))&1)){
        rec(row,col+1,covered|(1<<(col+1)),down,uncov,cost+1,covAbove,prevUncov,res);
    }
    if(row+1<M && grid[row+1][col]!='*'){
        rec(row,col+1,covered,down|(1<<col),uncov,cost+1,covAbove,prevUncov,res);
    }
}
int main(){
    cin>>M>>N;
    grid.resize(M);
    for(int i=0;i<M;i++)cin>>grid[i];
    map<pair<int,int>,int> dp;
    dp[make_pair(0,0)]=0;
    for(int r=0;r<M;r++){
        map<pair<int,int>,int> ndp;
        for(auto&kv:dp){
            int covAbove=kv.first.first;
            int prevUncov=kv.first.second;
            int base=kv.second;
            map<pair<int,int>,int> res;
            rec(r,0,0,0,0,0,covAbove,prevUncov,res);
            for(auto&rr:res){
                int down=rr.first.first;
                int uncov=rr.first.second;
                auto key=make_pair(down,uncov);
                int c=base+rr.second;
                auto it=ndp.find(key);
                if(it==ndp.end()||it->second>c) ndp[key]=c;
            }
        }
        dp=ndp;
    }
    int ans=INF;
    for(auto&kv:dp){
        if(kv.first.first==0) ans=min(ans,kv.second);
    }
    cout<<ans<<endl;
}
