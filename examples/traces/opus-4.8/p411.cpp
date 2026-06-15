#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;
string s1,s2;int n1,n2;
vector<ull> h1,p,h2;
ull H(vector<ull>&h,int l,int r){return h[r+1]-h[l]*p[r+1-l];}
void build(string&s,vector<ull>&h){int n=s.size();h.assign(n+1,0);for(int i=0;i<n;i++)h[i+1]=h[i]*131+s[i];}
bool isPal(int l,int r){while(l<r){if(s1[l]!=s1[r])return false;l++;r--;}return true;}
int bestPos;
bool check(int L){
    if(L<1||L>n2||L>n1)return false;
    unordered_set<ull> st;st.reserve(n2*2);
    for(int i=0;i+L<=n2;i++)st.insert(H(h2,i,i+L-1));
    for(int i=0;i+L<=n1;i++) if(isPal(i,i+L-1)) if(st.count(H(h1,i,i+L-1))){bestPos=i;return true;}
    return false;
}
int main(){
    cin>>s1>>s2;n1=s1.size();n2=s2.size();
    int n=max(n1,n2)+2;
    p.assign(n+1,1);for(int i=1;i<=n;i++)p[i]=p[i-1]*131;
    build(s1,h1);build(s2,h2);
    int bL=0,bP=0;
    for(int par=0;par<2;par++){
        int lo=1,hi=min(n1,n2),res=-1,rp=0;
        while(lo<=hi){int mid=(lo+hi)/2;int L=(mid%2==par)?mid:mid-1;if(L<1){lo=mid+1;continue;}if(check(L)){res=L;rp=bestPos;lo=L+2;}else hi=L-1;}
        if(res>bL){bL=res;bP=rp;}
    }
    string ans="";
    if(bL>0)ans=s1.substr(bP,bL);
    cout<<ans<<"\n";
    return 0;
}
