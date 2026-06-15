#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem: Hexagonal Walkaround
 * A hexagonal tank starts at (0,0) heading in direction d1.
 * In one turn, it can either:
 * 1. Move forward in its current direction.
 * 2. Turn left once and move in the new direction.
 * Constraint: Cannot move more than b times in the same direction consecutively.
 * Target: (x, y) heading in direction d2.
 * Minimize the number of turns (each turn results in one move).
 * 
 * Strategy:
 * Let the directions be:
 * d=0: (1, 0)
 * d=1: (0, 1)
 * d=2: (-1, 1)
 * d=3: (-1, 0)
 * d=4: (0, -1)
 * d=5: (1, -1)
 * 
 * The tank uses a sequence of directions starting with either d1 or d1+1.
 * The directions follow the sequence: D_i = (D_0 + i) % 6.
 * The total number of steps N = sum(k_i) for i=0 to C.
 * Each k_i is in [1, b].
 * The directions used are a sequence of segments, where each direction j is used m_j times.
 * N = sum(K_j), where K_j is the total number of steps in direction j.
 * m_j <= K_j <= m_j * b.
 * 
 * The problem is to find the smallest N such that there exists C and K_j.
 * Since N is at least d((0,0), (x,y)), and N increases with C, 
 * we can binary search for the smallest C for each possible starting direction.
 */

typedef __int128_t int128;

struct Point {
    int128 x, y;
};

Point v[6] = {{1, 0}, {0, 1}, {-1, 1}, {-1, 0}, {0, -1}, {1, -1}};

long long dist(long long x, long long y) {
    if ((x > 0 && y < 0) || (x < 0 && y > 0)) return max(abs(x), abs(y));
    return abs(x) + abs(y);
}

bool is_in_hexagon(int128 x, int128 y, const vector<Point>& poly) {
    int n = poly.size();
    for (int i = 0; i < n; ++i) {
        int next = (i + 1) % n;
        int128 cross = (poly[next].x - poly[i].x) * (y - poly[i].y) - (poly[next].y - poly[i].y) * (x - poly[i].x);
        if (cross < 0) return false;
    }
    return true;
}

int main() {
    long long lx, ly, ld2, ld1, lb;
    if (!(cin >> lx >> ly >> ld2 >> ld1 >> lb)) return 0;
    if (lx == 0 && ly == 0 && ld1 == ld2) {
        cout << 0 << endl;
        return 0;
    }

    int128 x = lx, y = ly, b = lb;
    long long min_n = -1;

    for (int r_prime = 0; r_prime <= 1; ++r_prime) {
        int d0 = (ld1 + r_prime) % 6;
        int c_base = (ld2 - d0 + 6) % 6;

        long long low = 0, high = 2000000000000LL, best_m = -1;
        while (low <= high) {
            long long m = low + (high - low) / 2;
            long long C = 6 * m + c_base;
            
            vector<long long> m_j(6, 0);
            for (int i = 0; i <= C; ++i) {
                m_j[(d0 + i) % 6]++;
            }

            Point p0 = {0, 0};
            for (int j = 0; j < 6; ++j) {
                p0.x += (int128)m_j[j] * v[j].x;
                p0.y += (int128)m_j[j] * v[j].y;
            }

            vector<Point> poly;
            poly.push_back(p0);
            Point current = p0;
            for (int j = 0; j < 6; ++j) {
                current.x += (int128)m_j[j] * (b - 1) * v[j].x;
                current.y += (int128)m_j[j] * (b - 1) * v[j].y;
                poly.push_back(current);
            }

            if (is_in_hexagon(x, y, poly)) {
                best_m = m;
                high = m - 1;
            } else {
                low = m + 1;
            }
        }

        if (best_m != -1) {
            long long C = 6 * best_m + c_base;
            vector<long long> m_j(6, 0);
            int128 sum_mj = 0;
            for (int i = 0; i <= C; ++i) {
                m_j[(d0 + i) % 6]++;
            }
            for (int j = 0; j < 6; ++j) sum_mj += m_j[j];

            long long current_n = max((long long)dist(lx, ly), (long long)sum_mj);
            if (min_n == -1 || current_n < min_n) min_n = current_n;
        }
    }

    cout << min_n << endl;

    return 0;
}

