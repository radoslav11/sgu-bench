#include <bits/stdc++.h>
using namespace std;

int main(){
    int K;
    cin >> K;
    if(K > 18){ cout << 0; return 0; }
    long long lo = 1;
    for(int i=1;i<K;i++) lo*=10;
    long long hi = lo*10;
    long long cnt=0;
    auto P=[](long long n)->long long{long long p=1;while(n>0){int d=n%10;if(d==0)return 0;p*=d;n/=10;}return p;};
    auto good=[&](long long n)->bool{long long p=P(n);return p!=0&&n%p==0;};
    for(long long n=lo;n<hi;n++) if(good(n)&&good(n+1)) cnt++;
    cout<<cnt;
}
