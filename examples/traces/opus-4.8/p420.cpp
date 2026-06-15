#include <bits/stdc++.h>
using namespace std;

long long fact[20];
long long pw10[20];
long long L_a, L_b;
int Lg;
long long result;
int cntArr[10];

long long totalValid(int cnt[10], int L){
    long long denom=1;
    for(int i=0;i<10;i++) denom*=fact[cnt[i]];
    long long full = fact[L]/denom;
    long long lz=0;
    if(cnt[0]>0){
        lz = fact[L-1]*cnt[0]/denom;
    }
    return full-lz;
}

long long countLE(int cnt[10], long long value, int L){
    if(value<=0) return 0;
    int s[25]; int len=0; long long t=value;
    while(t>0){ s[len++]=(int)(t%10); t/=10; }
    if(len<L) return 0;
    if(len>L) return totalValid(cnt,L);
    int d[25];
    for(int i=0;i<L;i++) d[i]=s[L-1-i];
    int rem[10]; for(int i=0;i<10;i++) rem[i]=cnt[i];
    long long denom=1;
    for(int i=0;i<10;i++) denom*=fact[rem[i]];
    long long res=0;
    for(int i=0;i<L;i++){
        int len2=L-i;
        int start=(i==0?1:0);
        for(int dd=start; dd<d[i]; dd++){
            if(rem[dd]>0){
                res += fact[len2-1]*rem[dd]/denom;
            }
        }
        if(rem[d[i]]>0){
            denom/=rem[d[i]];
            rem[d[i]]--;
        } else {
            return res;
        }
    }
    res += 1;
    return res;
}

void process(){
    int cnt[10];
    for(int i=0;i<10;i++) cnt[i]=cntArr[i];
    // all zeros?
    if(cnt[0]==Lg) return;
    long long valid=totalValid(cnt,Lg);
    if(valid<=0) return;
    // minperm
    long long mn=0;
    {
        int tmp[10]; for(int i=0;i<10;i++) tmp[i]=cnt[i];
        int f=-1;
        for(int d=1;d<10;d++) if(tmp[d]>0){ f=d; break; }
        mn=f; tmp[f]--;
        for(int d=0;d<10;d++) for(int c=0;c<tmp[d];c++) mn=mn*10+d;
    }
    // maxperm
    long long mx=0;
    {
        for(int d=9;d>=0;d--) for(int c=0;c<cnt[d];c++) mx=mx*10+d;
    }
    if(mx < L_a || mn > L_b) return;
    long long cntIn;
    if(mn>=L_a && mx<=L_b){
        cntIn=valid;
    } else {
        cntIn = countLE(cnt, L_b, Lg) - countLE(cnt, L_a-1, Lg);
    }
    if(cntIn==2) result+=2;
}

void rec(int digit, int remaining){
    if(digit==9){
        cntArr[9]=remaining;
        process();
        return;
    }
    for(int c=0;c<=remaining;c++){
        cntArr[digit]=c;
        rec(digit+1, remaining-c);
    }
}

int main(){
    fact[0]=1;
    for(int i=1;i<20;i++) fact[i]=fact[i-1]*i;
    pw10[0]=1;
    for(int i=1;i<20;i++) pw10[i]=pw10[i-1]*10;
    long long l,r;
    cin>>l>>r;
    result=0;
    for(int L=1;L<=16;L++){
        long long low = pw10[L-1];
        long long high = pw10[L]-1;
        long long a=max(l,low);
        long long b=min(r,high);
        if(a>b) continue;
        L_a=a; L_b=b; Lg=L;
        rec(0,L);
    }
    cout<<result<<endl;
    return 0;
}
