#include <bits/stdc++.h>
using namespace std;

typedef vector<int> Big; // base 1e9, little endian
const long long BASE = 1000000000LL;

Big fromInt(long long x){
    Big r;
    if(x==0){r.push_back(0);return r;}
    while(x){r.push_back(x%BASE);x/=BASE;}
    return r;
}

Big add(const Big&a,const Big&b){
    Big r;
    long long carry=0;
    for(size_t i=0;i<a.size()||i<b.size()||carry;i++){
        long long s=carry;
        if(i<a.size())s+=a[i];
        if(i<b.size())s+=b[i];
        r.push_back(s%BASE);
        carry=s/BASE;
    }
    return r;
}

// a - b, assume a>=b
Big sub(const Big&a,const Big&b){
    Big r;
    long long borrow=0;
    for(size_t i=0;i<a.size();i++){
        long long s=a[i]-borrow-(i<b.size()?b[i]:0);
        if(s<0){s+=BASE;borrow=1;}else borrow=0;
        r.push_back(s);
    }
    while(r.size()>1&&r.back()==0)r.pop_back();
    return r;
}

Big pow2(int n){
    Big r=fromInt(1);
    for(int i=0;i<n;i++){
        // multiply by 2
        long long carry=0;
        for(size_t j=0;j<r.size();j++){
            long long v=(long long)r[j]*2+carry;
            r[j]=v%BASE;
            carry=v/BASE;
        }
        if(carry)r.push_back(carry);
    }
    return r;
}

void printBig(const Big&a){
    printf("%d",a.back());
    for(int i=(int)a.size()-2;i>=0;i--){
        printf("%09d",a[i]);
    }
    printf("\n");
}

int main(){
    int N,M;
    scanf("%d %d",&N,&M);
    Big res=add(pow2(N),pow2(M));
    res=sub(res,fromInt(2));
    printBig(res);
    return 0;
}
