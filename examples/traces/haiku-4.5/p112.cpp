#include<bits/stdc++.h>
using namespace std;

typedef vector<int> BigInt;

// Multiply a big integer by a small integer
BigInt multiply(BigInt a, int b) {
    BigInt result = a;
    int carry = 0;
    for(int i = 0; i < result.size(); i++) {
        long long prod = (long long)result[i] * b + carry;
        result[i] = prod % 10;
        carry = prod / 10;
    }
    while(carry) {
        result.push_back(carry % 10);
        carry /= 10;
    }
    return result;
}

// Compute base^exp using big integer arithmetic
BigInt power(int base, int exp) {
    BigInt result;
    result.push_back(1);
    for(int i = 0; i < exp; i++) {
        result = multiply(result, base);
    }
    return result;
}

// Compare two big integers (returns true if a > b)
bool greater(BigInt a, BigInt b) {
    if(a.size() != b.size()) return a.size() > b.size();
    for(int i = a.size() - 1; i >= 0; i--) {
        if(a[i] != b[i]) return a[i] > b[i];
    }
    return false;
}

// Subtract b from a (assuming a >= b)
BigInt subtract(BigInt a, BigInt b) {
    BigInt result = a;
    int borrow = 0;
    for(int i = 0; i < b.size(); i++) {
        int diff = result[i] - b[i] - borrow;
        if(diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result[i] = diff;
    }
    while(borrow && !result.empty()) {
        result[result.size()-1]--;
        borrow = 0;
    }
    while(result.size() > 1 && result.back() == 0) result.pop_back();
    return result;
}

void print(BigInt a) {
    for(int i = a.size()-1; i >= 0; i--) cout << a[i];
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int a, b;
    cin >> a >> b;
    
    BigInt ab = power(a, b);
    BigInt ba = power(b, a);
    
    if(greater(ab, ba)) {
        print(subtract(ab, ba));
    } else if(greater(ba, ab)) {
        cout << "-";
        print(subtract(ba, ab));
    } else {
        cout << "0";
    }
    cout << "\n";
    
    return 0;
}
