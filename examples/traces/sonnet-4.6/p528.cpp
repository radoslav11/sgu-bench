#include <bits/stdc++.h>
using namespace std;

long long m;
string s;
int n;
int best;

bool parse(int &pos);

bool parse(int &pos) {
    if (pos >= n) return false;
    char c = s[pos];
    if (c == 'i') {
        pos++;
        best = pos;
        if (pos >= n) return false;
        if (!isdigit(s[pos])) return false;
        int start = pos;
        while (pos < n && isdigit(s[pos])) pos++;
        if (pos - start > 1 && s[start] == '0') return false;
        best = pos;
        if (pos >= n) return false;
        if (s[pos] != 'e') return false;
        pos++;
        return true;
    } else if (c == 'l' || c == 'd') {
        pos++;
        best = pos;
        while (pos < n && s[pos] != 'e') {
            if (!parse(pos)) return false;
            best = pos;
        }
        if (pos >= n) return false;
        pos++;
        return true;
    } else if (isdigit(c)) {
        int start = pos;
        while (pos < n && isdigit(s[pos])) pos++;
        long long len = 0;
        bool overflow = false;
        for (int i = start; i < pos; i++) { len = len*10+(s[i]-'0'); if(len>1e15) overflow=true; }
        if (pos >= n || s[pos] != ':') { best = pos; return false; }
        pos++;
        best = pos;
        if (overflow || len > m) return false;
        if (pos + len > n) { pos = n; return false; }
        pos += (int)len;
        return true;
    }
    return false;
}

int main(){
    cin >> m >> s;
    n = s.size();
    best = 0;
    int pos = 0;
    bool ok = parse(pos);
    if (ok && pos == n && n <= m) { cout << "ok\n"; }
    else { cout << "Error at position " << best << "!\n"; }
}
