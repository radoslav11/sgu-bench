#include<iostream>
using namespace std;
int gcd(int a,int b){return b==0?a:gcd(b,a%b);}
int main(){
    int n;cin>>n;
    int cnt=0;
    for(int i=1;i<=n;i++)if(gcd(n,i)==1)cnt++;
    cout<<cnt<<endl;
}
