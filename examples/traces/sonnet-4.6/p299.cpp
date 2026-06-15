#include <bits/stdc++.h>
using namespace std;
int cmp(const string& a, const string& b) {
    if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
    return a < b ? -1 : (a > b ? 1 : 0);
}
string add(const string& a, const string& b) {
    string result;
    int carry = 0;
    int i = a.size() - 1, j = b.size() - 1;
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        result.push_back('0' + sum % 10);
        carry = sum / 10;
    }
    reverse(result.begin(), result.end());
    return result.empty() ? "0" : result;
}
int main(){
    int n; cin >> n;
    vector<string> segs(n);
    for (int i = 0; i < n; i++) {
        cin >> segs[i];
        int start = 0;
        while (start < (int)segs[i].size() - 1 && segs[i][start] == '0') start++;
        segs[i] = segs[i].substr(start);
    }
    sort(segs.begin(), segs.end(), [](const string& a, const string& b){ return cmp(a,b) < 0; });
    for (int i = 0; i + 2 < n; i++) {
        string sum = add(segs[i], segs[i+1]);
        if (cmp(sum, segs[i+2]) > 0) {
            cout << segs[i] << " " << segs[i+1] << " " << segs[i+2] << endl;
            return 0;
        }
    }
    cout << "0 0 0" << endl;
}
