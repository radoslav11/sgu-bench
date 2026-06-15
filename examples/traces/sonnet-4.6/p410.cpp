#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<long long>a(n);
    for(auto&x:a)cin>>x;
    // Each science day: ONE planet is doubled. S = total doublings.
    // Minimize F + S where F = a[i]*2^(d[i]), S = sum(d[i])
    // But need to ensure on each flying day all planets >=1
    // Since we schedule science days first then flying days (or interleave),
    // simplest: do all science days first, then flying days
    // Then we just need F = a[i]*2^(d[i]) for all i, minimize F + sum(d[i])
    
    set<long long>cands;
    for(int i=0;i<n;i++) for(int k=0;k<=60;k++){
        if((__int128)a[i]*(1LL<<k)>(long long)2e15)break;
        cands.insert(a[i]*(1LL<<k));
    }
    
    long long best=-1;
    int bestF_days=0, bestS_days=0;
    vector<int> bestD;
    
    for(long long F:cands){
        int sumD=0; bool ok=true;
        vector<int>D(n);
        for(int i=0;i<n;i++){
            if(F%a[i]!=0){ok=false;break;}
            long long r=F/a[i];
            if(r==0||(r&(r-1))!=0){ok=false;break;}
            D[i]=__builtin_ctzll(r);
            sumD+=D[i];
        }
        if(!ok)continue;
        long long cost=F+sumD;
        if(best<0||cost<best){best=cost;bestD=D;bestF_days=(int)F;bestS_days=sumD;}
    }
    
    cout<<best<<"\n";
    if(best<=1000){
        // Output science days (each doubles one planet), then flying days
        for(int i=0;i<n;i++)
            for(int j=0;j<bestD[i];j++)
                cout<<"science mission to the planet "<<(i+1)<<"\n";
        for(int f=0;f<bestF_days;f++)
            cout<<"flying mission\n";
    }
}
