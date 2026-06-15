#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> spf(n+1, 0);
    for(int i = 2; i <= n; i++){
        if(spf[i] == 0){
            for(int j = i; j <= n; j += i){
                if(spf[j] == 0) spf[j] = i;
            }
        }
    }
    
    vector<int> omega(n+1, 0);
    for(int i = 2; i <= n; i++){
        omega[i] = omega[i/spf[i]] + 1;
    }
    
    int M = *max_element(omega.begin(), omega.end()) + 1;
    
    cout << M << "\n";
    for(int i = 1; i <= n; i++){
        if(i > 1) cout << " ";
        cout << omega[i]+1;
    }
    cout << "\n";
    
    return 0;
}
