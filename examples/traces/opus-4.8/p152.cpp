#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; 
    if(!(cin>>n)) return 0;
    vector<long long> a(n);
    long long total=0;
    for(int i=0;i<n;i++){cin>>a[i]; total+=a[i];}
    if(total==0){ cout<<"No solution\n"; return 0; }
    vector<long long> fl(n);
    long long S=0;
    for(int i=0;i<n;i++){ fl[i]=(100*a[i])/total; S+=fl[i]; }
    long long need=100-S;
    for(int i=0;i<n && need>0;i++){
        long long rem=(100*a[i])%total;
        if(rem!=0){ fl[i]++; need--; }
    }
    for(int i=0;i<n;i++){ cout<<fl[i]; if(i+1<n) cout<<' '; }
    cout<<"\n";
    return 0;
}
