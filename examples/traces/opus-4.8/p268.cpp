#include <bits/stdc++.h>
using namespace std;
int N;
bool almost(const string&w){for(int i=0;i<(int)w.size();i++){vector<int>cnt(256,0);bool ok=true;for(int j=0;j<(int)w.size();j++)if(j!=i){int c=w[j]-'A';if(c<0||c>=N){ok=false;break;}cnt[w[j]]++;}if(!ok)continue;bool good=true;for(int k=0;k<N;k++)if(cnt['A'+k]!=1){good=false;break;}if(good)return true;}return false;}
bool hyper(const string&s){for(int i=0;i+N+1<=(int)s.size();i++)if(!almost(s.substr(i,N+1)))return false;return true;}
int main(){
    string s1,s2;cin>>N>>s1>>s2;
    string best;bool has=false;
    auto consider=[&](const string&a,const string&b){
        for(int ov=N;ov>=0;ov--){
            if(a.substr(a.size()-ov)!=b.substr(0,ov)) continue;
            string cand=a+b.substr(ov);
            if(cand.find(s1)!=string::npos&&cand.find(s2)!=string::npos&&hyper(cand)){
                if(!has||cand.size()<best.size()){best=cand;has=true;}
            }
        }
    };
    consider(s1,s2);consider(s2,s1);
    cout<<best<<"\n";
}
