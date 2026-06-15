#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    for(int cas=1;cas<=n;cas++){
        string s; cin>>s;
        vector<char> chars; map<char,int> idx;
        for(char c:s) if(!idx.count(c)){idx[c]=chars.size();chars.push_back(c);}
        int k=chars.size(),len=s.size();
        vector<long long> coeff(k,0); long long pw=1;
        for(int i=len-1;i>=0;i--){coeff[idx[s[i]]]+=pw;pw*=10;}
        int fi=idx[s[0]]; long long g=0;
        vector<int> chosen(k); vector<bool> used(10,false);
        function<void(int)> gen=[&](int d){
            if(d==k){if(chosen[fi]==0)return;long long num=0;for(int i=0;i<k;i++)num+=coeff[i]*chosen[i];g=__gcd(g,num);return;}
            for(int x=0;x<=9;x++)if(!used[x]){used[x]=true;chosen[d]=x;gen(d+1);used[x]=false;}
        };
        gen(0);
        vector<long long> divs;
        for(long long d=1;d*d<=g;d++)if(g%d==0){divs.push_back(d);if(d!=g/d)divs.push_back(g/d);}
        sort(divs.begin(),divs.end());
        cout<<"Case "<<cas<<":";for(long long d:divs)cout<<" "<<d;cout<<"\n";
    }
}
