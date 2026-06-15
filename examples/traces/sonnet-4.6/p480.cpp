#include <bits/stdc++.h>
using namespace std;
int main(){
    int c,k; cin>>c>>k;
    vector<vector<long long>> a(c,vector<long long>(k));
    for(int i=0;i<c;i++) for(int j=0;j<k;j++) cin>>a[i][j];
    vector<long long> cur_rem(k+1,0);
    cur_rem[k]=1;
    for(int j=k;j>0;j--) cur_rem[j-1]=cur_rem[j]*4;
    vector<tuple<int,int,long long>> ans;
    for(int i=0;i<c;i++){
        vector<long long> avail(k+1,0);
        for(int j=0;j<k;j++) avail[j]=a[i][j];
        for(int j=0;j<k;j++){
            long long u=min(avail[j],cur_rem[j]);
            if(u>0){ans.push_back({i+1,j,u});cur_rem[j]-=u;avail[j]-=u;}
            avail[j+1]+=avail[j]/4;
        }
        bool done=true;for(auto x:cur_rem)if(x>0)done=false;
        if(done)break;
    }
    bool ok=true;for(auto x:cur_rem)if(x>0)ok=false;
    if(!ok){cout<<"NO SOLUTION";return 0;}
    cout<<ans.size()<<"\n";
    for(auto&[col,sz,cnt]:ans) cout<<col<<" "<<sz<<" "<<cnt<<"\n";
}
