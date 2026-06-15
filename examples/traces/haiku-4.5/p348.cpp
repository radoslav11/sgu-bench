#include<bits/stdc++.h>
using namespace std;

set<long long> computeW(long long p) {
    set<long long> result;
    
    // Find number of bits in p
    int bits = 0;
    long long temp = p;
    while(temp > 0) {
        bits++;
        temp >>= 1;
    }
    
    // Generate all cyclic shifts
    long long mask = (1LL << bits) - 1;
    long long shifted = p;
    for(int i = 0; i < bits; i++) {
        result.insert(shifted);
        // Cyclic shift: move MSB to LSB
        long long msb = (shifted >> (bits - 1)) & 1;
        shifted = ((shifted << 1) & mask) | msb;
    }
    
    return result;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long n;
    cin >> n;
    
    set<long long> unionSet;
    long long p = 0;
    
    // Keep increasing p until union contains all numbers 1 to n
    while(unionSet.size() < n || *unionSet.rbegin() < n) {
        p++;
        set<long long> wp = computeW(p);
        for(auto x : wp) {
            unionSet.insert(x);
        }
        
        // Check if we have all numbers 1 to n
        bool complete = true;
        for(long long i = 1; i <= n; i++) {
            if(unionSet.find(i) == unionSet.end()) {
                complete = false;
                break;
            }
        }
        
        if(complete) {
            cout << p << endl;
            return 0;
        }
    }
    
    return 0;
}
