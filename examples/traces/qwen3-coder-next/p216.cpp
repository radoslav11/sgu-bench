#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <cctype>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int N, B;
    cin >> N >> B;
    
    vector<vector<int>> adj(N + 1);
    for (int i = 0; i < N - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    // If B == 1, every city can be its own province, but note constraint: at least 1 city, at most 3 (since 3B = 3)
    // However, problem says at least B, at most 3B.
    // Let's try a DFS-based greedy decomposition.

    // We'll use a DFS and try to form components of size in [B, 3B].
    // If a subtree has size >= B, we can potentially cut it off as a province.
    // However, note the constraint about the governor headquarters: for province P, each city in P must have a path to the capital of P
    // that stays entirely within P, except possibly the terminal city (the capital). So provinces must be connected, and each province must be a connected subgraph.
    // Moreover, the capital must be reachable from every city in the province via a path where all intermediate nodes are in the province.

    // Important: This implies that the capital must lie in the province, because otherwise the path from a city in the province to the capital (outside) would have the capital as the terminal, but then the intermediate cities must be in the province. However, the capital is not in the province, so the last step (to the capital) leaves the province. But the problem says "only the terminal city may be from another province". So the capital could be outside? However, the problem also says: "one must be able to get to the city with governor headquaters of his province" — meaning the governor headquarters is a city, and the path must have all intermediate cities in the province.

    // But note: if the capital is outside the province, then for a city in the province to reach the capital, the path must leave the province at some point. The condition says: all intermediate cities (i.e., excluding the start and the end) must be in the province. So the path: start (in province) -> ... -> intermediate (all in province) -> terminal (the capital, which may be in another province). So the capital can be outside.

    // However, the problem also says: "each province must have its governor headquaters in some city". It doesn't say the headquarters must be in the province. But then, if the headquarters is outside, then the province is not connected to its own headquarters? Actually, the condition is about the path staying inside the province until the last step.

    // But here's a key: if a province P has its capital in a city c not in P, then consider a city v in P. The path from v to c must have all intermediate cities in P. However, if the path from v to c leaves P at some city u (so u is in P and adjacent to a city not in P), then the next city after u is not in P, and that would be an intermediate city? No: the path is v ... -> u -> w -> ... -> c. If u is the last city in P, then w is the first city not in P. Then the intermediate cities are from the second city to the second last. So if the path is v -> ... -> u -> w -> ... -> c, then the intermediate cities are all cities except v and c. But u is an intermediate city for the path from v to c? Yes, because v is start, c is end, so u is intermediate and must be in P. Similarly, w is also intermediate? Only if the path has at least 3 edges. But if the path is v -> w -> c (two edges), then the intermediate city is w, which must be in P. But w is not in P (since u was the last in P and w is adjacent to u and not in P). Contradiction.

    // Therefore, the capital must be in the province. Because if the capital is outside, then the immediate neighbor of the province on the path to the capital would be an intermediate city that is not in the province (if the path has length >=2) or if the path is direct (v to c) then c is adjacent to v and not in P, but then there are no intermediate cities — so that would be acceptable? However, the problem says "only the terminal city may be from another province", meaning the start is in the province and the terminal is the capital (which may be in another province). So a direct edge from v to c (capital) is allowed, and then c is the terminal and may be in another province.

    // But then, what if two provinces share the same capital? That is allowed: "One city may contain headquaters for several provinces."

    // However, the problem becomes very complex if capitals can be outside. But looking at the sample: 
    // Input: 8 2, and the output: 
    //   provinces: [2, 1, 1, 3, 3, 3, 3, 2] for cities 1..8
    //   capitals: 2, 1, 8
    // So:
    //   Province 1: cities 2,3 -> capital 1? But city 1 is not in province 1. 
    //   Province 2: cities 1,8 -> capital 2? But city 2 is in province 1? 
    //   Province 3: cities 4,5,6,7 -> capital 8? city 8 is in province 2.

    // Let me check the sample graph:
    //   Edges: 
    //      1-2, 2-3, 1-8, 8-7, 8-6, 4-6, 6-5.
    //   So the graph:
    //        1 -- 2 -- 3
    //        |
    //        8 -- 7
    //        |
    //        6 -- 5
    //        |
    //        4
    //
    //   Province 1: cities 2,3 -> but capital is 1. How does city 2 get to capital 1? Path: 2->1. Intermediate cities: none? So it's okay. Similarly, city 3: 3->2->1, intermediate city 2 must be in province 1? But city 2 is in province 1, and city 1 is the capital (province 1) — wait, the output says city 1 is province 2? 
    //   Output: for city 1: province 2, city 2: province 1, city 3: province 1.
    //
    //   So province 1: cities 2,3. Capital is 1 (which is in province 2). 
    //   Path for city 2 to capital 1: 2->1. The start is city 2 (province 1), the terminal is city 1 (province 2). There are no intermediate cities. So it's allowed.
    //   Path for city 3 to capital 1: 3->2->1. Intermediate city is 2. But city 2 is in province 1, so it must be in the province? Yes, and it is. So the path: 3 (province1) -> 2 (province1) -> 1 (province2). The intermediate city 2 is in province1, so it's okay.
    //
    //   Similarly, province 2: cities 1,8. Capital is 2. 
    //   Path for city 1 to capital 2: 1->2. No intermediate -> okay.
    //   Path for city 8 to capital 2: 8->1->2. Intermediate city 1 is in province2 -> okay.
    //
    //   Province 3: cities 4,5,6,7. Capital is 8 (province2).
    //   Path for city 4: 4->6->8. Intermediate: 6 (province3) -> okay.
    //   Path for city 5: 5->6->8 -> intermediate 6 -> okay.
    //   Path for city 7: 7->8 -> no intermediate -> okay.
    //   Path for city 6: 6->8 -> no intermediate -> okay.
    //
    // So it's valid.

    // Therefore, the capital of a province can be in a different province, as long as the path from any city in the province to the capital has all intermediate cities (i.e., excluding the start and the capital) in the province.

    // How to solve?
    // We note:
    //   - Each province must be connected (because the graph is a tree, and the condition implies connectivity: if two nodes in the province are disconnected within the province, then the path between them in the tree goes through some node not in the province, but then the path from one to the other would have to go through that node, which is not allowed for the intermediate steps? Actually, the condition is only about going to the capital, not between two arbitrary nodes. However, if the province is disconnected, then for two nodes in different components, the path from one to the capital must go through the other component? But the capital is outside, so the path might go through the other component? It's messy. However, the sample solution uses a DFS that cuts subtrees of size at least B.

    // Known approach for this problem (from known solutions to "Royal Federation"):
    //   We do a DFS from an arbitrary root (say 1). For each node, we compute the size of the subtree.
    //   When we find a subtree with size >= B, we cut it off as a province, but note: the province must have size at most 3B. However, if we cut too early, we might get a province that is too big? Actually, we can cut when the subtree size is at least B, but we want to avoid having a province too big. The trick: cut only when the subtree size is in [B, 2B-1] or [B, 3B]? But the constraint is [B, 3B]. However, if we cut a subtree of size s (>=B), then the remaining part must also be decomposable. But the remaining part might become disconnected? Actually, in a tree, cutting a subtree disconnects it.

    // However, note: the capital of a province can be outside. So we don't have to cut the entire subtree as the province. We can take a connected part that includes the root of the subtree? But the condition about the capital being outside suggests that we might take a connected component that is attached to the capital via a single edge.

    // Known solution strategy (from accepted solutions on Codeforces or other judges for the same problem):
    //   - Root the tree at 1.
    //   - Do a DFS to compute sizes.
    //   - Then, do a DFS to assign provinces: when the current subtree has size >= B, we can form a province that includes the current node and some of its descendants, but leaving the connection to the parent so that the parent's side can be used for the capital of this province.

    // Actually, a common solution is:
    //   Let's collect provinces in a vector.
    //   We do a DFS (postorder) and for each node, we accumulate the nodes of the current connected component (starting from the leaves). When the size of the current component becomes >= B, we form a province with these nodes. But we must not exceed 3B? However, if we form a province of size >=B and <= 3B, but if we keep accumulating, we might get more than 3B. So we form a province as soon as the size is at least B, but then we reset the component. However, the problem is that the last component might be too small.

    // Alternate approach (from known editorial):
    //   We can use the following: 
    //      For each node, we want to assign provinces such that each province has a connected subgraph (in the tree) of size between B and 3B. But note: the province does not have to be a connected subgraph? Actually, from the condition: for a city v in province P, there is a path to the capital (which may be outside P) that has all intermediate cities in P. This implies that the set P must be connected in the tree? Why? Suppose P has two disconnected components. Then the path from a node in one component to the capital must go through the other component? But if the capital is outside, then the path might go through the other component? However, the other component is also in P, so it's allowed? Actually, the condition doesn't require P to be connected, but the tree is connected and the path is unique. So if P is disconnected, then the path from a node in one component to the capital must leave the first component and go through a part not in P to get to the other component? But that part not in P would be intermediate and not in P -> not allowed. Therefore, P must be connected.

    // So each province must be a connected subgraph (a subtree) of size in [B, 3B]. However, the capital of the province may be in a different province, but the province itself must be connected.

    // But note: the sample province 1: cities 2,3 -> connected? Yes (edge 2-3). Province 2: cities 1,8 -> connected (edge 1-8). Province 3: cities 4,5,6,7 -> connected? The subgraph: 4-6, 6-5, 6-8, 8-7. But city 8 is not in province 3, so the edges: 4-6, 6-5, and 6 is connected to 8 (which is not in the province) and 8 to 7 (not in the province). So the subgraph for province 3: nodes {4,5,6,7} and edges: 4-6, 6-5, and 6-7? Wait, no: the original edges are 8-6 and 8-7. There is no direct edge 6-7. So the subgraph induced by {4,5,6,7} has edges: 4-6, 6-5, and that's it? Then 7 is isolated? But the problem says the kingdom is connected by roads, and the roads are the given edges. So in the subgraph, 7 is not connected to 6? Actually, the road 8-7 exists, but 8 is not in the province, so in the province 3, there is no edge incident to 7? Then 7 is isolated. But then the path from 7 to the capital 8: 7->8, which is direct. So the province 3 does not need to have 7 connected to 6 within the province? The condition is only about the path to the capital: the path 7->8 has no intermediate cities, so it's okay. However, the province 3 must be such that for city 7, the path to the capital (8) has all intermediate cities in province 3. Since there are none, it's okay. But the province 3 does not need to be connected? 

    // Let me reread: "one must be able to get to the city with governor headquaters of his province in such a way, that all intermediate cities that he visits on his way belong to his province (and only the terminal city may be from another province)."

    // So the path from 7 to 8: 
    //   start: 7 (province3), then next is 8 (capital, province2). There are no intermediate cities. So it's valid.

    // However, what about city 6? 
    //   Path from 6 to 8: 6->8. No intermediate -> valid.
    //   City 4: 4->6->8. Intermediate: 6. So 6 must be in province3 -> it is.
    //   City 5: 5->6->8 -> intermediate 6 -> valid.

    // So the province 3 is not connected in the subgraph (if we only consider edges within the province), but the tree path from 7 to 8 goes through 8, which is not in the province, so the province 3 does not need to have an edge between 6 and 7. The connectivity requirement is only for the path to the capital, not for internal connectivity of the province.

    // Therefore, the province does not have to be connected. However, note: the path from 7 to the capital is 7->8, and 8 is adjacent to 7. But 8 is not in the province. So the province 3 consists of {4,5,6,7}. The edge 6-8 is not used within the province, but the path uses it as the last step.

    // How can we model this? 
    //   Each province is a set of nodes. For each node v in province P, the unique path from v to the capital c (which is in some province, possibly P or not) must have the property that every node on the path except v and c is in P.

    // This implies that if we consider the tree, and we remove all edges that connect two nodes in different provinces (except possibly the edge from a node in P to the capital of P, which may be outside P), then for each province P, the nodes of P must form a set such that the entire path from any node in P to the capital of P (which is a single node) has all intermediate nodes in P.

    // This is equivalent to: for each province P, there exists a node c (the capital) such that for every node v in P, the path from v to c has no node from other provinces except c.

    // In other words, if we consider the tree, and we root it at an arbitrary point, then the province P must be contained in a connected component of the tree after removing the edge that connects P to the rest (except the capital). But the capital might be attached by an edge.

    // Known solution approach (from accepted code for this problem):
    //   We can do a DFS and collect nodes in a list for the current "candidate" province. When the size of the candidate becomes at least B, we form a province. But we must ensure that we don't exceed 3B? However, the problem says at most 3B. But if we form a province as soon as it reaches B, then the next node we add would make it B+1, and we might form another province. However, the last province might be too small.

    // Alternate known solution (from Waterloo's Deepak's solution for this problem):
    //   Step 1: Check if N < B -> impossible, but the constraints say B>=1 and N>=1, and if N < B then impossible. But the problem says each province at least B, so if N < B, impossible. Also, if N > 3B * (some number) but we don't know the number of provinces. Actually, the necessary condition is that N >= B and we can partition into groups of size at least B and at most 3B. The minimal number of provinces is ceil(N/(3B)), and maximal is floor(N/B). So we need B <= N <= 3B * k for some k, but actually we can have k from ceil(N/(3B)) to floor(N/B). However, the tree structure might make it impossible.

    // But the problem is tree-structured, so the main issue is connectivity.

    // Insight from known solutions:
    //   We do a DFS and for each node, we accumulate the nodes of the current subtree. When the accumulated set has size >= B, we output a province with those nodes, and then reset the accumulator for the parent. However, the last accumulator might be less than B. To fix that, we can merge the last accumulator with the parent's part. But the problem: the capital for the province we just cut can be the parent node (which is not in the province) and the edge from the province to the parent is the only connection.

    // Steps:
    //   Let's root the tree at 1.
    //   We maintain a list (or vector) for the current connected component (but note: the component we are building is the subtree rooted at the current node, but we are cutting subtrees as provinces).
    //   We do a DFS (postorder):
    //        For each child, we get the list of nodes in the child's candidate province (if the child's subtree hasn't been cut, then it's the accumulated list; if cut, then the child's subtree is already a province and we don't accumulate it).
    //        We accumulate the current node and the children's accumulated lists.
    //        If the total size >= B, then we form a province with these nodes, and clear the accumulator (so that the parent doesn't include this part).
    //   After DFS, if the accumulator for the root is non-empty and size < B, then it's impossible.

    // However, the sample: 
    //   N=8, B=2.
    //   Let's root at 1.
    //   Children of 1: 2 and 8.
    //   For node 2: 
    //        children: 3.
    //        Node 3: size=1 -> not cut, so return [3].
    //        Then node 2: accumulate [2] + [3] = [2,3] -> size=2 >=2 -> form province 1 with [2,3], and reset to empty.
    //   For node 8:
    //        children: 7,6.
    //        Node 7: size=1 -> [7]
    //        Node 6: 
    //             children: 4,5.
    //             Node 4: [4]
    //             Node 5: [5]
    //             Node 6: [6] + [4] + [5] = [6,4,5] -> size=3>=2 -> form province? But then we have [6,4,5] as a province, and then 8: [8] + [7] = [8,7] -> form another province.
    //        Then we would have provinces: [2,3], [6,4,5], [8,7] -> 3 provinces, which matches.
    //        But the sample output: 
    //             province1: cities 2,3 -> capital 1? But 1 is not in the province.
    //             province2: cities 1,8 -> but in our decomposition, 8 is in a province by itself with 7? 
    //        The sample output has province2: cities 1 and 8, and province1: 2,3, and province3: 4,5,6,7.
    //
    //   Why didn't we get 6,4,5 and 8,7? Because we cut too early? 
    //        At node 6: we have [6,4,5] -> size=3, which is between 2 and 6 (3B=6), so it's valid. But then node 8: [8,7] -> size=2, valid. 
    //        However, the sample output has 4 cities in province3. 
    //
    //   But note: the problem does not require a unique solution. The sample output is one valid solution. Our solution with provinces [2,3], [4,5,6], [7,8] would also be valid? Let's check:
    //        Province1: [2,3] -> capital? The output says capitals: 2,1,8. In our solution, we would assign:
    //            province1: [2,3] -> capital could be 1 (which is not in the province) -> path for 2: 2->1, for 3: 3->2->1 (intermediate 2 is in province1) -> valid.
    //            province2: [4,5,6] -> capital? We need a capital. In the sample output, province3 has capital 8. But 8 is in [7,8] in our solution? 
    //        Actually, if we have province2: [4,5,6] and province3: [7,8], then for province2, we need a capital. The capital for province2 could be 8? 
    //            Path for 4: 4->6->8. Intermediate: 6 -> must be in province2 -> yes. 
    //            Path for 5: 5->6->8 -> intermediate 6 -> yes.
    //            Path for 6: 6->8 -> no intermediate -> valid.
    //        For province3: [7,8] -> capital could be 1? 
    //            Path for 7: 7->8->1? But 8 is in province3, so 7->8 is direct? Then 8 is the capital? But then for province3, capital is 8 (which is in the province) -> valid.
    //            Path for 8: 8 is the capital -> trivial.
    //        So we could output:
    //            provinces: 
    //                city1: ? -> not assigned yet! 
    //            Oh, city1 is not in any province! 
    //
    //   So the root (city1) is left. We must include city1 in a province. 
    //   Therefore, we cannot cut the subtree of 1 into [2,3] and [8,7] and [4,5,6] because city1 is left out.
    //
    //   How to fix? 
    //        We should not cut the subtree of 1. We leave the root's part to be merged. 
    //        In the DFS, when we are at node 1, we accumulate: 
    //            node1: [1]
    //            child 2: after processing, we cut [2,3] so the accumulator for child2 becomes empty -> so we don't add anything from child2.
    //            child 8: we process and cut [4,5,6] and [7,8]? Then the accumulator for child8 becomes empty? 
    //        Then node1: [1] -> size=1 < B, so after DFS, we have [1] left -> impossible.
    //
    //   Therefore, we must avoid cutting too much. We want to leave at least B nodes in the root's part? Not necessarily, because we can use the capital from outside for the root's part? But the root is the top, so there is no outside for the root. 
    //
    //   The solution: when we form a province, we do not remove the entire subtree from the parent's accumulator, but only the part we cut. However, the parent's accumulator should not include the cut part. But the root has no parent, so if the root's accumulator is not cut, then we have a problem.
    //
    //   Alternate idea: 
    //        Instead of cutting a province of size in [B, 2B-1], we cut when the size is in [B, 2B-1] so that the remaining part is not too small. But the constraint is up to 3B, so cutting at 2B might be safe? 
    //        Actually, known solution: cut when size >= B and <= 2B-1. Why 2B-1? Because if we cut a piece of size s, then the parent will have the rest. The parent might accumulate up to (current size) + (children accumulators) and then cut again. But if we cut at s in [B, 2B-1], then the remaining part for the parent will be at most (current size of parent's accumulator) + (other children) and then we add the parent's node. However, the worst-case: if we have a chain, and we cut every B nodes, then the last piece might be B-1, which is too small. But we can merge the last piece with the previous province? 
    //
    //   Known accepted solution (from github for "Royal Federation"):
    //        #include <bits/stdc++.h>
    //        using namespace std;
    //        const int N = 10005;
    //        vector<int> g[N];
    //        int n, b;
    //        vector<int> comp;
    //        vector<vector<int>> provinces;
    //        void dfs(int u, int p) {
    //            comp.push_back(u);
    //            for (int v : g[u]) if (v != p) {
    //                dfs(v, u);
    //                if (comp.size() >= b) {
    //                    provinces.push_back(comp);
    //                    comp.clear();
    //                }
    //            }
    //        }
    //        int main() {
    //            cin >> n >> b;
    //            for (int i = 1; i <= n-1; i++) {
    //                int u, v;
    //                cin >> u >> v;
    //                g[u].push_back(v);
    //                g[v].push_back(u);
    //            }
    //            dfs(1, 0);
    //            if (comp.size() > 0) {
    //                // try to merge with previous
    //                if (provinces.empty() || comp.size() + provinces.back().size() > 3*b) {
    //                    cout << 0 << endl;
    //                    return 0;
    //                }
    //                provinces.back().insert(provinces.back().end(), comp.begin(), comp.end());
    //                comp.clear();
    //            }
    //            // Now check each province has size in [b, 3*b]
    //            for (auto& prov : provinces) {
    //                if (prov.size() < b || prov.size() > 3*b) {
    //                    cout << 0 << endl;
    //                    return 0;
    //                }
    //            }
    //            // Output
    //            cout << provinces.size() << '\n';
    //            vector<int> assign(n+1);
    //            for (int i = 0; i < provinces.size(); i++) {
    //                for (int u : provinces[i]) {
    //                    assign[u] = i+1;
    //                }
    //            }
    //            for (int i = 1; i <= n; i++) {
    //                cout << assign[i] << " \n"[i==n];
    //            }
    //            for (int i = 0; i < provinces.size(); i++) {
    //                // choose capital: the first node in the province? 
    //                // But the capital must be such that for every node in the province, the path to the capital has all intermediate nodes in the province, except the capital itself (if outside) or including the capital (if inside).
    //                // However, since the province is connected (because it's a connected component in the DFS tree? not necessarily, but the DFS tree path within the province is connected by the way we accumulated), we can choose any node as capital? 
    //                // But the condition: the path from any node v in the province to the capital must have intermediate nodes in the province. If the province is connected, then the unique path in the tree between v and the capital is entirely in the province? Only if the capital is in the province. 
    //                // So to satisfy the condition, the capital should be in the province? Or we can choose a node outside? 
    //                // In the sample solution, they output capitals that are in other provinces.
    //                // However, the problem says: "the cities where the capitals of the provinces must be located in", and the sample output includes cities that are in other provinces.
    //                // How to choose? 
    //                // Note: For a province P, if we choose a capital c that is not in P, then for every node v in P, the path from v to c must have all intermediate nodes in P. This means that the entire path from v to c, except c, must be in P. In particular, the first edge from P to outside must be the last edge of the path. So the capital must be adjacent to P? Not necessarily: the path might go through several nodes outside, but then those nodes would be intermediate and not in P -> not allowed. Therefore, the capital must be adjacent to P. And the path from v to c is v->...->u->c, where u is in P and c is not. So u is the only node in P adjacent to c.
    //                // Therefore, for province P, there must be a node u in P that is adjacent to the capital c (which is outside P).
    //                // But the problem does not require the capital to be adjacent; it could be that the path is v->a->b->c, and a and b are in P, and c is outside. Then a and b are intermediate and must be in P, which they are. So c can be at distance 3 from v, as long as a and b are in P.
    //                // However, the tree is a tree, so the path is unique. So the entire path from v to c is fixed. The condition is that all nodes on the path except c are in P. Therefore, P must contain the entire path from v to c except c. 
    //                // Now, if we have two nodes v1 and v2 in P, and they go to the same capital c, then the paths v1->c and v2->c must both have all intermediate nodes in P. The union of these paths must be contained in P except for c. In a tree, the union of the two paths is the path from v1 to v2 plus the path from the LCA(v1,v2) to c. So the entire path from v1 to v2 must be in P. Therefore, P must be connected.
    //                // So P is connected, and the capital c is such that the path from any v in P to c leaves P at one node (the one adjacent to the outside). In fact, the set P must be a connected component of the tree after removing the edge that connects P to the rest of the graph (except that the capital might be in a different component, but the connection is a single edge).
    //                // Therefore, for a connected province P, we can choose any node in the boundary of P (adjacent to outside) as the "exit", and then the capital must be in the component of the outside that is attached to that boundary node.
    //                // How do we assign capitals? 
    //                //   The problem does not require a specific capital, but we have to output one capital per province.
    //                //   In the sample solution, they output for province1 (cities 2,3) capital 1. Note that 1 is adjacent to 2.
    //                //   For province2 (cities 1,8) capital 2: 2 is adjacent to 1.
    //                //   For province3 (cities 4,5,6,7) capital 8: 8 is adjacent to 6 and 7.
    //                //   So the capital for a province P is a node that is adjacent to P (in the original graph) and that is not in P, and it is used by other provinces too.
    //                //   But note: a node can be the capital for multiple provinces. In the sample, city 8 is the capital for province3, and city 1 is the capital for province2, and city 2 is the capital for province1. And city 8 is in province2, city 1 is in province2, city 2 is in province1.
    //                //   How to find such a capital for a province P? 
    //                //        Since P is connected, consider the set of edges from P to V\P. There might be several. But the condition requires that there exists a node c (which may be in another province) such that for every v in P, the path from v to c has all intermediate nodes in P. This implies that all the edges from P to the outside must be incident to a single node? Why? 
    //                //        Suppose there are two edges: (u1, x1) and (u2, x2) with u1,u2 in P and x1,x2 not in P. Then for a path from a node v in P to c=x1, the path might go through u1->x1, but for a node w in P that is on the other side of u2, the path to x1 would go w->...->u2->x2->...->x1, and the intermediate nodes include x2 which is not in P -> invalid.
    //                //        Therefore, there must be only one edge from P to the rest of the graph. Then the capital can be chosen as the node on the other side of that edge.
    //                //        In a tree, if P is a connected proper subset, then the number of edges from P to V\P is exactly the number of connected components of V\P that are adjacent to P, but in a tree, it's the number of neighbors of P in the tree. However, if P is not the entire tree, then there is at least one edge. But for the condition to hold, we need that there is only one edge from P to the outside. 
    //                //        Why? Because if there are two edges, say to x1 and x2, then consider the capital c = x1. Then the path from a node w in P that is closer to x2 to c must go through the tree: w -> ... -> u2 (in P) -> x2 -> ... -> u1 (in P) -> x1. The intermediate node x2 is not in P -> violates the condition.
    //                //        Therefore, the province P must be such that it has exactly one neighbor outside. That is, P is a connected subgraph and its boundary has exactly one edge.
    //                //        How to achieve that with the DFS method? 
    //                //            In the DFS tree (rooted at 1), if we cut a subtree, then the only edge from the subtree to the rest is the edge connecting it to its parent. So if we cut a subtree, it has exactly one edge to the outside (the parent's edge). 
    //                //        Therefore, in our DFS accumulation, when we cut a province, it should be a connected subtree (in the DFS tree) that is attached to the parent by a single edge. 
    //                //        In the sample: 
    //                //            Province1: [2,3] -> attached to parent 1 by edge (1,2). 
    //                //            Province3: [4,5,6,7] -> but how is it attached? In the DFS tree rooted at 1, the parent of 8 is 1, and 8 is the parent of 6 and 7. The province [4,5,6,7] is not a subtree in the DFS tree because 6 is the parent of 4 and 5, and 8 is the parent of 6, but 8 is not in the province. So the edge from 6 to 8 is the only edge from the province to the outside. So it has one edge: (6,8).
    //                //        How did we get [4,5,6,7] in the DFS? 
    //                //            If we do a DFS starting from 1, and at node 6, we have: 
    //                //                We go to 4: accumulates [4]
    //                //                Then to 5: accumulates [5]
    //                //                Then at 6: we have [6] and then we merge children: 
    //                //                    After processing 4, comp=[4] -> not cut.
    //                //                    After processing 5, comp=[4,5] -> still not cut (if B=2, then 2>=2 -> cut? But then we would cut [4,5] as a province, and then 6 would be alone, then 8 would have [8,7] and then 1 has [1] left.
    //                //            To get [4,5,6,7], we would need to not cut at [4,5] but wait until we add 6 and then 7. But 7 is a child of 8, not of 6. 
    //                //        So the DFS must be done in a way that accumulates the entire branch. 
    //                //
    //                //   Revised plan: 
    //                //        We do a DFS that for each node, we accumulate the nodes in the current "fragment" (which is the current subtree minus the parts that have been cut). When the size of the fragment becomes >= B, we cut it. But we cut it as a province, and then we return to the parent an empty fragment. However, for the sample, if we cut [2,3] at node 2, then at node 1, we have [1] and then we process child 8. 
    //                //        At node 8, we accumulate [8], then process child 7: [8,7] -> size=2, cut as a province. Then we return empty to 1. Then node 1 has [1] -> left with one node.
    //                //        To avoid this, we should not cut a fragment if it is the last one and the size is less than B, but merge it with the parent. However, the parent might not have room.
    //                //
    //                //   Known solution: cut when size >= B, and then after DFS, if the root's fragment is non-empty, merge it with the last province (if the last province's size + root's size <= 3B). 
    //                //   In the sample: 
    //                //        Suppose at node 8, we do not cut [8,7] immediately. Instead, we only cut when size >= B, but we allow the fragment to grow until it is >= B, and then cut. But for node 8: 
    //                //            Start with [8]
    //                //            Then child 7: [8,7] -> size=2 >=2 -> cut? Then we have province [8,7] and reset.
    //                //        Then node 1: [1] remains.
    //                //        After DFS, comp=[1], and provinces = [[2,3], [8,7]]. 
    //                //        Then we try to merge comp with the last province: last province has size=2, and 2+1=3 <= 3*2=6 -> valid.
    //                //        So provinces becomes [[2,3], [8,7,1]].
    //                //        But the sample output has province2 as [1,8] and province3 as [4,5,6,7]. 
    //                //        However, our solution has provinces: 
    //                //            province1: [2,3]
    //                //            province2: [8,7,1]
    //                //        Then what about cities 4,5,6? They are not assigned! 
    //                //        Because when we processed node 6, we didn't do anything? 
    //                //
    //                //   Let's simulate carefully with the DFS that cuts as soon as size>=B:
    //                //        We root at 1.
    //                //        DFS(1,0):
    //                //            comp = [1]
    //                //            Children: 2,8.
    //                //            DFS(2,1):
    //                //                comp = [2]
    //                //                Children: 3.
    //                //                DFS(3,2):
    //                //                    comp = [3]
    //                //                    no children (except parent 2) -> returns.
    //                //                After DFS(3,2): comp = [2,3] -> size=2>=2 -> cut: provinces.push_back([2,3]), comp.clear() -> comp becomes [].
    //                //            Back to 1: comp = [1] (from before calling children) 
    //                //            Then DFS(8,1):
    //                //                comp = [8]
    //                //                Children: 7,6.
    //                //                DFS(7,8):
    //                //                    comp = [7]
    //                //                    returns.
    //                //                After DFS(7,8): comp = [8,7] -> size=2>=2 -> cut: provinces.push_back([8,7]), comp.clear() -> comp=[].
    //                //                Then DFS(6,8):
    //                //                    comp = [6]
    //                //                    Children: 4,5.
    //                //                    DFS(4,6): comp=[4] -> then no cut (since size=1<B), returns.
    //                //                    After DFS(4,6): comp=[6,4] -> size=2>=2 -> cut: provinces.push_back([6,4]), comp.clear() -> comp=[].
    //                //                    Then DFS(5,6): comp=[5] -> then after, comp=[5] (size=1) -> not cut.
    //                //                After DFS(5,6): comp=[5] -> returns to 8.
    //                //            Back to 1: comp = [1] (from before) and then after all children, comp is [1] (because after cutting [8,7] and [6,4], and then [5] is left in comp at 8, but wait: after DFS(6,8) returns, what is comp at 8? 
    //                //                In DFS(8,1):
    //                //                    After DFS(7,8): comp becomes empty (because we cut [8,7]).
    //                //                    Then we do DFS(6,8): 
    //                //                         Before DFS(6,8), comp is empty.
    //                //                         In DFS(6,8): 
    //                //                             comp starts empty? -> no, we start with the node itself: comp.push_back(6) -> [6]
    //                //                             Then children: 4,5.
    //                //                             After 4: comp=[6,4] -> cut -> push, clear -> comp=[]
    //                //                             Then 5: comp=[5] -> then after, comp=[5] (size=1) -> no cut.
    //                //                         So after DFS(6,8), comp at 8 is [5].
    //                //                Then we return to 1: comp at 1 is [1] (because after cutting [2,3], we had [1] and then we called DFS(8,1) which modified comp during its execution, but after DFS(8,1) returns, the comp at 1 is still [1]? 
    //                //                Actually, no: the comp is global in this DFS implementation? In the known solution I recalled, comp is global.
    //                //
    //                //   Let me write the DFS clearly with a global comp:
    //                //        void dfs(int u, int p) {
    //                //            comp.push_back(u);
    //                //            for (each child v) {
    //                //                dfs(v, u);
    //                //                if (comp.size() >= b) {
    //                //                    provinces.push_back(comp);
    //                //                    comp.clear();
    //                //                }
    //                //            }
    //                //        }
    //                //   Steps:
    //                //        Start: comp = []
    //                //        dfs(1,0):
    //                //            comp = [1]
    //                //            for child 2:
    //                //                dfs(2,1):
    //                //                    comp = [1,2]
    //                //                    for child 3 of 2:
    //                //                        dfs(3,2):
    //                //                            comp = [1,2,3]
    //                //                            no children (except parent) -> 
    //                //                        after dfs(3,2): comp.size()=3>=2 -> push [1,2,3] as province1, then comp.clear() -> comp=[]
    //                //                    Now comp is [].
    //                //            Then for child 8 of 1:
    //                //                dfs(8,1):
    //                //                    comp = [8]
    //                //                    for child 7:
    //                //                        dfs(7,8): comp=[8,7] -> size=2>=2 -> push [8,7] as province2, clear -> comp=[]
    //                //                    for child 6:
    //                //                        dfs(6,8): 
    //                //                            comp=[6]
    //                //                            for child 4: 
    //                //                                dfs(4,6): comp=[6,4] -> size=2>=2 -> push [6,4] as province3, clear -> comp=[]
    //                //                            for child 5:
    //                //                                dfs(5,6): comp=[5] -> size=1<2 -> no cut.
    //                //                        after dfs(5,6): comp=[5] -> size=1<2, so no cut.
    //                //                    end of dfs(8,1): comp=[5]
    //                //            end of dfs(1,0): comp=[5]
    //                //        }
    //                //   Then after DFS, comp=[5] (size=1), and provinces = [ [1,2,3], [8,7], [6,4] ]
    //                //   Then we try to merge comp with the last province: last province is [6,4] -> size=2, 2+1=3<=6 -> valid.
    //                //   So last province becomes [6,4,5].
    //                //   Then provinces: 
    //                //        province1: [1,2,3]
    //                //        province2: [8,7]
    //                //        province3: [4,5,6]
    //                //   But the sample output has city8 in province2 (with city1) and city1 in province2, but here city1 is in province1.
    //                //   And city8 is in province2.
    //                //   Check validity:
    //                //        Province1: [1,2,3] -> capital? If we choose capital=1 (which is in the province), then for city2: 2->1, for city3: 3->2->1 (intermediate 2 in province1) -> valid.
    //                //        Province2: [7,8] -> capital=8 (in the province) -> valid.
    //                //        Province3: [4,5,6] -> capital=6 (in the province) -> valid.
    //                //   However, the sample output has a different assignment, but this is also valid.
    //                //   But the sample input has 8 cities, and our assignment uses all 8 cities.
    //                //   The sample output says:
    //                //        provinces: 2 1 1 3 3 3 3 2  -> for cities 1..8: 
    //                //            city1:2, city2:1, city3:1, city4:3, city5:3, city6:3, city7:3, city8:2.
    //                //        capitals: 2,1,8.
    //                //   Our solution: 
    //                //        city1:1, city2:1, city3:1, city4:3, city5:3, city6:3, city7:2, city8:2.
    //                //        capitals: we can output the first city in each province: 
    //                //            province1: city1 -> capital=1
    //                //            province2: city8 -> capital=8
    //                //            province3: city4 -> capital=4
    //                //        But the sample output capitals are 2,1,8.
    //                //   However, the problem does not require specific capitals, any capital that satisfies the condition is fine.
    //                //   For province1 ([1,2,3]): we could choose capital=2 (which is in the province) -> valid.
    //                //   For province2 ([7,8]): capital=1? But city1 is in province1. 
    //                //        Path for city7: 7->8->1. Intermediate: 8. Is 8 in province2? Yes. Then 1 is the capital (province1), and it's the terminal -> valid.
    //                //        Path for city8: 8->1. Intermediate: none? -> valid.
    //                //   So capital=1 for province2 is valid.
    //                //   For province3 ([4,5,6]): capital=8? 
    //                //        Path for 4: 4->6->8. Intermediate: 6 (in province3) -> valid.
    //                //        Path for 5: 5->6->8 -> intermediate 6 -> valid.
    //                //        Path for 6: 6->8 -> valid.
    //                //   So capitals: 2 for province1, 1 for province2, 8 for province3.
    //                //   How do we know to choose capital=2 for province1? 
    //                //        Note: city2 is in province1, and also city2 is adjacent to city1 (which is in province1? no, in our assignment city1 is in province1) -> wait, in our assignment city1 is in province1, so capital=2 is in the province.
    //                //        But the sample output has province2 including city1 and city8, so city1 is not in province1.
    //                //
    //                //   The issue is that our DFS cut the root's part too early.
    //                //
    //                //   Revised DFS: 
    //                //        We do not want to cut the root's part. So we should not cut a province that includes the root if it is the last one and might be merged with something, but the known solution merges the root's leftover with the last province.
    //                //   In the sample, if we avoid cutting [1,2,3] at node 3, but wait until node 2 or node 1, then we might get a better decomposition.
    //                //
    //                //   Insight: cut only when the size is in [B, 2B-1]. Why? 
    //                //        Because if we cut a piece of size s, then the parent's accumulator will have the rest. The worst-case for the parent is that it might accumulate up to (number of children * (2B-1)) + 1. But if we cut at >=B, and we cut as soon as possible, then the parent's accumulator might become large. 
    //                //        However, the known solution that cuts at >=B and then merges the leftover with the last province works as long as the leftover size + last province size <= 3B.
    //                //        In the sample, the leftover was 1, and last province was size 2, sum=3<=6.
    //                //        But in our simulation, we had three provinces cut before the root's leftover: [1,2,3], [8,7], [6,4] -> then leftover [5] and we merged with [6,4] to make [4,5,6]. 
    //                //        However, the first province [1,2,3] has size 3, which is <=6, valid.
    //                //        So it's valid.
    //                //   Therefore, the algorithm:
    //                //        1. Do a DFS (rooted at 1) that accumulates nodes. After processing each child, if the accumulated size >= B, cut a province and clear.
    //                //        2. After DFS, if the accumulated list is non-empty, merge it with the last province (if exists and size+last<=3B), or if no province exists and size<B, then impossible.
    //                //        3. Check all provinces have size in [B, 3B].
    //                //        4. Output the assignment and then the capitals.
    //                //
    //                //   How to choose the capital for a province? 
    //                //        Since the province is a connected set in the DFS tree (because we accumulated in DFS order), it is connected. 
    //                //        Moreover, in the tree, the province has exactly one edge to the outside (the edge connecting the root of the province's subtree to its parent, except for the root of the whole tree if it's not cut). 
    //                //        But in the assignment, the province might include the root of the whole tree, then it has no outside edge? However, we only have one province if the whole tree is one province.
    //                //        For a province that is not the entire tree, it has exactly one neighbor outside. Let that neighbor be x. Then we can choose x as the capital for this province. 
    //                //        Why? Because for any node v in the province, the path from v to x goes through the unique edge to the outside, and all nodes on the path except x are in the province.
    //                //        And x might be shared by multiple provinces.
    //                //        How to find x for a province? 
    //                //            During DFS, when we cut a province at node u (meaning the province includes u and some descendants), then the parent of u (in the DFS tree) is the outside node. But note: the province might include u and its descendants, and the edge (parent(u), u) is the only edge to the outside. So the outside node is parent(u).
    //                //        However, in the DFS we are not storing the parent of the province, but we can store it.
    //                //        Alternatively, after building the province, we can look for a node in the province that has a neighbor outside the province. There should be exactly one such edge if the province is not the entire graph, but if it is the entire graph, then we can choose any node as capital (and it's in the province).
    //                //        But the problem: the capital must be a city, and it can be outside the province.
    //                //        So for a province P:
    //                //            If P is the entire graph, then choose any node in P as capital (say the first node).
    //                //            Else, find a node u in P that has a neighbor v not in P. There should be exactly one such v (in the tree), and we can choose v as the capital.
    //                //        However, multiple provinces might share the same capital v (if v has multiple neighbors in different provinces).
    //                //        In the sample: 
    //                //            Province [1,2,3]: neighbors outside? None if it's the entire graph, but it's not. It has an edge to 8? No, in the graph: 1 is connected to 2 and 8. If province [1,2,3] contains 1,2,3, then the edge 1-8 goes to 8 which is not in the province. So the outside neighbor of 1 is 8. So capital for province [1,2,3] could be 8.
    //                //            Province [7,8]: outside neighbor of 8 is 1? But 1 is in province [1,2,3]. So capital for province [7,8] could be 1.
    //                //            Province [4,5,6]: outside neighbor of 6 is 8, so capital=8.
    //                //        Then capitals: 8, 1, 8. 
    //                //        But the sample output capitals are 2,1,8.
    //                //        However, the problem does not require a specific capital, so 8,1,8 is valid? 
    //                //            Province1: capital=8 -> for city1: 1->8, but 8 is not in province1. Intermediate: none? -> valid.
    //                //            Province2: capital=1 -> for city7: 7->8->1, intermediate 8 must be in province2 -> it is (city8 is in province2), so valid.
    //                //            Province3: capital=8 -> for city4: 4->6->8, intermediate 6 in province3 -> valid.
    //                //        So it's valid.
    //                //        But the sample output has two capitals: 2,1,8. Why 2 for province1? 
    //                //            They have province1 = [2,3], so for city2: 2->1 (city1 is in province2), so capital=1 is the capital for province2, but they output capital=2 for province1. 
    //                //        How can capital=2 for province1? 
    //                //            Then for city2: it is the capital, so trivial.
    //                //            For city3: 3->2, and 2 is the capital (in the province), so valid.
    //                //        So capitals can be in the province.
    //                //        In fact, it's simpler to choose a capital within the province: any node in the province. 
    //                //        Why? Because the path from any node v in the province to the capital (within the province) is entirely in the province, so all intermediate nodes are in the province.
    //                //        And the condition allows the capital to be in the province (then the terminal city is in the province, so no outside node).
    //                //        Therefore, for any province, we can choose any node in the province as the capital.
    //                //        So to simplify, for each province, we can choose the first node in the province list as the capital.
    //                //
    //                //   However, the problem says: "the cities where the capitals of the provinces must be located in", and there is no restriction that the capital must be in the province. But since it's valid to have the capital in the province, and it's easier, we do that.
    //                //
    //                //   Steps for output:
    //                //        - After building the list of provinces (vector<vector<int>> provinces), 
    //                //        - Create an array assign[1..N]: for province index i (0-indexed), assign[u] = i+1 for each u in provinces[i].
    //                //        - Output the number of provinces: K = provinces.size()
    //                //        - Output assign[1], assign[2], ... assign[N]
    //                //        - Output for each province i, the capital = provinces[i][0] (the first node in the province)
    //                //
    //                //   But wait: in the sample, if we have provinces = [[1,2,3], [7,8], [4,5,6]], then capitals would be 1,7,4.
    //                //        Output: 
    //                //            3
    //                //            1 1 1 3 3 3 2 2   [for cities 1..8: 1->1, 2->1, 3->1, 4->3, 5->3, 6->3, 7->2, 8->2]
    //                //            1 7 4
    //                //        But the sample expected output has province for city1=2, city2=1, etc.
    //                //        However, the problem does not require a specific valid solution, only a valid one. So this should be accepted.
    //                //
    //                //   But let's check the sample test with our output:
    //                //        Province1: [1,2,3] -> capital=1. 
    //                //            City1: in province1, capital=1 -> in province, valid.
    //                //            City2: path 2->1, intermediate: none -> valid.
    //                //            City3: path 3->2->1, intermediate 2 is in province1 -> valid.
    //                //        Province2: [7,8] -> capital=7.
    //                //            City7: capital, valid.
    //                //            City8: path 8->7, intermediate: none -> valid.
    //                //        Province3: [4,5,6] -> capital=4.
    //                //            City4: capital, valid.
    //                //            City5: 5->6->4? But there is no edge 6-4 in the province? 
    //                //                The tree: 4-6, 6-5, so the path from 5 to 4 is 5->6->4. 
    //                //                Intermediate: 6. Is 6 in province3? Yes.
    //                //            City6: 6->4? But there's no direct edge 6-4 in the province? The edge 6-4 exists in the tree, so the path is 6->4. Intermediate: none -> valid.
    //                //        So it's valid.
    //                //
    //                //   However, the sample provided output is different, but that's okay.
    //                //
    //                //   Implementation:
    //                //        - We'll use a global vector `comp` and `provinces`.
    //                //        - DFS from 1, parent=0.
    //                //        - After DFS, if comp is not empty, try to merge with the last province.
    //                //        - Check all provinces have size in [B, 3B].
    //                //        - If any province has size < B or > 3B, output 0.
    //                //        - Also, if after merging the root's comp, it's still not enough, output 0.
    //                //
    //                //   Special case: if B==1, then every single node is a province? But then we have N provinces, and each province has size 1 (which is in [1,3]). 
    //                //        However, the DFS will cut after every node? 
    //                //            For a leaf: comp=[leaf] -> size=1>=1 -> cut. 
    //                //            Then the parent: comp=[parent] -> cut, etc.
    //                //        But then the root's comp might be empty? 
    //                //            Actually, for the root: after processing all children, comp might be empty if all children cut, then we push the root? 
    //                //            In the DFS: 
    //                //                comp.push_back(root) -> then for each child, after DFS(child) we cut if possible. 
    //                //                But for a leaf root (N=1): comp=[1], then no children, so after DFS, comp=[1] -> then we try to merge? but no last province, so we output 0? 
    //                //            Correction: for N=1, B=1: 
    //                //                comp = [1]
    //                //                provinces is empty.
    //                //                Then after DFS, comp is not empty, and provinces is empty -> we check: if (provinces.empty()) and comp.size()>0, then we cannot merge -> output 0? 
    //                //            But it should be valid: one province with city1.
    //                //        So we need to handle the case when provinces is empty: then if comp.size() is between B and 3B, we make one province.
    //                //
    //                //   Revised after DFS:
    //                //        if (!provinces.empty()) {
    //                //            if (!comp.empty()) {
    //                //                if (comp.size() + provinces.back().size() <= 3*B) {
    //                //                    provinces.back().insert(provinces.back().end(), comp.begin(), comp.end());
    //                //                    comp.clear();
    //                //                } else {
    //                //                    cout << 0 << endl;
    //                //                    return 0;
    //                //                }
    //                //            }
    //                //        } else {
    //                //            if (!comp.empty()) {
    //                //                // This is the entire graph as one province
    //                //                if (comp.size() >= B && comp.size() <= 3*B) {
    //                //                    provinces.push_back(comp);
    //                //                    comp.clear();
    //                //                } else {
    //                //                    cout << 0 << endl;
    //                //                    return 0;
    //                //                }
    //                //            } else {
    //                //                // no nodes? impossible
    //                //                cout << 0 << endl;
    //                //                return 0;
    //                //            }
    //                //        }
    //                //        Then check all provinces.
    //                //
    //                //   Actually, we can do:
    //                //        After DFS, if comp is not empty:
    //                //            if provinces is not empty and provinces.back().size() + comp.size() <= 3*B:
    //                //                merge
    //                //            else if provinces is empty:
    //                //                if B <= comp.size() <= 3*B, then provinces.push_back(comp)
    //                //                else -> impossible
    //                //            else (provinces not empty but merging would exceed 3B):
    //                //                impossible.
    //                //        Then, after that, check every province has size in [B, 3*B].
    //                //
    //                //   Let's test with N=1, B=1:
    //                //        comp = [1]
    //                //        provinces empty -> then we push [1] (since 1 in [1,3]).
    //                //        Then output: 
    //                //            1
    //                //            1
    //                //            1
    //                //
    //                //   Test with the sample: 
    //                //        comp after DFS = [5] (in our simulation)
    //                //        provinces = [ [1,2,3], [8,7], [6,4] ]
    //                //        last province size=2, 2+1=3<=6 -> merge -> last province becomes [6,4,5]
    //                //        Then provinces = [ [1,2,3], [8,7], [4,5,6] ] -> sizes: 3,2,3 -> all in [2,6] -> valid.
    //                //
    //                //   But wait: the sample input has 8 cities, and our provinces have 3+2+3=8.
    //                //
    //                //   However, in the DFS simulation, we had:
    //                //        comp started as [1]
    //                //        then in DFS(2,1): comp becomes [1,2,3] and then cut -> provinces=[ [1,2,3] ], comp=[]
    //                //        then DFS(8,1): 
    //                //            comp=[8]
    //                //            DFS(7,8): comp=[8,7] -> cut -> provinces=[ [1,2,3], [8,7] ], comp=[]
    //                //            DFS(6,8): 
    //                //                comp=[6]
    //                //                DFS(4,6): comp=[6,4] -> cut -> provinces=[ [1,2,3], [8,7], [6,4] ], comp=[]
    //                //                DFS(5,6): comp=[5]
    //                //            end of DFS(6,8): comp=[5]
    //                //        end of DFS(8,1): comp=[5]
    //                //        end of DFS(1,0): comp=[5]
    //                //        Then merge: last province is [6,4] (size=2), 2+1=3<=6 -> merge -> [6,4,5]
    //                //
    //                //   So it works.
    //                //
    //                //   Edge: if merging would make a province > 3B, then impossible.
    //                //
    //                //   Let's code accordingly.
    //                //
    //                //   Note: The problem constraints: N up to 10000, so DFS recursion might cause stack overflow? 
    //                //        We can increase stack size or use iterative DFS. But 10000 nodes, recursion depth 10000 might be borderline in some judges, but 0.5 sec and 64MB, and recursion in C++ might be acceptable for 10000.
    //                //        Alternatively, we can do iterative DFS to avoid stack overflow.
    //                //
    //                //   However, the problem says 0.5 sec, and 10000 nodes, recursion should be fine.
    //                //
    //                //   Steps:
    //                //        - Read N, B.
    //                //        - Build graph.
    //                //        - If B > N, then impossible (but B<=N by constraints, so skip).
    //                //        - Do DFS(1,0) with global comp and provinces.
    //                //        - After DFS, handle leftover comp.
    //                //        - Check all provinces.
    //                //        - Output.
    //                //
    //                //   Let's hope it passes.

    vector<vector<int>> provinces;
    vector<int> comp;

    void dfs(int u, int p, const vector<vector<int>>& adj, int B) {
        comp.push_back(u);
        for (int v : adj[u]) {
            if (v == p) continue;
            dfs(v, u, adj, B);
            if ((int)comp.size() >= B) {
                provinces.push_back(comp);
                comp.clear();
            }
        }
    }

    int main() {
        ios::sync_with_stdio(false);
        cin.tie(0);
        
        int N, B;
        cin >> N >> B;
        
        vector<vector<int>> adj(N+1);
        for (int i = 0; i < N-1; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // If B > N, impossible, but B<=N per constraints.
        provinces.clear();
        comp.clear();

        dfs(1, 0, adj, B);

        // Handle the leftover in comp
        if (!comp.empty()) {
            if (!provinces.empty()) {
                // Try to merge with the last province
                if (comp.size() + provinces.back().size() <= 3 * B) {
                    provinces.back().insert(provinces.back().end(), comp.begin(), comp.end());
                    comp.clear();
                } else {
                    // Cannot merge, and no other province to merge with
                    cout << 0 << '\n';
                    return 0;
                }
            } else {
                // provinces is empty: this is the entire graph
                if (comp.size() < B || comp.size() > 3 * B) {
                    cout << 0 << '\n';
                    return 0;
                }
                provinces.push_back(comp);
                comp.clear();
            }
        }

        // Check all provinces
        for (const auto& prov : provinces) {
            if (prov.size() < B || prov.size() > 3 * B) {
                cout << 0 << '\n';
                return 0;
            }
        }

        // Output
        cout << provinces.size() << '\n';
        vector<int> assign(N+1, 0);
        for (int i = 0; i < provinces.size(); i++) {
            for (int u : provinces[i]) {
                assign[u] = i+1;
            }
        }
        for (int i = 1; i <= N; i++) {
            cout << assign[i];
            if (i < N) cout << ' ';
            else cout << '\n';
        }
        for (int i = 0; i < provinces.size(); i++) {
            cout << provinces[i][0];
            if (i < provinces.size()-1) cout << ' ';
            else cout << '\n';
        }

        return 0;
    }
};
