#include <bits/stdc++.h>
using namespace std;
typedef vector<int> BN; // base 1e9, little endian
const long long BASE=1000000000LL;
BN fromInt(long long x){BN r;while(x){r.push_back(x%BASE);x/=BASE;}return r;}
void trim(BN&a){while(a.size()&&a.back()==0)a.pop_back();}
int cmp(const BN&a,const BN&b){if(a.size()!=b.size())return a.size()<b.size()?-1:1;for(int i=a.size()-1;i>=0;i--)if(a[i]!=b[i])return a[i]<b[i]?-1:1;return 0;}
BN add(const BN&a,const BN&b){BN r;long long c=0;for(size_t i=0;i<max(a.size(),b.size())||c;i++){long long s=c;if(i<a.size())s+=a[i];if(i<b.size())s+=b[i];r.push_back(s%BASE);c=s/BASE;}trim(r);return r;}
BN sub(const BN&a,const BN&b){BN r;long long c=0;for(size_t i=0;i<a.size();i++){long long s=(long long)a[i]-c-(i<b.size()?b[i]:0);if(s<0){s+=BASE;c=1;}else c=0;r.push_back(s);}trim(r);return r;}
BN mul(const BN&a,const BN&b){if(a.empty()||b.empty())return BN();vector<long long>t(a.size()+b.size(),0);for(size_t i=0;i<a.size();i++){long long c=0;for(size_t j=0;j<b.size()||c;j++){long long cur=t[i+j]+c+(j<b.size()?(long long)a[i]*b[j]:0);t[i+j]=cur%BASE;c=cur/BASE;}}BN r(t.begin(),t.end());trim(r);return r;}
BN mod(BN a,const BN&m){if(cmp(a,m)<0)return a;
  // long division
  BN cur;BN res;
  // process from top: use simple schoolbook with binary search per digit
  BN result;
  // do digit by digit (base BASE) division
  BN rem;
  for(int i=a.size()-1;i>=0;i--){
    rem.insert(rem.begin(),a[i]);trim(rem);
    int lo=0,hi=BASE-1,q=0;
    while(lo<=hi){int mid=(lo+hi)/2;BN t=mul(m,fromInt(mid));if(cmp(t,rem)<=0){q=mid;lo=mid+1;}else hi=mid-1;}
    rem=sub(rem,mul(m,fromInt(q)));
  }
  return rem;
}
BN mulmod(const BN&a,const BN&b,const BN&m){return mod(mul(a,b),m);}
BN powmod(BN base,BN e,const BN&m){BN r=fromInt(1);base=mod(base,m);while(!e.empty()){if(e[0]&1)r=mulmod(r,base,m);base=mulmod(base,base,m);
  // e/=2
  long long c=0;for(int i=e.size()-1;i>=0;i--){long long cur=c*BASE+e[i];e[i]=cur/2;c=cur%2;}trim(e);}
  return r;}
int main(){int b,n;cin>>b>>n;
  int bb=b;map<int,int>fac;for(int p=2;p*p<=bb;p++)while(bb%p==0){fac[p]++;bb/=p;}if(bb>1)fac[bb]++;
  vector<BN>m;for(auto&pr:fac){BN pk=fromInt(1),pe=fromInt(pr.first);BN ex=fromInt((long long)n*pr.second);pk=powmod(fromInt(pr.first),ex,fromInt(LLONG_MAX/2)); // not modded properly
    // need actual p^(n*e) without mod -> compute directly
    pk=fromInt(1);long long cnt=(long long)n*pr.second;for(long long i=0;i<cnt;i++)pk=mul(pk,fromInt(pr.first));m.push_back(pk);}
  BN total=fromInt(1);for(auto&x:m)total=mul(total,x);
  int r=m.size();vector<BN>e(r);
  for(int i=0;i<r;i++){BN Mi=fromInt(1);for(int j=0;j<r;j++)if(j!=i)Mi=mul(Mi,m[j]);
    // inverse of Mi mod m[i] via euler: phi(p^a)=p^(a-1)(p-1)
    int p=0;{int idx=0;for(auto&pr:fac){if(idx==i){p=pr.first;}idx++;}}
    BN phi=sub(m[i],div_helper:fromInt(0)); // placeholder
    // phi = m[i] - m[i]/p
    // m[i]/p:
    BN mip=mod(m[i],fromInt(p)); // not needed
    BN quo;{BN rem;BN a=m[i];BN res;BN remm;for(int k=a.size()-1;k>=0;k--){remm.insert(remm.begin(),a[k]);trim(remm);int lo=0,hi=BASE-1,q=0;BN pp=fromInt(p);while(lo<=hi){int mid=(lo+hi)/2;if(cmp(mul(pp,fromInt(mid)),remm)<=0){q=mid;lo=mid+1;}else hi=mid-1;}remm=sub(remm,mul(pp,fromInt(q)));res.insert(res.begin(),q);}trim(res);quo=res;}
    phi=sub(m[i],quo);
    BN exp=sub(phi,fromInt(1));
    BN inv=powmod(mod(Mi,m[i]),exp,m[i]);
    e[i]=mulmod(Mi,inv,total);
  }
  vector<string>ans;
  string digs="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  // threshold b^(n-1)
  BN thr=fromInt(1);for(int i=0;i<n-1;i++)thr=mul(thr,fromInt(b));
  for(int mask=0;mask<(1<<r);mask++){BN x;for(int i=0;i<r;i++)if(mask&(1<<i))x=add(x,e[i]);x=mod(x,total);
    if(cmp(x,thr)<0)continue;
    // convert to base b
    string s;BN cur=x;if(cur.empty())s="0";else{while(!cur.empty()){int lo=0,hi=BASE-1,q=0;BN bb2=fromInt(b);BN remm;BN res;BN a=cur;for(int k=a.size()-1;k>=0;k--){remm.insert(remm.begin(),a[k]);trim(remm);int L=0,H=BASE-1,Q=0;while(L<=H){int mid=(L+H)/2;if(cmp(mul(bb2,fromInt(mid)),remm)<=0){Q=mid;L=mid+1;}else H=mid-1;}remm=sub(remm,mul(bb2,fromInt(Q)));res.insert(res.begin(),Q);}trim(res);int dig=(remm.empty()?0:remm[0]);s+=digs[dig];cur=res;}}
    while((int)s.size()<n)s+='0';reverse(s.begin(),s.end());ans.push_back(s);}
  cout<<ans.size()<<"\n";for(auto&s:ans)cout<<s<<"\n";
}
