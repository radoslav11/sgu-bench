#include <bits/stdc++.h>
using namespace std;
int main(){
    int k; cin>>k;
    // Catalan number C(k)
    unsigned long long cat=1;
    for(int i=0;i<k;i++){
        cat = cat*(2*(unsigned long long)k - i)/(i+1);
    }
    cout<<cat<<" "<<(k+1)<<"\n";
}
