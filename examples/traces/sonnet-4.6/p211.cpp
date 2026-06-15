#include <bits/stdc++.h>
using namespace std;
int main(){
    int N; cin>>N;
    int M; cin>>M;
    vector<int> A(N,0);
    for(int q=0;q<M;q++){
        int pos; cin>>pos;
        vector<pair<int,int>> changes;
        int carry=1,cur=pos;
        while(carry&&cur<N){
            int nv=A[cur]+carry;
            if(nv<=2){A[cur]=nv;changes.push_back({cur,nv});carry=0;}
            else{A[cur]=1;changes.push_back({cur,1});carry=1;cur++;}
        }
        cout<<changes.size();
        for(auto&p:changes)cout<<" "<<p.first<<" "<<p.second;
        cout<<"\n";
    }
}
