#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    if(!(cin>>n)) return 0;
    if(n==1){
        cout<<"1 0\n";
        return 0;
    }
    if(n==2){
        cout<<"2 1\n";
        cout<<"1 2\n";
        return 0;
    }
    if(n==3){
        cout<<"3 3\n";
        cout<<"1 2\n";
        cout<<"2 3\n";
        cout<<"1 3\n";
        return 0;
    }
    // n >= 4 : K(2, n-2), hubs 1 and 2, leaves 3..n
    int V=n;
    int E=2*(n-2);
    cout<<V<<" "<<E<<"\n";
    for(int i=3;i<=n;i++){
        cout<<1<<" "<<i<<"\n";
        cout<<2<<" "<<i<<"\n";
    }
    return 0;
}
