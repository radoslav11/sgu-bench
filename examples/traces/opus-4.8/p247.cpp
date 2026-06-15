#include <bits/stdc++.h>
using namespace std;

typedef vector<long long> Big; // base 1e9, little endian
const long long BASE = 1000000000LL;

void mulSmall(Big &a, long long m){
    long long carry=0;
    for(size_t i=0;i<a.size();i++){
        long long cur = a[i]*m + carry;
        a[i] = cur % BASE;
        carry = cur / BASE;
    }
    while(carry){
        a.push_back(carry%BASE);
        carry/=BASE;
    }
    while(a.size()>1 && a.back()==0) a.pop_back();
}

void divSmall(Big &a, long long d){
    long long rem=0;
    for(int i=(int)a.size()-1;i>=0;i--){
        long long cur = rem*BASE + a[i];
        a[i] = cur / d;
        rem = cur % d;
    }
    while(a.size()>1 && a.back()==0) a.pop_back();
}

void subSmall(Big &a, long long s){
    // assume a>=s
    long long borrow=s;
    for(size_t i=0;i<a.size() && borrow;i++){
        long long cur = a[i]-borrow;
        if(cur<0){
            cur+=BASE;
            borrow=1;
        } else borrow=0;
        a[i]=cur;
    }
    while(a.size()>1 && a.back()==0) a.pop_back();
}

void addSmall(Big &a, long long s){
    long long carry=s;
    for(size_t i=0;i<a.size() && carry;i++){
        long long cur=a[i]+carry;
        a[i]=cur%BASE;
        carry=cur/BASE;
    }
    while(carry){
        a.push_back(carry%BASE);
        carry/=BASE;
    }
}

string toStr(Big &a){
    string s = to_string(a.back());
    for(int i=(int)a.size()-2;i>=0;i--){
        string part = to_string(a[i]);
        while(part.size()<9) part="0"+part;
        s+=part;
    }
    return s;
}

int main(){
    int n;
    if(!(cin>>n)) return 0;
    while(n--){
        long long P;
        cin>>P;
        // C(2P,P) = product_{i=1}^{P} (P+i)/i
        Big res; res.push_back(1);
        for(long long i=1;i<=P;i++){
            mulSmall(res, P+i);
            divSmall(res, i);
        }
        subSmall(res,2);
        divSmall(res,P);
        addSmall(res,2);
        cout<<toStr(res)<<"\n";
    }
    return 0;
}
