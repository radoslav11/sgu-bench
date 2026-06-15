#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <iomanip>

using namespace std;

// Point structure using long long to maintain precision for coordinates
struct Point {
    long long x, y, z;
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        return z < other.z;
    }
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

// Parse a coordinate string with exactly 9 decimal places into a long long
long long parse(string s) {
    bool neg = false;
    if (s[0] == '-') {
        neg = true;
        s = s.substr(1);
    }
    size_t dot = s.find('.');
    long long res = 0;
    if (dot == string::npos) {
        res = stoll(s) * 1000000000LL;
    } else {
        string integer_part = s.substr(0, dot);
        string fractional_part = s.substr(dot + 1);
        while (fractional_part.size() < 9) fractional_part += '0';
        if (fractional_part.size() > 9) fractional_part = fractional_part.substr(0, 9);
        res = stoll(integer_part) * 1000000000LL + stoll(fractional_part);
    }
    return neg ? -res : res;
}

// Print the long long coordinate back as a string with 9 decimal places
void printCoord(long long val) {
    if (val < 0) {
        cout << "-";
        val = -val;
    }
    long long integer_part = val / 1000000000LL;
    long long fractional_part = val % 1000000000LL;
    cout << integer_part << "." << setfill('0') << setw(9) << fractional_part;
}

int V;
vector<int> adj[105];
bool visited[105];
int path[105];
Point all_vertices[105];

// Backtracking with Warnsdorff's heuristic and degree-based pruning
bool dfs(int u, int count, int start_node) {
    if (count == V) {
        for (int v : adj[u]) {
            if (v == start_node) return true;
        }
        return false;
    }

    // Pruning: check if any unvisited vertex has fewer than 2 available edges
    for (int i = 0; i < V; ++i) {
        if (!visited[i] && i != start_node) {
            int deg = 0;
            for (int v : adj[i]) {
                if (!visited[v] || v == start_node || v == u) deg++;
            }
            if (deg < 2) return false;
        }
    }

    // Warnsdorff's heuristic: sort neighbors by their available degrees
    vector<pair<int, int>> neighbors;
    for (int v : adj[u]) {
        if (!visited[v]) {
            int deg = 0;
            for (int nn : adj[v]) {
                if (!visited[nn]) deg++;
            }
            neighbors.push_back({deg, v});
        }
    }
    sort(neighbors.begin(), neighbors.end());

    for (auto& p : neighbors) {
        int v = p.second;
        visited[v] = true;
        path[count] = v;
        if (dfs(v, count + 1, start_node)) return true;
        visited[v] = false;
    }
    return false;
}

int main() {
    // Speed up input and output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int f;
    if (!(cin >> f)) return 0;
    map<Point, int> pt_to_idx;
    vector<Point> unique_pts;

    // Build graph from facet information
    for (int i = 0; i < f; ++i) {
        int k;
        cin >> k;
        vector<int> face_indices;
        for (int j = 0; j < k; ++j) {
            string sx, sy, sz;
            cin >> sx >> sy >> sz;
            Point p = {parse(sx), parse(sy), parse(sz)};
            if (pt_to_idx.find(p) == pt_to_idx.end()) {
                pt_to_idx[p] = unique_pts.size();
                unique_pts.push_back(p);
            }
            face_indices.push_back(pt_to_idx[p]);
        }
        for (int j = 0; j < k; ++j) {
            int u = face_indices[j];
            int v = face_indices[(j + 1) % k];
            if (u == v) continue;
            bool exists = false;
            for (int neighbor : adj[u]) {
                if (neighbor == v) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
        }
    }

    V = unique_pts.size();
    for (int i = 0; i < V; ++i) all_vertices[i] = unique_pts[i];

    // A Hamiltonian cycle must visit all vertices
    for (int i = 0; i < V; ++i) visited[i] = false;
    path[0] = 0;
    visited[0] = true;

    if (dfs(0, 1, 0)) {
        cout << "Yes" << "\n";
        cout << V << "\n";
        for (int i = 0; i < V; ++i) {
            printCoord(all_vertices[path[i]].x);
            cout << " ";
            printCoord(all_vertices[path[i]].y);
            cout << " ";
            printCoord(all_vertices[path[i]].z);
            cout << "\n";
        }
    } else {
        cout << "No" << "\n";
    }

    return 0;
}

