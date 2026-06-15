#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,A0,B0;
    cin>>N>>A0>>B0;
    vector<pair<int,int>> sols;
    for(int x=0;x<N;x++)
        for(int y=0;y<N;y++)
            if((1LL*A0*x+1LL*B0*y)%N==0)
                sols.push_back({x,y});
    vector<pair<int,int>> res;
    for(int a=0;a<N;a++)
        for(int b=0;b<N;b++){
            bool ok=true;
            for(auto&p:sols)
                if((1LL*a*p.first+1LL*b*p.second)%N!=0){ok=false;break;}
            if(ok)res.push_back({a,b});
        }
    cout<<res.size()<<"\n";
    for(auto&p:res)cout<<p.first<<" "<<p.second<<"\n";
}
