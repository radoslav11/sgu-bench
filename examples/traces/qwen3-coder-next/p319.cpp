#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

struct Rect {
    long long x_min, x_max, y_min, y_max, area;
    int idx;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int N;
    cin >> N;
    long long W, H;
    cin >> W >> H;

    vector<Rect> rects(N);
    for (int i = 0; i < N; i++) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        rects[i].x_min = min(x1, x2);
        rects[i].x_max = max(x1, x2);
        rects[i].y_min = min(y1, y2);
        rects[i].y_max = max(y1, y2);
        rects[i].area = (rects[i].x_max - rects[i].x_min) * (rects[i].y_max - rects[i].y_min);
        rects[i].idx = i;
    }

    sort(rects.begin(), rects.end(), [](const Rect& a, const Rect& b) {
        if (a.x_min != b.x_min) 
            return a.x_min < b.x_min;
        if (a.x_max != b.x_max)
            return a.x_max > b.x_max;
        if (a.y_min != b.y_min)
            return a.y_min < b.y_min;
        return a.y_max > b.y_max;
    });

    stack<Rect> st;
    vector<long long> child_sum(N, 0);
    vector<int> parent(N, -1);

    for (int i = 0; i < N; i++) {
        while (!st.empty()) {
            const Rect& top = st.top();
            if (top.x_min >= rects[i].x_min || top.x_max <= rects[i].x_max || top.y_min >= rects[i].y_min || top.y_max <= rects[i].y_max) {
                st.pop();
            } else {
                break;
            }
        }
        if (!st.empty()) {
            parent[i] = st.top().idx;
        }
        st.push(rects[i]);
    }

    for (int i = 0; i < N; i++) {
        if (parent[i] != -1) {
            child_sum[parent[i]] += rects[i].area;
        }
    }

    vector<long long> regions;
    long long total = W * H;
    long long sum_outer = 0;
    for (int i = 0; i < N; i++) {
        if (parent[i] == -1) {
            sum_outer += rects[i].area;
        }
    }
    regions.push_back(total - sum_outer);

    for (int i = 0; i < N; i++) {
        regions.push_back(rects[i].area - child_sum[i]);
    }

    sort(regions.begin(), regions.end());

    for (int i = 0; i < regions.size(); i++) {
        if (i > 0) cout << " ";
        cout << regions[i];
    }
    cout << endl;

    return 0;
}
