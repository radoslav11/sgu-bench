#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,m;
    cin>>n>>m;
    vector<vector<int>> seq(n);
    vector<set<int>> sets(n);
    for(int i=0;i<n;i++){
        int k;cin>>k;
        seq[i].resize(k);
        for(int j=0;j<k;j++){cin>>seq[i][j];sets[i].insert(seq[i][j]);}
    }
    while(m--){
        int l;cin>>l;
        vector<int> q(l);
        for(int j=0;j<l;j++)cin>>q[j];
        vector<int> found;
        for(int i=0;i<n;i++){
            bool ok=true;
            for(int x:q){
                if(x>0){if(!sets[i].count(x)){ok=false;break;}}
                else{if(sets[i].count(-x)){ok=false;break;}}
            }
            if(ok)found.push_back(i);
        }
        cout<<found.size()<<"\n";
        for(int i:found){
            cout<<seq[i].size();
            for(int x:seq[i])cout<<" "<<x;
            cout<<"\n";
        }
    }
}
