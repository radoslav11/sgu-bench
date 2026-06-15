#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Segment {
    int x1, y1, x2, y2;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int K;
    if (!(cin >> K)) return 0;

    vector<Segment> segments(K);
    for (int i = 0; i < K; ++i) {
        cin >> segments[i].x1 >> segments[i].y1 >> segments[i].x2 >> segments[i].y2;
    }

    int X0, Y0;
    cin >> X0 >> Y0;

    bool on_border = false;
    for (int i = 0; i < K; ++i) {
        int x1 = segments[i].x1;
        int y1 = segments[i].y1;
        int x2 = segments[i].x2;
        int y2 = segments[i].y2;

        if (x1 == x2) {
            if (x1 == X0 && Y0 >= min(y1, y2) && Y0 <= max(y1, y2)) {
                on_border = true;
                break;
            }
        } else if (y1 == y2) {
            if (y1 == Y0 && X0 >= min(x1, x2) && X0 <= max(x1, x2)) {
                on_border = true;
                break;
            }
        }
    }

    if (on_border) {
        cout << "BORDER\n";
        return 0;
    }

    int count = 0;
    for (int i = 0; i < K; ++i) {
        int x1 = segments[i].x1;
        int y1 = segments[i].y1;
        int x2 = segments[i].x2;
        int y2 = segments[i].y2;

        if (x1 == x2) { // vertical segment
            if (x1 > X0) {
                int min_y = min(y1, y2);
                int max_y = max(y1, y2);
                if (min_y <= Y0 && max_y > Y0) {
                    count++;
                }
            }
        }
    }

    if (count % 2 == 1) {
        cout << "INSIDE\n";
    } else {
        cout << "OUTSIDE\n";
    }

    return 0;
}
