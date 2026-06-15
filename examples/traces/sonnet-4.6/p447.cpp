#include <bits/stdc++.h>
using namespace std;
int main(){
    string line; getline(cin,line);
    long long total=0;
    int i=0,n=line.size();
    while(i<n){i++;int b=0;while(i<n&&isdigit(line[i]))b=b*10+(line[i++]-'0');int k=0;while(i<n&&line[i]=='.'){k++;i++;}long long val=64LL*((1<<(k+1))-1)/(b*(1<<k));total+=val;}
    // enumerate all valid commands
    vector<pair<string,int>> cmds;
    int bases[]={1,2,4,8,16,32,64};
    for(int b:bases){for(int k=0;b*(1<<k)<=64;k++){int val=64*((1<<(k+1))-1)/(b*(1<<k));string s="R"+to_string(b)+string(k,'.');cmds.push_back({s,val});}}
    sort(cmds.begin(),cmds.end());
    int T=total;
    vector<int> dp(T+1,INT_MAX);dp[0]=0;
    for(int v=0;v<=T;v++){if(dp[v]==INT_MAX)continue;for(auto&[s,val]:cmds){if(v+val<=T&&dp[v+val]>dp[v]+1)dp[v+val]=dp[v]+1;}}
    string ans="";int v=0;
    while(v<T){for(auto&[s,val]:cmds){if(v+val<=T&&dp[v+val]==dp[v]-1){ans+=s;v+=val;break;}}}
    cout<<ans<<"\n";
}
