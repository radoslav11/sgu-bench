#include <bits/stdc++.h>
using namespace std;

int main(){
    int k;
    cin >> k;
    
    __int128 cat = 1;
    for(int n = 1; n <= k; n++){
        cat = cat * 2 * (2*n - 1) / (n + 1);
    }
    
    long long P = k + 1;
    long long N = (long long)cat;
    
    cout << N << " " << P << endl;
    
    return 0;
}
