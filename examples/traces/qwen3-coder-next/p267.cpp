#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;

struct Pie {
    int area;
    int usable_area;
};

int main() {
    int N, K;
    cin >> N >> K;

    vector<Pie> pies;
    for (int i = 0; i < N; i++) {
        vector<long long> coord(12);
        for (int j = 0; j < 12; j++) {
            cin >> coord[j];
        }

        long long x1 = coord[0], y1 = coord[1];
        long long x2 = coord[2], y2 = coord[3];
        long long x3 = coord[4], y3 = coord[5];
        long long x4 = coord[6], y4 = coord[7];
        long long cx1 = coord[8], cy1 = coord[9];
        long long cx2 = coord[10], cy2 = coord[11];

        vector<long long> d;
        d.push_back((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        d.push_back((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1));
        d.push_back((x4 - x1) * (x4 - x1) + (y4 - y1) * (y4 - y1));
        sort(d.begin(), d.end());
        long long area_sq = d[0] * d[1];
        int area_val = static_cast<int>(round(sqrt(static_cast<double>(area_sq))));

        long long ox4 = x1 + x2 + x3 + x4;
        long long oy4 = y1 + y2 + y3 + y4;

        long long ax4 = 4 * cx1;
        long long ay4 = 4 * cy1;
        long long bx4 = 4 * cx2;
        long long by4 = 4 * cy2;

        long long vax = ax4 - ox4;
        long long vay = ay4 - oy4;
        long long vbx = bx4 - ox4;
        long long vby = by4 - oy4;

        long long cross = vax * vby - vay * vbx;
        long long dot = vax * vbx + vay * vby;

        bool usable = false;
        if ((ax4 == ox4 && ay4 == oy4) || (bx4 == ox4 && by4 == oy4)) {
            usable = false;
        } else if (cross != 0 || dot < 0) {
            usable = true;
        } else {
            usable = false;
        }

        int usable_area = usable ? area_val : 0;
        pies.push_back({area_val, usable_area});
    }

    if (K == 0) {
        cout << "0.000 0.000" << endl;
        return 0;
    }

    sort(pies.begin(), pies.end(), [](const Pie& a, const Pie& b) {
        return a.area > b.area;
    });

    int r = pies[K-1].area;
    int m = 0;
    for (int i = 0; i < K; i++) {
        if (pies[i].area > r) {
            m++;
        }
    }

    vector<int> usable_r;
    for (int i = 0; i < N; i++) {
        if (pies[i].area == r) {
            usable_r.push_back(pies[i].usable_area);
        }
    }

    int t = usable_r.size();
    int L = K - m;

    sort(usable_r.begin(), usable_r.end());
    long long minL = 0;
    long long maxL = 0;
    for (int i = 0; i < L; i++) {
        minL += usable_r[i];
        maxL += usable_r[t - 1 - i];
    }

    long long fixed_usable = 0;
    for (int i = 0; i < m; i++) {
        fixed_usable += pies[i].usable_area;
    }

    long long min_usable = fixed_usable + minL;
    long long max_usable = fixed_usable + maxL;

    double pessimist = static_cast<double>(min_usable) / 2.0;
    double optimist = static_cast<double>(max_usable) / 2.0;

    cout << fixed << setprecision(3) << pessimist << " " << optimist << endl;

    return 0;
}
