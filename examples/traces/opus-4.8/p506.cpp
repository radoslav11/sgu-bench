#include <bits/stdc++.h>
using namespace std;

int main(){
    string t, m;
    getline(cin, t);
    getline(cin, m);
    int n = t.size();
    vector<array<int,26>> nxt(n+1);
    for(int c=0;c<26;c++) nxt[n][c]=n;
    for(int i=n-1;i>=0;i--){
        nxt[i]=nxt[i+1];
        nxt[i][t[i]-'a']=i;
    }
    long long ans=0;
    for(int i=0;i<n;i++){
        int pos=i;
        bool ok=true;
        for(char ch:m){
            int p=nxt[pos][ch-'a'];
            if(p==n){ok=false;break;}
            pos=p+1;
        }
        if(ok){
            int j=pos-1;
            ans += (long long)(n - j);
        }
    }
    cout<<ans<<endl;
    return 0;
}
