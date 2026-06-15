#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

// Convert string to reversed digit vector
vector<int> stringToDigits(const string& s) {
    vector<int> digits;
    for (int i = s.size() - 1; i >= 0; i--) {
        digits.push_back(s[i] - '0');
    }
    return digits;
}

// Convert digit vector to string
string digitsToString(const vector<int>& digits) {
    string s;
    // Remove leading zeros
    int i = digits.size() - 1;
    while (i > 0 && digits[i] == 0) i--;
    for (; i >= 0; i--) {
        s += char(digits[i] + '0');
    }
    return s;
}

// Multiply big integer (vector<int> in reverse order) by int
vector<int> multiply(const vector<int>& a, int b) {
    vector<int> result;
    int carry = 0;
    for (int i = 0; i < a.size(); i++) {
        int prod = a[i] * b + carry;
        result.push_back(prod % 10);
        carry = prod / 10;
    }
    while (carry > 0) {
        result.push_back(carry % 10);
        carry /= 10;
    }
    return result;
}

// Subtract two positive big integers (a - b), assume a and b are in reverse digit order
// Returns negative if result is negative, represented as negative string or handle separately
string subtract(const vector<int>& a, const vector<int>& b) {
    // Compare a and b
    if (a.size() < b.size()) return "-" + subtract(b, a);
    if (a.size() > b.size()) {
        // Proceed with subtraction
    } else {
        // Same length, compare lexicographically from highest digit
        for (int i = a.size() - 1; i >= 0; i--) {
            if (a[i] < b[i]) return "-" + subtract(b, a);
            if (a[i] > b[i]) break;
        }
    }
    
    // Now compute a - b (a >= b)
    vector<int> result;
    int borrow = 0;
    for (int i = 0; i < a.size(); i++) {
        int diff = a[i] - borrow;
        if (i < b.size()) diff -= b[i];
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.push_back(diff);
    }
    
    // Remove trailing zeros (in reversed representation, leading zeros at the end)
    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }
    
    return digitsToString(result);
}

// Compute base^exp
vector<int> pow(int base, int exp) {
    if (exp == 0) return {1};
    vector<int> result = {1};
    for (int i = 0; i < exp; i++) {
        result = multiply(result, base);
    }
    return result;
}

int main() {
    int a, b;
    cin >> a >> b;
    
    vector<int> ab = pow(a, b);
    vector<int> ba = pow(b, a);
    
    string result = subtract(ab, ba);
    cout << result << endl;
    
    return 0;
}
