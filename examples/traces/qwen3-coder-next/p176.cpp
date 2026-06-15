#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <cstring>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int from, to;
    long long cap, flow, required;
    int idx;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<Edge> edges(M);
    vector<int> must_fill(M);

    // Build adjacency list for graph (to find paths and flows)
    vector<vector<int>> adj(N + 1);

    for (int i = 0; i < M; i++) {
        cin >> edges[i].from >> edges[i].to >> edges[i].cap >> edges[i].required;
        edges[i].idx = i;
        edges[i].flow = 0;
        must_fill[i] = edges[i].required;
        adj[edges[i].from].push_back(i);
    }

    // Step 1: Check feasibility: must_fill edges must be covered by some flow
    // We can think of this as a circulation problem with lower bounds.

    // Let L(e) = 1 if must_fill[e] = 1, else 0.
    // Let U(e) = cap(e)

    // We want to find a flow f such that:
    // - For each edge e: L(e) <= f(e) <= U(e)
    // - Flow conservation at all nodes except source (1) and sink (N)
    // - net outflow from source = net inflow to sink = F (to minimize F)

    // Transform to circulation with lower bounds:
    // For each edge e: f'(e) = f(e) - L(e), so 0 <= f'(e) <= U(e) - L(e)
    // Then for each node v, define:
    //   D(v) = sum_{in edges e} L(e) - sum_{out edges e} L(e)
    // For source node 1: D(1) = - (sum of L(e) for e out of 1)
    // For sink node N: D(N) = (sum of L(e) for e into N)
    // For other nodes: D(v) = (in demand) - (out demand) of lower bounds.

    // Add a super source SS and super sink TT.
    // For each node v with D(v) > 0: add edge from SS to v with capacity D(v)
    // For each node v with D(v) < 0: add edge from v to TT with capacity -D(v)
    // Also add edge from N to 1 with infinite capacity (to allow circulation)

    // Then check if max flow from SS to TT saturates all edges from SS.

    // But note: we also want to minimize the total flow F from 1 to N.

    // Alternative approach: Since the problem asks for minimal production speed at node 1,
    // we can binary search on F (the total flow from 1 to N).

    // For a given F, check if there exists a flow such that:
    //   - flow from 1 to N is exactly F (or at least F, but we want minimal so equality is fine)
    //   - for each edge e: if must_fill[e] then f(e) >= 1, else f(e) >= 0
    //   - f(e) <= cap(e)
    //   - flow conservation at intermediate nodes

    // We can model this as a flow problem with lower bounds:
    //   l(e) = must_fill[e] ? 1 : 0
    //   u(e) = cap(e)
    //   We require: 
    //       net_out(1) = F
    //       net_in(N) = F
    //       net_out(v) = 0 for v not in {1, N}

    // How to check for a fixed F?
    // We can add an edge from N to 1 with capacity F (to convert to circulation)
    // Then we have:
    //   l'(e) = l(e) for original edges, and l'(N->1) = 0
    //   u'(e) = u(e) for original, u'(N->1) = F
    // Then check if there's a feasible circulation with lower bounds.

    // Standard method: subtract lower bounds, create D(v), then connect to super source/sink.

    // Since N <= 100, M <= 100*99/2 (but given M, so at most 100*100?), we can do binary search on F.

    // What is the range of F?
    // Lower bound: max_{e: must_fill[e]} l(e) = number of must-fill edges that are on any path? Actually, minimal F must be at least max( sum of must_fill on any path? ) but easier: 
    //   F_min = 0
    //   F_max: sum of capacities of edges out of source (or into sink) but also must fill some edges, so F_max <= sum of capacities of edges out of 1.

    // Since capacities up to 1e5 and M <= 100, F_max <= 100 * 1e5 = 1e7.

    // But 1e7 * (running max flow on graph with N+2 nodes and M+1 edges) might be too heavy: max flow algorithm (Dinic) on unit capacity? Actually capacities are up to 1e5, but graph is small (N<=100, M<=100? but M given, up to 10000? but problem says M is given and N<=100, so M <= 100*99 = 9900? but problem says "any pair ... only one pipe", so M <= N*(N-1)/2 <= 4950). But 1e7 * (Dinic on 102 nodes, 10000 edges) -> Dinic on such graph is O(V^2 * E) = 102^2 * 10000 ~ 100e6 per test? and 1e7 * 100e6 is 1e15, too slow.

    // Alternative: we can avoid binary search by using a technique for minimum flow with lower bounds.

    // Standard approach for minimum flow from s to t with lower bounds:
    //   1. Add edge from t to s with capacity INF.
    //   2. Find feasible circulation (with lower bounds) in the new graph.
    //   3. Then the minimum flow from s to t is the flow on the edge t->s in the feasible circulation.

    // Steps for feasible circulation with lower bounds:
    //   Let l(e), u(e) for each edge e.
    //   Define D(v) = sum_{e in in(v)} l(e) - sum_{e in out(v)} l(e)
    //   Add super source SS and super sink TT.
    //   For each v: 
    //        if D(v) > 0: add edge SS->v with capacity D(v)
    //        if D(v) < 0: add edge v->TT with capacity -D(v)
    //   Also add edge t->s with capacity INF (in our case, N->1 with capacity INF)
    //   Then compute max flow from SS to TT. If the flow saturates all edges from SS (i.e., flow value = sum_{D(v)>0} D(v)), then feasible.

    // Then, to get the minimum flow from s to t, we do:
    //   Let f0 be the flow on the edge t->s in the feasible circulation.
    //   Then the minimum flow from s to t is f0.
    // Why? Because the circulation must have flow f0 on t->s, which corresponds to a flow of f0 from s to t in the original graph.

    // However, note: in our problem, the lower bounds are not 0 for all edges, but l(e)=1 for must-fill edges.

    // Steps:
    //   Step 1: Build the graph with lower bounds.
    //   Step 2: Create extended graph with SS and TT.
    //   Step 3: Check feasibility of circulation (with the added edge N->1, capacity INF).
    //   Step 4: If feasible, then the flow on the edge N->1 is the minimal production speed.

    // But note: the problem says "substance can't be accumulated", so flow conservation at all nodes except 1 and N. However, by adding the edge N->1, we make it a circulation.

    // How to recover the flow on each original edge?
    //   In the feasible circulation, the flow on original edges is f(e) = f'(e) + l(e), where f'(e) is the flow in the reduced graph (without lower bounds).

    // Implementation:
    //   We'll create a new graph for max flow (with SS and TT).
    //   Nodes: 0 (SS), 1..N (original), N+1 (TT)
    //   Total nodes = N+2.

    // Let's define:
    //   lower[e] = must_fill[e] ? 1 : 0
    //   We'll create an array D of size N+2 (indexed 1..N) for the imbalance.

    vector<long long> D(N+1, 0); // D[1..N]

    // We'll also need to build the graph for the max flow problem (the extended graph for circulation check)
    // We'll use Dinic's algorithm.

    // First, compute D:
    for (int i = 0; i < M; i++) {
        int u = edges[i].from, v = edges[i].to;
        long long low = must_fill[i] ? 1 : 0;
        D[u] -= low;
        D[v] += low;
    }

    // Now, we add an edge from N to 1 with capacity INF (to allow circulation)
    // But note: in the lower bound transformation, we don't need to add this edge's lower bound? 
    // Actually, for the circulation, we set the edge N->1 to have l=0, u=INF.
    // So we don't change D for that edge.

    // Now, build the graph for max flow (only for the circulation check with lower bounds):

    // We'll create a graph for Dinic with N+2 nodes (0 = SS, 1..N, N+1 = TT)

    struct DinicEdge {
        int to, rev;
        long long cap, flow;
        bool is_original; // not necessary, but for clarity
    };

    vector<vector<DinicEdge>> g(N+2);
    auto add_edge = [&](int from, int to, long long cap) {
        DinicEdge e1 = {to, (int)g[to].size(), cap, 0, false};
        DinicEdge e2 = {from, (int)g[from].size(), 0, 0, false};
        g[from].push_back(e1);
        g[to].push_back(e2);
    };

    long long total_demand = 0;
    for (int i = 1; i <= N; i++) {
        if (D[i] > 0) {
            total_demand += D[i];
            add_edge(0, i, D[i]); // SS -> i
        } else if (D[i] < 0) {
            add_edge(i, N+1, -D[i]); // i -> TT
        }
    }

    // Add the edge from N to 1 with capacity INF (for circulation)
    add_edge(N, 1, INF);

    // Now run max flow from SS=0 to TT=N+1
    auto dinic = [&](int s, int t) -> long long {
        vector<int> level(N+2), ptr(N+2);
        auto bfs = [&]() -> bool {
            fill(level.begin(), level.end(), -1);
            queue<int> q;
            q.push(s);
            level[s] = 0;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (const auto& e : g[u]) {
                    if (e.cap - e.flow > 0 && level[e.to] == -1) {
                        level[e.to] = level[u] + 1;
                        q.push(e.to);
                    }
                }
            }
            return level[t] != -1;
        };

        long long push_flow = [&](int u, int t, long long flow) {
            if (u == t) return flow;
            for (int& i = ptr[u]; i < (int)g[u].size(); i++) {
                auto& e = g[u][i];
                if (e.cap - e.flow > 0 && level[e.to] == level[u] + 1) {
                    long long pushed = push_flow(e.to, t, min(flow, e.cap - e.flow));
                    if (pushed > 0) {
                        e.flow += pushed;
                        g[e.to][e.rev].flow -= pushed;
                        return pushed;
                    }
                }
            }
            return 0LL;
        };

        long long flow = 0;
        while (bfs()) {
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = push_flow(s, t, INF)) {
                flow += pushed;
            }
        }
        return flow;
    };

    long long max_flow_value = dinic(0, N+1);

    // Check if feasible: max_flow_value must equal total_demand
    if (max_flow_value != total_demand) {
        cout << "Impossible" << endl;
        return 0;
    }

    // Now, the flow on the edge from N to 1 (which we added) is the minimal production speed.
    // How to get the flow on that edge?
    // In our graph, the edge from N to 1 is stored in g[N] and g[1].
    // We added it as: 
    //   g[N].push_back( {1, index_in_g[1], INF, 0} )
    //   and g[1].push_back( {N, index_in_g[N], 0, 0} )
    // So we look at the edge from N to 1: it's the last edge we added to g[N] (but we added other edges too).
    // Instead, we can remember the index.

    // Alternatively, we can compute: the flow on the edge N->1 is the flow that went through that edge.
    // Since we set capacity INF, the flow is at most the flow that was pushed.

    // Actually, in the residual graph, the flow on the edge N->1 is the reverse edge's flow (in g[1] for the edge pointing to N) but negative? 

    // Better: when we added the edge from N to 1, we know the index in g[N] and g[1]. But we didn't store it.

    // Instead, we can compute the flow on the edge N->1 by: 
    //   The flow from N to 1 = the flow that we pushed in the edge N->1.
    //   How? In the Dinic algorithm, the flow on the edge from N to 1 is stored in the forward edge's flow variable.
    //   But we don't have a pointer to it.

    // Alternative: the flow on the edge N->1 is equal to the total flow that left N to 1, which we can get by looking at the net flow from N to 1.
    //   However, note: in the circulation, the flow from N to 1 is the same as the flow from 1 to N in the original graph (but in reverse).
    //   Actually, the flow on the edge N->1 in the circulation is the amount of flow that we are sending from N back to 1, which corresponds to the flow from 1 to N in the original network.

    // How about: 
    //   We know that in the circulation, the net flow out of node 1 (excluding the edge N->1) is F, and the edge N->1 brings in F (so total net out of 1 is 0). Similarly, net out of N is -F (so the edge N->1 brings out F, making net out of N = 0).

    // Therefore, the flow on the edge N->1 is F, and that is the production speed.

    // But how to get it? 
    //   We can look at the reverse edge: the edge from 1 to N (which is the residual edge) has flow = - (flow on N->1). Actually, the flow on the forward edge N->1 is the negative of the flow on the residual edge 1->N.

    // Actually, in our graph representation:
    //   We added an edge from N to 1: 
    //        g[N][k] = {1, rev_index, INF, flow_val}
    //   Then the flow on that edge is flow_val.
    //   How to get k? We can store it when adding.

    // Let's modify the add_edge to return the index, but we don't need to: we can just remember the last two edges we added? 
    // Instead, we can compute the flow on N->1 by: 
    //   Look at the edge from 1 to N in the residual graph: the flow on the reverse edge (which is the residual edge) is the negative of the flow on the forward edge? 
    //   Actually, the flow conservation: the flow on the edge N->1 = the amount that was pushed, which we can get by looking at the reverse edge.

    // Since we added the edge from N to 1, and then the reverse edge (1 to N) has capacity 0 and flow = - (flow on N->1). But we store the flow on the forward edge.

    // We can do: after running max flow, iterate over g[N] to find the edge to 1, and then get its flow.

    long long flow_on_N_to_1 = 0;
    for (auto& e : g[N]) {
        if (e.to == 1) {
            flow_on_N_to_1 = e.flow;
            break;
        }
    }

    // This flow_on_N_to_1 is the production speed.

    // Now, we need to recover the flow on each original edge.

    // How? 
    //   In the reduced graph (without lower bounds), we have a flow f' on the extended graph (with SS and TT) that satisfies:
    //        f'(e) in [0, u(e)-l(e)] for original edges
    //        and flow conservation at all nodes (including the added SS, TT, and the N->1 edge) but SS and TT are only for balancing.

    //   The flow on an original edge e (in the reduced graph) is f'(e) = (the flow we pushed on that edge in the extended graph).
    //   Then the actual flow is f(e) = f'(e) + l(e).

    // How to get f'(e) for an original edge?
    //   In the extended graph, the original edges have capacity = u(e) - l(e) (but note: we did not explicitly set that in the graph? Actually, we built the graph with capacities = u(e) for the original edges? No, wait: we did not add the original edges explicitly to the extended graph? 

    // Correction: 
    //   The standard method for circulation with lower bounds does not require adding the original edges with reduced capacity? 
    //   Actually, the graph we built for the max flow (with SS and TT) is only for the balancing part. The original edges are already in the graph with their original capacities? 

    // I think I confused: 
    //   The graph we built for the max flow (with SS and TT) does NOT include the original pipes! 
    //   We only added edges from SS and to TT, and the edge N->1.

    // But we need to include the original pipes in the circulation graph? 

    // Actually, the circulation graph (without the balancing edges) should have the original edges with capacity = u(e) - l(e) (if we subtract the lower bounds). However, in the standard method, we build the graph for the max flow as:
    //   For each original edge e=(u,v) with lower bound l and capacity u, we add an edge from u to v with capacity = u - l.
    //   Then we add the balancing edges (SS->v for D(v)>0, etc.)

    // So we missed adding the original edges to the graph g.

    // Let me restart the graph building:

    // Steps for building the circulation graph (with lower bounds) for max flow:
    //   We have original edges: for each edge i, from u to v, with lower bound l_i and capacity c_i.
    //   Then we create a new graph where:
    //        capacity of edge (u,v) = c_i - l_i
    //   Then we compute D(v) = (sum of l_i for edges entering v) - (sum of l_i for edges leaving v)
    //   Then add edges from SS to v (if D(v)>0) with capacity D(v), and from v to TT (if D(v)<0) with capacity -D(v).
    //   And add edge from t to s (N to 1) with capacity INF.

    // Therefore, we must add the original edges (with reduced capacity) to the graph g.

    // So we need to rebuild the graph:

    // Clear g and redo.

    g = vector<vector<DinicEdge>>(N+2);
    // Recompute D again (we have the edges, so we can recompute)

    // But note: we already computed D above? Actually, we did. But let's recompute to be safe.

    fill(D.begin(), D.end(), 0);
    for (int i = 0; i < M; i++) {
        int u = edges[i].from, v = edges[i].to;
        long long low = must_fill[i] ? 1 : 0;
        D[u] -= low;
        D[v] += low;
    }

    // Now, add the original edges with reduced capacity
    for (int i = 0; i < M; i++) {
        int u = edges[i].from, v = edges[i].to;
        long long low = must_fill[i] ? 1 : 0;
        long long cap_red = edges[i].cap - low;
        // Only add if cap_red > 0, but even if 0, we can add (but then no flow possible, so it's ok)
        add_edge(u, v, cap_red);
    }

    // Add SS and TT edges
    total_demand = 0;
    for (int i = 1; i <= N; i++) {
        if (D[i] > 0) {
            total_demand += D[i];
            add_edge(0, i, D[i]);
        } else if (D[i] < 0) {
            add_edge(i, N+1, -D[i]);
        }
    }

    // Add edge from N to 1 with capacity INF
    add_edge(N, 1, INF);

    // Now run max flow
    max_flow_value = dinic(0, N+1);

    if (max_flow_value != total_demand) {
        cout << "Impossible" << endl;
        return 0;
    }

    // Now, get the flow on the edge N->1 (for the production speed)
    flow_on_N_to_1 = 0;
    for (auto& e : g[N]) {
        if (e.to == 1) {
            flow_on_N_to_1 = e.flow;
            break;
        }
    }

    // Now, get the flow on each original edge:
    vector<long long> ans_flow(M, 0);
    // For each original edge i, we need to find the corresponding edge in the graph g.
    // But note: we added the original edges in the same order? 
    //   We added: for i=0..M-1: edge from u to v with capacity cap_red = edges[i].cap - low_i.
    //   So the i-th original edge is the i-th edge added to the graph (after the SS/TT edges? no, we added original edges first, then SS/TT, then N->1).

    // How to map? 
    //   We added: 
    //        for i in [0, M-1]: add_edge(u, v, cap_red)
    //        then for SS: ... 
    //        then add_edge(N,1,INF)
    //   So the first M edges added are the original edges (in order).

    // But our add_edge adds two edges (forward and backward). So the forward edge for the i-th original edge is at:
    //   In g[u], the edge that goes to v and has capacity = cap_red (but we don't store capacity, and flow is updated) -> but we know the order: the i-th call to add_edge for original edges.

    // Alternatively, we can store for each original edge the index in the graph.

    // Since M is small, we can do:
    //   For each original edge i, with from=u, to=v, we look in g[u] for an edge to v that has capacity > 0 (initially) but now we know the flow.

    // Actually, we can store the index when adding.

    // Let's refactor: we'll create a vector to store the forward edge index for each original edge.

    // But for simplicity, since M is small, we can do:
    for (int i = 0; i < M; i++) {
        int u = edges[i].from, v = edges[i].to;
        long long low = must_fill[i] ? 1 : 0;
        // Look for the edge from u to v in g[u]
        for (auto& e : g[u]) {
            if (e.to == v && e.cap > 0) { // note: e.cap was set to cap_red, and we don't change cap, only flow
                // This is the forward edge for edge i
                ans_flow[i] = e.flow + low;
                break;
            }
        }
    }

    // Output
    cout << flow_on_N_to_1 << '\n';
    for (int i = 0; i < M; i++) {
        if (i > 0) cout << " ";
        cout << ans_flow[i];
    }
    cout << '\n';

    return 0;
}
