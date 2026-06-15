#include<bits/stdc++.h>
using namespace std;

long long countTrailingZeros(long long n) {
    long long count = 0;
    long long power = 5;
    while (power <= n) {
        count += n / power;
        if (power > n / 5) break;
        power *= 5;
    }
    return count;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long Q;
    cin >> Q;
    
    if (Q == 0) {
        cout << 0 << endl;
        return 0;
    }
    
    long long left = 1, right = 8000000000LL;
    long long result = -1;
    
    while (left <= right) {
        long long mid = left + (right - left) / 2;
        long long zeros = countTrailingZeros(mid);
        
        if (zeros >= Q) {
            if (zeros == Q) result = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    
    if (result == -1) {
        cout << "No solution" << endl;
    } else {
        cout << result << endl;
    }
    
    return 0;
}
