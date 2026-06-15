#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Vertex {
    int x, y, z;
    bool operator==(const Vertex& o) const {
        return x == o.x && y == o.y && z == o.z;
    }
    bool operator<(const Vertex& o) const {
        if (x != o.x) return x < o.x;
        if (y != o.y) return y < o.y;
        return z < o.z;
    }
};

struct Triangle {
    int id;
    int layer;
    int face_order;
    int sum_x;
    int sum_y;
    int type;
    int u, v;
    Vertex v_pts[3];
    
    bool operator<(const Triangle& other) const {
        if (layer != other.layer) return layer < other.layer;
        if (face_order != other.face_order) return face_order < other.face_order;
        
        int dir1 = (face_order < 2) ? 1 : -1;
        int dir2 = (other.face_order < 2) ? 1 : -1;
        
        int val1 = dir1 * sum_x;
        int val2 = dir2 * other.sum_x;
        
        if (val1 != val2) return val1 < val2;
        if (sum_y != other.sum_y) return sum_y < other.sum_y;
        if (type != other.type) return type < other.type;
        if (u != other.u) return u < other.u;
        return v < other.v;
    }
};

void generate_face(int face_id, int n, vector<Triangle>& tris) {
    for (int u = 0; u < n; ++u) {
        for (int v = 0; u + v < n; ++v) {
            Vertex v1, v2, v3;
            if (face_id == 0) {
                v1 = {-u, -v, n - u - v};
                v2 = {-(u+1), -v, n - u - 1 - v};
                v3 = {-u, -(v+1), n - u - v - 1};
            } else if (face_id == 1) {
                v1 = {u, -v, n - u - v};
                v2 = {u+1, -v, n - u - 1 - v};
                v3 = {u, -(v+1), n - u - v - 1};
            } else if (face_id == 2) {
                v1 = {u, v, n - u - v};
                v2 = {u+1, v, n - u - 1 - v};
                v3 = {u, v+1, n - u - v - 1};
            } else if (face_id == 3) {
                v1 = {-u, v, n - u - v};
                v2 = {-(u+1), v, n - u - 1 - v};
                v3 = {-u, v+1, n - u - v - 1};
            } else if (face_id == 4) {
                v1 = {-u, -v, -(n - u - v)};
                v2 = {-(u+1), -v, -(n - u - 1 - v)};
                v3 = {-u, -(v+1), -(n - u - v - 1)};
            } else if (face_id == 5) {
                v1 = {u, -v, -(n - u - v)};
                v2 = {u+1, -v, -(n - u - 1 - v)};
                v3 = {u, -(v+1), -(n - u - v - 1)};
            } else if (face_id == 6) {
                v1 = {u, v, -(n - u - v)};
                v2 = {u+1, v, -(n - u - 1 - v)};
                v3 = {u, v+1, -(n - u - v - 1)};
            } else if (face_id == 7) {
                v1 = {-u, v, -(n - u - v)};
                v2 = {-(u+1), v, -(n - u - 1 - v)};
                v3 = {-u, v+1, -(n - u - v - 1)};
            }
            
            if (v2 < v1) swap(v1, v2);
            if (v3 < v2) swap(v2, v3);
            if (v2 < v1) swap(v1, v2);
            
            int layer = (face_id < 4) ? (u + v) : (2 * n - 1 - (u + v));
            tris.push_back({0, layer, face_id, v1.x+v2.x+v3.x, v1.y+v2.y+v3.y, 1, u, v, {v1, v2, v3}});
            
            if (u + v < n - 1) {
                Vertex tv1, tv2, tv3;
                if (face_id == 0) {
                    tv1 = {-(u+1), -v, n - u - 1 - v};
                    tv2 = {-(u+1), -(v+1), n - u - v - 2};
                    tv3 = {-u, -(v+1), n - u - v - 1};
                } else if (face_id == 1) {
                    tv1 = {u+1, -v, n - u - 1 - v};
                    tv2 = {u+1, -(v+1), n - u - v - 2};
                    tv3 = {u, -(v+1), n - u - v - 1};
                } else if (face_id == 2) {
                    tv1 = {u+1, v, n - u - 1 - v};
                    tv2 = {u+1, v+1, n - u - v - 2};
                    tv3 = {u, v+1, n - u - v - 1};
                } else if (face_id == 3) {
                    tv1 = {-(u+1), v, n - u - 1 - v};
                    tv2 = {-(u+1), v+1, n - u - v - 2};
                    tv3 = {-u, v+1, n - u - v - 1};
                } else if (face_id == 4) {
                    tv1 = {-(u+1), -v, -(n - u - 1 - v)};
                    tv2 = {-(u+1), -(v+1), -(n - u - v - 2)};
                    tv3 = {-u, -(v+1), -(n - u - v - 1)};
                } else if (face_id == 5) {
                    tv1 = {u+1, -v, -(n - u - 1 - v)};
                    tv2 = {u+1, -(v+1), -(n - u - v - 2)};
                    tv3 = {u, -(v+1), -(n - u - v - 1)};
                } else if (face_id == 6) {
                    tv1 = {u+1, v, -(n - u - 1 - v)};
                    tv2 = {u+1, v+1, -(n - u - v - 2)};
                    tv3 = {u, v+1, -(n - u - v - 1)};
                } else if (face_id == 7) {
                    tv1 = {-(u+1), v, -(n - u - 1 - v)};
                    tv2 = {-(u+1), v+1, -(n - u - v - 2)};
                    tv3 = {-u, v+1, -(n - u - v - 1)};
                }
                
                if (tv2 < tv1) swap(tv1, tv2);
                if (tv3 < tv2) swap(tv2, tv3);
                if (tv2 < tv1) swap(tv1, tv2);
                
                tris.push_back({0, layer, face_id, tv1.x+tv2.x+tv3.x, tv1.y+tv2.y+tv3.y, 2, u, v, {tv1, tv2, tv3}});
            }
        }
    }
}

