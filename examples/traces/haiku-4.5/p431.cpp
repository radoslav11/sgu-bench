#include<bits/stdc++.h>
using namespace std;

bool matches(const string& pattern, const string& filename) {
    int p = 0, f = 0;
    int p_star = -1, f_star = -1;
    
    while (f < (int)filename.length()) {
        if (p < (int)pattern.length() && (pattern[p] == '?' || pattern[p] == filename[f])) {
            p++;
            f++;
        } else if (p < (int)pattern.length() && pattern[p] == '*') {
            p_star = p;
            f_star = f;
            p++;
        } else if (p_star != -1) {
            p = p_star + 1;
            f_star++;
            f = f_star;
        } else {
            return false;
        }
    }
    
    while (p < (int)pattern.length() && pattern[p] == '*') {
        p++;
    }
    
    return p == (int)pattern.length();
}

bool isValid(const string& pattern, const vector<string>& yes, const vector<string>& no) {
    for (const auto& s : yes) {
        if (!matches(pattern, s)) return false;
    }
    for (const auto& s : no) {
        if (matches(pattern, s)) return false;
    }
    return true;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<string> yes(n), no(m);
    for (int i = 0; i < n; i++) cin >> yes[i];
    for (int i = 0; i < m; i++) cin >> no[i];
    
    // Try patterns based on positive examples
    // Try each positive example with wildcards added
    for (const auto& base : yes) {
        // Try the base itself
        if (isValid(base, yes, no)) {
            cout << base << endl;
            return 0;
        }
        
        // Try with * at start and end
        if (isValid("*" + base + "*", yes, no)) {
            cout << "*" + base + "*" << endl;
            return 0;
        }
        
        // Try with * at start
        if (isValid("*" + base, yes, no)) {
            cout << "*" + base << endl;
            return 0;
        }
        
        // Try with * at end
        if (isValid(base + "*", yes, no)) {
            cout << base + "*" << endl;
            return 0;
        }
        
        // Try replacing characters with ?
        string pattern = base;
        for (int i = 0; i < (int)pattern.length(); i++) {
            char orig = pattern[i];
            pattern[i] = '?';
            if (isValid(pattern, yes, no)) {
                cout << pattern << endl;
                return 0;
            }
            pattern[i] = orig;
        }
    }
    
    // Try some other common patterns
    if (isValid("*", yes, no)) {
        cout << "*" << endl;
        return 0;
    }
    
    cout << "OOPS" << endl;
    return 0;
}
