#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using boost::multiprecision::cpp_int;

const long long CAP = 10001;

long long Ccap(int n, int r, long long cap = CAP) {
    if (r < 0 || r > n) return 0;
    r = min(r, n - r);
    cpp_int res = 1;
    for (int i = 1; i <= r; ++i) {
        res *= (n - r + i);
        res /= i;
        if (res >= cap) return cap;
    }
    return (long long)res;
}

cpp_int productByIdx(const vector<long long>& v, const vector<int>& idx) {
    cpp_int p = 1;
    for (int x : idx) p *= v[x];
    return p;
}

struct CombNode {
    cpp_int prod;
    vector<int> idx;
};

struct CombCmpDesc {
    bool operator()(const CombNode& a, const CombNode& b) const {
        return a.prod < b.prod;
    }
};

struct CombCmpAsc {
    bool operator()(const CombNode& a, const CombNode& b) const {
        return a.prod > b.prod;
    }
};

vector<cpp_int> genCombProducts(const vector<long long>& v, int t, int K, bool descending) {
    vector<cpp_int> res;
    int n = (int)v.size();

    if (t == 0) {
        res.push_back(1);
        return res;
    }
    if (n < t) return res;

    vector<int> start(t);
    iota(start.begin(), start.end(), 0);

    set<vector<int>> used;
    used.insert(start);

    if (descending) {
        priority_queue<CombNode, vector<CombNode>, CombCmpDesc> pq;
        pq.push({productByIdx(v, start), start});

        while (!pq.empty() && (int)res.size() < K) {
            auto cur = pq.top();
            pq.pop();
            res.push_back(cur.prod);

            for (int i = 0; i < t; ++i) {
                vector<int> nxt = cur.idx;
                int limit = (i == t - 1 ? n : cur.idx[i + 1]);
                if (nxt[i] + 1 < limit) {
                    nxt[i]++;
                    if (!used.count(nxt)) {
                        used.insert(nxt);
                        pq.push({productByIdx(v, nxt), nxt});
                    }
                }
            }
        }
    } else {
        priority_queue<CombNode, vector<CombNode>, CombCmpAsc> pq;
        pq.push({productByIdx(v, start), start});

        while (!pq.empty() && (int)res.size() < K) {
            auto cur = pq.top();
            pq.pop();
            res.push_back(cur.prod);

            for (int i = 0; i < t; ++i) {
                vector<int> nxt = cur.idx;
                int limit = (i == t - 1 ? n : cur.idx[i + 1]);
                if (nxt[i] + 1 < limit) {
                    nxt[i]++;
                    if (!used.count(nxt)) {
                        used.insert(nxt);
                        pq.push({productByIdx(v, nxt), nxt});
                    }
                }
            }
        }
    }

    return res;
}

struct PairNode {
    cpp_int prod;
    int i, j;
};

struct PairCmpDesc {
    bool operator()(const PairNode& a, const PairNode& b) const {
        return a.prod < b.prod;
    }
};

struct PairCmpAsc {
    bool operator()(const PairNode& a, const PairNode& b) const {
        return a.prod > b.prod;
    }
};

