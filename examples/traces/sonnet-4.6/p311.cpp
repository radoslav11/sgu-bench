#include <bits/stdc++.h>
using namespace std;

const int MAXP = 1000001;
long long cnt[MAXP+1], sm[MAXP+1];

void update(int i, long long dc, long long ds) {
    for (int x = i; x <= MAXP; x += x & (-x)) {
        cnt[x] += dc;
        sm[x] += ds;
    }
}

pair<long long,long long> query(int i) {
    long long c = 0, s = 0;
    for (int x = i; x > 0; x -= x & (-x)) {
        c += cnt[x];
        s += sm[x];
    }
    return {c, s};
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    string op;
    while(cin >> op) {
        if(op == "ARRIVE") {
            long long n, c;
            cin >> n >> c;
            update(c, n, n*c);
        } else {
            long long n, t;
            cin >> n >> t;
            auto [tc, ts] = query(MAXP);
            if(tc < n) { cout << "UNHAPPY\n"; continue; }
            // binary search for smallest price where cumulative count >= n
            int lo = 1, hi = MAXP, ans = MAXP;
            while(lo <= hi) {
                int mid = (lo+hi)/2;
                if(query(mid).first >= n) { ans = mid; hi = mid-1; }
                else lo = mid+1;
            }
            auto [c1, s1] = query(ans-1);
            long long need = n - c1;
            long long cost = s1 + need * ans;
            if(cost <= t) {
                cout << "HAPPY\n";
                update(ans, -need, -need*ans);
            } else {
                cout << "UNHAPPY\n";
            }
        }
    }
}
