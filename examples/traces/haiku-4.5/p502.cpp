#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string n;
    cin >> n;
    
    // Sort digits to start with smallest permutation
    sort(n.begin(), n.end());
    
    // Try all permutations
    do {
        // Skip if starts with 0
        if(n[0] == '0') continue;
        
        // Check if divisible by 17
        long long num = 0;
        for(char c : n) {
            num = (num * 10 + (c - '0')) % 17;
        }
        
        if(num == 0) {
            cout << n << endl;
            return 0;
        }
    } while(next_permutation(n.begin(), n.end()));
    
    cout << -1 << endl;
    return 0;
}
