#include <iostream>
#include <vector>
using namespace std;

struct Point {
    long long x, y;
    Point(long long x = 0, long long y = 0) : x(x), y(y) {}
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

struct Segment {
    Point p1, p2;
    Segment(long long x1, long long y1, long long x2, long long y2) {
        p1 = Point(x1, y1);
        p2 = Point(x2, y2);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N;
    cin >> N;
    vector<Segment> segs;
    for (int i = 0; i < N; i++) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        segs.push_back(Segment(x1, y1, x2, y2));
    }

    long long count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            Point a1 = segs[i].p1, a2 = segs[i].p2;
            Point b1 = segs[j].p1, b2 = segs[j].p2;

            if (a1 == b1) {
                long long vx1 = a2.x - a1.x;
                long long vy1 = a2.y - a1.y;
                long long vx2 = b2.x - b1.x;
                long long vy2 = b2.y - b1.y;
                if (vx1 * vx2 + vy1 * vy2 == 0) {
                    count++;
                }
            } else if (a1 == b2) {
                long long vx1 = a2.x - a1.x;
                long long vy1 = a2.y - a1.y;
                long long vx2 = b1.x - b2.x;
                long long vy2 = b1.y - b2.y;
                if (vx1 * vx2 + vy1 * vy2 == 0) {
                    count++;
                }
            } else if (a2 == b1) {
                long long vx1 = a1.x - a2.x;
                long long vy1 = a1.y - a2.y;
                long long vx2 = b2.x - b1.x;
                long long vy2 = b2.y - b1.y;
                if (vx1 * vx2 + vy1 * vy2 == 0) {
                    count++;
                }
            } else if (a2 == b2) {
                long long vx1 = a1.x - a2.x;
                long long vy1 = a1.y - a2.y;
                long long vx2 = b1.x - b2.x;
                long long vy2 = b1.y - b2.y;
                if (vx1 * vx2 + vy1 * vy2 == 0) {
                    count++;
                }
            }
        }
    }

    cout << count << '\n';
    return 0;
}
