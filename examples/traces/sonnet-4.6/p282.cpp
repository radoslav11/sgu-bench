#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef vector<ll> Big;
const ll BASE = 1000000000LL;

Big fromLL(ll v) {
    Big r;
    if (v == 0) { r.push_back(0); return r; }
    while (v > 0) { r.push_back(v % BASE); v /= BASE; }
    return r;
}

Big add(const Big& a, const Big& b) {
    Big r;
    ll c = 0;
    for (int i = 0; i < (int)max(a.size(), b.size()) || c; i++) {
        ll s = c + (i < (int)a.size() ? a[i] : 0) + (i < (int)b.size() ? b[i] : 0);
        r.push_back(s % BASE);
        c = s / BASE;
    }
    return r;
}

Big mulLL(const Big& a, ll v) {
    if (v == 0) return fromLL(0);
    Big r;
    ll c = 0;
    for (int i = 0; i < (int)a.size() || c; i++) {
        ll s = c + (i < (int)a.size() ? a[i] * v : 0);
        r.push_back(s % BASE);
        c = s / BASE;
    }
    while (r.size() > 1 && r.back() == 0) r.pop_back();
    return r;
}

Big mulBig(const Big& a, const Big& b) {
    Big r(a.size() + b.size(), 0);
    for (int i = 0; i < (int)a.size(); i++) {
        ll c = 0;
        for (int j = 0; j < (int)b.size() || c; j++) {
            __int128 cur = (__int128)r[i + j] + c;
            if (j < (int)b.size()) cur += (__int128)a[i] * b[j];
            r[i + j] = (ll)(cur % BASE);
            c = (ll)(cur / BASE);
        }
    }
    while (r.size() > 1 && r.back() == 0) r.pop_back();
    return r;
}

Big divExact(const Big& a, ll v) {
    Big r(a.size());
    ll rem = 0;
    for (int i = (int)a.size() - 1; i >= 0; i--) {
        ll cur = rem * BASE + a[i];
        r[i] = cur / v;
        rem = cur % v;
    }
    while (r.size() > 1 && r.back() == 0) r.pop_back();
    return r;
}

ll modBig(const Big& a, ll P) {
    ll r = 0;
    ll base = BASE % P;
    for (int i = (int)a.size() - 1; i >= 0; i--) {
        r = ((__int128)r * base + a[i] % P) % P;
    }
    return r;
}

int N, M;
ll P;
Big ans;

void solve(int rem, int maxPart, vector<pair<int,int>>& counts) {
    if (rem == 0) {
        ll ec = 0;
        for (auto& [l, c] : counts) ec += (ll)c * (l / 2);
        for (int i = 0; i < (int)counts.size(); i++) {
            auto [li, ci] = counts[i];
            ec += (ll)ci * (ci - 1) / 2 * li;
            for (int j = i + 1; j < (int)counts.size(); j++) {
                auto [lj, cj] = counts[j];
                ec += (ll)ci * cj * __gcd(li, lj);
            }
        }
        
        Big Mec = fromLL(1);
        for (ll i = 0; i < ec; i++) Mec = mulLL(Mec, (ll)M);
        
        Big mult = fromLL(1);
        for (int i = 1; i <= N; i++) mult = mulLL(mult, (ll)i);
        for (auto& [l, cnt] : counts) {
            for (int i = 0; i < cnt; i++) mult = divExact(mult, (ll)l);
            for (int i = 1; i <= cnt; i++) mult = divExact(mult, (ll)i);
        }
        
        ans = add(ans, mulBig(mult, Mec));
        return;
    }
    for (int p = min(rem, maxPart); p >= 1; p--) {
        bool found = false;
        for (auto& [l, c] : counts) if (l == p) { c++; found = true; break; }
        if (!found) counts.push_back({p, 1});
        solve(rem - p, p, counts);
        for (int i = 0; i < (int)counts.size(); i++) {
            if (counts[i].first == p) {
                counts[i].second--;
                if (counts[i].second == 0) counts.erase(counts.begin() + i);
                break;
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> N >> M >> P;
    ans = fromLL(0);
    vector<pair<int,int>> counts;
    solve(N, N, counts);
    for (int i = 1; i <= N; i++) ans = divExact(ans, (ll)i);
    cout << modBig(ans, P) << endl;
    return 0;
}
