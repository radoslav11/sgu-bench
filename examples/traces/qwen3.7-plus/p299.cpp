#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

string strip(string s) {
    int i = 0;
    while (i < (int)s.length() - 1 && s[i] == '0') i++;
    return s.substr(i);
}

bool comp(const string& a, const string& b) {
    if (a.length() != b.length()) return a.length() < b.length();
    return a < b;
}

string add(string a, string b) {
    if (a.length() < b.length()) swap(a, b);
    string res = "";
    int carry = 0;
    int n = a.length(), m = b.length();
    for (int i = 0; i < n; ++i) {
        int sum = carry + (a[n - 1 - i] - '0');
        if (i < m) sum += (b[m - 1 - i] - '0');
        res += (char)('0' + (sum % 10));
        carry = sum / 10;
    }
    if (carry) res += (char)('0' + carry);
    reverse(res.begin(), res.end());
    return res;
}

bool is_greater(const string& a, const string& b) {
    if (a.length() != b.length()) return a.length() > b.length();
    return a > b;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    if (!(cin >> n)) return 0;
    vector<string> s(n);
    for (int i = 0; i < n; ++i) {
        cin >> s[i];
        s[i] = strip(s[i]);
    }
    sort(s.begin(), s.end(), comp);
    for (int i = 0; i < n - 2; ++i) {
        string sum = add(s[i], s[i+1]);
        if (is_greater(sum, s[i+2])) {
            cout << s[i] << " " << s[i+1] << " " << s[i+2] << "\n";
            return 0;
        }
    }
    cout << "0 0 0\n";
    return 0;
}
