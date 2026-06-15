#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

const long long INF = 1e15;

struct Subtree {
    long long weight;
    vector<int> nodes;
};

int n;
int D1, C1, D2, C2;
int p[205];
vector<int> adj[205];

void get_dist(int u, int p_node, int d, vector<int>& dists) {
    dists[u] = d;
    for (int v : adj[u]) {
        if (v != p_node) get_dist(v, u, d + 1, dists);
    }
}

vector<Subtree> find_all_valid_subtrees(int D, int C) {
    vector<Subtree> valid_subtrees;
    if (D == 0) {
        for (int i = 1; i <= n; ++i) {
            valid_subtrees.push_back({(long long)p[i], {i}});
        }
        return valid_subtrees;
    }

    if (D % 2 == 0) {
        int R = D / 2;
        for (int u = 1; u <= n; ++u) {
            vector<int> dist_u(n + 1);
            get_dist(u, 0, 0, dist_u);
            
            vector<vector<int>> branches;
            for (int v : adj[u]) {
                vector<int> branch_nodes;
                queue<pair<int, int>> q;
                q.push({v, u});
                while (!q.empty()) {
                    auto curr = q.front(); q.pop();
                    int node = curr.first, parent = curr.second;
                    if (dist_u[node] <= R) {
                        branch_nodes.push_back(node);
                        for (int neighbor : adj[node]) {
                            if (neighbor != parent && dist_u[neighbor] <= R) {
                                q.push({neighbor, node});
                            }
                        }
                    }
                }
                if (!branch_nodes.empty()) branches.push_back(branch_nodes);
            }

            vector<vector<pair<long long, vector<int>>>> branch_data(branches.size());
            for (int i = 0; i < branches.size(); ++i) {
                // dp[v][k]: max weight of subtree rooted at v with k nodes at distance R
                // To find this, we can use a simple DP on the branch
                // However, since n is small, we can do it simpler.
                // For a fixed branch, we want to pick a subset of nodes that form a connected subtree
                // containing the node adjacent to u, with k nodes at dist R.
                // Since we only need the best for each k, we can use DP.
                // For each node v in the branch, compute its max weight subtree.
                
                // First, find which nodes in this branch are at distance R from u
                // and which are not.
                // branch_data[i][k] = {max_weight, nodes}
                // This is still complex. Let's use a simpler DP for each branch.
                // We need to find for each k in [0, C], the max weight subtree in this branch
                // that is connected to u and has k nodes at dist R.
                
                // Let dp[v][k] be the max weight of a subtree rooted at v with k nodes at distance R.
                // For each branch, we only care about k from 0 to C.
                // The branch starts at the neighbor of u.
                
                // Let's re-root the branch at the neighbor of u.
                int root = -1;
                for(int v : adj[u]) {
                    bool in_branch = false;
                    for(int bn : branches[i]) if(bn == v) in_branch = true;
                    if(in_branch) { root = v; break; }
                }
                
                // dp[v][k] where v is in branch, k is number of nodes in v's subtree at dist R from u
                // We use a vector of maps or vectors for the DP.
                // Since n is small, vector<pair<long long, vector<int>>> is okay.
                
                auto solve_branch = [&](int r, int parent_u) {
                    vector<vector<pair<long long, vector<int>>>> dp(n + 1, vector<pair<long long, vector<int>>>(C + 1, {-INF, {}}));
                    
                    auto dfs = [&](auto self, int v, int p_v) -> void {
                        int dist_v = dist_u[v];
                        int k_v = (dist_v == R ? 1 : 0);
                        
                        if (k_v <= C) {
                            dp[v][k_v] = {(long long)p[v], {v}};
                        }
                        
                        for (int neighbor : adj[v]) {
                            if (neighbor != p_v && dist_u[neighbor] <= R) {
                                self(self, neighbor, v);
                                vector<vector<pair<long long, vector<int>>>> next_dp(C + 1, {-INF, {}});
                                for (int k1 = 0; k1 <= C; ++k1) {
                                    if (dp[v][k1].first <= -INF) continue;
                                    // Option 1: Don't include neighbor
                                    if (dp[v][k1].first > next_dp[k1].first) next_dp[k1] = dp[v][k1];
                                    // Option 2: Include neighbor
                                    for (int k2 = 0; k1 + k2 <= C; ++k2) {
                                        if (dp[neighbor][k2].first <= -INF) continue;
                                        long long new_w = dp[v][k1].first + dp[neighbor][k2].first;
                                        if (new_w > next_dp[k1 + k2].first) {
                                            vector<int> new_nodes = dp[v][k1].second;
                                            new_nodes.insert(new_nodes.end(), dp[neighbor][k2].second.begin(), dp[neighbor][k2].second.end());
                                            next_dp[k1 + k2] = {new_w, new_nodes};
                                        }
                                    }
                                }
                                dp[v] = next_dp;
                            }
                        }
                    };
                    dfs(dfs, r, parent_u);
                    return dp;
                };

                auto branch_dp = solve_branch(root, u);
                for (int k = 0; k <= C; ++k) {
                    branch_data[i].push_back(branch_dp[root][k]);
                }
            }

            // Now combine branches using DP.
            // dp_comb[i][k][num_large] = max weight using first i branches, total k endpoints,
            // and num_large branches have k_i > 0.
            // Since we only need num_large >= 2, we can just use num_large = 0, 1, or 2 (where 2 means >=2).
            vector<vector<pair<long long, vector<int>>>> dp_comb(C + 1, vector<pair<long long, vector<int>>>(3, {-INF, {}}));
            dp_comb[0][0] = { (long long)p[u], {u} };

            for (int i = 0; i < branches.size(); ++i) {
                vector<vector<pair<long long, vector<int>>>> next_dp(C + 1, vector<pair<long long, vector<int>>>(3, {-INF, {}}));
                for (int k = 0; k <= C; ++k) {
                    for (int l = 0; l < 3; ++l) {
                        if (dp_comb[k][l].first <= -INF) continue;

                        // Option 1: Don't include this branch (k_i = 0, num_large doesn't change)
                        // But a "small" branch must be connected to u. Wait, if k_i=0, the branch is small.
                        // If k_i=0, the branch could be a subtree with max_dist < R.
                        // To make it connected to u, it MUST include the neighbor.
                        // So "not including this branch" means k_i=0 and we don't add any nodes.
                        if (dp_comb[k][l].first > next_dp[k][l].first) next_dp[k][l] = dp_comb[k][l];

                        // Option 2: Include this branch with k_i > 0
                        for (int ki = 1; k + ki <= C; ++ki) {
                            if (branch_data[i][ki].first <= -INF) continue;
                            int nl = min(2, l + 1);
                            long long nw = dp_comb[k][l].first + branch_data[i][ki].first;
                            if (nw > next_dp[k + ki][nl].first) {
                                vector<int> nn = dp_comb[k][l].second;
                                nn.insert(nn.end(), branch_data[i][ki].second.begin(), branch_data[i][ki].second.end());
                                next_dp[k + ki][nl] = {nw, nn};
                            }
                        }
                        // Option 3: Include this branch with k_i = 0, but it is "small" (max_dist < R)
                        // Actually, if k_i=0, it's not "large", so l doesn't change.
                        // We need to find the max weight of a "small" subtree in this branch.
                        // Let's find it by searching branch_data[i][0].
                        // But we also need to consider that a small subtree might not reach dist R.
                        // In our solve_branch, branch_data[i][0] is the max weight subtree with k=0.
                        // This subtree's max distance is < R. 
                        // If k_i=0 and it's NOT empty, it's still not a "large" branch.
                        // Let's find the max weight for k_i = 0.
                        if (branch_data[i][0].first > -INF) {
                            int nl = l;
                            long long nw = dp_comb[k][l].first + branch_data[i][0].first;
                            if (nw > next_dp[k][nl].first) {
                                vector<int> nn = dp_comb[k][l].second;
                                nn.insert(nn.end(), branch_data[i][0].second.begin(), branch_data[i][0].second.end());
                                next_dp[k][nl] = {nw, nn};
                            }
                        }
                    }
                }
                dp_comb = next_dp;
            }

            for (int k = 0; k <= C; ++k) {
                if (dp_comb[k][2].first > -INF) {
                    valid_subtrees.push_back({dp_comb[k][2].first, dp_comb[k][2].second});
                }
            }
        }
    } else { // D is odd, D = 2R + 1
        int R = D / 2;
        for (int u = 1; u <= n; ++u) {
            for (int v : adj[u]) {
                if (u < v) {
                    // Center is edge (u, v)
                    // S = S_u \cup S_v where S_u \subseteq T_u, S_v \subseteq T_v
                    // and max_dist(u, S_u) = R, max_dist(v, S_v) = R
                    
                    auto solve_side = [&](int root, int other, int target_R) {
                        vector<vector<pair<long long, vector<int>>>> dp(n + 1, vector<pair<long long, vector<int>>>(C + 1, {-INF, {}}));
                        vector<int> dist_root(n + 1);
                        get_dist(root, other, 0, dist_root);

                        auto dfs = [&](auto self, int curr, int p_curr) -> void {
                            int k_curr = (dist_root[curr] == target_R ? 1 : 0);
                            if (k_curr <= C) dp[curr][k_curr] = {(long long)p[curr], {curr}};

                            for (int neighbor : adj[curr]) {
                                if (neighbor != p_curr && dist_root[neighbor] <= target_R) {
                                    self(self, neighbor, curr);
                                    vector<pair<long long, vector<int>>> next_dp_curr(C + 1, {-INF, {}});
                                    for (int k1 = 0; k1 <= C; ++k1) {
                                        if (dp[curr][k1].first <= -INF) continue;
                                        if (dp[curr][k1].first > next_dp_curr[k1].first) next_dp_curr[k1] = dp[curr][k1];
                                        for (int k2 = 0; k1 + k2 <= C; ++k2) {
                                            if (dp[neighbor][k2].first <= -INF) continue;
                                            long long nw = dp[curr][k1].first + dp[neighbor][k2].first;
                                            if (nw > next_dp_curr[k1 + k2].first) {
                                                vector<int> nn = dp[curr][k1].second;
                                                nn.insert(nn.end(), dp[neighbor][k2].second.begin(), dp[neighbor][k2].second.end());
                                                next_dp_curr[k1 + k2] = {nw, nn};
                                            }
                                        }
                                    }
                                    dp[curr] = next_dp_curr;
                                }
                            }
                        };
                        dfs(dfs, root, other);
                        return dp;
                    };

                    auto dp_u = solve_side(u, v, R);
                    auto dp_v = solve_side(v, u, R);

                    for (int ku = 1; ku <= C; ++ku) {
                        if (dp_u[u][ku].first <= -INF) continue;
                        for (int kv = 1; ku + kv <= C; ++kv) {
                            if (dp_v[v][kv].first <= -INF) continue;
                            long long total_w = dp_u[u][ku].first + dp_v[v][kv].first;
                            vector<int> total_nodes = dp_u[u][ku].second;
                            total_nodes.insert(total_nodes.end(), dp_v[v][kv].second.begin(), dp_v[v][kv].second.end());
                            valid_subtrees.push_back({total_w, total_nodes});
                        }
                    }
                }
            }
        }
    }
    return valid_subtrees;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n)) return 0;
    cin >> D1 >> C1 >> D2 >> C2;
    for (int i = 1; i <= n; ++i) cin >> p[i];
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<Subtree> S1s = find_all_valid_subtrees(D1, C1);
    vector<Subtree> S2s = find_all_valid_subtrees(D2, C2);

    if (S1s.empty() || S2s.empty()) {
        cout << -1 << endl;
        return 0;
    }

    // To optimize, for each S2, we only care about the best S1.
    // We can pre-calculate max weight S1 for each component.
    // A component of X2 is a subtree T_{v,u} where u is forbidden and v is not.
    
    // Pre-calculate max weight S1 for each possible component T_{v,u}.
    // T_{v,u} is the component containing v when edge (u,v) is removed.
    vector<pair<long long, vector<int>>> max_S1_in_comp(n * n * 2, {-INF, {}}); 
    auto get_comp_idx = [&](int v, int u) {
        // Find index for T_{v,u}
        int idx = 0;
        for(int i=1; i<=n; ++i) {
            for(int j : adj[i]) {
                if (i == v && j == u) return idx;
                idx++;
            }
        }
        return -1;
    };
    
    // A better way to index components: (v, u)
    // We can use a 2D array max_S1[v][u]
    vector<vector<pair<long long, vector<int>>>> max_S1_comp(n + 1, vector<pair<long long, vector<int>>>(n + 1, {-INF, {}}));

    for (auto& s1 : S1s) {
        vector<bool> in_s1(n + 1, false);
        for (int node : s1.nodes) in_s1[node] = true;

        for (int u = 1; u <= n; ++u) {
            for (int v : adj[u]) {
                // Check if S1 is in T_{v,u}
                // S1 is in T_{v,u} iff edge (u,v) is not used by S1.
                // Since S1 is connected, this means all nodes in S1 are on the v-side.
                // We can check if u is in S1. If u is in S1, then S1 is NOT in T_{v,u}.
                // If u is not in S1, we need to check if any node in S1 can reach u without using v.
                // Actually, if u is not in S1, we can check if v is on the path from any node in S1 to u.
                // But if S1 is connected and u is not in S1, then u is in some other component.
                // So S1 is in T_{v,u} iff u is not in S1 and for all nodes in S1, the path to u goes through v.
                // Wait, if S1 is in T_{v,u}, then for all node s in S1, the path from s to u must pass through v.
                // This means v must be on the path from every s in S1 to u.
                // Let's just use BFS to find all nodes in T_{v,u}.
                
                // For each edge (u,v), find all nodes in T_{v,u}
                // This is $O(n^2)$ edges. Total $O(n^3)$ to find all T_{v,u}.
            }
        }
    }
    
    // Let's re-do the pre-calculation more efficiently.
    // For each S1, its nodes form a connected component.
    // For any edge (u,v), S1 is in T_{v,u} iff u is not in S1 and all nodes in S1 are in T_{v,u}.
    // A simpler way: S1 is in T_{v,u} iff v is in S1's component when (u,v) is removed.
    // Since S1 is connected, this is equivalent to saying u is not in S1 and v is the neighbor of u
    // that leads to S1. 
    // Even simpler: S1 is in T_{v,u} iff for all s in S1, dist(s, v) < dist(s, u).
    
    for (auto& s1 : S1s) {
        vector<bool> in_s1(n + 1, false);
        for (int node : s1.nodes) in_s1[node] = true;
        for (int u = 1; u <= n; ++u) {
            if (in_s1[u]) continue;
            for (int v : adj[u]) {
                // Check if all s in S1 are in T_{v,u}
                // This is true if v is the neighbor of u on the path to all s in S1.
                // Let's pick any s in S1, and check if v is on the path from s to u.
                // And also check if all other s are also in T_{v,u}.
                bool all_in = true;
                // We can use a BFS/DFS to find all nodes in T_{v,u}
                // But since we do this for each s1, it's O(n^4).
                // Let's pre-calculate T_{v,u} for all (v,u).
                // (Already done below)
            }
        }
    }
    
    // Corrected Pre-calculation:
    vector<vector<vector<int>>> T(n + 1, vector<vector<int>>(n + 1));
    for (int u = 1; u <= n; ++u) {
        for (int v : adj[u]) {
            // T[v][u] is the component containing v when (u,v) is removed
            vector<int> component;
            queue<int> q;
            q.push(v);
            vector<bool> visited(n + 1, false);
            visited[u] = true;
            visited[v] = true;
            component.push_back(v);
            while (!q.empty()) {
                int curr = q.front(); q.pop();
                for (int neighbor : adj[curr]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        component.push_back(neighbor);
                        q.push(neighbor);
                    }
                }
            }
            T[v][u] = component;
        }
    }

    for (auto& s1 : S1s) {
        vector<bool> in_s1(n + 1, false);
        for (int node : s1.nodes) in_s1[node] = true;
        for (int u = 1; u <= n; ++u) {
            if (in_s1[u]) continue;
            for (int v : adj[u]) {
                // Is S1 \subseteq T[v][u]?
                bool all_in = true;
                // We need to check if all nodes in S1 are in T[v][u].
                // A node is in T[v][u] if its distance to v is less than its distance to u.
                // Actually, we can just check if the first node of S1 is in T[v][u]
                // and if the number of nodes in S1 \cap T[v][u] is equal to |S1|.
                // But since S1 is connected, we only need to check if s1.nodes[0] is in T[v][u]
                // and that the path from s1.nodes[0] to any other node in S1 doesn't pass through u.
                // Since u is not in S1, and S1 is connected, this is always true.
                // So we just need to check if s1.nodes[0] is in T[v][u].
                
                // Let's use a more robust check:
                // S1 \subseteq T[v][u] iff u is not in S1 and v is on the path from u to any node in S1.
                // Since S1 is connected, we just need to check if the first node is in T[v][u].
                // But wait, if u is not in S1, and S1 is connected, then S1 is entirely in some 
                // component of T \ {u}. That component is T[v][u] for some neighbor v of u.
                // So we just need to find which neighbor v of u is in the component containing S1.
                // If we find such a v, then S1 \subseteq T[v][u].
                
                // Let's use: S1 \subseteq T[v][u] iff the first node of S1 is in T[v][u].
                // Wait, a node x is in T[v][u] iff it's in the component of T \ {u} containing v.
                // Let's use a pre-calculated membership.
            }
        }
    }

    // Let's use a simpler way for membership: membership[u][v][x] is true if x \in T[v][u]
    // This is O(n^3).
    static bool member[205][205][205];
    for (int u = 1; u <= n; ++u) {
        for (int v : adj[u]) {
            for (int x : T[v][u]) member[u][v][x] = true;
        }
    }

    for (auto& s1 : S1s) {
        vector<bool> in_s1(n + 1, false);
        for (int node : s1.nodes) in_s1[node] = true;
        for (int u = 1; u <= n; ++u) {
            if (in_s1[u]) continue;
            for (int v : adj[u]) {
                bool all_in = true;
                for (int s_node : s1.nodes) {
                    if (!member[u][v][s_node]) {
                        all_in = false;
                        break;
                    }
                }
                if (all_in) {
                    if (s1.weight > max_S1_comp[v][u].first) {
                        max_S1_comp[v][u] = s1;
                    }
                }
            }
        }
    }

    long long max_total_weight = -1;
    Subtree best_s1, best_s2;

    for (auto& s2 : S2s) {
        vector<bool> forbidden(n + 1, false);
        for (int node : s2.nodes) {
            forbidden[node] = true;
            for (int neighbor : adj[node]) forbidden[neighbor] = true;
        }

        for (int u = 1; u <= n; ++u) {
            if (forbidden[u]) {
                for (int v : adj[u]) {
                    if (!forbidden[v]) {
                        // The component is T[v][u].
                        if (max_S1_comp[v][u].first > -INF) {
                            if (s2.weight + max_S1_comp[v][u].first > max_total_weight) {
                                max_total_weight = s2.weight + max_S1_comp[v][u].first;
                                best_s1 = max_S1_comp[v][u];
                                best_s2 = s2;
                            }
                        }
                    }
                }
            }
        }
        // Also check if there are any components of X2 that are not adjacent to forbidden nodes.
        // This happens if S2 is such that some nodes are not in V2 or adj(V2).
        // But in a tree, all components of X2 will be T[v][u] for some edge (u,v) where u is forbidden and v is not.
        // What if X2 has a component that is not attached to any forbidden node?
        // This only happens if the whole tree is X2, but that would mean V2 and adj(V2) are empty.
    }
    
    // One special case: X2 could be the entire tree (if S2 is empty, but S2 is not empty).
    // Wait, if S2 is not empty, then V2 or adj(V2) is not empty, so every component of X2 
    // is adjacent to some forbidden node.
    // Exception: If n is such that all nodes are in V2 or adj(V2).
    // One more: if X2 is not empty, but no component of X2 is T[v][u].
    // This could only happen if there are no forbidden-to-nonforbidden edges.
    // But if there's at least one forbidden node and one non-forbidden node, there must be such an edge.
    // If all nodes are forbidden, X2 is empty.
    // If no nodes are forbidden, X2 is the whole tree. But S2 is not empty, so there's at least one forbidden node.

    if (max_total_weight == -1) {
        cout << -1 << endl;
    } else {
        vector<bool> in_s12(n + 1, false);
        for (int node : best_s1.nodes) in_s12[node] = true;
        for (int node : best_s2.nodes) in_s12[node] = true;

        vector<int> destroyed;
        long long total_cost = 0;
        for (int i = 1; i <= n; ++i) {
            if (!in_s12[i]) {
                destroyed.push_back(i);
                total_cost += p[i];
            }
        }
        cout << total_cost << endl;
        for (int i = 0; i < destroyed.size(); ++i) {
            cout << destroyed[i] << (i == destroyed.size() - 1 ? "" : " ");
        }
        cout << endl;
    }

    return 0;
}

