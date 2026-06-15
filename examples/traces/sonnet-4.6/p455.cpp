#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long A, B, C;
    cin >> A >> B >> C;
    
    unordered_map<long long, int> seen;
    seen.reserve(1 << 22);
    seen.max_load_factor(0.25);
    
    long long x = 1;
    int limit = 2000000;
    
    for(int i = 0; i <= limit; i++){
        if(seen.count(x)){
            cout << i << endl;
            return 0;
        }
        seen[x] = i;
        x = (A * x + B) % C;
    }
    
    cout << -1 << endl;
    return 0;
}
