#include <bits/stdc++.h>
using namespace std;
int main(){
    long long N,A0,B0;
    cin>>N>>A0>>B0;
    A0%=N; B0%=N;
    set<pair<long long,long long>> s;
    for(long long k=0;k<N;k++){
        s.insert({(A0*k)%N,(B0*k)%N});
    }
    cout<<s.size()<<"\n";
    for(auto&p:s) cout<<p.first<<" "<<p.second<<"\n";
    return 0;
}
