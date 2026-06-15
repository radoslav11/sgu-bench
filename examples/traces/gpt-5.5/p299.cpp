#include <bits/stdc++.h>
using namespace std;

string normalize(string s) {
    size_t p = s.find_first_not_of('0');
    if (p == string::npos) return "0";
    return s.substr(p);
}

int cmpNum(const string& a, const string& b) {
    if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
    if (a == b) return 0;
    return a < b ? -1 : 1;
}

string addNum(const string& a, const string& b) {
    string res;
    int i = (int)a.size() - 1, j = (int)b.size() - 1, carry = 0;
    
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        res.push_back(char('0' + sum % 10));
        carry = sum / 10;
    }
    
    reverse(res.begin(), res.end());
    return res;
}

bool sumGreater(const string& a, const string& b, const string& c) {
    string s = addNum(a, b);
    return cmpNum(s, c) > 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<string> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        a[i] = normalize(a[i]);
    }

    sort(a.begin(), a.end(), [](const string& x, const string& y) {
        return cmpNum(x, y) < 0;
    });

    for (int i = 0; i + 2 < n; ++i) {
        if (sumGreater(a[i], a[i + 1], a[i + 2])) {
            cout << a[i] << ' ' << a[i + 1] << ' ' << a[i + 2] << '\n';
            return 0;
        }
    }

    cout << "0 0 0\n";
    return 0;
}
