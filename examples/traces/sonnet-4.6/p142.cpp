#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    string s; cin>>s;
    vector<int> b(n);
    for(int i=0;i<n;i++) b[i]=(s[i]=='b')?1:0;
    for(int L=1;L<=21;L++){
        int total=1<<L;
        vector<bool> present(total,false);
        if(L<=20){
            int mask=total-1,cur=0;
            for(int i=0;i<n;i++){
                cur=((cur<<1)|b[i])&mask;
                if(i>=L-1) present[cur]=true;
            }
            for(int x=0;x<total;x++){
                if(!present[x]){
                    cout<<L<<"\n";
                    string res="";
                    for(int i=L-1;i>=0;i--) res+=((x>>i)&1)?'b':'a';
                    cout<<res<<"\n";
                    return 0;
                }
            }
        } else {
            unordered_set<string> ps;
            for(int i=0;i<=n-L&&i>=0;i++) ps.insert(s.substr(i,L));
            for(int x=0;x<total;x++){
                string t="";
                for(int i=L-1;i>=0;i--) t+=((x>>i)&1)?'b':'a';
                if(!ps.count(t)){cout<<L<<"\n"<<t<<"\n";return 0;}
            }
        }
    }
}
