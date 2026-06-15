#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    int N=n*n;
    // cells 1..N
    vector<int> alive;
    for(int i=1;i<=N;i++) alive.push_back(i);
    int k=n;
    // while more than one alive, remove
    // group by parity of (r+c)
    ...
}
