#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <iomanip>

using namespace std;

struct Point {
    double x, y, z;
    bool operator<(const Point& other) const {
        if (abs(x - other.x) > 1e-7) return x < other.x;
        if (abs(y - other.y) > 1e-7) return y < other.y;
        return z < other.z - 1e-7;
    }
};

double dist(Point a, Point b) { return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2)); }

struct Face {
    int axis; int coord; int min1, max1, min2, max2;
    bool operator<(const Face& other) const {
        if (axis != other.axis) return axis < other.axis;
        if (coord != other.coord) return coord < other.coord;
        if (min1 != other.min1) return min1 < other.min1;
        if (max1 != other.max1) return max1 < other.max1;
        if (min2 != other.min2) return min2 < other.min2;
        return max2 < other.max2;
    }
};

struct Node {
    int id; double d;
    bool operator>(const Node& other) const { return d > other.d; }
};

int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    int n; if (!(cin >> n)) return 0;
    vector<Point> cubes(n);
    for (int i = 0; i < n; ++i) cin >> cubes[i].x >> cubes[i].y >> cubes[i].z;
    Point S, D; cin >> S.x >> S.y >> S.z >> D.x >> D.y >> D.z;

    map<Face, int> face_counts;
    for (int i = 0; i < n; ++i) {
        int x = (int)cubes[i].x, y = (int)cubes[i].y, z = (int)cubes[i].z;
        int vals[3] = {x, y, z};
        for (int a = 0; a < 3; ++a) {
            for (int s : {-1, 1}) {
                Face f; f.axis = a; f.coord = vals[a] + s * 50;
                int o[2], idx = 0;
                for (int b = 0; b < 3; ++b) if (a != b) o[idx++] = b;
                f.min1 = vals[o[0]] - 50; f.max1 = vals[o[0]] + 50;
                f.min2 = vals[o[1]] - 50; f.max2 = vals[o[1]] + 50;
                face_counts[f]++;
            }
        }
    }
    vector<Face> surface_faces;
    for (auto const& [face, count] : face_counts) if (count == 1) surface_faces.push_back(face);

    map<Point, int> pt_to_id; vector<Point> id_to_pt;
    auto get_id = [&](Point p) {
        auto it = pt_to_id.lower_bound(p);
        if (it != pt_to_id.end() && dist(it->first, p) < 1e-7) return it->second;
        pt_to_id[p] = id_to_pt.size(); id_to_pt.push_back(p);
        return pt_to_id[p];
    };

    int s_id = get_id(S), d_id = get_id(D);
    int K = 30;
    vector<vector<int>> face_nodes(surface_faces.size());
    for (int i = 0; i < (int)surface_faces.size(); ++i) {
        Face f = surface_faces[i];
        int o[2], idx = 0;
        for (int b = 0; b < 3; ++b) if (f.axis != b) o[idx++] = b;
        auto add_p = [&](double v[3]) { get_id({v[0], v[1], v[2]}); };
        double c[3]; c[f.axis] = f.coord;
        double c1[2] = {(double)f.min1, (double)f.max1}, c2[2] = {(double)f.min2, (double)f.max2};
        for(int j=0; j<2; ++j) for(int k=0; k<2; ++k) {
            c[o[0]] = c1[j]; c[o[1]] = c2[k]; add_p(c);
        }
        for(int k=1; k<K; ++k) {
            c[o[0]] = c1[0] + (c1[1]-c1[0])*k/(double)K; c[o[1]] = c2[0]; add_p(c);
            c[o[1]] = c2[1]; add_p(c);
            c[o[1]] = c2[0] + (c2[1]-c2[0])*k/(double)K; c[o[0]] = c1[0]; add_p(c);
            c[o[0]] = c1[1]; add_p(c);
        }
        for (Point p : {S, D}) {
            double cp[3]; cp[f.axis] = f.coord;
            if (f.axis == 0) { cp[1] = p.y; cp[2] = p.z; }
            else if (f.axis == 1) { cp[0] = p.x; cp[2] = p.z; }
            else { cp[0] = p.x; cp[1] = p.y; }
            if (abs(p.x-cp[0])<1e-7 && abs(p.y-cp[1])<1e-7 && abs(p.z-cp[2])<1e-7) {
                bool in = false;
                if (f.axis == 0) in = (p.y>=f.min1-1e-7 && p.y<=f.max1+1e-7 && p.z>=f.min2-1e-7 && p.z<=f.max2+1e-7);
                else if (f.axis == 1) in = (p.x>=f.min1-1e-7 && p.x<=f.max1+1e-7 && p.z>=f.min2-1e-7 && p.z<=f.max2+1e-7);
                else in = (p.x>=f.min1-1e-7 && p.x<=f.max1+1e-7 && p.y>=f.min2-1e-7 && p.y<=f.max2+1e-7);
                if (in) get_id(p);
            }
        }
        // Refresh face_nodes with actual IDs
        for (auto const& [p, id] : pt_to_id) {
            double v[3]; v[f.axis] = f.coord;
            if (f.axis == 0) { v[o[0]] = p.y; v[o[1]] = p.z; }
            else if (f.axis == 1) { v[o[0]] = p.x; v[o[1]] = p.z; }
            else { v[o[0]] = p.x; v[o[1]] = p.y; }
            if (abs(p.x-v[0])<1e-7 && abs(p.y-v[1])<1e-7 && abs(p.z-v[2])<1e-7 &&
                ((f.axis==0 ? (p.y>=f.min1-1e-7 && p.y<=f.max1+1e-7 && p.z>=f.min2-1e-7 && p.z<=f.max2+1e-7) :
                 (f.axis==1 ? (p.x>=f.min1-1e-7 && p.x<=f.max1+1e-7 && p.z>=f.min2-1e-7 && p.z<=f.max2+1e-7) :
                 (p.x>=f.min1-1e-7 && p.x<=f.max1+1e-7 && p.y>=f.min2-1e-7 && p.y<=f.max2+1e-7))))) {
                face_nodes[i].push_back(id);
            }
        }
    }

    int m = id_to_pt.size();
    vector<vector<pair<int, double>>> adj(m);
    for (int i = 0; i < (int)surface_faces.size(); ++i) {
        for (int j = 0; j < (int)face_nodes[i].size(); ++j) {
            for (int k = j + 1; k < (int)face_nodes[i].size(); ++k) {
                int u = face_nodes[i][j], v = face_nodes[i][k];
                double d = dist(id_to_pt[u], id_to_pt[v]);
                adj[u].push_back({v, d}); adj[v].push_back({u, d});
            }
        }
    }

    priority_queue<Node, vector<Node>, greater<Node>> pq;
    vector<double> min_d(m, 1e18);
    min_d[s_id] = 0; pq.push({s_id, 0});
    while (!pq.empty()) {
        Node top = pq.top(); pq.pop();
        if (top.d > min_d[top.id]) continue;
        if (top.id == d_id) break;
        for (auto& edge : adj[top.id]) {
            if (min_d[edge.first] > top.d + edge.second) {
                min_d[edge.first] = top.d + edge.second;
                pq.push({edge.first, min_d[edge.first]});
            }
        }
    }
    cout << fixed << setprecision(10) << min_d[d_id] << endl;
    return 0;
}

