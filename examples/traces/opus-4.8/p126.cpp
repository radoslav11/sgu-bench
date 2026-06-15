#include <bits/stdc++.h>
using namespace std;
int main(){
    long long A,B;
    cin>>A>>B;
    long long cnt=0;
    while(A!=0 && B!=0){
        long long small=min(A,B), large=max(A,B);
        if(large<small){cnt=-1;break;}
        long long nl=large-small;
        long long ns=small*2;
        if(A<B){A=ns;B=nl;} else {B=ns;A=nl;}
        cnt++;
        if(cnt>100){cnt=-1;break;}
    }
    cout<<cnt<<endl;
}
