#include <bits/stdc++.h>
using namespace std;

int main(){
    int k;
    cin >> k;
    
    // k-th Catalan number: C_k = (2k choose k) / (k+1)
    long long catalan = 1;
    for(int i = 0; i < k; i++){
        catalan = catalan * (2*k - i) / (i + 1);
    }
    
    long long P = k + 1;
    
    cout << catalan << " " << P << endl;
    
    return 0;
}
