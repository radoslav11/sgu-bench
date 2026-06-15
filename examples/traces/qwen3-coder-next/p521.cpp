#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class FenwTreeMax {
private:
    vector<int> tree;
    int n;
public:
    FenwTreeMax(int size) {
        n = size;
        tree.assign(n+1, 0);
    }

    void update(int idx, int val) {
        while (idx <= n) {
            if (val > tree[idx]) {
                tree[idx] = val;
            }
            idx += idx & -idx;
        }
    }

    int query(int idx) {
        if (idx <= 0) return 0;
        int res = 0;
        while (idx > 0) {
            if (tree[idx] > res) {
                res = tree[idx];
            }
            idx -= idx & -idx;
        }
        return res;
    }
};

struct City {
    int x, y, idx;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;

    vector<City> cities;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        cities.push_back({x, y, i+1});
    }

    vector<int> ys;
    for (const auto& c : cities) {
        ys.push_back(c.y);
    }
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    int total_ys = ys.size();

    auto get_pos = [&](int y) {
        return lower_bound(ys.begin(), ys.end(), y) - ys.begin() + 1;
    };

    vector<int> dp1_arr(n, 0);
    sort(cities.begin(), cities.end(), [](const City& a, const City& b) {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    });

    FenwTreeMax fenw1(total_ys);
    for (int i = 0; i < n; ) {
        int j = i;
        while (j < n && cities[i].x == cities[j].x) j++;
        vector<int> temp;
        for (int k = i; k < j; k++) {
            int pos = get_pos(cities[k].y);
            int prev_max = (pos > 1) ? fenw1.query(pos-1) : 0;
            dp1_arr[cities[k].idx-1] = prev_max + 1;
            temp.push_back(pos);
        }
        for (int k = i; k < j; k++) {
            int pos = get_pos(cities[k].y);
            fenw1.update(pos, dp1_arr[cities[k].idx-1]);
        }
        i = j;
    }

    vector<int> dp2_arr(n, 0);
    sort(cities.begin(), cities.end(), [](const City& a, const City& b) {
        if (a.x != b.x) return a.x > b.x;
        return a.y > b.y;
    });

    FenwTreeMax fenw2(total_ys);
    for (int i = 0; i < n; ) {
        int j = i;
        while (j < n && cities[i].x == cities[j].x) j++;
        for (int k = i; k < j; k++) {
            int pos = get_pos(cities[k].y);
            int z = total_ys - pos + 1;
            int prev_max = (z > 1) ? fenw2.query(z-1) : 0;
            dp2_arr[cities[k].idx-1] = prev_max + 1;
        }
        for (int k = i; k < j; k++) {
            int pos = get_pos(cities[k].y);
            int z = total_ys - pos + 1;
            fenw2.update(z, dp2_arr[cities[k].idx-1]);
        }
        i = j;
    }

    int M = *max_element(dp1_arr.begin(), dp1_arr.end());

    vector<vector<int>> levels(M+1);
    vector<int> A_list;
    for (int i = 0; i < n; i++) {
        if (dp1_arr[i] + dp2_arr[i] - 1 == M) {
            A_list.push_back(i+1);
            levels[dp1_arr[i]].push_back(i+1);
        }
    }

    vector<int> B_list;
    for (int k = 1; k <= M; k++) {
        if (levels[k].size() == 1) {
            B_list.push_back(levels[k][0]);
        }
    }

    sort(A_list.begin(), A_list.end());
    sort(B_list.begin(), B_list.end());

    cout << A_list.size();
    for (int a : A_list) {
        cout << " " << a;
    }
    cout << "\n";

    cout << B_list.size();
    for (int b : B_list) {
        cout << " " << b;
    }
    cout << "\n";

    return 0;
}
