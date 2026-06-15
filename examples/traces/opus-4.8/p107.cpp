#include <bits/stdc++.h>
using namespace std;
int main(){
    long long N; cin>>N;
    long long M=1000000000LL, T=987654321LL;
    long long M2=512, M5=1953125;
    vector<long long> s2,s5;
    for(long long x=0;x<M2;x++) if(x*x%M2==T%M2) s2.push_back(x);
    for(long long x=0;x<M5;x++) if(x*x%M5==T%M5) s5.push_back(x);
    // CRT
    vector<long long> sols;
    long long inv2=0; // inverse of M5 mod M2 etc. Use formula
    // x = a (mod M2), x = b (mod M5)
    // x = a + M2*t, M2*t ≡ b-a (mod M5)
    long long invM2 = [&](){ long long g=1; for(long long i=0;i<M5;i++) if(M2%M5*i%M5==1){return i;} return 0LL;}();
    // better compute inverse properly
    // inverse of M2 mod M5
    {
        long long a=M2%M5,m=M5,x0=1,x1=0,a0=a,m0=m;
        // extended euclid
        long long old_r=a,r=m,old_s=1,s=0;
        while(r){long long q=old_r/r;long long t=old_r-q*r;old_r=r;r=t;t=old_s-q*s;old_s=s;s=t;}
        invM2=((old_s%M5)+M5)%M5;
    }
    for(long long a:s2)for(long long b:s5){
        long long t=( (b-a)%M5 +M5)%M5 * invM2 %M5;
        long long x=a+M2*t;
        sols.push_back(x%M);
    }
    sort(sols.begin(),sols.end());
    // count N-digit numbers x in [10^(N-1),10^N-1] with x%M in sols
    // Use big integer: answer = sum over sols of count in range
    // Range as big number. Implement via per-residue count.
    // L=10^(N-1), R=10^N-1
    // count of n in [L,R], n≡r (mod M) = floor((R-r)/M)-floor((L-1-r)/M)
    // Need bignum. Represent numbers as decimal strings; division by M.
    auto floordiv=[&](string num, bool neg)->string{
        // num/M floored, num given as decimal string (nonneg), if neg means value is -num
        // we handle: compute q = floor(num/M)
        // long division
        string q; long long rem=0;
        for(char c:num){ rem=rem*10+(c-'0'); long long d=rem/M; rem%=M; q+=(char)('0'+d);} 
        // strip leading zeros
        int i=0;while(i+1<(int)q.size()&&q[i]=='0')i++; q=q.substr(i);
        if(neg){ // floor(-num/M) = -ceil(num/M) = -( (num+M-1)/M )
        }
        return q;
    };
    // simpler: compute count using big arithmetic of (R - r) etc but r small, L,R huge.
    // floor((R-r)/M): R = 10^N -1. R-r decimal. 
    // I'll build strings for R-r and L-1-r and divide.
    auto subSmall=[&](string num,long long s)->string{
        long long borrow=s; for(int i=num.size()-1;i>=0&&borrow;i--){long long d=(num[i]-'0')-borrow%10;borrow/=10;if(d<0){d+=10;borrow++;}num[i]='0'+d;} int i=0;while(i+1<(int)num.size()&&num[i]=='0')i++;return num.substr(i);};
    string Rstr(N,'9'); // 10^N-1 has N nines
    string Lm1; // 10^(N-1)-1 = N-1 nines (for N>=1); if N==1 it's 0... 10^0=1, L-1=0
    if(N==1) Lm1="0"; else Lm1=string(N-1,'9');
    // bignum add
    auto add=[&](string a,string b)->string{ if(a.size()<b.size())swap(a,b); int carry=0; for(int i=0;i<(int)a.size();i++){int d=a[a.size()-1-i]-'0'+carry; if(i<(int)b.size())d+=b[b.size()-1-i]-'0'; carry=d/10; a[a.size()-1-i]='0'+d%10;} if(carry)a=string(1,'0'+carry)+a; return a;};
    auto divM=[&](string num)->string{ string q;long long rem=0;for(char c:num){rem=rem*10+(c-'0');q+=(char)('0'+rem/M);rem%=M;}int i=0;while(i+1<(int)q.size()&&q[i]=='0')i++;return q.substr(i);};
    string ans="0";
    for(long long r:sols){
        string A=divM(subSmall(Rstr,r));
        string B=divM(subSmall(Lm1,r));
        // ans += A - B  (A>=B always)
        // subtract B from A
        // do A-B
        string a=A,b=B; string diff; int borrow=0; int n=a.size();
        for(int i=0;i<n;i++){int d=a[n-1-i]-'0'-borrow-(i<(int)b.size()?b[b.size()-1-i]-'0':0); if(d<0){d+=10;borrow=1;}else borrow=0; diff+=('0'+d);} reverse(diff.begin(),diff.end()); int i=0;while(i+1<(int)diff.size()&&diff[i]=='0')i++; diff=diff.substr(i);
        ans=add(ans,diff);
    }
    cout<<ans<<endl;
}
