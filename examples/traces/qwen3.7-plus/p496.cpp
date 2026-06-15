#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Endpt {
    int x, y, id;
    int dx, dy;
    bool operator<(const Endpt& o) const {
        if (x != o.x) return x < o.x;
        if (y != o.y) return y < o.y;
        return id < o.id;
    }
};

int main() {
    // Optimize standard I/O operations for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    vector<Endpt> ends(2 * N);
    for (int i = 1; i <= N; ++i) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        // Store both endpoints of the segment along with the direction vector away from that endpoint
        ends[2 * i - 2] = {x1, y1, i, x2 - x1, y2 - y1};
        ends[2 * i - 1] = {x2, y2, i, x1 - x2, y1 - y2};
    }

    // Sort endpoints to group segments that share the same endpoint
    sort(ends.begin(), ends.end());

    long long ans = 0;
    size_t i = 0;
    while (i < ends.size()) {
        size_t j = i;
        // Find the block of identical endpoints
        while (j < ends.size() && ends[j].x == ends[i].x && ends[j].y == ends[i].y) {
            j++;
        }
        
        // Check all unique pairs of segments sharing this endpoint
        for (size_t a = i; a < j; ++a) {
            long long dx_a = ends[a].dx;
            long long dy_a = ends[a].dy;
            for (size_t b = a + 1; b < j; ++b) {
                // Two segments form a right angle if their direction vectors' dot product is 0
                if (dx_a * ends[b].dx + dy_a * ends[b].dy == 0) {
                    ans++;
                }
            }
        }
        i = j;
    }

    cout << ans << "\n";
    return 0;
}
