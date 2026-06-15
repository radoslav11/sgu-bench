#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,m;
    cin>>n>>m;
    vector<string> phrases(m);
    for(int i=0;i<m;i++) cin>>phrases[i];
    cout<<phrases[(n-1)%m]<<endl;
    return 0;
}
