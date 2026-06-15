#include <bits/stdc++.h>
using namespace std;
int main(){
    long long n; cin>>n;
    for(long long k=1;14*k+2<=n;k++){
        long long s=n-14*k;
        if(s>=2&&s<=12){cout<<k<<endl;return 0;}
    }
    cout<<-1<<endl;
    return 0;
}
