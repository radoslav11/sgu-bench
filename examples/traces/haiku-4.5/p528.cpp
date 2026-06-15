#include <bits/stdc++.h>
using namespace std;

long long m;
string c;
int pos;
long long charCount;

bool canParseString(int& p, long long& cnt);
bool canParseInt(int& p, long long& cnt);
bool canParseList(int& p, long long& cnt);
bool canParseDict(int& p, long long& cnt);
bool canParseValue(int& p, long long& cnt);

bool canParseString(int& p, long long& cnt) {
    if (p >= c.length() || !isdigit(c[p])) return false;
    
    long long len = 0;
    int start = p;
    while (p < c.length() && isdigit(c[p])) {
        len = len * 10 + (c[p] - '0');
        p++;
    }
    
    if (p >= c.length() || c[p] != ':') {
        p = start;
        return false;
    }
    cnt += (p - start) + 1; // digits + colon
    p++;
    
    if (p + len > c.length()) {
        cnt += c.length() - p;
        return false;
    }
    
    cnt += len;
    p += len;
    return cnt <= m;
}

bool canParseInt(int& p, long long& cnt) {
    if (p >= c.length() || c[p] != 'i') return false;
    
    int start = p;
    cnt++; // 'i'
    p++;
    
    if (p >= c.length()) return false;
    
    if (c[p] == '-') {
        p++;
        return false; // negative numbers not allowed
    }
    
    if (!isdigit(c[p])) return false;
    
    while (p < c.length() && isdigit(c[p])) {
        cnt++;
        p++;
    }
    
    if (p >= c.length() || c[p] != 'e') {
        return false;
    }
    
    cnt++; // 'e'
    p++;
    return cnt <= m;
}

bool canParseList(int& p, long long& cnt) {
    if (p >= c.length() || c[p] != 'l') return false;
    
    cnt++; // 'l'
    p++;
    
    while (p < c.length() && c[p] != 'e' && cnt <= m) {
        if (!canParseValue(p, cnt)) return false;
    }
    
    if (p >= c.length() || c[p] != 'e') return false;
    
    cnt++; // 'e'
    p++;
    return cnt <= m;
}

bool canParseDict(int& p, long long& cnt) {
    if (p >= c.length() || c[p] != 'd') return false;
    
    cnt++; // 'd'
    p++;
    
    while (p < c.length() && c[p] != 'e' && cnt <= m) {
        if (!canParseString(p, cnt)) return false;
        if (!canParseValue(p, cnt)) return false;
    }
    
    if (p >= c.length() || c[p] != 'e') return false;
    
    cnt++; // 'e'
    p++;
    return cnt <= m;
}

bool canParseValue(int& p, long long& cnt) {
    if (p >= c.length()) return false;
    
    if (isdigit(c[p])) return canParseString(p, cnt);
    if (c[p] == 'i') return canParseInt(p, cnt);
    if (c[p] == 'l') return canParseList(p, cnt);
    if (c[p] == 'd') return canParseDict(p, cnt);
    
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> m >> c;
    
    int p = 0;
    long long cnt = 0;
    
    if (canParseValue(p, cnt) && p == c.length()) {
        cout << "ok\n";
    } else {
        cout << "Error at position " << p << "!\n";
    }
    
    return 0;
}
