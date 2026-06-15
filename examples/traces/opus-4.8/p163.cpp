#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,e;
    cin>>n>>e;
    long long sum=0;
    for(int i=0;i<n;i++){
        int x;cin>>x;
        long long v=1;
        for(int j=0;j<e;j++)v*=x;
        if(v>0)sum+=v;
    }
    cout<<sum<<endl;
}
