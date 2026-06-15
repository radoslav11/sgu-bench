#include <bits/stdc++.h>
using namespace std;
int main(){
    string s;
    cin>>s;
    int n=s.size();
    vector<int>d(n);
    for(int i=0;i<n;i++)d[i]=s[i]-'0';
    int full=1<<n;
    vector<vector<int>>prev(full,vector<int>(17,-1));
    vector<vector<bool>>dp(full,vector<bool>(17,false));
    for(int i=0;i<n;i++){
        if(d[i]!=0){
            int mask=1<<i,rem=d[i]%17;
            if(!dp[mask][rem]){dp[mask][rem]=true;prev[mask][rem]=i;}
        }
    }
    for(int mask=1;mask<full;mask++)
        for(int rem=0;rem<17;rem++){
            if(!dp[mask][rem])continue;
            for(int i=0;i<n;i++){
                if(mask&(1<<i))continue;
                int nm=mask|(1<<i),nr=(rem*10+d[i])%17;
                if(!dp[nm][nr]){dp[nm][nr]=true;prev[nm][nr]=i;}
            }
        }
    int full1=full-1;
    if(!dp[full1][0]){cout<<-1;return 0;}
    string ans="";
    int mask=full1,rem=0;
    while(mask!=0){
        int i=prev[mask][rem];
        ans+=(char)('0'+d[i]);
        int pm=mask^(1<<i);
        int pr=(pm==0)?0:((rem-d[i]%17+17)*12)%17;
        mask=pm;rem=pr;
    }
    reverse(ans.begin(),ans.end());
    cout<<ans;
}
