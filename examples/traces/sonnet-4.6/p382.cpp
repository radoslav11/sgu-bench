#include<bits/stdc++.h>
using namespace std;
typedef vector<long long> Big;
const long long BASE=1000000000LL;

void trim(Big&a){while(a.size()>1&&a.back()==0)a.pop_back();}

Big fromLL(long long x){
    Big r;
    if(x==0){r.push_back(0);return r;}
    while(x>0){r.push_back(x%BASE);x/=BASE;}
    return r;
}

Big add(const Big&a,const Big&b){
    Big r;long long c=0;
    for(int i=0;i<(int)max(a.size(),b.size())||c;i++){
        long long s=c;
        if(i<(int)a.size())s+=a[i];
        if(i<(int)b.size())s+=b[i];
        r.push_back(s%BASE);c=s/BASE;
    }
    return r;
}

int cmp(const Big&a,const Big&b){
    if(a.size()!=b.size())return a.size()<b.size()?-1:1;
    for(int i=a.size()-1;i>=0;i--)if(a[i]!=b[i])return a[i]<b[i]?-1:1;
    return 0;
}

Big sub(const Big&a,const Big&b){
    Big r=a;long long c=0;
    for(int i=0;i<(int)r.size();i++){
        long long s=r[i]-c-(i<(int)b.size()?b[i]:0);
        if(s<0){s+=BASE;c=1;}else c=0;
        r[i]=s;
    }
    trim(r);return r;
}

Big mul(const Big&a,const Big&b){
    Big r(a.size()+b.size(),0);
    for(int i=0;i<(int)a.size();i++){
        long long c=0;
        for(int j=0;j<(int)b.size();j++){
            long long cur=r[i+j]+a[i]*b[j]+c;
            r[i+j]=cur%BASE;c=cur/BASE;
        }
        if(c)r[i+b.size()]+=c;
    }
    trim(r);return r;
}

Big mulL(const Big&a,long long x){
    Big r;long long c=0;
    for(int i=0;i<(int)a.size()||c;i++){
        long long cur=(i<(int)a.size()?a[i]:0)*x+c;
        r.push_back(cur%BASE);c=cur/BASE;
    }
    trim(r);return r;
}

pair<Big,Big> divmod(const Big&a,const Big&b){
    Big q(a.size(),0),r=fromLL(0);
    for(int i=(int)a.size()-1;i>=0;i--){
        r=mulL(r,BASE);
        r=add(r,fromLL(a[i]));
        long long lo=0,hi=BASE-1;
        while(lo<hi){
            long long mid=(lo+hi+1)/2;
            if(cmp(mulL(b,mid),r)<=0)lo=mid;else hi=mid-1;
        }
        q[i]=lo;
        r=sub(r,mulL(b,lo));
    }
    trim(q);return{q,r};
}

Big gcd(Big a,Big b){
    while(!(b.size()==1&&b[0]==0)){
        auto[q,r]=divmod(a,b);
        a=b;b=r;
    }
    return a;
}

string str(const Big&a){
    string s=to_string(a.back());
    for(int i=(int)a.size()-2;i>=0;i--){
        string t=to_string(a[i]);
        s+=string(9-t.size(),'0')+t;
    }
    return s;
}

struct Frac{Big p,q;};

Frac red(Frac f){
    Big g=gcd(f.p,f.q);
    f.p=divmod(f.p,g).first;
    f.q=divmod(f.q,g).first;
    return f;
}

Frac addF(Frac a,Frac b){return red({add(mul(a.p,b.q),mul(b.p,a.q)),mul(a.q,b.q)});}
Frac mulF(Frac a,Frac b){return red({mul(a.p,b.p),mul(a.q,b.q)});}
Frac divF(Frac a,Frac b){return red({mul(a.p,b.q),mul(a.q,b.p)});}

int main(){
    int n;cin>>n;
    vector<Frac> I(n+1);
    I[0]={fromLL(1),fromLL(1)};
    
    vector<vector<Big>> C(n+1,vector<Big>(n+1,fromLL(0)));
    for(int i=0;i<=n;i++){
        C[i][0]=fromLL(1);
        for(int j=1;j<=i;j++)C[i][j]=add(C[i-1][j-1],C[i-1][j]);
    }
    
    for(int m=1;m<=n;m++){
        Frac s={fromLL(1),fromLL(1)};
        for(int k=0;k<m;k++)
            s=addF(s,mulF({C[m][k],fromLL(1)},I[k]));
        // denominator: 3*2^m - 2
        // for m<=50, 3*2^50 fits in long long
        long long den=3LL;
        for(int i=0;i<m;i++)den*=2;
        den-=2;
        I[m]=divF(s,{fromLL(den),fromLL(1)});
    }
    
    cout<<str(I[n].p)<<"/"<<str(I[n].q)<<"\n";
}
