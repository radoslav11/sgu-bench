#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 2000000000000000000LL;

struct Point {
    long long x, y;
};

Point rem1_arr[20005];
Point rem2_arr[20005];

bool check(long long d, const vector<Point>& points) {
    int n = points.size();
    long long X_min = INF, X_max = -INF;
    long long Y_min = INF, Y_max = -INF;
    for (int i = 0; i < n; i++) {
        if (points[i].x < X_min) X_min = points[i].x;
        if (points[i].x > X_max) X_max = points[i].x;
        if (points[i].y < Y_min) Y_min = points[i].y;
        if (points[i].y > Y_max) Y_max = points[i].y;
    }
    if (X_max - X_min <= d && Y_max - Y_min <= d) return true;

    struct Square {
        long long min_x, max_x, min_y, max_y;
    };

    Square sq1_candidates[4] = {
        {X_min, X_min + d, Y_min, Y_min + d},
        {X_min, X_min + d, Y_max - d, Y_max},
        {X_max - d, X_max, Y_min, Y_min + d},
        {X_max - d, X_max, Y_max - d, Y_max}
    };

    for (int c1 = 0; c1 < 4; c1++) {
        Square sq1 = sq1_candidates[c1];
        int r1_n = 0;
        long long r1_X_min = INF, r1_X_max = -INF;
        long long r1_Y_min = INF, r1_Y_max = -INF;
        
        for (int i = 0; i < n; i++) {
            long long px = points[i].x;
            long long py = points[i].y;
            if (px < sq1.min_x || px > sq1.max_x || py < sq1.min_y || py > sq1.max_y) {
                rem1_arr[r1_n].x = px;
                rem1_arr[r1_n].y = py;
                r1_n++;
                if (px < r1_X_min) r1_X_min = px;
                if (px > r1_X_max) r1_X_max = px;
                if (py < r1_Y_min) r1_Y_min = py;
                if (py > r1_Y_max) r1_Y_max = py;
            }
        }
        
        if (r1_n == 0) return true;

        Square sq2_candidates[4] = {
            {r1_X_min, r1_X_min + d, r1_Y_min, r1_Y_min + d},
            {r1_X_min, r1_X_min + d, r1_Y_max - d, r1_Y_max},
            {r1_X_max - d, r1_X_max, r1_Y_min, r1_Y_min + d},
            {r1_X_max - d, r1_X_max, r1_Y_max - d, r1_Y_max}
        };

        for (int c2 = 0; c2 < 4; c2++) {
            Square sq2 = sq2_candidates[c2];
            int r2_n = 0;
            long long r2_X_min = INF, r2_X_max = -INF;
            long long r2_Y_min = INF, r2_Y_max = -INF;

            for (int i = 0; i < r1_n; i++) {
                long long px = rem1_arr[i].x;
                long long py = rem1_arr[i].y;
                if (px < sq2.min_x || px > sq2.max_x || py < sq2.min_y || py > sq2.max_y) {
                    rem2_arr[r2_n].x = px;
                    rem2_arr[r2_n].y = py;
                    r2_n++;
                    if (px < r2_X_min) r2_X_min = px;
                    if (px > r2_X_max) r2_X_max = px;
                    if (py < r2_Y_min) r2_Y_min = py;
                    if (py > r2_Y_max) r2_Y_max = py;
                }
            }

            if (r2_n == 0) return true;
            
            if (r2_X_max - r2_X_min <= d && r2_Y_max - r2_Y_min <= d) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    if (!(cin >> n)) return 0;
    vector<Point> points(n);
    for (int i = 0; i < n; i++) {
        cin >> points[i].x >> points[i].y;
    }
    
    long long low = 0, high = 2000000005LL;
    long long ans = high;
    
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (check(mid, points)) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    
    cout << ans << "\n";
    return 0;
}
