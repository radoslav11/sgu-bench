#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,m;cin>>n>>m;
    long long total=0;
    for(int i=0;i<m;i++){long long s;cin>>s;total+=s%n;}
    cout<<total%n<<endl;
}
