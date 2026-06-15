#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <cstdio>

using namespace std;

/**
 * Problem Summary:
 * - A series of cells (1 to 10^6) can hold cubes.
 * - Towers are contiguous non-empty cells.
 * - Towers are ordered from left to right.
 * - Commands: put, tput, towers, cubes, length, tcubes.
 * 
 * Constraints:
 * - N = 10^6 commands.
 * - Cell range: 1 to 10^6.
 * - Time: 1.25s, Memory: 64MB.
 * 
 * Data Structures:
 * - Fenwick tree `ft_cubes` to maintain the number of cubes in each cell (supports point update, range sum).
 * - Fenwick tree `ft_towers` to maintain the starting positions of towers (supports binary lifting to find k-th tower).
 * - `std::set<pair<int, int>> towers_set` to keep track of existing tower ranges [L, R].
 * 
 * Complexity:
 * - Each operation takes O(log M) or O(log N), where M = 10^6.
 * - Memory: ~44-56 MB, which fits within 64 MB.
 */

const int MAXM = 1000005;
long long ft_cubes[MAXM];
int ft_towers[MAXM];
const int M = 1000000;

// Point update for Fenwick tree ft_cubes
void ft_cubes_update(int idx, long long val) {
    for (; idx <= M; idx += idx & -idx)
        ft_cubes[idx] += val;
}

// Prefix sum query for Fenwick tree ft_cubes
long long ft_cubes_query(int idx) {
    long long res = 0;
    for (; idx > 0; idx -= idx & -idx)
        res += ft_cubes[idx];
    return res;
}

// Point update for Fenwick tree ft_towers
void ft_towers_update(int idx, int val) {
    for (; idx <= M; idx += idx & -idx)
        ft_towers[idx] += val;
}

// Find the starting position of the k-th tower using binary lifting on ft_towers
int ft_towers_find_kth(int k) {
    int idx = 0;
    for (int i = 1 << 19; i > 0; i >>= 1) {
        if (idx + i <= M && ft_towers[idx + i] < k) {
            idx += i;
            k -= ft_towers[idx];
        }
    }
    return idx + 1;
}

set<pair<int, int>> towers_set;

// Handles 'put x c' command
void put(int x, int c) {
    // Check if x is already in a tower
    auto it = towers_set.upper_bound({x, 2000000});
    if (it != towers_set.begin()) {
        auto it_prev = prev(it);
        if (it_prev->first <= x && x <= it_prev->second) {
            ft_cubes_update(x, c);
            return;
        }
    }

    // Find left and right adjacent towers to check for merging
    pair<int, int> left_tower = {0, 0};
    auto it_left = towers_set.upper_bound({x, 2000000});
    if (it_left != towers_set.begin()) {
        auto it_prev = prev(it_left);
        if (it_prev->second == x - 1) {
            left_tower = *it_prev;
        }
    }

    pair<int, int> right_tower = {0, 0};
    auto it_right = towers_set.lower_bound({x, 0});
    if (it_right != towers_set.end()) {
        if (it_right->first == x + 1) {
            right_tower = *it_right;
        }
    }

    int new_L = x, new_R = x;
    if (left_tower.first != 0 && right_tower.first != 0) {
        // Merge both left and right towers
        towers_set.erase(left_tower);
        ft_towers_update(left_tower.first, -1);
        towers_set.erase(right_tower);
        ft_towers_update(right_tower.first, -1);
        new_L = left_tower.first;
        new_R = right_tower.second;
    } else if (left_tower.first != 0) {
        // Merge with left tower
        towers_set.erase(left_tower);
        ft_towers_update(left_tower.first, -1);
        new_L = left_tower.first;
        new_R = x;
    } else if (right_tower.first != 0) {
        // Merge with right tower
        towers_set.erase(right_tower);
        ft_towers_update(right_tower.first, -1);
        new_L = x;
        new_R = right_tower.second;
    }

    // Insert the (potentially merged) new tower
    towers_set.insert({new_L, new_R});
    ft_towers_update(new_L, 1);
    ft_cubes_update(x, c);
}

int main() {
    int N;
    if (scanf("%d", &N) != 1) return 0;
    char cmd[20];
    while (N--) {
        if (scanf("%s", cmd) != 1) break;
        if (cmd[0] == 'p') { // put x c
            int x, c;
            scanf("%d %d", &x, &c);
            put(x, c);
        } else if (cmd[0] == 't' && cmd[1] == 'p') { // tput t x c
            int t, x, c;
            scanf("%d %d %d", &t, &x, &c);
            int start_pos = ft_towers_find_kth(t);
            int cell = start_pos + x - 1;
            ft_cubes_update(cell, c);
        } else if (cmd[0] == 't' && cmd[1] == 'o') { // towers
            printf("%d towers\n", (int)towers_set.size());
        } else if (cmd[0] == 'c' && cmd[1] == 'u') { // cubes t
            int t;
            scanf("%d", &t);
            int start_pos = ft_towers_find_kth(t);
            auto it = towers_set.lower_bound({start_pos, 0});
            printf("%lld cubes in %dth tower\n", ft_cubes_query(it->second) - ft_cubes_query(it->first - 1), t);
        } else if (cmd[0] == 'l') { // length t
            int t;
            scanf("%d", &t);
            int start_pos = ft_towers_find_kth(t);
            auto it = towers_set.lower_bound({start_pos, 0});
            printf("length of %dth tower is %d\n", t, it->second - it->first + 1);
        } else if (cmd[0] == 't' && cmd[1] == 'c') { // tcubes t x
            int t, x;
            scanf("%d %d", &t, &x);
            int start_pos = ft_towers_find_kth(t);
            int cell = start_pos + x - 1;
            printf("%lld cubes in %dth column of %dth tower\n", ft_cubes_query(cell) - ft_cubes_query(cell - 1), x, t);
        }
    }
    return 0;
}

