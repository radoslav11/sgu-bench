#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <map>

using namespace std;

/**
 * Problem Summary:
 * A matrix T is a "control matrix" if the first row is different from every other row.
 * A function f: T_values -> {0, 1, ..., 50} is a "control function" if f(T) is also a control matrix.
 * That is, for each row i > 1, there exists at least one j such that f(T_{1,j}) != f(T_{i,j}).
 *
 * Analysis:
 * For each row i > 1, the condition to satisfy is that the row is not monochromatic in terms
 * of the edges it defines. An edge is (T_{1,j}, T_{i,j}) for all j where T_{1,j} != T_{i,j}.
 * Let G be a hypergraph where each row i defines a "hyperedge" of edges. We need a coloring
 * such that no hyperedge is monochromatic.
 * A simpler condition is: pick one edge (u_i, v_i) from each row i such that u_i != v_i.
 * We want to color the graph with these (n-1) edges such that every edge is non-monochromatic.
 * This is a proper graph coloring problem. A graph with E edges has chromatic number chi(G) <= sqrt(2E) + 1.
 * For n=1000, E=999, chi(G) <= sqrt(1998) + 1 approx 45.7.
 * Since we have 51 colors available (0 to 50), a solution always exists.
 *
 * A randomized approach of assigning f(x) = rand() % 51 will very quickly find a solution.
 */

int main() {
    // Optimize I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<int>> T(n, vector<int>(m));
    vector<int> all_values;
    all_values.reserve(n * m);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> T[i][j];
            all_values.push_back(T[i][j]);
        }
    }

    // Get unique values and map them to indices 0...K-1
    sort(all_values.begin(), all_values.end());
    all_values.erase(unique(all_values.begin(), all_values.end()), all_values.end());

    int K = all_values.size();
    // Replace T[i][j] with its index in all_values to speed up access
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            T[i][j] = lower_bound(all_values.begin(), all_values.end(), T[i][j]) - all_values.begin();
        }
    }

    // Random number generator
    mt19937 rng(1337);
    uniform_int_distribution<int> dist(0, 50);

    vector<int> f(K);
    while (true) {
        for (int i = 0; i < K; ++i) {
            f[i] = dist(rng);
        }

        bool possible = true;
        for (int i = 1; i < n; ++i) {
            bool row_ok = false;
            for (int j = 0; j < m; ++j) {
                if (f[T[0][j]] != f[T[i][j]]) {
                    row_ok = true;
                    break;
                }
            }
            if (!row_ok) {
                possible = false;
                break;
            }
        }

        if (possible) {
            cout << "Yes" << "\n";
            for (int i = 0; i < K; ++i) {
                cout << all_values[i] << " -> " << f[i] << "\n";
            }
            break;
        }
    }

    return 0;
}

