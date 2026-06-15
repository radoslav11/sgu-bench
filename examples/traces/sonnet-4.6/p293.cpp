#include<bits/stdc++.h>
using namespace std;
int main(){
    string lab; int n;
    while(cin>>lab>>n){
        string s; cin>>s;
        int len=2*n-1;
        vector<pair<int,int>> moves;
        bool ok=true;
        for(int pos=1;pos<=len;pos++){
            if(pos%2==1){
                int m=pos;
                vector<pair<int,int>> mis;
                for(int i=0;i<m/2;i++) if(s[i]!=s[m-1-i]) mis.push_back({i,m-1-i});
                if(mis.size()==0) moves.push_back({0,0});
                else if(mis.size()==2){moves.push_back({mis[0].first+1,mis[0].second+1});swap(s[mis[0].first],s[mis[0].second]);}
                else{ok=false;break;}
            } else moves.push_back({0,0});
        }
        cout<<lab<<"\n";
        if(!ok) cout<<"He\n";
        else{cout<<"Qc\n";for(auto&p:moves)cout<<p.first<<" "<<p.second<<"\n";}
    }
}
