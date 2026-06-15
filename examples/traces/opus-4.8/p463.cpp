#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,m;
    cin>>n>>m;
    vector<string> g(n);
    for(int i=0;i<n;i++) cin>>g[i];
    string path; cin>>path;
    vector<vector<int>> cnt(n, vector<int>(m,0));
    int dr[4]={0,-1,0,1};
    int dc[4]={1,0,-1,0};
    int dir=0;
    int r=0,c=0;
    long long sat=0;
    auto pass=[&](int br,int bc){
        if(br<0||br>=n||bc<0||bc>=m) return;
        int b=g[br][bc]-'0';
        if(cnt[br][bc]==0) sat+=b;
        else sat+=b/2;
        cnt[br][bc]++;
    };
    for(char ch:path){
        if(ch=='L') dir=(dir+1)%4;
        else if(ch=='R') dir=(dir+3)%4;
        else{
            int nr=r+dr[dir], nc=c+dc[dir];
            if(dr[dir]==0){
                int bc=min(c,nc);
                pass(r-1,bc);
                pass(r,bc);
            } else {
                int br=min(r,nr);
                pass(br,c-1);
                pass(br,c);
            }
            r=nr;c=nc;
        }
    }
    cout<<sat<<endl;
    return 0;
}
