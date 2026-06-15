#include <bits/stdc++.h>
using namespace std;
int main(){
    int m; cin>>m;
    int n=1; while(n*(n-1)/2!=m) n++;
    multiset<int> S;
    for(int i=0;i<m;i++){int x;cin>>x;S.insert(x);}
    vector<int> A;
    A.push_back(0);
    while((int)A.size()<n){
        for(auto it=S.begin();it!=S.end();it++){
            int cand=*it;
            multiset<int> tmp=S;
            bool ok=true;
            for(int a:A){
                auto f=tmp.find(a^cand);
                if(f==tmp.end()){ok=false;break;}
                tmp.erase(f);
            }
            if(ok){A.push_back(cand);S=tmp;break;}
        }
    }
    for(int x:A) cout<<x<<" ";
}
