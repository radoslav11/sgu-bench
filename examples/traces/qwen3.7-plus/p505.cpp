#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

using namespace std;

struct BIT {
    int n;
    vector<int> tree;
    BIT(int n) : n(n), tree(n + 1, 0) {}
    void add(int i, int delta) {
        for (; i <= n; i += i & -i) tree[i] += delta;
    }
    int query(int i) {
        int sum = 0;
        for (; i > 0; i -= i & -i) sum += tree[i];
        return sum;
    }
};

struct SubQuery {
    int x, y, sign, q_id;
    bool operator<(const SubQuery& other) const {
        return x < other.x;
    }
};

int get_start(const vector<string>& arr, const string& p) {
    return lower_bound(arr.begin(), arr.end(), p) - arr.begin();
}

int get_end(const vector<string>& arr, const string& p) {
    string p_next = p;
    p_next.back()++;
    return lower_bound(arr.begin(), arr.end(), p_next) - arr.begin() - 1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<string> genomes(n);
    for (int i = 0; i < n; ++i) {
        cin >> genomes[i];
    }

    vector<int> A(n), B(n);
    iota(A.begin(), A.end(), 0);
    iota(B.begin(), B.end(), 0);

    sort(A.begin(), A.end(), [&](int i, int j) {
        return genomes[i] < genomes[j];
    });

    vector<string> rev_genomes(n);
    for (int i = 0; i < n; ++i) {
        rev_genomes[i] = genomes[i];
        reverse(rev_genomes[i].begin(), rev_genomes[i].end());
    }

    sort(B.begin(), B.end(), [&](int i, int j) {
        return rev_genomes[i] < rev_genomes[j];
    });

    vector<int> rankA(n), rankB(n);
    for (int i = 0; i < n; ++i) {
        rankA[A[i]] = i + 1;
        rankB[B[i]] = i + 1;
    }

    vector<int> points(n + 1);
    for (int i = 0; i < n; ++i) {
        points[rankA[i]] = rankB[i];
    }

    vector<string> sorted_A(n), sorted_B(n);
    for (int i = 0; i < n; ++i) {
        sorted_A[i] = genomes[A[i]];
        sorted_B[i] = rev_genomes[B[i]];
    }

    int m;
    if (!(cin >> m)) return 0;

    vector<SubQuery> sub_queries;
    sub_queries.reserve(m * 4);

    for (int q = 0; q < m; ++q) {
        string p, s;
        cin >> p >> s;

        int x1 = get_start(sorted_A, p) + 1;
        int x2 = get_end(sorted_A, p) + 1;
        
        string rev_s = s;
        reverse(rev_s.begin(), rev_s.end());
        int y1 = get_start(sorted_B, rev_s) + 1;
        int y2 = get_end(sorted_B, rev_s) + 1;

        if (x1 <= x2 && y1 <= y2) {
            sub_queries.push_back({x2, y2, 1, q});
            sub_queries.push_back({x1 - 1, y2, -1, q});
            sub_queries.push_back({x2, y1 - 1, -1, q});
            sub_queries.push_back({x1 - 1, y1 - 1, 1, q});
        }
    }

    sort(sub_queries.begin(), sub_queries.end());

    vector<int> ans(m, 0);
    BIT bit(n);
    int current_x = 0;

    for (const auto& sq : sub_queries) {
        while (current_x < sq.x) {
            current_x++;
            if (current_x <= n) {
                bit.add(points[current_x], 1);
            }
        }
        if (sq.x > 0 && sq.y > 0) {
            ans[sq.q_id] += sq.sign * bit.query(sq.y);
        }
    }

    for (int q = 0; q < m; ++q) {
        cout << ans[q] << "\n";
    }

    return 0;
}
