#include <bits/stdc++.h>
using namespace std;
typedef vector<long long> Big;
const long long BASE = 1000000000LL;

Big fromLL(long long x) {
    if (x == 0) return {0};
    Big r;
    while (x) { r.push_back(x % BASE); x /= BASE; }
    return r;
}

void trim(Big& a) {
    while (a.size() > 1 && !a.back()) a.pop_back();
}

Big mulB(Big a, long long b) {
    long long c = 0;
    for (auto& x : a) {
        long long t = x * b + c;
        x = t % BASE;
        c = t / BASE;
    }
    if (c) a.push_back(c);
    trim(a);
    return a;
}

Big addB(Big a, const Big& b) {
    if (a.size() < b.size()) a.resize(b.size(), 0);
    long long c = 0;
    for (int i = 0; i < (int)b.size() || c; i++) {
        if (i == (int)a.size()) a.push_back(0);
        long long t = a[i] + (i < (int)b.size() ? b[i] : 0) + c;
        a[i] = t % BASE;
        c = t / BASE;
    }
    trim(a);
    return a;
}

int cmpB(const Big& a, const Big& b) {
    if (a.size() != b.size()) return (int)a.size() - (int)b.size();
    for (int i = (int)a.size() - 1; i >= 0; i--)
        if (a[i] != b[i]) return a[i] > b[i] ? 1 : -1;
    return 0;
}

Big subB(Big a, const Big& b) {
    long long c = 0;
    for (int i = 0; i < (int)b.size() || c; i++) {
        long long t = a[i] - (i < (int)b.size() ? b[i] : 0) - c;
        if (t < 0) { t += BASE; c = 1; } else c = 0;
        a[i] = t;
    }
    trim(a);
    return a;
}

bool isZero(const Big& a) { return a.size() == 1 && a[0] == 0; }

pair<Big, Big> divmod(const Big& a, const Big& b) {
    if (cmpB(a, b) < 0) return {fromLL(0), a};
    Big q, r = fromLL(0);
    q.resize(a.size(), 0);
    for (int i = (int)a.size() - 1; i >= 0; i--) {
        r = mulB(r, BASE);
        r = addB(r, fromLL(a[i]));
        long long lo = 0, hi = BASE - 1;
        while (lo < hi) {
            long long mid = (lo + hi + 1) / 2;
            if (cmpB(mulB(b, mid), r) <= 0) lo = mid; else hi = mid - 1;
        }
        q[i] = lo;
        r = subB(r, mulB(b, lo));
    }
    trim(q); trim(r);
    return {q, r};
}

Big gcdB(Big a, Big b) {
    while (!isZero(b)) {
        auto [q, r] = divmod(a, b);
        a = b; b = r;
    }
    return a;
}

string toString(const Big& a) {
    string s = to_string(a.back());
    for (int i = (int)a.size() - 2; i >= 0; i--) {
        string t = to_string(a[i]);
        s += string(9 - t.size(), '0') + t;
    }
    return s;
}

int main() {
    int K, N;
    cin >> K >> N;
    int m = N - K;
    
    vector<Big> D(max(m + 1, 2));
    D[0] = fromLL(1);
    D[1] = fromLL(0);
    for (int i = 2; i <= m; i++) {
        D[i] = mulB(addB(D[i-1], D[i-2]), (long long)(i-1));
    }
    
    if (isZero(D[m])) { cout << 0; return 0; }
    
    Big num = D[m];
    Big den = fromLL(1);
    for (int i = 1; i <= K; i++) den = mulB(den, (long long)i);
    for (int i = 1; i <= m; i++) den = mulB(den, (long long)i);
    
    Big g = gcdB(num, den);
    auto [na, ra] = divmod(num, g);
    auto [da, rb] = divmod(den, g);
    
    if (cmpB(da, fromLL(1)) == 0) {
        cout << toString(na);
    } else {
        cout << toString(na) << "/" << toString(da);
    }
    return 0;
}