long long solve(int total_nodes, const vector<vector<int>>& adj, const vector<bool>& is_white, vector<bool>& matched) {
    int u = -1;
    int min_deg = 4;
    for (int i = 0; i < total_nodes; ++i) {
        if (is_white[i] && !matched[i]) {
            int deg = 0;
            for (int v : adj[i]) {
                if (is_white[v] && !matched[v]) deg++;
            }
            if (deg == 0) return 0;
            if (deg < min_deg) {
                min_deg = deg;
                u = i;
            }
        }
    }
    if (u == -1) return 1;

    long long ways = 0;
    matched[u] = true;
    for (int v : adj[u]) {
        if (is_white[v] && !matched[v]) {
            matched[v] = true;
            ways += solve(total_nodes, adj, is_white, matched);
            matched[v] = false;
        }
    }
    matched[u] = false;
    return ways;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    if (!(cin >> n)) return 0;
    
    vector<string> layers(2 * n);
    for (int i = 0; i < 2 * n; ++i) {
        cin >> layers[i];
    }
    
    vector<Triangle> tris;
    for (int i = 0; i < 8; ++i) {
        generate_face(i, n, tris);
    }
    
    vector<vector<int>> adj(tris.size());
    for (int i = 0; i < tris.size(); ++i) {
        tris[i].id = i;
        for (int j = i + 1; j < tris.size(); ++j) {
            int shared = 0;
            for (int a = 0; a < 3; ++a) {
                for (int b = 0; b < 3; ++b) {
                    if (tris[i].v_pts[a].x == tris[j].v_pts[b].x &&
                        tris[i].v_pts[a].y == tris[j].v_pts[b].y &&
                        tris[i].v_pts[a].z == tris[j].v_pts[b].z) {
                        shared++;
                    }
                }
            }
            if (shared == 2) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
        }
    }
    
    sort(tris.begin(), tris.end());
    
    vector<int> old_to_new(tris.size());
    for (int i = 0; i < tris.size(); ++i) {
        old_to_new[tris[i].id] = i;
    }
    
    vector<vector<int>> new_adj(tris.size());
    for (int i = 0; i < tris.size(); ++i) {
        for (int neighbor : adj[i]) {
            new_adj[old_to_new[i]].push_back(old_to_new[neighbor]);
        }
    }
    adj = move(new_adj);
    
    vector<bool> is_white(tris.size(), false);
    int current_idx = 0;
    for (int i = 0; i < 2 * n; ++i) {
        for (int j = 0; j < layers[i].size(); ++j) {
            if (layers[i][j] == '.') {
                is_white[current_idx] = true;
            }
            current_idx++;
        }
    }
    
    int white_count = 0;
    for (int i = 0; i < tris.size(); ++i) {
        if (is_white[i]) white_count++;
    }
    
    if (white_count % 2 != 0) {
        cout << 0 << "\n";
        return 0;
    }
    
    vector<bool> matched(tris.size(), false);
    cout << solve(tris.size(), adj, is_white, matched) << "\n";
    
    return 0;
}
