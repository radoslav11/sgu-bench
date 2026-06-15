#include <iostream>
#include <vector>
#include <cstdio>
using namespace std;

const uint32_t base = 1000000;

void multiply(vector<uint32_t> &a, int b) {
    long long carry = 0;
    for (int i = 0; i < a.size(); i++) {
        long long temp = (long long)a[i] * b + carry;
        a[i] = temp % base;
        carry = temp / base;
    }
    while (carry) {
        a.push_back(carry % base);
        carry /= base;
    }
}

void divide(vector<uint32_t> &a, int b) {
    long long remainder = 0;
    for (int i = (int)a.size() - 1; i >= 0; i--) {
        long long temp = (long long)a[i] + remainder * base;
        a[i] = temp / b;
        remainder = temp % b;
    }
    while (a.size() > 1 && a.back() == 0) {
        a.pop_back();
    }
}

void add(vector<uint32_t> &a, const vector<uint32_t> &b) {
    int i = 0;
    long long carry = 0;
    while (i < a.size() || i < b.size() || carry) {
        long long temp = carry;
        if (i < a.size()) temp += a[i];
        if (i < b.size()) temp += b[i];
        carry = temp / base;
        if (i < a.size()) {
            a[i] = temp % base;
        } else {
            a.push_back(temp % base);
        }
        i++;
    }
}

void print(const vector<uint32_t> &a) {
    if (a.empty()) {
        cout << 0 << endl;
        return;
    }
    printf("%u", a.back());
    for (int i = (int)a.size() - 2; i >= 0; i--) {
        printf("%06u", a[i]);
    }
    cout << endl;
}

int main() {
    int n;
    cin >> n;

    vector<uint32_t> fact;
    fact.push_back(1);
    for (int i = 2; i <= n-1; i++) {
        multiply(fact, i);
    }

    vector<uint32_t> total = fact;
    vector<uint32_t> term = fact;

    for (int j = 1; j <= n-3; j++) {
        multiply(term, n);
        divide(term, j);
        add(total, term);
    }

    divide(total, 2);
    print(total);

    return 0;
}
