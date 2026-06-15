#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
bool isAlmostLucky(ll n){
    string s=to_string(n);
    int len=s.size();
    if(len%2!=0)return false;
    int N=len/2;
    int sl=0,sr=0;
    for(int i=0;i<N;i++)sl+=s[i]-'0';
    for(int i=N;i<len;i++)sr+=s[i]-'0';
    int diff=sl-sr;
    for(int i=0;i<len;i++){
        int d=s[i]-'0';
        int need;
        if(i<N)need=d-diff;
        else need=d+diff;
        if(need<0||need>9||need==d)continue;
        if(i==0&&need==0)continue;
        return true;
    }
    return false;
}
int main(){
    ll A,B;
    cin>>A>>B;
    ll cnt=0;
    for(ll i=A;i<=B;i++)if(isAlmostLucky(i))cnt++;
    cout<<cnt<<endl;
}
