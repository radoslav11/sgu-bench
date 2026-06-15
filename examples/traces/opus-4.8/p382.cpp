#include <bits/stdc++.h>
using namespace std;
typedef vector<long long> B;
const long long BASE=1000000000LL;
B norm(B a){while(a.size()>1&&a.back()==0)a.pop_back();return a;}
B fromll(long long x){B r;if(x==0){r.push_back(0);return r;}while(x){r.push_back(x%BASE);x/=BASE;}return r;}
B add(const B&a,const B&b){B r;long long c=0;for(size_t i=0;i<max(a.size(),b.size())||c;i++){long long s=c;if(i<a.size())s+=a[i];if(i<b.size())s+=b[i];r.push_back(s%BASE);c=s/BASE;}return norm(r);}
int cmp(const B&a,const B&b){if(a.size()!=b.size())return a.size()<b.size()?-1:1;for(int i=a.size()-1;i>=0;i--)if(a[i]!=b[i])return a[i]<b[i]?-1:1;return 0;}
B sub(const B&a,const B&b){B r;long long c=0;for(size_t i=0;i<a.size();i++){long long s=a[i]-c-(i<b.size()?b[i]:0);if(s<0){s+=BASE;c=1;}else c=0;r.push_back(s);}return norm(r);}
B mul(const B&a,const B&b){vector<long long>res(a.size()+b.size(),0);for(size_t i=0;i<a.size();i++){long long c=0;for(size_t j=0;j<b.size()||c;j++){long long cur=res[i+j]+c+a[i]*(j<b.size()?b[j]:0);res[i+j]=cur%BASE;c=cur/BASE;}}B r(res.begin(),res.end());return norm(r);}
bool isZero(const B&a){return a.size()==1&&a[0]==0;}
pair<B,B> divmod(const B&a,const B&b){
    B cur=fromll(0),q;q.assign(a.size(),0);
    for(int i=(int)a.size()-1;i>=0;i--){
        cur.insert(cur.begin(),a[i]);cur=norm(cur);
        long long lo=0,hi=BASE-1,x=0;
        while(lo<=hi){long long m=(lo+hi)/2;if(cmp(mul(b,fromll(m)),cur)<=0){x=m;lo=m+1;}else hi=m-1;}
        q[i]=x;cur=sub(cur,mul(b,fromll(x)));
    }
    return {norm(q),cur};
}
B gcd(B a,B b){while(!isZero(b)){B r=divmod(a,b).second;a=b;b=r;}return a;}
string tostr(const B&a){string s=to_string(a.back());for(int i=(int)a.size()-2;i>=0;i--){string t=to_string(a[i]);s+=string(9-t.size(),'0')+t;}return s;}
int main(){
    int n;cin>>n;
    vector<B> num(n+1),den(n+1);
    static long long C[51][51];
    for(int i=0;i<=50;i++){C[i][0]=1;for(int j=1;j<=i;j++)C[i][j]=(j==i)?1:C[i-1][j-1]+C[i-1][j];}
    num[0]=fromll(1);den[0]=fromll(1);
    for(int k=1;k<=n;k++){
        B rn=fromll(1),rd=fromll(1);
        for(int j=0;j<k;j++){
            B cn=mul(fromll(C[k][j]),num[j]);
            B cd=den[j];
            B nn=add(mul(rn,cd),mul(cn,rd));
            B dd=mul(rd,cd);
            B g=gcd(nn,dd);if(!isZero(g)){nn=divmod(nn,g).first;dd=divmod(dd,g).first;}
            rn=nn;rd=dd;
        }
        long long coef=0;{long long p=1;for(int i=0;i<k;i++)p*=2;coef=3*p-2;}
        B nn=rn, dd=mul(rd,fromll(coef));
        B g=gcd(nn,dd);if(!isZero(g)){nn=divmod(nn,g).first;dd=divmod(dd,g).first;}
        num[k]=nn;den[k]=dd;
    }
    cout<<tostr(num[n])<<"/"<<tostr(den[n])<<"\n";
}
