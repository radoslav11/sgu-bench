#include <bits/stdc++.h>
using namespace std;
int baseVal[]={64,32,16,8,4,2,1};
const char* baseStr[]={"1","2","4","8","16","32","64"};
struct Tok{string s;int v;};
vector<Tok> toks;
bool cmpConcat(const string&a,const string&b){return a+b<b+a;}
string assemble(vector<string> v){sort(v.begin(),v.end(),cmpConcat);string r;for(auto&x:v)r+=x;return r;}
int main(){
    for(int i=0;i<7;i++){
        int v=baseVal[i];string s=string("R")+baseStr[i];int cur=v;
        toks.push_back({s,v});
        int dots=0;int half=v;
        while(half>1){half/=2;dots++;cur+=half;toks.push_back({s+string(dots,'.'),cur});}
    }
    vector<long long> cost(64,LLONG_MAX);vector<vector<string>> bestT(64);
    cost[0]=0;bestT[0]={};
    bool ch=true;
    while(ch){ch=false;
        for(int v=0;v<64;v++){if(cost[v]==LLONG_MAX)continue;
            for(auto&t:toks){int nv=v+t.v;if(nv>=64)continue;
                long long nc=cost[v]+(long long)t.s.size();
                vector<string> cand=bestT[v];cand.push_back(t.s);
                if(nc<cost[nv]){cost[nv]=nc;bestT[nv]=cand;ch=true;}
                else if(nc==cost[nv]){string a=assemble(cand),b=assemble(bestT[nv]);if(a<b){bestT[nv]=cand;ch=true;}}
            }
        }
    }
    string in;getline(cin,in);
    long long total=0;int i=0;int n=in.size();
    while(i<n){
        if(in[i]!='R'){i++;continue;}
        i++;
        string num;while(i<n&&isdigit(in[i]))num+=in[i++];
        int idx=0;for(int k=0;k<7;k++)if(baseStr[k]==num)idx=k;
        int v=baseVal[idx];total+=v;int half=v;
        while(i<n&&in[i]=='.'){half/=2;total+=half;i++;}
    }
    long long W=total/64;long long r=total%64;
    vector<string> all;
    for(long long k=0;k<W;k++)all.push_back("R1");
    for(auto&s:bestT[r])all.push_back(s);
    cout<<assemble(all)<<"\n";
}
