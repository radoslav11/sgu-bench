#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
int main(){
    ll A,B;
    cin>>A>>B;
    if(A==0||B==0){cout<<0;return 0;}
    ll g=__gcd(A,B);
    ll r=(A+B)/g;
    if(r&(r-1)){cout<<-1;return 0;}
    ll a=A/g,b=B/g,cnt=0;
    while(a&&b){
        if(a>b)swap(a,b);
        b-=a;a*=2;cnt++;
    }
    cout<<cnt;
}
