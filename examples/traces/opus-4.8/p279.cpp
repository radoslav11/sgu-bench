#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; 
    if(!(cin>>n))return 0;
    vector<int> a(n+1);
    vector<int> cnt(n,0);
    bool ok=true;
    for(int i=1;i<=n;i++){
        cin>>a[i];
        if(a[i]<0||a[i]>=n){ok=false;}
        else cnt[a[i]]++;
    }
    for(int v=0;v<n;v++) if(cnt[v]!=1) ok=false;
    if(!ok){ cout<<"NO\n"; return 0;}
    // rank: player with score s -> position
    vector<int> bysc(n);
    for(int i=1;i<=n;i++) bysc[a[i]]=i;
    cout<<"YES\n";
    // print descending score primes, then ascending positives
    vector<int> out;
    for(int s=n-1;s>=0;s--) out.push_back(-bysc[s]);
    for(int s=0;s<n;s++) out.push_back(bysc[s]);
    for(int i=0;i<(int)out.size();i++){
        cout<<out[i];
        if(i+1<(int)out.size())cout<<" ";
    }
    cout<<"\n";
    return 0;
}
