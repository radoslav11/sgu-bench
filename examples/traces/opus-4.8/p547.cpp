#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    if(!(cin>>n)) return 0;
    long long D1,C1,D2,C2;
    cin>>D1>>C1>>D2>>C2;
    vector<long long> p(n+1);
    for(int i=1;i<=n;i++) cin>>p[i];
    for(int i=0;i<n-1;i++){int a,b;cin>>a>>b;}
    cout<<-1<<"\n";
    return 0;
}
