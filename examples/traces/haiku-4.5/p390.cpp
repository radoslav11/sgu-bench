#include<bits/stdc++.h>
using namespace std;

long long sumOfDigits(long long n) {
    long long sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

long long sumDigitsInRange(long long start, long long end) {
    long long sum = 0;
    for (long long i = start; i <= end; i++) {
        sum += sumOfDigits(i);
    }
    return sum;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long l, r, k;
    cin >> l >> r >> k;
    
    long long passengers = 0;
    long long current = l;
    
    while (current <= r) {
        long long left = current;
        long long right = r;
        long long endPos = current - 1;
        
        while (left <= right) {
            long long mid = left + (right - left) / 2;
            long long digitSum = sumDigitsInRange(current, mid);
            
            if (digitSum >= k) {
                endPos = mid;
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        
        if (endPos < current) break;
        
        passengers++;
        current = endPos + 1;
    }
    
    cout << passengers << endl;
    
    return 0;
}
