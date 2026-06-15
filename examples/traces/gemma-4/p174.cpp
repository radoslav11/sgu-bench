#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem Analysis:
 * We are given M walls (segments). We need to find the index of the first wall that completes 
 * a cycle. The problem states that walls can only cross at their endpoints and cannot coincide. 
 * This implies the walls form a planar graph structure where the vertices are the endpoints 
 * and the edges are the segments. In a planar graph, any cycle encloses a region.
 * Thus, the problem is to find the first edge that connects two nodes already in the 
 * same connected component, which is a standard application of Disjoint Set Union (DSU).
 * 
 * Constraints:
 * M <= 200,000, Coordinates up to 10^9.
 * Time limit: 0.75s. Memory limit: 32 MB.
 * 
 * Complexity:
 * - Reading and storing walls: O(M)
 * - Sorting and deduplicating endpoints: O(M log M)
 * - Finding endpoint indices using binary search: O(M log M)
 * - DSU operations: O(M alpha(M))
 * Total time: O(M log M), total memory: O(M).
 * 
 * Memory check:
 * walls[200005]: 200005 * 16 bytes = 3.2 MB
 * pts[400005]: 400005 * 8 bytes = 3.2 MB
 * parent[400005]: 400005 * 4 bytes = 1.6 MB
 * sz[400005]: 400005 * 4 bytes = 1.6 MB
 * u_idx[200005]: 200005 * 4 bytes = 0.8 MB
 * v_idx[200005]: 200005 * 4 bytes = 0.8 MB
 * Total: approx 11.2 MB, which fits comfortably in 32 MB.
 */

struct Point {
    int x, y;
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

struct Wall {
    int x1, y1, x2, y2;
};

const int MAXM = 200005;
const int MAXP = 400005;

Wall walls[MAXM];
Point pts[MAXP];
int parent[MAXP];
int sz[MAXP];
int u_idx[MAXM];
int v_idx[MAXM];

// DSU find operation with path compression
int find(int i) {
    int root = i;
    while (parent[root] != root)
        root = parent[root];
    while (parent[i] != root) {
        int next = parent[i];
        parent[i] = root;
        i = next;
    }
    return root;
}

// DSU unite operation with union by size
void unite(int i, int j) {
    int root_i = find(i);
    int root_j = find(j);
    if (root_i != root_j) {
        if (sz[root_i] < sz[root_j]) swap(root_i, root_j);
        parent[root_j] = root_i;
        sz[root_i] += sz[root_j];
    }
}

int main() {
    int M;
    if (scanf("%d", &M) != 1) return 0;
    
    int pt_count = 0;
    for (int i = 0; i < M; ++i) {
        if (scanf("%d %d %d %d", &walls[i].x1, &walls[i].y1, &walls[i].x2, &walls[i].y2) != 4) break;
        pts[pt_count++] = {walls[i].x1, walls[i].y1};
        pts[pt_count++] = {walls[i].x2, walls[i].y2};
    }

    // Pre-process all unique points to map coordinates to unique vertex IDs
    sort(pts, pts + pt_count);
    int num_unique = unique(pts, pts + pt_count) - pts;

    // For each wall, get the indices of its endpoints in the sorted 'pts' array
    for (int i = 0; i < M; ++i) {
        Point p1 = {walls[i].x1, walls[i].y1};
        Point p2 = {walls[i].x2, walls[i].y2};
        u_idx[i] = lower_bound(pts, pts + num_unique, p1) - pts;
        v_idx[i] = lower_bound(pts, pts + num_unique, p2) - pts;
    }

    // Initialize DSU components
    for (int i = 0; i < num_unique; ++i) {
        parent[i] = i;
        sz[i] = 1;
    }

    // Add walls one by one and check for cycles using DSU
    for (int i = 0; i < M; ++i) {
        if (find(u_idx[i]) == find(v_idx[i])) {
            printf("%d\n", i + 1);
            return 0;
        }
        unite(u_idx[i], v_idx[i]);
    }

    // If no wall completes a cycle, output 0
    printf("0\n");
    return 0;
}