vector<cpp_int> mergeProducts(const vector<cpp_int>& A, const vector<cpp_int>& B, int K, bool descending) {
    vector<cpp_int> res;
    if (A.empty() || B.empty()) return res;

    set<pair<int, int>> used;
    used.insert({0, 0});

    if (descending) {
        priority_queue<PairNode, vector<PairNode>, PairCmpDesc> pq;
        pq.push({A[0] * B[0], 0, 0});

        while (!pq.empty() && (int)res.size() < K) {
            auto cur = pq.top();
            pq.pop();
            res.push_back(cur.prod);

            if (cur.i + 1 < (int)A.size() && !used.count({cur.i + 1, cur.j})) {
                used.insert({cur.i + 1, cur.j});
                pq.push({A[cur.i + 1] * B[cur.j], cur.i + 1, cur.j});
            }
            if (cur.j + 1 < (int)B.size() && !used.count({cur.i, cur.j + 1})) {
                used.insert({cur.i, cur.j + 1});
                pq.push({A[cur.i] * B[cur.j + 1], cur.i, cur.j + 1});
            }
        }
    } else {
        priority_queue<PairNode, vector<PairNode>, PairCmpAsc> pq;
        pq.push({A[0] * B[0], 0, 0});

        while (!pq.empty() && (int)res.size() < K) {
            auto cur = pq.top();
            pq.pop();
            res.push_back(cur.prod);

            if (cur.i + 1 < (int)A.size() && !used.count({cur.i + 1, cur.j})) {
                used.insert({cur.i + 1, cur.j});
                pq.push({A[cur.i + 1] * B[cur.j], cur.i + 1, cur.j});
            }
            if (cur.j + 1 < (int)B.size() && !used.count({cur.i, cur.j + 1})) {
                used.insert({cur.i, cur.j + 1});
                pq.push({A[cur.i] * B[cur.j + 1], cur.i, cur.j + 1});
            }
        }
    }

    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<long long> pos, negAbs;
    int zeros = 0;

    for (int i = 0; i < n; ++i) {
        long long x;
        cin >> x;
        if (x > 0) pos.push_back(x);
        else if (x < 0) negAbs.push_back(-x);
        else zeros++;
    }

    int P = pos.size();
    int Q = negAbs.size();

    long long positiveCnt = 0;
    long long negativeCnt = 0;

    for (int q = 0; q <= m; ++q) {
        int p = m - q;
        long long ways = min(CAP, Ccap(P, p) * Ccap(Q, q));
        if (ways > CAP) ways = CAP;

        if (q % 2 == 0) positiveCnt = min(CAP, positiveCnt + ways);
        else negativeCnt = min(CAP, negativeCnt + ways);
    }

    long long nonZeroCnt = Ccap(P + Q, m);
    long long allCnt = Ccap(n, m);
    long long zeroCnt = max(0LL, min(CAP, allCnt - nonZeroCnt));
    if (allCnt == CAP && nonZeroCnt < CAP && zeros > 0) zeroCnt = CAP;

    sort(pos.begin(), pos.end(), greater<long long>());
    sort(negAbs.begin(), negAbs.end(), greater<long long>());

    vector<long long> posAsc = pos;
    vector<long long> negAsc = negAbs;
    sort(posAsc.begin(), posAsc.end());
    sort(negAsc.begin(), negAsc.end());

    if (k <= positiveCnt) {
        vector<cpp_int> candidates;

        for (int q = 0; q <= m; q += 2) {
            int p = m - q;
            if (p < 0 || p > P || q > Q) continue;

            auto A = genCombProducts(pos, p, k, true);
            auto B = genCombProducts(negAbs, q, k, true);
            auto C = mergeProducts(A, B, k, true);

            candidates.insert(candidates.end(), C.begin(), C.end());
        }

        sort(candidates.begin(), candidates.end(), [](const cpp_int& a, const cpp_int& b) {
            return a > b;
        });

        cout << candidates[k - 1] << '\n';
    } else if (k <= positiveCnt + zeroCnt) {
        cout << 0 << '\n';
    } else {
        int need = k - positiveCnt - zeroCnt;

        vector<cpp_int> candidates;

        for (int q = 1; q <= m; q += 2) {
            int p = m - q;
            if (p < 0 || p > P || q > Q) continue;

            auto A = genCombProducts(posAsc, p, need, false);
            auto B = genCombProducts(negAsc, q, need, false);
            auto C = mergeProducts(A, B, need, false);

            candidates.insert(candidates.end(), C.begin(), C.end());
        }

        sort(candidates.begin(), candidates.end(), [](const cpp_int& a, const cpp_int& b) {
            return a < b;
        });

        cout << -candidates[need - 1] << '\n';
    }

    return 0;
}
