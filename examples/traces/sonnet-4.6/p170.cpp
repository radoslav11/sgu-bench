#include <bits/stdc++.h>
using namespace std;

int main() {
    string s1, s2;
    cin >> s1 >> s2;
    
    vector<int> plus1, minus1, plus2, minus2;
    for (int i = 0; i < (int)s1.size(); i++) {
        if (s1[i] == '+') plus1.push_back(i);
        else minus1.push_back(i);
    }
    for (int i = 0; i < (int)s2.size(); i++) {
        if (s2[i] == '+') plus2.push_back(i);
        else minus2.push_back(i);
    }
    
    if (plus1.size() != plus2.size() || minus1.size() != minus2.size()) {
        cout << -1 << endl;
        return 0;
    }
    
    long long ans = 0;
    for (int i = 0; i < (int)plus1.size(); i++) {
        ans += abs(plus1[i] - plus2[i]);
    }
    
    cout << ans << endl;
    return 0;
}
