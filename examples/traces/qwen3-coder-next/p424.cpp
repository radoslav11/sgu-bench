#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;

    // Calculate the maximum number of edges for a beautiful graph with n vertices
    int E = (3 * (n - 1)) / 2;
    cout << n << " " << E << endl;

    if (n == 1) {
        return 0;
    }

    vector<pair<int, int>> edges;
    int current = 2;
    int t = (n - 1) / 2;  // number of triangles we can form (each uses 2 new vertices)

    for (int i = 0; i < t; i++) {
        edges.push_back({1, current});
        edges.push_back({current, current + 1});
        edges.push_back({current + 1, 1});
        current += 2;
    }

    // If n is even, we have one vertex left (current == n) to connect
    if (n % 2 == 0) {
        edges.push_back({1, current});
    }

    // Output all edges
    for (auto &e : edges) {
        cout << e.first << " " << e.second << endl;
    }

    return 0;
}
