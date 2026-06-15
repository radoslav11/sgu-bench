#include <bits/stdc++.h>
using namespace std;
char names[4]={'b','r','y','w'};
int colorIdx(char c){
    switch(c){case 'b':return 0;case 'r':return 1;case 'y':return 2;case 'w':return 3;}
    return -1;
}
int main(){
    int cnt[4];
    for(int i=0;i<4;i++) cin>>cnt[i];
    int rule[4][4];
    memset(rule,0,sizeof(rule));
    for(int c=0;c<4;c++){
        for(int k=0;k<cnt[c];k++){
            string s; cin>>s;
            int a=colorIdx(s[0]), b=colorIdx(s[1]);
            rule[a][b]|=(1<<c);
        }
    }
    string row; cin>>row;
    int n=row.size();
    vector<vector<int>> dp(n, vector<int>(n,0));
    for(int i=0;i<n;i++) dp[i][i]=1<<colorIdx(row[i]);
    for(int len=2;len<=n;len++){
        for(int i=0;i+len-1<n;i++){
            int j=i+len-1; int res=0;
            for(int k=i;k<j;k++){
                int L=dp[i][k], R=dp[k+1][j];
                if(!L||!R) continue;
                for(int a=0;a<4;a++) if(L&(1<<a))
                    for(int b=0;b<4;b++) if(R&(1<<b))
                        res|=rule[a][b];
            }
            dp[i][j]=res;
        }
    }
    int ans=dp[0][n-1];
    string out;
    for(int c=0;c<4;c++) if(ans&(1<<c)) out+=names[c];
    if(out.empty()) cout<<"Nobody\n";
    else cout<<out<<"\n";
    return 0;
}
