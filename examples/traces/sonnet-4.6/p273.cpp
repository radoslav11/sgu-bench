#include <bits/stdc++.h>
using namespace std;
int main(){
    string line;
    auto cidx=[](char c)->int{return c=='b'?0:c=='r'?1:c=='y'?2:3;};
    while(getline(cin,line)){
        if(line.find("Test")==string::npos) continue;
        int cnt[4]; cin>>cnt[0]>>cnt[1]>>cnt[2]>>cnt[3];
        int rule[4][4]={};
        for(int c=0;c<4;c++) for(int k=0;k<cnt[c];k++){string r;cin>>r;rule[cidx(r[0])][cidx(r[1])]|=(1<<c);}
        string s; cin>>s; int n=s.size();
        vector<vector<int>>dp(n,vector<int>(n,0));
        for(int i=0;i<n;i++) dp[i][i]=(1<<cidx(s[i]));
        for(int len=2;len<=n;len++) for(int i=0;i+len-1<n;i++){int j=i+len-1;for(int k=i;k<j;k++) for(int a=0;a<4;a++) if(dp[i][k]&(1<<a)) for(int b=0;b<4;b++) if(dp[k+1][j]&(1<<b)) dp[i][j]|=rule[a][b];}
        string res="",cols="bryw";
        for(int c=0;c<4;c++) if(dp[0][n-1]&(1<<c)) res+=cols[c];
        sort(res.begin(),res.end());
        cout<<(res.empty()?"Nobody":res)<<"\n";
        string tmp; getline(cin,tmp);
    }
}
