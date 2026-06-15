#include <bits/stdc++.h>
using namespace std;
typedef vector<long long> Big;
const long long BASE = 1000000000LL;
Big fromString(const string& s){Big res;int i=s.size();while(i>0){int st=max(0,i-9);res.push_back(stoll(s.substr(st,i-st)));i=st;}return res;}
string toString(const Big& a){if(a.empty())return "0";string res=to_string(a.back());for(int i=(int)a.size()-2;i>=0;i--){string p=to_string(a[i]);res+=string(9-p.size(),'0')+p;}return res;}
int cmp(const Big& a,const Big& b){if(a.size()!=b.size())return a.size()<b.size()?-1:1;for(int i=(int)a.size()-1;i>=0;i--)if(a[i]!=b[i])return a[i]<b[i]?-1:1;return 0;}
Big add(const Big& a,const Big& b){Big res;long long carry=0;for(int i=0;i<(int)max(a.size(),b.size())||carry;i++){long long cur=carry;if(i<(int)a.size())cur+=a[i];if(i<(int)b.size())cur+=b[i];res.push_back(cur%BASE);carry=cur/BASE;}return res;}
Big divBy2(const Big& a){Big res=a;long long carry=0;for(int i=(int)res.size()-1;i>=0;i--){long long cur=res[i]+carry*BASE;res[i]=cur/2;carry=cur%2;}while(res.size()>1&&res.back()==0)res.pop_back();return res;}
Big mul(const Big& a,const Big& b){Big res(a.size()+b.size(),0);for(int i=0;i<(int)a.size();i++){long long carry=0;for(int j=0;j<(int)b.size()||carry;j++){__int128 cur=(__int128)res[i+j]+carry;if(j<(int)b.size())cur+=(__int128)a[i]*b[j];res[i+j]=cur%BASE;carry=cur/BASE;}}while(res.size()>1&&res.back()==0)res.pop_back();return res;}
int main(){string s;cin>>s;Big X=fromString(s);Big lo=fromString("1"),hi=fromString("1"+string(502,'0'));while(true){Big mid=divBy2(add(lo,hi));if(cmp(mid,lo)==0)break;Big sq=mul(mid,mid);if(cmp(sq,X)<=0)lo=mid;else hi=mid;}cout<<toString(lo)<<endl;}
