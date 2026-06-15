#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <cstring>
#include <algorithm>

using namespace std;

int main() {
    // Precompute all primes under 1000 (only odd primes >= 3)
    vector<bool> is_prime(1000, true);
    is_prime[0] = false;
    is_prime[1] = false;
    for (int i = 2; i * i < 1000; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j < 1000; j += i) {
                is_prime[j] = false;
            }
        }
    }
    
    map<int, string> precomputed;
    
    // Iterate over all odd primes from 3 to 999
    for (int p = 3; p < 1000; p++) {
        if (!is_prime[p] || p % 2 == 0) {
            continue;
        }
        
        vector<int> digits;
        digits.push_back(1); // start with 1
        
        int n = 2 * p;
        // Compute C(2p, p) = product_{i=1}^{p} (2p - i + 1) / i
        for (int i = 1; i <= p; i++) {
            int num1 = n - i + 1;
            // Multiply by num1
            int carry = 0;
            for (int j = 0; j < digits.size(); j++) {
                long long temp = (long long)digits[j] * num1 + carry;
                digits[j] = temp % 10;
                carry = temp / 10;
            }
            while (carry) {
                digits.push_back(carry % 10);
                carry /= 10;
            }
            
            // Divide by i
            int carry2 = 0;
            for (int j = digits.size() - 1; j >= 0; j--) {
                int temp = digits[j] + carry2 * 10;
                digits[j] = temp / i;
                carry2 = temp % i;
            }
            // Remove leading zeros (from the back)
            while (digits.size() > 1 && digits.back() == 0) {
                digits.pop_back();
            }
        }
        
        // Add 2*(p-1)
        int add_val = 2 * (p - 1);
        int carry3 = add_val;
        for (int j = 0; j < digits.size() && carry3; j++) {
            digits[j] += carry3;
            carry3 = digits[j] / 10;
            digits[j] %= 10;
        }
        while (carry3) {
            digits.push_back(carry3 % 10);
            carry3 /= 10;
        }
        
        // Divide by p
        int carry4 = 0;
        for (int j = digits.size() - 1; j >= 0; j--) {
            int temp = digits[j] + carry4 * 10;
            digits[j] = temp / p;
            carry4 = temp % p;
        }
        // Remove leading zeros
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
        
        // Convert to string
        string s = "";
        for (int i = digits.size() - 1; i >= 0; i--) {
            s += char(digits[i] + '0');
        }
        precomputed[p] = s;
    }
    
    int N;
    cin >> N;
    while (N--) {
        int p;
        cin >> p;
        cout << precomputed[p] << '\n';
    }
    
    return 0;
}
