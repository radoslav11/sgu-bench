#include<bits/stdc++.h>
using namespace std;

string s;
int n;
map<pair<int,int>, pair<bool,string>> memo;

pair<bool,string> solve(int l, int r) {
    if (l > r) {
        return {true, ""};
    }
    if (l == r) {
        if (s[l] == '*') return {true, "a"};
        if (s[l] == '?') return {true, "a"};
        if (s[l] == '!') return {true, "aaa"};
        return {true, string(1, s[l])};
    }
    
    auto key = make_pair(l, r);
    if (memo.count(key)) return memo[key];
    
    pair<bool,string> res = {false, ""};
    
    char lc = s[l], rc = s[r];
    
    // Both are regular characters
    if (isalpha(lc) && isalpha(rc)) {
        if (lc == rc) {
            auto inner = solve(l+1, r-1);
            if (inner.first) {
                res = {true, string(1,lc) + inner.second + string(1,rc)};
            }
        }
        return memo[key] = res;
    }
    
    // Left is *, right is *
    if (lc == '*' && rc == '*') {
        auto inner = solve(l+1, r-1);
        if (inner.first) {
            res = {true, inner.second};
        }
        return memo[key] = res;
    }
    
    // Left is *, right is regular or special (non-*)
    if (lc == '*') {
        // Try matching * with 0 chars
        auto inner = solve(l+1, r);
        if (inner.first) {
            res = {true, inner.second};
        }
        return memo[key] = res;
    }
    
    // Right is *, left is not
    if (rc == '*') {
        // Try matching * with 0 chars
        auto inner = solve(l, r-1);
        if (inner.first) {
            res = {true, inner.second};
        }
        return memo[key] = res;
    }
    
    // Left is ?, right is ?
    if (lc == '?' && rc == '?') {
        auto inner = solve(l+1, r-1);
        if (inner.first) {
            res = {true, string(1,'a') + inner.second + string(1,'a')};
        }
        return memo[key] = res;
    }
    
    // Left is !, right is !
    if (lc == '!' && rc == '!') {
        auto inner = solve(l+1, r-1);
        if (inner.first) {
            res = {true, "aaa" + inner.second + "aaa"};
        }
        return memo[key] = res;
    }
    
    // Mixed cases - try to match with 'a'
    if ((lc == '?' || lc == '*') && isalpha(rc)) {
        auto inner = solve(l+1, r-1);
        if (inner.first) {
            res = {true, string(1,rc) + inner.second + string(1,rc)};
        }
        return memo[key] = res;
    }
    
    if (isalpha(lc) && (rc == '?' || rc == '*')) {
        auto inner = solve(l+1, r-1);
        if (inner.first) {
            res = {true, string(1,lc) + inner.second + string(1,lc)};
        }
        return memo[key] = res;
    }
    
    return memo[key] = res;
}

int main(){
    cin >> s;
    n = s.length();
    
    auto result = solve(0, n-1);
    
    if (result.first) {
        cout << "YES\n" << result.second << "\n";
    } else {
        cout << "NO\n";
    }
    
    return 0;
}
