#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

string dfs(string s, int count, int L, int n, set<string>& curSet) {
    if (static_cast<int>(s.size()) == L) {
        if (count == n) {
            return s;
        }
        return "";
    }

    for (char c = 'a'; c <= 'z'; c++) {
        vector<string> newSubs;
        for (int i = 0; i <= static_cast<int>(s.size()); i++) {
            string candidate = s.substr(i) + c;
            if (curSet.find(candidate) == curSet.end()) {
                newSubs.push_back(candidate);
            }
        }
        int add = newSubs.size();
        int new_count = count + add;
        if (new_count > n) {
            continue;
        }

        int k = L - static_cast<int>(s.size()) - 1;
        if (k > 0) {
            long long max_add = static_cast<long long>(k) * (2 * static_cast<int>(s.size()) + 1 + k) / 2;
            if (new_count + max_add < n) {
                continue;
            }
        }

        for (string& sub : newSubs) {
            curSet.insert(sub);
        }

        string res;
        if (k == 0) {
            if (new_count == n) {
                res = s + c;
            } else {
                res = "";
            }
        } else {
            res = dfs(s + c, new_count, L, n, curSet);
        }

        for (string& sub : newSubs) {
            curSet.erase(sub);
        }

        if (res != "") {
            return res;
        }
    }
    return "";
}

int main() {
    int n;
    cin >> n;

    for (int L = 1; ; L++) {
        set<string> curSet;
        string res = dfs("", 0, L, n, curSet);
        if (res != "") {
            cout << res << endl;
            return 0;
        }
    }
}
