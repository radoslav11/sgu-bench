#include<bits/stdc++.h>
using namespace std;

bool isAlmostLucky(long long num) {
    string s = to_string(num);
    int len = s.length();
    
    // Must have even number of digits
    if (len % 2 != 0) return false;
    
    int n = len / 2;
    
    // Calculate sums
    int leftSum = 0, rightSum = 0;
    for (int i = 0; i < n; i++) {
        leftSum += s[i] - '0';
    }
    for (int i = n; i < len; i++) {
        rightSum += s[i] - '0';
    }
    
    // Try changing each digit
    for (int i = 0; i < len; i++) {
        int oldDigit = s[i] - '0';
        
        // Try all possible new digits
        for (int newDigit = 0; newDigit <= 9; newDigit++) {
            // Skip if no change
            if (newDigit == oldDigit) continue;
            
            // Skip leading zeros
            if (i == 0 && newDigit == 0) continue;
            
            int newLeftSum = leftSum;
            int newRightSum = rightSum;
            
            if (i < n) {
                // Changing left half
                newLeftSum = leftSum - oldDigit + newDigit;
            } else {
                // Changing right half
                newRightSum = rightSum - oldDigit + newDigit;
            }
            
            if (newLeftSum == newRightSum) {
                return true;
            }
        }
    }
    
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long A, B;
    cin >> A >> B;
    
    long long count = 0;
    
    for (long long i = A; i <= B; i++) {
        if (isAlmostLucky(i)) {
            count++;
        }
    }
    
    cout << count << endl;
    
    return 0;
}
