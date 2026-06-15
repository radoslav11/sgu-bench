#include <iostream>
#include <vector>

using namespace std;

vector<int> multiply(const vector<int>& a, int b) {
    vector<int> res;
    int carry = 0;
    for (int digit : a) {
        int cur = digit * b + carry;
        res.push_back(cur % 10);
        carry = cur / 10;
    }
    while (carry > 0) {
        res.push_back(carry % 10);
        carry /= 10;
    }
    return res;
}

vector<int> power(int a, int b) {
    vector<int> res = {1};
    for (int i = 0; i < b; ++i) {
        res = multiply(res, a);
    }
    return res;
}

int cmp(const vector<int>& a, const vector<int>& b) {
    if (a.size() != b.size()) {
        return a.size() < b.size() ? -1 : 1;
    }
    for (int i = (int)a.size() - 1; i >= 0; --i) {
        if (a[i] != b[i]) {
            return a[i] < b[i] ? -1 : 1;
        }
    }
    return 0;
}

vector<int> subtract(vector<int> a, const vector<int>& b) {
    int carry = 0;
    vector<int> res;
    for (size_t i = 0; i < a.size(); ++i) {
        int diff = a[i] - carry;
        if (i < b.size()) {
            diff -= b[i];
        }
        if (diff < 0) {
            diff += 10;
            carry = 1;
        } else {
            carry = 0;
        }
        res.push_back(diff);
    }
    while (res.size() > 1 && res.back() == 0) {
        res.pop_back();
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int a, b;
    if (cin >> a >> b) {
        vector<int> a_pow_b = power(a, b);
        vector<int> b_pow_a = power(b, a);
        int c = cmp(a_pow_b, b_pow_a);
        
        if (c == 0) {
            cout << 0 << "\n";
        } else if (c == 1) {
            vector<int> res = subtract(a_pow_b, b_pow_a);
            for (int i = (int)res.size() - 1; i >= 0; --i) {
                cout << res[i];
            }
            cout << "\n";
        } else {
            cout << "-";
            vector<int> res = subtract(b_pow_a, a_pow_b);
            for (int i = (int)res.size() - 1; i >= 0; --i) {
                cout << res[i];
            }
            cout << "\n";
        }
    }
    return 0;
}
