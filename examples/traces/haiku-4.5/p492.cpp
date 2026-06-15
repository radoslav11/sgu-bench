#include<bits/stdc++.h>
using namespace std;

long long findNth(long long n) {
    vector<int> digits;
    long long temp = n;
    
    // Convert to base-9 (1-indexed becomes 0-indexed)
    n--;
    while(n > 0) {
        digits.push_back(n % 9);
        n /= 9;
    }
    if(digits.empty()) digits.push_back(0);
    
    reverse(digits.begin(), digits.end());
    
    // Map base-9 digits to valid decimal digits, skipping 3
    // Map: 0→0, 1→1, 2→2, 3→4, 4→5, 5→6, 6→7, 7→8, 8→9
    long long result = 0;
    for(int d : digits) {
        result = result * 10 + (d < 3 ? d : d + 1);
    }
    
    return result;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while(t--) {
        long long n;
        cin >> n;
        cout << findNth(n) << "\n";
    }
    
    return 0;
}
