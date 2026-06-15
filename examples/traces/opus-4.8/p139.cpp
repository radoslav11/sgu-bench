#include <bits/stdc++.h>
using namespace std;
int main(){
    int a[16];
    int zeroRow=0;
    for(int i=0;i<16;i++){
        cin>>a[i];
        if(a[i]==0) zeroRow=i/4;
    }
    int inv=0;
    vector<int> v;
    for(int i=0;i<16;i++) if(a[i]!=0) v.push_back(a[i]);
    for(int i=0;i<v.size();i++)
        for(int j=i+1;j<v.size();j++)
            if(v[i]>v[j]) inv++;
    // blank on row counted from bottom
    int rowFromBottom=4-zeroRow;
    // solvable if inv+rowFromBottom is odd (for target with blank bottom-right)
    if((inv+rowFromBottom)%2==1) cout<<"YES\n";
    else cout<<"NO\n";
    return 0;
}
