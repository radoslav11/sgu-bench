#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,x;
    scanf("%d %d",&n,&x);
    vector<int> a(n);
    for(auto&v:a)scanf("%d",&v);
    // prefix[i] = reachable sums using coins 0..i-1
    vector<bitset<10001>> pre(n+1), suf(n+1);
    pre[0][0]=1;
    for(int i=0;i<n;i++){
        pre[i+1]=pre[i]|(pre[i]<<a[i]);
    }
    suf[n][0]=1;
    for(int i=n-1;i>=0;i--){
        suf[i]=suf[i+1]|(suf[i+1]<<a[i]);
    }
    vector<int> res;
    for(int i=0;i<n;i++){
        // sums without coin i: pre[i] combined with suf[i+1]
        bool ok=false;
        // check if exists s in pre[i] and (x-s) in suf[i+1]
        for(int s=0;s<=x;s++){
            if(pre[i][s] && suf[i+1][x-s]){ok=true;break;}
        }
        if(!ok) res.push_back(a[i]);
    }
    printf("%d\n",(int)res.size());
    for(int i=0;i<(int)res.size();i++){
        printf("%d%c",res[i], i+1<(int)res.size()?' ':'\n');
    }
    if(res.empty())printf("\n");
    return 0;
}
