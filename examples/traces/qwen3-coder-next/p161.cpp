#include <bits/stdc++.h>
using namespace std;

// We represent elements of H as bitsets of vertices (since |X| <= 100, but we'll use vector<bool> or vector<int>)
// But note: H is the set of *antichains* (sets of pairwise incomparable elements)
// However, the problem says: H consists of all a in X such that any two different x,y from R are incomparable.
// Actually: H = { a ⊆ X : for all x,y ∈ a, x ≠ y ⇒ neither x ≤ y nor y ≤ x }

// Important: The construction uses:
//   x ≤ y iff there's a path from x to y (including zero-length => reflexive)
// Max(M) = { x ∈ M : ¬∃ y ∈ M, y ≠ x, x ≤ y } — the set of maximal elements of M.
// But note: for an antichain a, Max(a) = a (since no two elements are comparable, so every element is maximal).

// Operations:
//   a => b = { x ∈ b : ¬∃ y ∈ a, x ≤ y }
//   a ∧ b = Max(a ∪ b)
//   a ∨ b = Max( { x ∈ X : ∃ y ∈ a, z ∈ b, x ≤ y and x ≤ z } )
//   0 = Max(X)  [Note: Max(X) = set of maximal elements of entire X, i.e., sinks]
//   1 = ∅
//   ¬a = (a => 0)
//   a == b = (a => b) ∧ (b => a)

// However, note: the problem says:
//   0 = Max(X) and 1 = empty set.
// But in intuitionistic logic, 0 is usually the bottom (false) and 1 is top (true).
// Here, they define 0 = Max(X) (the set of maximal elements of the whole graph, i.e., the sinks) and 1 = ∅.

// Important: The Heyting algebra structure is defined on H (the set of antichains). 
// But note: for any subset M, Max(M) is an antichain (by definition: if two elements of Max(M) were comparable, say x ≤ y with x,y ∈ Max(M), then x ≤ y and y ∈ M implies x is not maximal in M, contradiction). So Max(M) ∈ H.

// Steps:
// 1. Precompute the reachability relation: for each pair (i, j), is there a path from i to j?
//    We can use Floyd-Warshall or BFS from each node (N <= 100, so O(N*(N+M)) is acceptable).

// 2. Represent each antichain (element of H) as a bitmask (since N<=100, but 2^100 is too big). 
//    However, the problem says: "the number D = |H| doesn't exceed 100", so we can enumerate all antichains.

// Approach:
//   - Enumerate all antichains (subsets of vertices that are pairwise incomparable) and assign an index to each.
//   - Precompute the operations (=>, ∧, ∨, ¬, etc.) as functions that take two antichains (represented by their index) and return an antichain (index).

// But note: the problem says "Sum(Di) <= 10^6", meaning the total work over all formulae is acceptable.

// Alternative: Since D <= 100, we can store all antichains as sets (vector<bool> of size N or bitset<100>).
//   We'll represent each antichain by a sorted vector of vertices? Or by a bitset? Since N<=100, we can use bitset<100> or vector<int>.

// However, the operations are defined on arbitrary subsets M, but then we take Max(M). But note: for antichains a and b, we can compute:
//   a => b: 
//        = { x ∈ b : ¬∃ y ∈ a, x ≤ y }
//        Since a is an antichain, but b is an antichain too.
//   a ∧ b = Max(a ∪ b) — but note: a ∪ b might not be an antichain, so we take maximal elements.
//   a ∨ b = Max( { x ∈ X : ∃ y ∈ a, z ∈ b, x ≤ y and x ≤ z } ) 
//        This set is the set of common lower bounds of a and b, then take maximal elements.

// How to compute:
//   Let rel[i][j] = true if there is a path from i to j (including i=j).

// Steps for precomputation:
//   Step 1: Build the graph and compute transitive closure (using BFS/DFS for each node, or Floyd-Warshall).
//   Step 2: Enumerate all antichains. Since D <= 100, we can do it by iterating over all subsets? But 2^100 is too big.
//        However, the note says D <= 100, so we don't have to enumerate 2^N, we just need to generate all antichains until we have 100? 
//        Actually, the problem says: "the number D = |H| of elements of H doesn't exceed 100", so we know there are at most 100 antichains.

// How to enumerate antichains? 
//   We can use backtracking (DFS) to generate all antichains. But worst-case the number of antichains can be huge, but the problem states D<=100, so we can do it.

// Alternatively, we can note: the poset is a DAG (since acyclic). There is a known way to enumerate antichains via the Dutour-Sikiric algorithm? 
//   But since D<=100, we can do a simple backtracking: 
//        Start from the set of all vertices, and try to include or exclude each vertex, but with the constraint that if we include a vertex, we cannot include any vertex comparable to it.

// However, a simpler way: 
//   Since N is small (<=100) but the antichain family is small (<=100), we can generate all antichains by iterating over all subsets? 
//   But 2^100 is too big. So we need a smarter way.

// Observation: In a DAG, the size of the largest antichain is at most the width. But the problem doesn't bound the width, but it bounds the total number of antichains (D<=100). So we can generate all antichains by DFS that respects the antichain condition.

// Algorithm to enumerate antichains:
//   We can use the following: 
//        We process vertices in topological order. For each vertex, we decide to include it or not.
//        If we include it, then we cannot include any vertex that is reachable from it (as a descendant) or that can reach it (as an ancestor) — but actually, we only need to avoid including any vertex that is comparable to an already included one.

//   However, since D is small (<=100), we can do:
//        Let's collect all antichains by starting from the empty set and then using a BFS/DFS that adds one element at a time, ensuring no two are comparable.

//   But worst-case the number of antichains is exponential, but the problem states D<=100, so we assume it's small.

// Steps:
//   Precomputation:
//     1. Read graph, build adjacency list.
//     2. Compute transitive closure: reach[i][j] = true if there's a path from i to j (including i=j).
//        Note: since it's acyclic, we can do topological sort and then DP.

//     3. Enumerate all antichains:
//          We'll generate all subsets S of {0,1,...,N-1} such that for any two distinct x,y in S, 
//             not (reach[x][y] or reach[y][x])
//          But 2^N is too big if N=100. However, the problem states D<=100, so we can use recursion with pruning.

//   Alternate idea: Since D<=100, we can use the following: 
//        The set H is closed under the operation of taking maximal elements? Actually, H is defined as the set of antichains. 
//        But note: for any subset M, Max(M) is an antichain, so H = { Max(M) : M ⊆ X }.
//        And the problem says D = |H| <= 100.

//        So we can iterate over all subsets M? But 2^N is too big.

//   However, note: the problem says "the number D = |H| of elements of H doesn't exceed 100", so we don't have to worry about the enumeration cost — it's bounded by 100. 
//        We can generate all antichains by a DFS that builds antichains step by step, and we stop when we have 100? But the problem says D<=100, so we can generate until we have D elements.

//   How to generate antichains without duplication?
//        We can use the following method (from "Generating all antichains of a poset" by connected components?):
//        But since D is small, we can do:

//        Let's consider the vertices in topological order (so that all edges go from lower to higher index). 
//        Then an antichain is a set of vertices such that no two are connected by a path.

//        We can use recursive backtracking:
//          - Sort vertices in topological order: v0, v1, ..., v_{n-1}
//          - We'll try to include or exclude each vertex, but if we include a vertex, we skip all vertices that are comparable to it (both above and below) — but in topological order, we only go forward? Actually, in topological order, if there's an edge from u to v, then u comes before v. But paths can go arbitrarily.

//        However, we can do:
//          Let's maintain a set of "forbidden" vertices (those that are comparable to any chosen vertex). 
//          But we don't know the comparable set until we have chosen.

//        Alternatively, we can use the following: 
//          The set of antichains is in bijection with the set of lower sets? Not exactly.

//        Given the small D, we can simply iterate over all subsets of vertices of size up to N, but skip subsets that are not antichains, and then take Max(S) for each S and collect distinct Max(S). 
//        But the number of subsets is 2^N which is too big.

//   Better: use the fact that H is small. We can use a BFS starting from the empty set and then adding one element at a time? 
//        But we need to avoid duplicates.

//   Actually, there is a standard algorithm: 
//        "The set of antichains of a poset can be generated by the following recursive procedure: 
//          - If the poset is empty, output the empty set.
//          - Pick an element x. 
//          - Generate all antichains that do not contain x (recursively on poset without x).
//          - Generate all antichains that contain x (recursively on the poset of elements that are incomparable to x, but only the elements that are >= some element? Actually, no: we remove all elements comparable to x, and then generate antichains on the remaining, and then add x to each)."

//   But since D<=100, we can use a simple DFS that builds antichains and prunes when the set becomes too big? 

//   Implementation:
//        We'll use a vector<vector<bool>> antichains; // each is a vector<bool> of length N: true if vertex is included.
//        We do DFS:
//          void dfs(int idx, vector<bool>& current, vector<vector<bool>>& res) {
//             if (idx == N) {
//                 // check: is current an antichain? Actually, we maintain it as antichain during construction.
//                 res.push_back(current);
//                 return;
//             }
//             // Option 1: skip idx
//             dfs(idx+1, current, res);
//             // Option 2: include idx, but only if it is incomparable to all in current.
//             bool can_include = true;
//             for (int i = 0; i < N; i++) {
//                 if (current[i] && (reach[i][idx] || reach[idx][i])) {
//                     can_include = false;
//                     break;
//                 }
//             }
//             if (can_include) {
//                 current[idx] = true;
//                 dfs(idx+1, current, res);
//                 current[idx] = false;
//             }
//          }

//        However, worst-case the number of antichains is exponential, but the problem states D<=100, so it's acceptable.

//   But note: the problem says D<=100, so we can generate all antichains and then we have at most 100.

// Steps summary for precomputation:
//   Step 1: Read graph, build adjacency list.
//   Step 2: Compute transitive closure (reach[i][j] for all i,j).
//        We can do:
//          reach = identity initially.
//          Then for k in topological order, for i, for j: 
//             reach[i][j] = reach[i][j] || (reach[i][k] && reach[k][j])
//        But since it's a DAG, we can do topological sort and then DP.

//        Alternatively, we do BFS from each node (N<=100, M<=5000, so 100 BFS is 100*(N+M) = 100*5100 = 510000, acceptable).

//   Step 3: Enumerate all antichains (H) as vector<vector<bool>> or vector<vector<int>>. We'll use vector<bool> for each antichain, and then we can map to an index.

//   Step 4: Precompute a mapping from antichain (bitmask or vector) to an index (0..D-1). Also, we'll store the antichains in a vector `antichains`.

//   Step 5: Precompute the operations as functions that take two indices and return an index.

//        For two antichains a and b (represented by vector<bool> of length N, or we can store the actual set of vertices?):
//          a => b: 
//             S = { x in b : for all y in a, not (x <= y) } 
//                 = { x ∈ b : ¬∃ y ∈ a such that reach[x][y] }
//             Then we need Max(S) — but note: S is a subset of b, and b is an antichain. However, S might not be an antichain? 
//                 Actually, if two elements x,y in S are comparable, say x<=y, then since y in b and x<=y, then x should not be in S? 
//                 Why? Because for y in b, and x<=y, then when we check for y: we see that x (if in a?) — but wait, the condition for y to be in S is: no y' in a such that y<=y'. 
//                 But note: if x<=y and x,y in b, and b is an antichain, then x and y cannot be comparable? So actually, S is a subset of an antichain, hence S is an antichain. 
//                 Therefore, Max(S)=S. So we can just take S as the antichain.

//          However, wait: the definition says: a => b = {x in b : not exists y in a: x <= y}. 
//          But note: if x and y are in b and x<=y, then since b is an antichain, x and y must be equal? But the set b has distinct elements, so x and y are distinct -> contradiction. So b is an antichain, so no two distinct elements are comparable. Therefore, S is an antichain.

//          So: a => b = S (as a set) — which is an antichain.

//          a ∧ b = Max(a ∪ b). Since a and b are antichains, but a ∪ b might not be. So we take the maximal elements of a ∪ b.
//             How: 
//                 S = a ∪ b
//                 Then M = { x in S : not exists y in S, y != x, such that x <= y }
//                 Since S is finite, we can compute M.

//          a ∨ b = Max( { x in X : exists y in a, z in b, such that x <= y and x <= z } )
//                 Let T = { x in X : (exists y in a, x<=y) and (exists z in b, x<=z) } 
//                 But note: the condition is: x <= y and x <= z, so x is a common lower bound of a and b.
//                 Then we take the maximal elements of T.

//   Step 6: Precompute constants:
//        0 = Max(X) — the set of maximal elements of the entire set X (i.e., vertices with no outgoing edge to a different vertex? but note: maximal element: no y in X such that x < y, i.e., no path from x to a different vertex? Actually, no: x is maximal if there is no y in X (y != x) such that x <= y. But note: x<=x, so we require: no y != x with x<=y.
//        1 = empty set.

//   Step 7: Parse the formulae and evaluate.

// How to evaluate a formula with variables?
//   We have to check: for *every* assignment of variables to elements of H, the formula evaluates to 1 (the empty set).
//   But the problem says: "a formula E is called valid if after substitution of arbitrary elements of H for the variables involved in E it evaluates to 1"

//   However, note: the formula might have multiple variables. The number of variables is at most 26, but D<=100, so the total assignments is D^(number of variables). 
//        But the problem says: "Sum(Di) <= 10^6", meaning the total work over all formulae is bounded by 10^6. 
//        How? 
//          For one formula with v variables, we need to check D^v assignments. 
//          But worst-case v=26, D=100 -> 100^26 is astronomical.

//   Rethink: the problem note says: "Sum(Di) <= 10^6". What is Di? 
//        Likely, Di is the size of the formula i (in terms of operations) or the number of assignments we have to try? 
//        Actually, the problem says: "the number D = |H| of elements of H doesn't exceed 100 and that Sum(Di) <= 10^6". 
//        So Di probably is the number of assignments for formula i? Or the work per formula?

//   Clarification from the problem: 
//        "Sum(Di) <= 10^6" — and D is |H|, so Di might be the number of assignments for formula i? But then if one formula has 26 variables, Di = D^26 which is huge.

//   Alternate interpretation: 
//        In the evaluation, we don't need to check all assignments? 
//        But the definition is: valid if for all assignments it holds.

//   However, the problem says: "Sum(Di) <= 10^6", so it must be that for each formula, the work is bounded by Di, and the sum over formulae is <= 10^6.

//   How can we avoid exponential blowup? 
//        The key: the formula is built from operations that are defined on H, and H has only D elements. 
//        But the number of assignments is D^(#variables). However, the problem states that the total work is bounded by 10^6, so the number of variables per formula must be small? 
//        But the sample has formulae with up to 4 variables (X,Y,A,B,C) — but 4 variables would be 100^4 = 100e6 which is too big.

//   Another idea: 
//        The problem says: "the number D = |H| of elements of H doesn't exceed 100 and that Sum(Di) <= 10^6". 
//        And note: the formulae are given as strings, and the total length is bounded (each line <=254, K<=20). 
//        But the work per formula might be the size of the formula tree times D? 

//   Insight: 
//        In Heyting algebras, to check validity, we can use a semantic tableau or use the fact that the algebra is finite. 
//        However, the standard way is to check all assignments. 
//        But the problem constraints say that the total work (over all formulae) is <= 10^6, so the number of assignments per formula must be small. 
//        How? 
//          Note: the formula might not use all 26 letters. The variables are only the ones that appear. 
//          And the number of distinct variables in one formula is at most the number of distinct capital letters in the string. 
//          Since the formula string is short (<=254), the number of distinct variables is at most 26, but likely small (the sample has at most 3: X,Y,A,B,C — but in one formula: A=>B=>C = (A&B)=>C -> 3 variables).

//        However, 100^3 = 1e6, and 100^4=100e6 which is too big. But the problem says Sum(Di)<=10^6, so the worst-case formula might have only 2 or 3 variables.

//        Looking at the sample input #2: 
//          Formulae: 
//            X|~X -> 1 variable -> 100 assignments
//            A=>B=>C = (A&B)=>C -> 3 variables -> 100^3 = 1e6
//          But the total sum is 10^6, so if one formula has 100^3=1e6, then we can only have one such formula.

//        So we assume: for a formula with v variables, we do D^v assignments, and the problem guarantees that the sum of D^v over the formulae is <= 10^6.

//        Therefore, we can do:
//          For each formula:
//             Let V = set of distinct variables in the formula.
//             v = |V|
//             For each assignment: a function from V to H (each variable assigned an antichain index)
//                 Evaluate the formula with that assignment -> get an antichain index.
//                 Check if it equals the index for 1 (empty set).
//             If any assignment yields not 1, then the formula is invalid.

//        We must be cautious: the problem says "Sum(Di) <= 10^6", so we assume that the total number of assignments we try over all formulae is <= 10^6.

// Implementation plan:

// Precomputation:
//   Step 1: Read N, M, build graph (0-indexed vertices).
//   Step 2: Compute transitive closure (reach[i][j] for i,j in [0, N-1]).
//        We'll do BFS from each node.

//   Step 3: Enumerate all antichains (H). We'll generate all subsets S of [0, N-1] that are antichains (pairwise incomparable) and also note that Max(M) for any M is an antichain, but actually the set H is exactly the set of antichains (because if S is an antichain, then Max(S)=S). So we only need to generate all antichains.

//        We do DFS to generate antichains. We'll store them in a vector<vector<bool>> antichainList.

//        However, note: the problem says H contains the empty set and all one-element subsets. So we are generating all antichains.

//   Step 4: Build a mapping from an antichain (represented as a sorted vector of vertices? or as a bitset) to an index. 
//        We'll represent each antichain as a sorted tuple (or as a bitset of N bits) and use a set<vector<bool>> or set<vector<int>>.

//        But since N<=100, we can use a bitset<100> and then convert to an integer? But 100 bits -> too big for integer. 
//        Alternatively, we can use a string of N chars, but that's heavy. 
//        Instead, we can use a vector<bool> and then use a set of vector<bool> (but vector<bool> is not hashable). 
//        We can use vector<int> and then use a set of vector<int>? Or we can use a bitset and then convert to a string of 0/1? 

//        Since D<=100, we can store the antichains as vector<int> (the list of vertices in sorted order) and then use a set<vector<int>>.

//        Steps for enumeration:
//          - Sort the vertices in topological order? Actually, for the DFS we don't need topological order, but we can iterate vertices in any order (0 to N-1) and it's fine.

//        Algorithm for enumeration:
//          vector<vector<int>> antichains;
//          vector<int> current;
//          function<void(int)> dfs = [&](int i) {
//             if (i == N) {
//                 antichains.push_back(current);
//                 return;
//             }
//             // Option 1: skip i
//             dfs(i+1);
//             // Option 2: include i, if it doesn't conflict with current
//             bool ok = true;
//             for (int j : current) {
//                 if (reach[j][i] || reach[i][j]) {
//                     ok = false;
//                     break;
//                 }
//             }
//             if (ok) {
//                 current.push_back(i);
//                 dfs(i+1);
//                 current.pop_back();
//             }
//          };
//          dfs(0);

//        But note: the problem says D<=100, so the DFS will not be too deep and the total number of nodes visited is bounded by 2 * D? Not exactly, but worst-case the number of antichains is D, and the DFS visits O(2^N) nodes, but the problem states D<=100, and N<=100, but worst-case 2^100 is too big.

//        We need a better enumeration. However, the problem states D<=100, so we hope that the DFS prunes a lot. 
//        But worst-case (a total order) the number of antichains is N+1, so it's small. 
//        Worst-case for antichain enumeration is a set of N/2 incomparable pairs? Then the number of antichains is 3^(N/2) which for N=100 is huge.

//        But the problem states D<=100, so the input is such that the poset has at most 100 antichains. 
//        How to enumerate without blowing up? 
//          We can use the following: 
//             Since D<=100, we can generate all antichains by iterating over all subsets of vertices of size 0,1,...,N, but skip non-antichains, and then take distinct Max(S). 
//          However, the number of subsets is 2^N which is too big.

//        Alternate known method: 
//          "Antichain enumeration by minimal elements" — but we don't know.

//        Given the constraints (D<=100) and that the total work over formulae is 10^6, and N<=100, but worst-case antichain enumeration might be heavy, we note that the problem says D<=100. 
//        There is an algorithm that runs in O(D * N) or something? 

//        Actually, we can use the following: 
//          The set of antichains is in bijection with the set of lower sets of the dual? Not helpful.

//        Since D is small (<=100), we can use a simple iterative method: 
//          Start with the empty set.
//          Then, for each vertex, we try to add it to existing antichains, but only if it is incomparable to all elements in the antichain.
//          We use a queue: 
//             queue<vector<int>> q;
//             set<vector<int>> seen;
//             q.push({}); 
//             seen.insert({});
//             while (!q.empty()) {
//                 auto a = q.front(); q.pop();
//                 antichains.push_back(a);
//                 for (int i = 0; i < N; i++) {
//                     // try to add i to a
//                     bool ok = true;
//                     for (int x : a) {
//                         if (reach[x][i] || reach[i][x]) {
//                             ok = false;
//                             break;
//                         }
//                     }
//                     if (ok) {
//                         vector<int> b = a;
//                         b.push_back(i);
//                         sort(b.begin(), b.end());
//                         if (seen.find(b) == seen.end()) {
//                             seen.insert(b);
//                             q.push(b);
//                         }
//                     }
//                 }
//             }
//          But this might generate duplicates and also it's BFS and the number of antichains is D, but the branching factor is N, so total nodes in BFS is D * N? But worst-case, the BFS queue might have D nodes, and each node branches to N, so D*N <= 100*100=10000, which is acceptable.

//        However, note: the same antichain can be built in different orders, but we sort and use a set, so duplicates are avoided.

//        But worst-case, the BFS might generate more than D nodes? Actually, the set `seen` ensures we only process each antichain once, and each antichain is generated once. 
//        The total number of states is D, and for each state we try N neighbors, so total operations D*N <= 100*100=10000.

//        This is acceptable.

//   Step 5: Precompute operations:
//        We'll create a struct for the Heyting algebra operations on the set of antichains (indexed 0..D-1).

//        For two antichains a and b (represented by vector<int> of vertices, or by the index we stored the list), we define:
//          op_imp: a => b
//             S = { x in b : for all y in a, not (x <= y) } 
//                 = { x in b : for all y in a, !reach[x][y] }
//             Then S is an antichain (as argued), so we can find the antichain index for S.

//          op_and: a ∧ b = Max(a ∪ b)
//             T = a ∪ b
//             S = { x in T : for all y in T, if x <= y then x==y } 
//                 = { x in T : for all y in T, if x != y then !reach[x][y] }
//             Then S is an antichain.

//          op_or: a ∨ b = Max( { x in X : exists y in a, z in b, such that x <= y and x <= z } )
//             Let U = { x in [0, N-1] : (exists y in a, reach[x][y]) and (exists z in b, reach[x][z]) }
//             Then S = { x in U : for all y in U, if x <= y then x==y } 
//                 = { x in U : for all y in U, if x != y then !reach[x][y] }

//        Note: We have the entire set of vertices [0, N-1].

//   Step 6: Precompute constants:
//        One_0: 0 = Max(X) = { x in [0, N-1] : for all y in [0, N-1], if x <= y then x==y } 
//                = { x : for all y != x, !reach[x][y] } 
//        One_1: 1 = empty set.

//   Step 7: For each formula, parse it and then for all assignments of variables, evaluate.

// How to parse the formula?
//   The tokens: 
//        Constants: "0", "1"
//        Variables: "A" to "Z"
//        Parentheses: "(", ")"
//        Operators: 
//            "~" for not (one character)
//            "&" for /\ (but the problem says: E1 /\ E2 /\ ... /\ En, and left associative)
//            "|" for \/
//            "=>" for => (two characters)
//            "=" for == (but note: == is represented by a single '=' token? The problem says: "tokens ... =", but then says "==". 
//            However, the sample input: 
//                "1=0", "X|~X", "A=>B=>C = (A&B)=>C", etc.
//            And the note: "the last five tokens stand for not, /\, \/, => and ==" — but the last is "==" meaning two '=' signs? 
//            But the problem says: "E1 == E2 == ... == En", and the tokens are listed as "=". 

//        Actually, the problem says: 
//            "Tokens can be separated by an arbitrary number of spaces. No line will be longer than 254 characters."
//            and tokens: 0, 1, A, ..., Z, (, ), ~, &, |, =>, =

//        So the token for implication is "=>" (two characters) and for equivalence is "=" (but note: the sample writes "1=0", meaning equivalence? 
//        And the problem says: "E1 == E2 == ... == En", so it's a chain of equivalences.

//        How to tokenize?
//          We traverse the string and:
//            - skip spaces
//            - if we see '0' or '1' -> token
//            - if we see a letter, then it's a variable (one capital letter)
//            - '(' and ')' are tokens.
//            - '~' is one token.
//            - '&' is one token.
//            - '|' is one token.
//            - if we see '>', then we might have seen a '<' or ' '? But the problem says the tokens are "=>", so we look for '=>' as a two-character token.

//        Actually, the problem says: the tokens are: 0,1,A..Z,(,),~, &, |, =>, = 
//        So '=' is a token by itself? But then how to distinguish '==' from two '='? 
//        The problem says: "E1 == E2 == ... == En" and the token is "=", so it's likely that the equivalence operator is represented by a single '=' and the chaining is done by the parser? 

//        However, the sample input: 
//            "1=0" -> this is a formula with one '=' -> meaning equivalence? 
//            and "A=>B=>C = (A&B)=>C" -> has one '=' in the middle and two '=>' on the left.

//        And the problem says: 
//            "Equivalence - E1 == E2 == ... == En. This expression is equal to (E1 == E2) /\ (E2 == E3) /\ ... /\ (En-1 => En)."
//        But wait: it says "En-1 => En" — that must be a typo: it should be "En-1 == En".

//        Actually, the sample output: 
//            Input #1: 
//                1=0 -> invalid
//            So 1=0 is invalid.

//        How to parse:
//            We'll break the string into tokens. 
//            Steps for tokenization:
//                tokens = []
//                i=0
//                while i < len(s):
//                  if s[i] is space: i++
//                  else if s[i] is '0' or '1': token = s[i], i++
//                  else if s[i] is letter: token = string(1, s[i]), i++
//                  else if s[i] is '(' or ')': token = s[i], i++
//                  else if s[i] is '~' or '&' or '|': token = s[i], i++
//                  else if s[i] is '=' and next is '='? -> but the problem says the token is "=", so it's one '='. 
//                  However, the problem says: the tokens are "=>" and "=", so we have to check for "=>" first.

//            Specifically:
//                if we see '>', then we look back: but the tokens are given as "=>", so we should see '=>' as two consecutive characters.

//            How about: 
//                if s[i]=='=' and s[i+1]=='=' -> then token "=="? 
//                But the problem says the token is "=", so it's one '='. 

//            Actually, the problem statement: 
//                "Tokens ... ="
//                and then: "E1 == E2 == ... == En"

//            This is confusing. Looking at the sample input: 
//                "1=0", "A=>B=>C = (A&B)=>C", etc.

//            And the output: 
//                Sample output #1: 
//                    invalid   [for 1=0]
//                    valid
//                    ...

//            So "1=0" is a formula with one '='. 

//            The problem says: "E1 == E2 == ... == En" and then defines it as (E1==E2) /\ (E2==E3) /\ ... 
//            But that would require two tokens per equivalence? 

//            Rethink: the problem says: 
//                "Equivalence - E1 == E2 == ... == En. This expression is equal to (E1 == E2) /\ (E2 == E3) /\ ... /\ (En-1 => En)."
//            There is a typo: it should be "En-1 == En", so the operator is a binary operator, and the chaining is left-associative? But the problem says "from right to left" for implication, and for equivalence it doesn't specify associativity.

//            However, the problem says: 
//                "The operations are listed from the highest priority to the lowest." 
//                and then: 
//                  * Negation
//                  * Conjunction, Disjunction
//                  * Implication
//                  * Equivalence? -> but it's not listed in the priority list.

//            Actually, the problem lists:
//                Constants, Variables, Parentheses, 
//                then: Negation, Conjunction, Disjunction, Implication, Equivalence.

//            And the priority (highest to lowest) is: 
//                Negation, then Conjunction and Disjunction (same level? but not specified), then Implication, then Equivalence? 
//            But the problem says: 
//                "The operations are listed from the highest priority to the lowest." 
//                and then lists: 
//                  * Negation - not E 
//                  * Conjunction - E1 /\ ... /\ En
//                  * Disjunction - E1 \/ ... \/ En
//                  * Implication - E1 => E2
//                  * Equivalence - E1 == E2 == ... == En

//            So the priority order (highest to lowest) is: 
//                Negation > Conjunction = Disjunction > Implication > Equivalence? 
//            But typically, equivalence has lower priority than implication.

//            However, the problem states the associativity: 
//                Conjunction and disjunction: left associative.
//                Implication: right associative.
//                Equivalence: ? -> the definition says it's a chain: (E1==E2) /\ (E2==E3) /\ ... 

//            So for equivalence, it is not associative in the usual sense, but the formula "E1==E2==E3" is interpreted as (E1==E2) /\ (E2==E3).

//        How to parse?
//          We can use recursive descent with operator precedence.

//          Precedence (lowest to highest):
//            Equivalence (==) < Implication (=>) < Disjunction (|) < Conjunction (&) < Negation (~) < Parentheses and atoms.

//          But the problem lists from highest to lowest: 
//            Negation (highest), then Conjunction, Disjunction, Implication, Equivalence (lowest).

//          So:
//            Level 1 (lowest): Equivalence (==) — right associative? or left? 
//                The problem says: "E1 == E2 == ... == En" is (E1==E2) /\ (E2==E3) /\ ... 
//                This is left associative for the conjunction of equivalences? But the entire expression is a conjunction of adjacent equivalences.

//            Actually, the problem says: 
//                "This expression is equal to (E1 == E2) /\ (E2 == E3) /\ ... /\ (En-1 == En)."
//            So the equivalence operator here is not a binary operator that chains, but a macro: the entire chain is expanded to a conjunction.

//            Therefore, in the parser, we can treat "==" as a binary operator, but the formula "a == b == c" is not primitive: 
//                The input string for equivalence will be given as "a==b==c", but the problem says the token is "=", so it's actually "a=b=c", meaning three tokens: 'a', '=', 'b', '=', 'c'. 
//                And the specification says: it is (a==b) /\ (b==c).

//            So we have to parse: 
//                First, split the formula by the equivalence operator? But the equivalence operator has the lowest precedence.

//          Steps for parsing:
//            We'll write a recursive parser that handles:
//                expr   : equivalence_expr
//                equivalence_expr : implication_expr ( "==" implication_expr )*
//                but note: the problem says it's (E1==E2) /\ (E2==E3) /\ ... 
//                So we can parse: 
//                    left = implication_expr
//                    while next token is "==", then 
//                         right = implication_expr
//                         left = (left == right)   [but note: the operation is not binary, but a conjunction of adjacent equivalences?]

//            However, the problem says: the equivalence expression is defined as the conjunction of (Ei == Ei+1) for i=1..n-1.

//            Therefore, we can parse:
//                equivalence_expr: 
//                  parse the first implication_expr -> let's call it base.
//                  then while the next token is "==", parse the next implication_expr, and form base = base AND (base_old == next)? 
//                But that's not standard.

//            Actually, the problem says: 
//                "E1 == E2 == ... == En" is equivalent to (E1==E2) /\ (E2==E3) /\ ... /\ (En-1==En)
//            So we can do:
//                tokens: 
//                  [E1, '=', E2, '=', E3] -> then we want (E1==E2) and (E2==E3) and then AND them.

//            How about: 
//                Let's collect all the sub-expressions separated by '=='. 
//                If we have k sub-expressions, then the result is the conjunction of (E_i == E_{i+1}) for i=0 to k-2.

//            So in the parser for equivalence:
//                vector<Expr> parts;
//                parse an implication_expr -> push to parts.
//                while (next token is '==') {
//                    next token is '==', skip it.
//                    parse an implication_expr -> push to parts.
//                }
//                Then if parts.size()==1, then it's just that expression.
//                Else, 
//                    Expr res = (parts[0] == parts[1]);
//                    for i from 2 to parts.size()-1:
//                         res = res AND (parts[i-1] == parts[i]);
//                But note: the problem says: (E1==E2) /\ (E2==E3) for three, so it's consecutive.

//            However, the problem does not say that the equivalence is associative, so this is the definition.

//        Given the complexity, and since the formulae are short, we can do:

//          Step: 
//            Split the expression by '==' first? But '==' is two '=' tokens? 
//            Actually, the token is '=' (one character), so the string "a==b" would be tokens: 'a', '=', '=', 'b'. 
//            But the problem says the token for equivalence is "=", so two '=' in a row means two tokens of '='.

//          How to tokenize:
//            We'll do:
//              tokens.clear();
//              for i from 0 to len-1:
//                 skip spaces.
//                 if s[i]=='=' and s[i+1]=='=' -> then we have two tokens: each is '='? 
//                 But the problem says the token is "=", so each '=' is a separate token.

//          Therefore, the formula "a==b" is tokenized as: [a, =, =, b] -> but that's four tokens.

//          However, the problem says: the equivalence operator is represented by the token "=", so the formula "a==b" is not valid? 
//          But the sample input: "1=0", so one '='.

//          And the problem says: "E1 == E2 == ... == En", so they are using "==" to mean two '=' tokens.

//          So the tokenization: 
//            '==' is two separate '=' tokens.

//          Then the parsing: 
//            We have to consider that the equivalence operator is binary, and the formula "a==b==c" is parsed as: 
//                (a == b) == c   OR   a == (b == c) 
//            But the problem says it's (a==b) /\ (b==c), so it's not a binary operator but a chain.

//        Given the complexity, and since the problem states the formulae are syntactically correct, and the sample inputs are given as strings, 
//        we can do:

//          The equivalence operator has the lowest precedence, so we split the expression by '=' tokens that are not part of '=>'. 

//        How to avoid confusion with '=>'? 
//          When we see a '>', then the previous token must be '=' to form '=>', so we can check: 
//             if we see '>', then we look back: if the last token we read was ' ', then we don't have an '=' before? 
//          Instead, we do a single pass:

//          vector<string> tokens;
//          for (int i=0; i<s.size(); ) {
//             if (isspace(s[i])) { i++; continue; }
//             if (s[i]=='0' || s[i]=='1') {
//                 tokens.push_back(string(1, s[i])); i++;
//             } else if (isupper(s[i])) {
//                 tokens.push_back(string(1, s[i])); i++;
//             } else if (s[i]=='(' || s[i]==')') {
//                 tokens.push_back(string(1, s[i])); i++;
//             } else if (s[i]=='~' || s[i]=='&' || s[i]=='|') {
//                 tokens.push_back(string(1, s[i])); i++;
//             } else if (s[i]=='=') {
//                 if (i+1 < s.size() && s[i+1]=='=') {
//                     tokens.push_back("==");   // but the problem says the token is "=", so maybe we should not do this?
//                     i += 2;
//                 } else {
//                     tokens.push_back("=");
//                     i++;
//                 }
//             } else if (s[i]=='>' || s[i]=='<') {
//                 // We expect "=>", so if we see '>', then we look for a preceding '='? 
//                 // But the tokens are given without spaces, so "=>" is two consecutive characters.
//                 // However, we are processing left to right, so we might have seen the '=' already? 
//                 // Instead, we check for "=>" at position i: but we are at '>', so we check i-1 and i.
//                 // But we are at i, so we can check if the previous token was '='? 
//                 // This is messy.
//             }
//          }

//        Actually, the problem says the tokens are: 0,1,A..Z,(,),~, &, |, =>, = 
//        So the token "=>" is two characters, and '=' is one character.

//        Therefore, when we see a '>', we should check if the previous character was '='? But we are reading left to right, so we might have read the '=' already.

//        Better: check for "=>" first. 
//          If at position i we have s[i]=='=' and s[i+1]=='>', then push "=>", and i+=2.
//          Otherwise, if s[i]=='=', push "=", i++.

//        Similarly, we don't have to worry about '<' because the token is "=>", not "<=".

//        So tokenization:
//          for i from 0 to len-1:
//             skip spaces.
//             if s[i]=='=' and i+1<len and s[i+1]=='>': 
//                 tokens.push_back("=>"); i+=2;
//             else if s[i]=='=': 
//                 tokens.push_back("="); i++;
//             else if ... (others)

//        Then, the formula "a==b" becomes tokens: ['a', '=', '=', 'b'] -> three tokens for the equivalence? 
//        But the problem says: the equivalence operator is "==" and is represented by two '=' tokens? 

//        However, the problem statement says: "the last five tokens stand for not, /\, \/, => and ==" — and then lists: 
//            ~, &, |, =>, = 
//        So they list "=>" and then "=", meaning two separate tokens.

//        And the sample input: 
//            "X => Y" -> tokens: ['X', '=>', 'Y'] 
//            "X == Y" -> tokens: ['X', '=', '=', 'Y']? 
//        But the problem says: "E1 == E2 == ... == En", so it's written with two '=' between each pair.

//        Therefore, we tokenize "==" as two separate '=' tokens.

//        Now, how to parse the equivalence chain?
//          We decide: 
//            The equivalence operator (each '=' token) has the lowest precedence, and it is left-associative? 
//            But the problem defines: 
//                E1 == E2 == E3 = (E1==E2) /\ (E2==E3)
//            So it's not a binary operator that chains left or right, but a chain that produces a conjunction of adjacent pairs.

//          Therefore, we can handle equivalence at the lowest level:
//            Parse as: 
//                expr -> implication_expr ( '=' implication_expr )*   [but note: the problem uses '=' for each equivalence link]
//            However, the issue is that the definition is not: 
//                (E1 = E2) = E3 
//            but: 
//                (E1 = E2) and (E2 = E3)

//          So we cannot parse as a binary operator. Instead, we do:
//            Let the equivalence expression be a sequence of implication_expr separated by '=' tokens.
//            Let the parts be [P0, P1, P2, ..., Pk] (k+1 parts, k equivalence tokens)
//            Then the whole expression is: 
//                (P0 == P1) AND (P1 == P2) AND ... AND (P_{k-1} == P_k)

//          Therefore, in the parser:
//            We'll parse the top level as equivalence_expr: 
//                Step 1: parse implication_expr for the first part.
//                Step 2: while next token is '=', then parse next implication_expr and record the part.
//                Then, if there are n parts, then:
//                   if n==1: result = P0
//                   else: 
//                         Expr res = bin_op(P0, P1, "==");   // which is (P0==P1) 
//                         for i from 2 to n-1:
//                             res = bin_op(res, bin_op(P_{i-1}, P_i, "=="), "&");   // AND the new equivalence
//                         But wait, the definition: (P0==P1) AND (P1==P2) -> so we need to AND the consecutive pairs.

//                Actually, we can do:
//                   Expr current = bin_op(P[0], P[1], "==");
//                   for i=2; i < n; i++) {
//                         Expr new_eq = bin_op(P[i-1], P[i], "==");
//                         current = bin_op(current, new_eq, "&");
//                   }

//          However, note: the problem says: 
//                (E1 == E2) /\ (E2 == E3) 
//          so it's the AND of the adjacent equivalences.

//        So in the parser for the top level (equivalence):
//            vector<Expr> parts;
//            parse an implication_expr -> push to parts.
//            while (next token is '=') {
//                skip the '='
//                parse an implication_expr -> push to parts.
//            }
//            if (parts.size() == 1) 
//                return parts[0];
//            else {
//                Expr res = make_equivalence(parts[0], parts[1]);   // which is (parts[0] == parts[1])
//                for (int i=2; i < parts.size(); i++) {
//                    Expr eq = make_equivalence(parts[i-1], parts[i]);
//                    res = make_and(res, eq);
//                }
//                return res;
//            }

//        Where:
//            make_equivalence(a,b) = (a=>b) AND (b=>a)

//        And make_and(a,b) = a AND b (which is the binary operation we defined).

//        Similarly, for implication: 
//            The problem says: E1 => E2 => E3 means E1 => (E2 => E3)
//            So it's right associative.

//        Therefore, for implication:
//            We parse from right to left: 
//                expr -> disjunction_expr (but actually, the next level up)
//            Alternatively, we can do:
//                parse the rightmost disjunction_expr, then for each '=>' from right to left, but it's easier to do recursively.

//        Given the complexity, and since the formulae are small, we can use a recursive descent with operator precedence.

//        Precedence levels (lowest to highest):
//            0: equivalence (==)  [but we handle it by splitting the formula by '=' first]
//            1: implication (=>)  [right associative]
//            2: disjunction (|)   [left associative]
//            3: conjunction (&)   [left associative]
//            4: negation (~)      [highest]

//        We'll write a parser that works from left to right with precedence.

//        Alternatively, we can use shunting yard or a recursive descent that handles each level.

//        Steps for recursive descent:

//          expr: 
//            return equivalence_expr()
//          equivalence_expr:
//            left = implication_expr()
//            while (next token is '=') {
//                next_token (skip the '=')
//                right = implication_expr()
//                left = make_equivalence(left, right)
//            }
//            return left
//          But note: the problem's definition of equivalence is not left-associative in the usual sense, but a chain of ANDs of adjacent equivalences.

//        However, the above while loop would do:
//            a = b = c 
//            -> first: left = a, then see '=', so left = (a==b), then see '=', then left = (a==b) == c -> which is not what we want.

//        So we must not do it as a binary operator. Instead, we collect all parts first.

//        Revised:

//          equivalence_expr:
//            vector<Expr> parts;
//            parts.push_back(implication_expr());
//            while (next token is '=') {
//                next_token (skip)
//                parts.push_back(implication_expr());
//            }
//            if (parts.size()==1) return parts[0];
//            Expr res = bin_op(parts[0], parts[1], "==");
//            for (int i=2; i<parts.size(); i++) {
//                Expr eq = bin_op(parts[i-1], parts[i], "==");
//                res = bin_op(res, eq, "&");
//            }
//            return res;

//        Similarly, for implication (which is right associative), we do:

//          implication_expr:
//            left = disjunction_expr()   // but wait, implication is lower than disjunction? 
//            Actually, the precedence: 
//                negation > conjunction > disjunction > implication > equivalence.
//            So implication is lower than disjunction, so disjunction_expr is the left part.

//            However, implication is right associative, so we parse from right to left.

//          How about: 
//            implication_expr:
//                expr = disjunction_expr();
//                if (next token is '=>') {
//                    skip '=>'
//                    expr2 = implication_expr();   // because right associative: a=>b=>c = a=>(b=>c)
//                    return make_implication(expr, expr2);
//                }
//                return expr;

//          But this would be left-associative? 
//            a=>b=>c: 
//                first: expr = a (a disjunction_expr), then see '=>', then call implication_expr() for b=>c.
//                In the call for b=>c: 
//                    expr = b, then see '=>', then call implication_expr() for c -> returns c.
//                then make_implication(b, c) -> (b=>c)
//                then make_implication(a, (b=>c)) -> (a=>(b=>c)) -> correct.

//          So for implication: right associative -> we do recursive call on the right.

//        For disjunction and conjunction: left associative.

//          disjunction_expr:
//            left = conjunction_expr()
//            while (next token is '|') {
//                skip '|'
//                right = conjunction_expr()
//                left = make_or(left, right);
//            }
//            return left;

//          conjunction_expr:
//            left = negation_expr()
//            while (next token is '&') {
//                skip '&'
//                right = negation_expr()
//                left = make_and(left, right);
//            }
//            return left;

//          negation_expr:
//            if (next token is '~') {
//                skip '~'
//                expr = negation_expr();
//                return make_not(expr);
//            }
//            return atom_expr();

//          atom_expr:
//            if token is '0' or '1', return constant
//            else if token is uppercase letter, return variable (store the letter)
//            else if token is '(', 
//                skip '('
//                expr = equivalence_expr()
//                skip ')'
//                return expr

//        But note: the precedence levels: 
//            negation is higher than conjunction, so the above is correct.

//        However, the problem says: 
//            Conjunction: E1 /\ E2 /\ ... /\ En, left associative.
//            Disjunction: left associative.
//            Implication: right associative.
//            Equivalence: as defined.

//        We'll implement the parser accordingly.

//        We'll create an AST node structure:

//          enum TokenType { 
//            TOKEN_0, TOKEN_1, TOKEN_VAR, TOKEN_NOT, TOKEN_AND, TOKEN_OR, TOKEN_IMPL, TOKEN_EQUIV, TOKEN_LPAREN, TOKEN_RPAREN 
//          };
//          But we don't need to tokenize to enum, we can use:
//            struct Token {
//                string s;
//                // or we can use: 
//                //   if s=="0" -> const0
//                //   if s=="1" -> const1
//                //   if s[0] is upper -> var, and the char
//                //   if s=="~" -> not
//                //   if s=="&" -> and
//                //   if s=="|" -> or
//                //   if s=="=>" -> impl
//                //   if s=="=" -> equiv (but note: we have to handle the chain, so we treat each '=' as equiv_token)
//                //   if s=="(" -> lparen
//                //   if s==")" -> rparen
//            };

//        However, for simplicity, we'll do the tokenization first, and then parse.

//        Given the constraints (K<=20, and formula length <=254), we can do a simple recursive parser.

// Implementation:

//   Precomputation for the Heyting algebra:
//      We'll create a class or just global functions.

//   Steps:

//      Step 1: Read input.
//      Step 2: Compute transitive closure.
//      Step 3: Enumerate all antichains (H) -> store in a vector `antichains`, and also create a mapping: 
//               map<vector<int>, int> antichainToIndex;
//      Step 4: Precompute:
//          const vector<int> X = {0,1,...,N-1}
//          vector<int> Max_of_set(const vector<int>& S) {
//             // returns the set of maximal elements of S
//             vector<int> res;
//             for (int x : S) {
//                 bool is_max = true;
//                 for (int y : S) {
//                     if (x == y) continue;
//                     if (reach[x][y]) { // x <= y
//                         is_max = false;
//                         break;
//                     }
//                 }
//                 if (is_max) res.push_back(x);
//             }
//             sort(res.begin(), res.end());
//             return res;
//          }

//      Step 5: Precompute operations as functions that take two antichains (by index) and return an index.
//          We'll create functions:
//             int op_imp(int a_idx, int b_idx) {
//                 vector<int> b_list = antichains[b_idx];
//                 vector<int> res_list;
//                 for (int x : b_list) {
//                     bool ok = true;
//                     for (int y : antichains[a_idx]) {
//                         if (reach[x][y]) {   // x <= y
//                             ok = false;
//                             break;
//                         }
//                     }
//                     if (ok) res_list.push_back(x);
//                 }
//                 // Since b_list is an antichain, res_list is an antichain -> we can find it in antichains?
//                 sort(res_list.begin(), res_list.end());
//                 return antichainToIndex[res_list];
//             }
//          Similarly for the others.

//          Note: For op_and (a and b):
//             vector<int> T;
//             for (int x : antichains[a_idx]) T.push_back(x);
//             for (int x : antichains[b_idx]) T.push_back(x);
//             vector<int> res_list = Max_of_set(T);
//             return antichainToIndex[res_list];

//          For op_or (a or b):
//             vector<int> U;
//             for (int x = 0; x < N; x++) {
//                 bool from_a = false;
//                 for (int y : antichains[a_idx]) {
//                     if (reach[x][y]) {
//                         from_a = true;
//                         break;
//                     }
//                 }
//                 bool from_b = false;
//                 for (int y : antichains[b_idx]) {
//                     if (reach[x][y]) {
//                         from_b = true;
//                         break;
//                     }
//                 }
//                 if (from_a && from_b) {
//                     U.push_back(x);
//                 }
//             }
//             vector<int> res_list = Max_of_set(U);
//             return antichainToIndex[res_list];

//          Constants:
//             int const0 = antichainToIndex[Max_of_set(vector<int>(X))];   // Max(X)
//             int const1 = antichainToIndex[vector<int>()];   // empty set

//      Step 6: For each formula, parse it and then:
//          Find all variables in the formula (set of chars)
//          Let v = variables.size()
//          For each assignment: a vector of length v, each element in [0, D-1]
//             Evaluate the AST under the assignment.
//             Check if the result is const1.
//          If any assignment yields not const1, then output "invalid", else "valid".

//   Important: The problem says the formula is syntactically correct.

// Let's code accordingly.

// Note: Performance: 
//   D <= 100, and the total work over formulae is <= 10^6, so even if one formula has 3 variables (100^3=1e6), it's acceptable.

// Implementation details:

//   We'll do:
//      Precomputation for the algebra (after reading graph)
//      Then for each formula:
//          tokenize
//          parse to AST (we'll build an expression tree)
//          Evaluate: 
//             struct Expr {
//                 enum Type { CONST0, CONST1, VAR, NOT, AND, OR, IMPL, EQUIV } type;
//                 char var;  // for VAR
//                 Expr* left;
//                 Expr* right;
//             };
//          But we can use a simple recursive evaluator with a mapping from char to antichain index.

//   Alternatively, we can evaluate on the fly without building a tree: 
//        We can do recursive descent evaluation with a global mapping for variables.

//   Given the small size, we'll build a tree.

//   However, to save time, we can do:

//        int evaluate(const ASTNode& node, const map<char, int>& assignment) {
//            switch(node.type) {
//                case CONST0: return const0;
//                case CONST1: return const1;
//                case VAR: return assignment.at(node.var);
//                case NOT: {
//                    int a = evaluate(node.left, assignment);
//                    // not a = a => const0
//                    return op_imp(a, const0);
//                }
//                case AND: {
//                    int a = evaluate(node.left, assignment);
//                    int b = evaluate(node.right, assignment);
//                    return op_and(a, b);
//                }
//                case OR: {
//                    int a = evaluate(node.left, assignment);
//                    int b = evaluate(node.right, assignment);
//                    return op_or(a, b);
//                }
//                case IMPL: {
//                    int a = evaluate(node.left, assignment);
//                    int b = evaluate(node.right, assignment);
//                    return op_imp(a, b);
//                }
//                case EQUIV: {
//                    // Note: EQUIV node: we stored the two parts for the equivalence, but the EQUIV node in the tree for a chain is not stored as a single node? 
//                    // Actually, in our parser for equivalence, we expand it to ANDs of adjacent equivalences, so the tree will have AND nodes and IMPL nodes.
//                    // So we won't have an EQUIV node in the tree; instead, we expand it during parsing.
//                    // Therefore, in the AST, we don't have EQUIV node; the EQUIV is expanded to AND(IMPL(a,b), IMPL(b,a)) for two, but for chain we expand to multiple ANDs.
//                    // So we won't have an EQUIV node in the tree.
//                    // Therefore, we don't need to handle EQUIV in the evaluator.
//                }
//            }
//        }

//   How to handle equivalence in the parser: 
//        When we parse an equivalence expression with more than two parts, we expand it to a conjunction of equivalence pairs.

//   So in the AST, we only have: 
//        CONST0, CONST1, VAR, NOT, AND, OR, IMPL.

//   Steps for parsing equivalence:
//        We'll have a function that parses an equivalence_expr and returns an AST that is the expansion.

//   Let's define the AST:

//        struct AST {
//            int type; // 0: const0, 1: const1, 2: var, 3: not, 4: and, 5: or, 6: impl
//            char var;
//            AST* left;
//            AST* right;
//        };

//   But to avoid memory leaks, we can use unique_ptr, but the formulae are small.

//   Alternatively, we can use a stack-based evaluation? But recursive descent is clearer.

//   Given time, we'll do recursive descent parsing with a global token list and an index.

//   Steps for parsing:

//        int token_index;
//        vector<Token> tokens;

//        AST* parse_equivalence() {
//            vector<AST*> parts;
//            parts.push_back(parse_implication());
//            while (token_index < tokens.size() && tokens[token_index].type == TOK_EQUIV) {
//                token_index++;
//                parts.push_back(parse_implication());
//            }
//            if (parts.size() == 1) {
//                return parts[0];
//            }
//            // Build: (parts[0] == parts[1]) AND (parts[1] == parts[2]) AND ... 
//            AST* res = make_equivalence_node(parts[0], parts[1]);
//            for (int i = 2; i < parts.size(); i++) {
//                AST* eq = make_equivalence_node(parts[i-1], parts[i]);
//                res = new AST{4, 0, res, eq}; // 4: AND
//            }
//            return res;
//        }

//        But note: the problem says the equivalence for two is (a=>b) AND (b=>a), so we can define:

//        AST* make_equivalence_node(AST* a, AST* b) {
//            AST* impl1 = new AST{6, 0, a, b}; // a=>b
//            AST* impl2 = new AST{6, 0, b, a}; // b=>a
//            return new AST{4, 0, impl1, impl2}; // impl1 AND impl2
//        }

//        However, the problem says for n parts: 
//            (a0==a1) AND (a1==a2) AND ... 
//        So for parts = [a,b,c], we want:
//            (a==b) AND (b==c) = [ (a=>b and b=>a) AND (b=>c and c=>b) ]

//        So the above is correct.

//   But note: the problem says the equivalence is defined as (E1==E2) /\ (E2==E3) for three, so consecutive pairs.

//   Now, let's code accordingly.

//   However, the problem says: "Sum(Di) <= 10^6", so the worst-case total assignments is 10^6, and the AST size is small, so the evaluation per assignment is fast.

// Implementation:

//   Steps:

//   Precomputation for the algebra (after reading N, M and the graph):

//      // Step 1: Build graph and compute reachability.
//      vector<vector<int>> adj(N);
//      for each edge (s,t): 
//          s--, t--; (if input is 1-indexed) — but the problem says: "si and ti", and vertices are from 1 to N.
//          So we convert to 0-indexed.

//      // Compute reach: N x N matrix, initially identity.
//      vector<vector<bool>> reach(N, vector<bool>(N, false));
//      for (int i=0; i<N; i++) reach[i][i] = true;
//      for (int i=0; i<N; i++) {
//          for (int j=0; j<adj[i].size(); j++) {
//              int v = adj[i][j];
//              reach[i][v] = true;
//          }
//      }
//      // Floyd-Warshall or BFS for DAG: since acyclic, we can do topological sort and then DP.
//      // We'll do BFS for each node.
//      for (int i = 0; i < N; i++) {
//          queue<int> q;
//          q.push(i);
//          vector<bool> visited(N, false);
//          visited[i] = true;
//          while (!q.empty()) {
//              int u = q.front(); q.pop();
//              for (int v : adj[u]) {
//                  if (!visited[v]) {
//                      visited[v] = true;
//                      reach[i][v] = true;
//                      q.push(v);
//                  }
//              }
//          }
//      }
//      // Now reach[i][j] = true if there's a path from i to j.

//      // Step 2: Enumerate antichains.
//      vector<vector<int>> antichains;
//      // We'll use BFS for antichains (with a set to avoid duplicates)
//      set<vector<int>> seen;
//      queue<vector<int>> q_anti;
//      vector<int> empty_set;
//      q_anti.push(empty_set);
//      seen.insert(empty_set);
//      while (!q_anti.empty()) {
//          vector<int> a = q_anti.front(); q_anti.pop();
//          antichains.push_back(a);
//          // Try to add each vertex not in a
//          for (int i = 0; i < N; i++) {
//              if (find(a.begin(), a.end(), i) != a.end()) continue;
//              // Check if i is incomparable to all in a
//              bool ok = true;
//              for (int x : a) {
//                  if (reach[x][i] || reach[i][x]) {
//                      ok = false;
//                      break;
//                  }
//              }
//              if (ok) {
//                  vector<int> b = a;
//                  b.push_back(i);
//                  sort(b.begin(), b.end());
//                  if (seen.find(b) == seen.end()) {
//                      seen.insert(b);
//                      q_anti.push(b);
//                  }
//              }
//          }
//      }
//      // But note: the BFS above might miss some antichains? 
//      // Actually, it generates all antichains that are subsets of the whole set, but it's a standard BFS for antichains (down-closed in the inclusion order? not exactly). 
//      // However, the set of antichains is not closed under adding elements in a simple way, but this BFS (adding one element at a time) will generate all antichains? 
//      // Yes, because any non-empty antichain has a minimal element (in the order of the vertex index? not necessarily) but we iterate all vertices.

//      // But it's known that this BFS works: 
//      //   Start from empty, then add vertices one by one that are incomparable to the current antichain.

//      // However, it might generate duplicates? We use `seen`.

//      // Build mapping: 
//      map<vector<int>, int> antichainToIndex;
//      for (int i=0; i<antichains.size(); i++) {
//          antichainToIndex[antichains[i]] = i;
//      }

//      // Constants:
//      vector<int> X(N);
//      iota(X.begin(), X.end(), 0);
//      vector<int> max_X = Max_of_set(X, reach);   // Max(X)
//      int const0 = antichainToIndex[max_X];
//      vector<int> empty_set_vec;
//      int const1 = antichainToIndex[empty_set_vec];

//      // Operations: 
//      auto op_imp = [&](int a_idx, int b_idx) {
//          vector<int> b_list = antichains[b_idx];
//          vector<int> res;
//          for (int x : b_list) {
//              bool ok = true;
//              for (int y : antichains[a_idx]) {
//                  if (reach[x][y]) {   // x <= y
//                      ok = false;
//                      break;
//                  }
//              }
//              if (ok) res.push_back(x);
//          }
//          sort(res.begin(), res.end());
//          return antichainToIndex[res];
//      };

//      auto op_and = [&](int a_idx, int b_idx) {
//          vector<int> T;
//          for (int x : antichains[a_idx]) T.push_back(x);
//          for (int x : antichains[b_idx]) T.push_back(x);
//          vector<int> res = Max_of_set(T, reach);
//          return antichainToIndex[res];
//      };

//      auto op_or = [&](int a_idx, int b_idx) {
//          vector<int> U;
//          for (int x = 0; x < N; x++) {
//              bool from_a = false;
//              for (int y : antichains[a_idx]) {
//                  if (reach[x][y]) {
//                      from_a = true;
//                      break;
//                  }
//              }
//              bool from_b = false;
//              for (int y : antichains[b_idx]) {
//                  if (reach[x][y]) {
//                      from_b = true;
//                      break;
//                  }
//              }
//              if (from_a && from_b) {
//                  U.push_back(x);
//              }
//          }
//          vector<int> res = Max_of_set(U, reach);
//          return antichainToIndex[res];
//      };

//      // Max_of_set function:
//      auto Max_of_set = [&](const vector<int>& S) -> vector<int> {
//          vector<int> res;
//          for (int x : S) {
//              bool is_max = true;
//              for (int y : S) {
//                  if (x == y) continue;
//                  if (reach[x][y]) {
//                      is_max = false;
//                      break;
//                  }
//              }
//              if (is_max) res.push_back(x);
//          }
//          sort(res.begin(), res.end());
//          return res;
//      };

//   Then, for each formula:

//      Step: tokenize the formula string.
//      Step: parse to AST.
//      Step: get all variables (set of chars) in the formula.
//      Step: for each assignment of variables to [0, D-1] (D = antichains.size()), evaluate the AST.
//      Step: if any assignment yields not const1, then invalid; else valid.

//   How to evaluate AST:

//        int eval(AST* node, const vector<int>& assignment, 
//                 int (*op_imp)(int,int), int (*op_and)(int,int), int (*op_or)(int,int),
//                 int const0, int const1) {
//            // But we have lambdas, so better to capture in a struct or use function objects.
//        }

//   Given the complexity, and since D is small, we'll do a recursive evaluation with lambdas captured.

//   Alternatively, we can write a function that uses the precomputed operations.

//   We'll create a function:

//        function<int(AST*)> evaluator = [&](AST* node) -> int {
//            if (node->type == 0) return const0;
//            if (node->type == 1) return const1;
//            if (node->type == 2) return assignment[node->var];
//            if (node->type == 3) {
//                int a = evaluator(node->left);
//                return op_imp(a, const0);
//            }
//            if (node->type == 4) {
//                int a = evaluator(node->left);
//                int b = evaluator(node->right);
//                return op_and(a, b);
//            }
//            if (node->type == 5) {
//                int a = evaluator(node->left);
//                int b = evaluator(node->right);
//                return op_or(a, b);
//            }
//            if (node->type == 6) {
//                int a = evaluator(node->left);
//                int b = evaluator(node->right);
//                return op_imp(a, b);
//            }
//            return -1;
//        };

//   But the assignment is passed as a vector indexed by the variable char? 
//        We can have: 
//            vector<int> assignment_vec;
//            but we need to map char to index in assignment_vec.
//        Instead, we can pass a map: char -> int.

//        Then in the evaluator, for a variable node, we do: assignment_map[node->var]

//   We'll do:

//        int eval_node(AST* node, const map<char, int>& assignment) {
//            ...
//            if (type==2) return assignment.at(node->var);
//        }

//   However, to avoid recursion and memory, and since the formulae are small, we'll build the AST and then evaluate.

//   Let's define the AST node as a struct with unique_ptr.

//   Steps for parsing:

//        We'll write a tokenizer that returns a list of tokens.

//        Token types: 
//            const0, const1, var, not, and, or, impl, equiv, lparen, rparen.

//        We'll use an enum or string.

//        For simplicity, we'll use a vector of strings for tokens, but the problem says the tokens are single-character except "=>" and "==" (but "==" is two '=' tokens).

//        So tokens:
//            "0" -> const0
//            "1" -> const1
//            string of one char, if uppercase letter -> var
//            "~" -> not
//            "&" -> and
//            "|" -> or
//            "=>" -> impl (but we already tokenized "=>" as a token of length 2, so we store it as a token "=>")
//            "=" -> equiv
//            "(" -> lparen
//            ")" -> rparen

//        How to tokenize:

//            vector<string> tokens;
//            for (int i=0; i<s.size(); ) {
//                if (isspace(s[i])) { i++; continue; }
//                if (s[i]=='0' || s[i]=='1') {
//                    tokens.push_back(string(1, s[i])); i++;
//                } else if (isupper(s[i])) {
//                    tokens.push_back(string(1, s[i])); i++;
//                } else if (s[i]=='(') {
//                    tokens.push_back("("); i++;
//                } else if (s[i]==')') {
//                    tokens.push_back(")"); i++;
//                } else if (s[i]=='~') {
//                    tokens.push_back("~"); i++;
//                } else if (s[i]=='&') {
//                    tokens.push_back("&"); i++;
//                } else if (s[i]=='|') {
//                    tokens.push_back("|"); i++;
//                } else if (s[i]=='=') {
//                    if (i+1 < s.size() && s[i+1]=='>') {
//                        tokens.push_back("=>"); 
//                        i += 2;
//                    } else {
//                        tokens.push_back("=");
//                        i++;
//                    }
//                } else {
//                    // should not happen
//                    i++;
//                }
//            }

//        Then, we have tokens.

//        Now, the parsing functions (recursive descent):

//            int pos = 0;
//            AST* parse_equivalence() {
//                vector<AST*> parts;
//                parts.push_back(parse_implication());
//                while (pos < tokens.size() && tokens[pos]=="=") {
//                    pos++;
//                    parts.push_back(parse_implication());
//                }
//                if (parts.size()==1) return parts[0];
//                AST* res = make_equivalence_node(parts[0], parts[1]);
//                for (int i=2; i<parts.size(); i++) {
//                    AST* eq = make_equivalence_node(parts[i-1], parts[i]);
//                    res = new AST{AND, 0, res, eq};
//                }
//                return res;
//            }

//            AST* parse_implication() {
//                AST* left = parse_disjunction();
//                if (pos < tokens.size() && tokens[pos]=="=>") {
//                    pos++;
//                    AST* right = parse_implication();   // right associative
//                    return new AST{IMPL, 0, left, right};
//                }
//                return left;
//            }

//            AST* parse_disjunction() {
//                AST* left = parse_conjunction();
//                while (pos < tokens.size() && tokens[pos]=="|") {
//                    pos++;
//                    AST* right = parse_conjunction();
//                    left = new AST{OR, 0, left, right};
//                }
//                return left;
//            }

//            AST* parse_conjunction() {
//                AST* left = parse_negation();
//                while (pos < tokens.size() && tokens[pos]=="&") {
//                    pos++;
//                    AST* right = parse_negation();
//                    left = new AST{AND, 0, left, right};
//                }
//                return left;
//            }

//            AST* parse_negation() {
//                if (pos < tokens.size() && tokens[pos]=="~") {
//                    pos++;
//                    AST* expr = parse_negation();
//                    return new AST{NOT, 0, expr, nullptr};
//                }
//                return parse_atom();
//            }

//            AST* parse_atom() {
//                if (tokens[pos]=="0") {
//                    pos++;
//                    return new AST{CONST0, 0, nullptr, nullptr};
//                }
//                if (tokens[pos]=="1") {
//                    pos++;
//                    return new AST{CONST1, 0, nullptr, nullptr};
//                }
//                if (tokens[pos].size()==1 && isupper(tokens[pos][0])) {
//                    char c = tokens[pos][0];
//                    pos++;
//                    return new AST{VAR, c, nullptr, nullptr};
//                }
//                if (tokens[pos]=="(") {
//                    pos++;
//                    AST* expr = parse_equivalence();
//                    if (pos < tokens.size() && tokens[pos]==")") {
//                        pos++;
//                    } else {
//                        // error, but problem says syntactically correct
//                    }
//                    return expr;
//                }
//                // error
//                return nullptr;
//            }

//        And helper for equivalence:
//            AST* make_equivalence_node(AST* a, AST* b) {
//                AST* impl1 = new AST{IMPL, 0, a, b};
//                AST* impl2 = new AST{IMPL, 0, b, a};
//                return new AST{AND, 0, impl1, impl2};
//            }

//        But note: the problem says the equivalence for two is (a=>b) AND (b=>a), so this is correct.

//   Finally, we evaluate.

//   Important: The problem says the formulae are syntactically correct, so we don't handle errors.

//   Memory: We'll use unique_ptr to avoid leaks.

//   Let's define:

//        struct ASTNode {
//            enum Type { CONST0, CONST1, VAR, NOT, AND, OR, IMPL } type;
//            char var;
//            unique_ptr<ASTNode> left, right;
//            ASTNode(Type t, char c=0, unique_ptr<ASTNode> l=nullptr, unique_ptr<ASTNode> r=nullptr) 
//                : type(t), var(c), left(move(l)), right(move(r)) {}
//        };

//   Then the parsing functions return unique_ptr<ASTNode>.

//   We'll adjust the parsing functions to return unique_ptr.

//   Steps for parse_equivalence:

//        vector<unique_ptr<ASTNode>> parts;
//        parts.push_back(parse_implication());
//        while (...) {
//            parts.push_back(parse_implication());
//        }
//        if (parts.size()==1) return move(parts[0]);
//        unique_ptr<ASTNode> res = make_equivalence(move(parts[0]), move(parts[1]));
//        for (int i=2; ...) {
//            unique_ptr<ASTNode> eq = make_equivalence(move(parts[i-1]), move(parts[i]));
//            res = make_unique<ASTNode>(ASTNode::AND, 0, move(res), move(eq));
//        }
//        return move(res);

//   Similarly for others.

//   Let's code accordingly.

//   Note: The constant 0 and 1 are represented as ASTNode with type CONST0, CONST1.

//   We'll define the ASTNode struct.

//   Due to the complexity, we'll write the entire program step by step.

//   Let's go.

//   Important: The graph might be empty (N=1, M=0) — then reach[0][0]=true, and no other edges.

//   For N=1, M=0:
//        Antichains: 
//            empty set -> index0
//            {0} -> index1
//        So D=2.
//        Max(X) = Max({0}) = {0} (since 0 is maximal? because no edge, so no y>0) -> so const0 = {0}
//        const1 = empty set.

//   Now, check sample input #1:
//        N=1, M=0
//        Formula: "1=0"
//        meaning: 1 == 0 -> (1=>0) AND (0=>1)
//        What is 1? empty set.
//        What is 0? {0}
//        Compute 1=>0: 
//            = { x in 0 : for all y in 1, not (x<=y) } 
//            But 1 is empty, so the condition "for all y in 1" is vacuously true -> so x can be any in 0 -> {0}
//            So 1=>0 = {0} = 0.
//        Compute 0=>1:
//            = { x in 1 : for all y in 0, not (x<=y) } 
//            = { x in empty set : ... } = empty set = 1.
//        So (1=>0) AND (0=>1) = 0 AND 1 = Max({0} ∪ {}) = Max({0}) = {0} = 0, not 1.
//        So the formula 1=0 evaluates to 0, not 1 -> invalid.

//        Which matches sample output: first line "invalid".

//   Next: "X|~X"
//        We need to check for all assignments to X (which can be either empty set or {0}).
//        Assignment 1: X = empty set (1)
//            ~X = empty set => 0 = {0} (as above) -> 0
//            X | ~X = 1 | 0 = Max( { } ∪ {0} ) = {0} = 0 -> not 1? 
//        But wait: 1 is the empty set, and 0 is {0}. 
//            X = 1 (empty set) -> X | ~X = empty set | {0} = Max({0}) = {0} = 0 -> not 1.
//        So it should be invalid? 
//        However, sample output says "valid".

//        What's wrong?

//        Let me re-read the operations:

//          a ∨ b = Max({x in X: exists y in a, z in b, x<=y and x<=z})

//        For a = empty set, b = {0}:
//            {x: exists y in empty, z in {0}: x<=y and x<=z} = empty, because there is no y in a.
//            So U = empty set.
//            Max(empty) = empty set = 1.

//        So X | ~X = 1.

//        Why? 
//          The set T = {x: x is a common lower bound of a and b}. 
//          For a=empty, there is no y in a, so no x qualifies.

//        Therefore, X | ~X = empty set = 1.

//        Similarly, if X = {0}:
//            ~X = {0} => 0 = { x in 0: for all y in {0}, not (x<=y) } 
//                 = { x in {0}: not (x<=x) } = { } = empty set.
//            Then X | ~X = {0} | empty = 
//                U = {x: exists y in {0}, z in empty: ... } = empty -> Max(empty)=empty=1.

//        So for both assignments, it's 1.

//        Therefore, valid.

//        So the operation for OR is correct.

//   Now, the sample input #2: 
//        "X|~X" -> invalid? 
//        But sample output #2: second line is "invalid", but wait sample output #2:
//            invalid
//            invalid
//            valid
//            invalid
//            valid
//            invalid
//            valid
//            valid
//            invalid
//            valid
//            valid

//        So first line "1=0" -> invalid, second line "X|~X" -> invalid? 
//        But in sample input #1, second line is valid.

//        Why? Because the graph is different.

//        For sample input #2: 
//            6 vertices, 6 edges: 
//                1->2, 2->3, 2->4, 3->5, 4->5, 5->6
//            So the graph is a DAG: 
//                1 -> 2 -> 3 -> 5 -> 6
//                      \-> 4 -/
//            The antichains: 
//                The poset is a tree? 
//                The comparabilities: 
//                  1<=2, 1<=3, 1<=4, 1<=5, 1<=6
//                  2<=3, 2<=4, 2<=5, 2<=6
//                  3<=5, 3<=6
//                  4<=5, 4<=6
//                  5<=6
//                So the maximal elements: 
//                  The only maximal element is 6? 
//                But 3 and 4 are incomparable? 
//                  3 and 4: no path from 3 to 4 or 4 to 3 -> incomparable.
//                So antichains can include 3 and 4 together.

//            Now, for X|~X: 
//                We need to check if for every assignment, X|~X = 1 (empty set).

//            Let X be the antichain {6} (the top). 
//                ~X = X => 0 = { x in 0 : ... } but 0 = Max(X) = {6} (since 6 is the only maximal element) -> so 0 = {6}
//                Then ~X = { x in {6} : for all y in {6}, not (x<=y) } 
//                    = {6} : but for y=6, x=6: 6<=6 -> so condition fails -> empty set? 
//                    So ~X = empty set = 1.
//                Then X | ~X = {6} | empty = as before, the set of common lower bounds is empty -> Max(empty)=empty=1.

//            Let X be the antichain {3,4} (which is an antichain). 
//                ~X = { x in 0={6} : for all y in {3,4}, not (x<=y) } 
//                    For x=6: 
//                         check y=3: 6<=3? -> no (because 3<6, so 3<=6, but 6<=3 is false) -> but we require not (6<=3) -> true.
//                         y=4: 6<=4? false.
//                    So 6 qualifies -> ~X = {6}
//                Then X | ~X = {3,4} | {6} = 
//                    U = {x: exists y in {3,4}, z in {6} such that x<=y and x<=z}
//                         x must be <= some y in {3,4} and <=6.
//                         For example, x=2: 
//                             2<=3 and 2<=6 -> yes.
//                         x=1: 1<=3,1<=6 -> yes.
//                         x=5: 5<=6, but 5<=3? no, 5<=4? no -> so not.
//                         x=3: 3<=3, 3<=6 -> yes.
//                         x=4: yes.
//                         x=6: 6<=3? no.
//                    So U = {1,2,3,4,6}? 
//                         But 5: 5<=6, but no y in {3,4} with 5<=y? -> no, so 5 not included.
//                    Now, Max(U): 
//                         The maximal elements of U: 
//                            3 and 4 and 6? 
//                         But 3<=6, so 3 is not maximal (because 3<=6 and 6 in U) -> similarly 4<=6, so 4 not maximal.
//                         6 is maximal? yes.
//                         Also, 1 and 2: 1<=2<=3, so 1 and 2 are not maximal.
//                    So Max(U) = {6}
//                Then the result = {6} = 0 (since 0 = {6}), not 1.

//            Therefore, for assignment X = {3,4}, we get 0, so the formula is invalid.

//        So sample output #2: second line "invalid" is correct.

//   Therefore, the operations are implemented correctly.

//   Now, let's code.

//   Note: The graph is given with vertices 1-indexed, so we convert to 0-indexed.

//   Steps:

//      Read N, M.
//      adj.resize(N)
//      for i in [0, M):
//          read s, t; s--, t--; 
//          adj[s].push_back(t)

//      Then compute reach (N x N) with BFS for each node.

//      Then enumerate antichains.

//      Precompute const0 = Max_of_set(vector<int>(N)) -> the set of maximal elements of the entire set.
//      const1 = empty set.

//      Then for each formula:

//          string line;
//          getline(cin, line);
//          token_list = tokenize(line)
//          parse to AST
//          get variables: set<char> vars;
//          D = antichains.size()
//          Generate all assignments: 
//             vector<int> assign(D_vars, 0);
//             for (int i=0; i < D_vars; i++) assign[i] = 0; // then we'll iterate in a nested loop or use recursion
//          Instead, we can do:
//             vector<char> var_list(vars.begin(), vars.end());
//             sort(var_list.begin(), var_list.end());
//             int num_vars = var_list.size();
//             for (int mask = 0; mask < (int)pow(D, num_vars); mask++) {
//                 // represent mask in base D
//                 map<char, int> assignment;
//                 int tmp = mask;
//                 for (int i=0; i<num_vars; i++) {
//                     assignment[var_list[i]] = tmp % D;
//                     tmp /= D;
//                 }
//                 // evaluate AST with assignment
//                 int res = eval(ast.get(), assignment, ...);
//                 if (res != const1) {
//                     valid_flag = false;
//                     break;
//                 }
//             }
//          If we found one invalid, output "invalid", else "valid"

//   However, note: D is at most 100, and num_vars is the number of distinct variables in the formula, which is at most 26, but the problem says the total work over formulae is <= 10^6, so we assume the product (D^num_vars) for all formulae is <= 10^6.

//   We'll iterate.

//   Let's code accordingly.

//   Note: We must be cautious about the performance of the evaluation: 
//        Each evaluation does a DFS on the AST, and the AST size is small (the formula is short).

//   We'll implement the eval function.

//   Due to the complexity, we write the entire program.

//   IMPORTANT: The problem says the formula string may have spaces, so we skip spaces in tokenization.

//   Let's go.

//   Note: The tokenization for "=>" must be done before '=' for the implication token.

//   We'll do:

//        tokens.clear();
//        for (int i=0; i<line.size(); ) {
//            if (isspace(line[i])) { i++; continue; }
//            if (line[i]=='0' || line[i]=='1') {
//                tokens.push_back(string(1, line[i])); i++;
//            } else if (isupper(line[i])) {
//                tokens.push_back(string(1, line[i])); i++;
//            } else if (line[i]=='(') {
//                tokens.push_back("("); i++;
//            } else if (line[i]==')') {
//                tokens.push_back(")"); i++;
//            } else if (line[i]=='~') {
//                tokens.push_back("~"); i++;
//            } else if (line[i]=='&') {
//                tokens.push_back("&"); i++;
//            } else if (line[i]=='|') {
//                tokens.push_back("|"); i++;
//            } else if (line[i]=='=') {
//                if (i+1 < line.size() && line[i+1]=='>') {
//                    tokens.push_back("=>"); i+=2;
//                } else {
//                    tokens.push_back("="); i++;
//                }
//            } else {
//                i++;
//            }
//        }

//   Now, parse.

//   We'll create the ASTNode struct and the parsing functions.

//   Let's do it.

//   Note: We assume the input is syntactically correct.

//   Due to the length, we write the entire program.

//   IMPORTANT: The problem says the number of formulae K (<=20) and the total work is bounded, so even if we do 100^4=1000000 for one formula, and K=20, worst-case 20e6 which might be borderline in C++ in 2.75 sec? 
//        But the problem says Sum(Di) <= 10^6, so we assume the total assignments over all formulae is <= 10^6.

//   So we'll assume that.

//   Let's code.

//   We'll use:
//        #include <bits/stdc++.h>
//        using namespace std;

//   And then the heavy code.

//   Note: The graph might have multiple components, but the reachability is computed for the entire graph.

//   Let's go.

//   We'll write the program in one block.

//   IMPORTANT: The problem says the graph is acyclic, so we don't worry about cycles.

//   Steps:

//      Read N, M.
//      Build graph (0-indexed).
//      Compute reachability.
//      Enumerate antichains (using BFS with set to avoid duplicates).
//      Build antichainToIndex map.
//      Precompute const0 and const1.
//      Define helper functions for operations (as lambdas, so we capture the antichains and reach).
//      Then for each formula:
//          Read line, skip empty lines? The problem says: K formulae, then K lines.
//          Tokenize.
//          Parse to AST.
//          Extract variables.
//          Generate all assignments (using nested loops or base-D representation).
//          Evaluate for each assignment.
//          Output "valid" or "invalid".

//   Let's code accordingly.

//   Note: The Max_of_set function: we define it as a lambda that uses the global `reach`.

//   Due to the complexity, we hope it's correct.

//   Let's test with the sample N=1, M=0.

//   For N=1:
//        reach[0][0]=true.
//        Antichains:
//            empty -> []
//            {0} -> [0]
//        So D=2.
//        Max(X) = Max({0}) = {0} -> const0 = index of {0} = 1.
//        const1 = empty = 0.

//        Operations:
//          op_imp(0,1): 
//             b = antichains[1] = [0]
//             for x=0: check y in a (a=0: empty) -> vacuously true -> include 0 -> [0] -> index1.
//          op_imp(1,0): 
//             b = antichains[0] = []
//             so res = [] -> index0.
//          op_and(1,0): 
//             T = [0] (from antichains[1]) and [] (from antichains[0]) -> [0]
//             Max([0]) = [0] -> index1.
//          But for 1=0: 
//             We have (1=>0) AND (0=>1) = op_and( op_imp(0,1), op_imp(1,0) ) = op_and(1,0) = 1? 
//             But we computed: 
//                 op_imp(0,1) = 1, op_imp(1,0)=0, then op_and(1,0)=1? 
//             But we want 1=0 to be 0 (the constant 0), not 1.

//          Actually, we have:
//             const0 = 1 (index1) and const1 = 0 (index0).
//             So the result for 1=0 is index1, which is const0, not const1.
//             Therefore, we check: if result == const1 (which is 0) -> false, so invalid.

//        So it's correct.

//   Now, for "X|~X" with N=1:
//        Let X be assigned 0 (empty set, which is const1) -> 
//            ~X = op_imp(0, const0) = op_imp(0,1) = 1 (which is const0? index1) -> but wait:
//                const0 = index1, const1 = index0.
//                op_imp(0,1) = 
//                    a=0 (antichain index0: empty), b=1 (antichain index1: [0])
//                    so res = [0] -> index1 -> which is const0.
//                Then ~X = const0.
//            X | ~X = op_or(0, 1) = 
//                U = {x: exists y in empty, z in [0]: ... } = empty -> Max(empty)=empty -> index0 = const1.
//            So evaluates to 1 (the top) -> valid.

//        Now, if X is assigned 1 (the antichain [0]):
//            ~X = op_imp(1,1) = 
//                b = [0], a = [0]
//                for x=0: check y in a: y=0 -> 0<=0 -> true, so skip -> res = [] -> index0 = const1.
//            Then X | ~X = op_or(1,0) = 
//                U = {x: exists y in [0], z in empty: ... } = empty -> index0 = const1.
//            So both assignments yield const1 -> valid.

//        Correct.

//   Now, for sample input #2: 
//        The second formula "X|~X" is invalid, as we reasoned.

//   We'll code accordingly.

//   Let's write the program.

//   Note: The problem says the formula string may be up to 254 characters, and K<=20.

//   We'll use unique_ptr for AST.

//   IMPORTANT: The equivalence expansion: 
//        For three parts: [a,b,c] -> (a==b) AND (b==c) = 
//            [ (a=>b) AND (b=>a) ] AND [ (b=>c) AND (c=>b) ]

//   This is what we want.

//   Let's code.

//   Due to the length, we write carefully.

//   Note: We assume the graph is 0-indexed internally.

//   Steps:

//      Read N, M.
//      If M>0, read edges.

//      Compute reach: 
//          vector<vector<bool>> reach(N, vector<bool>(N, false));
//          for (int i=0; i<N; i++) reach[i][i] = true;
//          for (int i=0; i<N; i++) {
//              for (int neighbor : adj[i]) {
//                  reach[i][neighbor] = true;
//              }
//          }
//          // Then use BFS for each node to fill the rest? 
//          // But the graph is small, so we do:
//          for (int k = 0; k < N; k++) {
//              for (int i = 0; i < N; i++) {
//                  for (int j = 0; j < N; j++) {
//                      if (reach[i][k] && reach[k][j]) {
//                          reach[i][j] = true;
//                      }
//                  }
//              }
//          }
//      Floyd-Warshall is O(N^3) = 1e6, acceptable.

//   Now, code.

//   Note: The problem says the graph is acyclic, so no need to worry about cycles.

//   Let's do Floyd-Warshall for simplicity.

//   Steps for antichain enumeration (BFS):

//        vector<vector<int>> antichains;
//        set<vector<int>> seen;
//        queue<vector<int>> q;
//        vector<int> empty;
//        q.push(empty);
//        seen.insert(empty);
//        while (!q.empty()) {
//            vector<int> a = q.front(); q.pop();
//            antichains.push_back(a);
//            for (int i = 0; i < N; i++) {
//                if (find(a.begin(), a.end(), i) != a.end()) continue;
//                bool ok = true;
//                for (int x : a) {
//                    if (reach[x][i] || reach[i][x]) {
//                        ok = false;
//                        break;
//                    }
//                }
//                if (ok) {
//                    vector<int> b = a;
//                    b.push_back(i);
//                    sort(b.begin(), b.end());
//                    if (!seen.count(b)) {
//                        seen.insert(b);
//                        q.push(b);
//                    }
//                }
//            }
//        }

//   But note: the empty set is included.

//   Now, build antichainToIndex.

//   Constants:
//        vector<int> all_vertices(N);
//        iota(all_vertices.begin(), all_vertices.end(), 0);
//        vector<int> max_X;
//        for (int i=0; i<N; i++) {
//            bool is_max = true;
//            for (int j=0; j<N; j++) {
//                if (i==j) continue;
//                if (reach[i][j]) {
//                    is_max = false;
//                    break;
//                }
//            }
//            if (is_max) max_X.push_back(i);
//        }
//        sort(max_X.begin(), max_X.end());
//        int const0 = antichainToIndex[max_X];
//        vector<int> empty_vec;
//        int const1 = antichainToIndex[empty_vec];

//   Then define operations:

//        auto op_imp = [&](int a_idx, int b_idx) {
//            const vector<int>& b_list = antichains[b_idx];
//            vector<int> res;
//            for (int x : b_list) {
//                bool ok = true;
//                for (int y : antichains[a_idx]) {
//                    if (reach[x][y]) {
//                        ok = false;
//                        break;
//                    }
//                }
//                if (ok) res.push_back(x);
//            }
//            sort(res.begin(), res.end());
//            return antichainToIndex[res];
//        };

//        auto op_and = [&](int a_idx, int b_idx) {
//            vector<int> T;
//            for (int x : antichains[a_idx]) T.push_back(x);
//            for (int x : antichains[b_idx]) T.push_back(x);
//            // Now compute Max(T)
//            vector<int> res;
//            for (int x : T) {
//                bool is_max = true;
//                for (int y : T) {
//                    if (x == y) continue;
//                    if (reach[x][y]) {
//                        is_max = false;
//                        break;
//                    }
//                }
//                if (is_max) res.push_back(x);
//            }
//            sort(res.begin(), res.end());
//            return antichainToIndex[res];
//        };

//        auto op_or = [&](int a_idx, int b_idx) {
//            vector<int> U;
//            for (int x = 0; x < N; x++) {
//                bool from_a = false;
//                for (int y : antichains[a_idx]) {
//                    if (reach[x][y]) {
//                        from_a = true;
//                        break;
//                    }
//                }
//                bool from_b = false;
//                for (int y : antichains[b_idx]) {
//                    if (reach[x][y]) {
//                        from_b = true;
//                        break;
//                    }
//                }
//                if (from_a && from_b) {
//                    U.push_back(x);
//                }
//            }
//            vector<int> res;
//            for (int x : U) {
//                bool is_max = true;
//                for (int y : U) {
//                    if (x == y) continue;
//                    if (reach[x][y]) {
//                        is_max = false;
//                        break;
//                    }
//                }
//                if (is_max) res.push_back(x);
//            }
//            sort(res.begin(), res.end());
//            return antichainToIndex[res];
//        };

//   Now, the ASTNode:

//        struct ASTNode {
//            enum Type { CONST0, CONST1, VAR, NOT, AND, OR, IMPL } type;
//            char var;
//            unique_ptr<ASTNode> left, right;
//            ASTNode(Type t, char c=0, unique_ptr<ASTNode> l=nullptr, unique_ptr<ASTNode> r=nullptr) 
//                : type(t), var(c), left(move(l)), right(move(r)) {}
//        };

//   Parsing functions (we'll write as lambda or separate functions). Since we are in main, we write lambdas that capture the tokens.

//   Due to the complexity, we write a recursive parser inside the main for formulae.

//   Steps for one formula:

//        vector<string> tokens = tokenize(line);
//        int pos = 0;
//        auto parse_equivalence = [&](auto&& self, vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//            vector<unique_ptr<ASTNode>> parts;
//            parts.push_back(self(self, tokens, pos)); // parse_implication
//            while (pos < tokens.size() && tokens[pos]=="=") {
//                pos++;
//                parts.push_back(self(self, tokens, pos));
//            }
//            if (parts.size() == 1) {
//                return move(parts[0]);
//            }
//            auto res = make_equivalence(move(parts[0]), move(parts[1]));
//            for (int i=2; i<parts.size(); i++) {
//                auto eq = make_equivalence(move(parts[i-1]), move(parts[i]));
//                res = make_unique<ASTNode>(ASTNode::AND, 0, move(res), move(eq));
//            }
//            return res;
//        };

//        But we have to define make_equivalence and the other parsers.

//        We'll do:

//        function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_implication, parse_disjunction, parse_conjunction, parse_negation, parse_atom;

//        auto parse_equivalence = [&](auto&& self, vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//            vector<unique_ptr<ASTNode>> parts;
//            parts.push_back(parse_implication(tokens, pos));
//            while (pos < tokens.size() && tokens[pos]=="=") {
//                pos++;
//                parts.push_back(parse_implication(tokens, pos));
//            }
//            if (parts.size() == 1) {
//                return move(parts[0]);
//            }
//            auto res = make_equivalence(move(parts[0]), move(parts[1]));
//            for (int i=2; i<parts.size(); i++) {
//                auto eq = make_equivalence(move(parts[i-1]), move(parts[i]));
//                res = make_unique<ASTNode>(ASTNode::AND, 0, move(res), move(eq));
//            }
//            return res;
//        };

//        But we need to define the parsers in order.

//        Alternatively, we can write iterative functions.

//   Given the time, we write a full recursive descent parser with separate functions, but since we are in main, we use lambdas with Y-combinator.

//   We'll do:

//        function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_equivalence;
//        function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_implication;
//        function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_disjunction;
//        function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_conjunction;
//        function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_negation;
//        function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_atom;

//        parse_atom = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//            if (pos >= tokens.size()) return nullptr;
//            if (tokens[pos]=="0") {
//                pos++;
//                return make_unique<ASTNode>(ASTNode::CONST0);
//            }
//            if (tokens[pos]=="1") {
//                pos++;
//                return make_unique<ASTNode>(ASTNode::CONST1);
//            }
//            if (tokens[pos].size()==1 && isupper(tokens[pos][0])) {
//                char c = tokens[pos][0];
//                pos++;
//                return make_unique<ASTNode>(ASTNode::VAR, c);
//            }
//            if (tokens[pos]=="(") {
//                pos++;
//                auto expr = parse_equivalence(tokens, pos);
//                if (pos < tokens.size() && tokens[pos]==")") {
//                    pos++;
//                }
//                return expr;
//            }
//            // Error, but problem says correct, so we return nullptr
//            return nullptr;
//        };

//        parse_negation = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//            if (pos < tokens.size() && tokens[pos]=="~") {
//                pos++;
//                auto expr = parse_negation(tokens, pos);
//                return make_unique<ASTNode>(ASTNode::NOT, 0, move(expr), nullptr);
//            }
//            return parse_atom(tokens, pos);
//        };

//        parse_conjunction = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//            auto left = parse_negation(tokens, pos);
//            while (pos < tokens.size() && tokens[pos]=="&") {
//                pos++;
//                auto right = parse_negation(tokens, pos);
//                left = make_unique<ASTNode>(ASTNode::AND, 0, move(left), move(right));
//            }
//            return left;
//        };

//        parse_disjunction = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//            auto left = parse_conjunction(tokens, pos);
//            while (pos < tokens.size() && tokens[pos]=="|") {
//                pos++;
//                auto right = parse_conjunction(tokens, pos);
//                left = make_unique<ASTNode>(ASTNode::OR, 0, move(left), move(right));
//            }
//            return left;
//        };

//        parse_implication = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//            auto left = parse_disjunction(tokens, pos);
//            if (pos < tokens.size() && tokens[pos]==">") {
//                // It should be "=>", but we tokenized "=>" as a token, so we expect tokens[pos]=="=>"
//                // But our tokenization: if we see "=>", we put "=>", so we check for "=>"
//                if (tokens[pos]=="=>") {
//                    pos++;
//                    auto right = parse_implication(tokens, pos);
//                    return make_unique<ASTNode>(ASTNode::IMPL, 0, move(left), move(right));
//                }
//            }
//            return left;
//        };

//        But wait, our tokenization: 
//            For "=>", we tokenized as "=>", so tokens[pos] is the string "=>", not "=". 
//            However, in the token list, we have a token "=>", so we check tokens[pos] == "=>"

//        So:

//        parse_implication = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//            auto left = parse_disjunction(tokens, pos);
//            if (pos < tokens.size() && tokens[pos] == "=>") {
//                pos++;
//                auto right = parse_implication(tokens, pos);
//                return make_unique<ASTNode>(ASTNode::IMPL, 0, move(left), move(right));
//            }
//            return left;
//        };

//        Then parse_equivalence:

//        parse_equivalence = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//            vector<unique_ptr<ASTNode>> parts;
//            parts.push_back(parse_implication(tokens, pos));
//            while (pos < tokens.size() && tokens[pos] == "=") {
//                pos++;
//                parts.push_back(parse_implication(tokens, pos));
//            }
//            if (parts.size() == 1) {
//                return move(parts[0]);
//            }
//            auto make_equivalence = [](unique_ptr<ASTNode> a, unique_ptr<ASTNode> b) {
//                auto impl1 = make_unique<ASTNode>(ASTNode::IMPL, 0, move(a), move(b));
//                auto impl2 = make_unique<ASTNode>(ASTNode::IMPL, 0, move(b), move(a));
//                return make_unique<ASTNode>(ASTNode::AND, 0, move(impl1), move(impl2));
//            };
//            auto res = make_equivalence(move(parts[0]), move(parts[1]));
//            for (int i = 2; i < parts.size(); i++) {
//                auto eq = make_equivalence(move(parts[i-1]), move(parts[i]));
//                res = make_unique<ASTNode>(ASTNode::AND, 0, move(res), move(eq));
//            }
//            return res;
//        };

//   Then, for a formula, we call:
//        int pos = 0;
//        auto ast = parse_equivalence(tokens, pos);

//   Now, evaluation:

//        function<int(unique_ptr<ASTNode>&, const map<char, int>&)> eval;
//        eval = [&](unique_ptr<ASTNode>& node, const map<char, int>& assignment) -> int {
//            ASTNode* n = node.get();
//            if (n->type == ASTNode::CONST0) {
//                return const0;
//            }
//            if (n->type == ASTNode::CONST1) {
//                return const1;
//            }
//            if (n->type == ASTNode::VAR) {
//                return assignment.at(n->var);
//            }
//            if (n->type == ASTNode::NOT) {
//                int a = eval(n->left, assignment);
//                return op_imp(a, const0);
//            }
//            if (n->type == ASTNode::AND) {
//                int a = eval(n->left, assignment);
//                int b = eval(n->right, assignment);
//                return op_and(a, b);
//            }
//            if (n->type == ASTNode::OR) {
//                int a = eval(n->left, assignment);
//                int b = eval(n->right, assignment);
//                return op_or(a, b);
//            }
//            if (n->type == ASTNode::IMPL) {
//                int a = eval(n->left, assignment);
//                int b = eval(n->right, assignment);
//                return op_imp(a, b);
//            }
//            return -1;
//        };

//   Now, for each formula:

//        // Extract variables
//        set<char> vars;
//        function<void(unique_ptr<ASTNode>&)> get_vars;
//        get_vars = [&](unique_ptr<ASTNode>& node) {
//            ASTNode* n = node.get();
//            if (n->type == ASTNode::VAR) {
//                vars.insert(n->var);
//            } else if (n->type == ASTNode::NOT || n->type == ASTNode::AND || n->type == OR || n->type == IMPL) {
//                get_vars(n->left);
//                if (n->type != ASTNode::NOT) {
//                    get_vars(n->right);
//                }
//            }
//        };
//        get_vars(ast);

//        vector<char> var_list(vars.begin(), vars.end());
//        sort(var_list.begin(), var_list.end());
//        int num_vars = var_list.size();
//        int D = antichains.size();
//        bool valid_flag = true;
//        // Iterate over all assignments: 
//        for (int mask = 0; mask < (int)pow(D, num_vars); mask++) {
//            map<char, int> assignment;
//            int tmp = mask;
//            for (int i=0; i<num_vars; i++) {
//                assignment[var_list[i]] = tmp % D;
//                tmp /= D;
//            }
//            int res = eval(ast, assignment);
//            if (res != const1) {
//                valid_flag = false;
//                break;
//            }
//        }

//        Then output "valid" if valid_flag, else "invalid".

//   Note: The pow(D, num_vars) might be computed as:
//        int total = 1;
//        for (int i=0; i<num_vars; i++) total *= D;
//        for (int mask=0; mask<total; mask++) ...

//   But num_vars is at most 26, and D<=100, but the problem says the total work is bounded by 10^6, so we assume total is not too big.

//   Let's compute total = 1;
//        for (int i=0; i<num_vars; i++) {
//            total *= D;
//            if (total > 1000000) break; // but the problem says Sum(Di)<=10^6, so we don't need to worry if it's too big per formula? 
//        }
//        But the problem guarantees that the total work (sum over formulae of D^(#vars)) <= 10^6, so we do the loop.

//   Now, write the entire program.

//   IMPORTANT: The problem says the first line is N and M, then M edges, then K, then K formulae.

//   Steps:

//        int N, M;
//        cin >> N >> M;
//        vector<vector<int>> adj(N);
//        for (int i=0; i<M; i++) {
//            int s, t;
//            cin >> s >> t;
//            s--; t--;
//            adj[s].push_back(t);
//        }

//        // Compute reachability with Floyd-Warshall
//        vector<vector<bool>> reach(N, vector<bool>(N, false));
//        for (int i=0; i<N; i++) reach[i][i] = true;
//        for (int i=0; i<N; i++) {
//            for (int j=0; j<N; j++) {
//                if (i != j) {
//                    for (int neighbor : adj[i]) {
//                        if (reach[neighbor][j]) {
//                            reach[i][j] = true;
//                        }
//                    }
//                }
//            }
//        }
//        // Floyd-Warshall
//        for (int k=0; k<N; k++) {
//            for (int i=0; i<N; i++) {
//                for (int j=0; j<N; j++) {
//                    if (reach[i][k] && reach[k][j]) {
//                        reach[i][j] = true;
//                    }
//                }
//            }
//        }

//        // Enumerate antichains
//        vector<vector<int>> antichains;
//        set<vector<int>> seen;
//        queue<vector<int>> q;
//        vector<int> empty_vec;
//        q.push(empty_vec);
//        seen.insert(empty_vec);
//        while (!q.empty()) {
//            vector<int> a = q.front(); q.pop();
//            antichains.push_back(a);
//            for (int i=0; i<N; i++) {
//                if (find(a.begin(), a.end(), i) != a.end()) continue;
//                bool ok = true;
//                for (int x : a) {
//                    if (reach[x][i] || reach[i][x]) {
//                        ok = false;
//                        break;
//                    }
//                }
//                if (ok) {
//                    vector<int> b = a;
//                    b.push_back(i);
//                    sort(b.begin(), b.end());
//                    if (seen.find(b) == seen.end()) {
//                        seen.insert(b);
//                        q.push(b);
//                    }
//                }
//            }
//        }

//        // Build map
//        map<vector<int>, int> antichainToIndex;
//        for (int i=0; i<antichains.size(); i++) {
//            antichainToIndex[antichains[i]] = i;
//        }

//        int D = antichains.size();
//        // Compute const0 = Max(X)
//        vector<int> all_vertices(N);
//        iota(all_vertices.begin(), all_vertices.end(), 0);
//        vector<int> max_X;
//        for (int i=0; i<N; i++) {
//            bool is_max = true;
//            for (int j=0; j<N; j++) {
//                if (i == j) continue;
//                if (reach[i][j]) {
//                    is_max = false;
//                    break;
//                }
//            }
//            if (is_max) max_X.push_back(i);
//        }
//        sort(max_X.begin(), max_X.end());
//        int const0 = antichainToIndex[max_X];
//        int const1 = antichainToIndex[empty_vec];

//        // Define operations as lambdas (capture reach, antichains, antichainToIndex)
//        auto op_imp = [&](int a_idx, int b_idx) -> int {
//            const vector<int>& b_list = antichains[b_idx];
//            vector<int> res;
//            for (int x : b_list) {
//                bool ok = true;
//                for (int y : antichains[a_idx]) {
//                    if (reach[x][y]) {
//                        ok = false;
//                        break;
//                    }
//                }
//                if (ok) {
//                    res.push_back(x);
//                }
//            }
//            sort(res.begin(), res.end());
//            return antichainToIndex[res];
//        };

//        auto op_and = [&](int a_idx, int b_idx) -> int {
//            vector<int> T;
//            for (int x : antichains[a_idx]) T.push_back(x);
//            for (int x : antichains[b_idx]) T.push_back(x);
//            vector<int> res;
//            for (int x : T) {
//                bool is_max = true;
//                for (int y : T) {
//                    if (x == y) continue;
//                    if (reach[x][y]) {
//                        is_max = false;
//                        break;
//                    }
//                }
//                if (is_max) {
//                    res.push_back(x);
//                }
//            }
//            sort(res.begin(), res.end());
//            return antichainToIndex[res];
//        };

//        auto op_or = [&](int a_idx, int b_idx) -> int {
//            vector<int> U;
//            for (int x = 0; x < N; x++) {
//                bool from_a = false;
//                for (int y : antichains[a_idx]) {
//                    if (reach[x][y]) {
//                        from_a = true;
//                        break;
//                    }
//                }
//                bool from_b = false;
//                for (int y : antichains[b_idx]) {
//                    if (reach[x][y]) {
//                        from_b = true;
//                        break;
//                    }
//                }
//                if (from_a && from_b) {
//                    U.push_back(x);
//                }
//            }
//            vector<int> res;
//            for (int x : U) {
//                bool is_max = true;
//                for (int y : U) {
//                    if (x == y) continue;
//                    if (reach[x][y]) {
//                        is_max = false;
//                        break;
//                    }
//                }
//                if (is_max) {
//                    res.push_back(x);
//                }
//            }
//            sort(res.begin(), res.end());
//            return antichainToIndex[res];
//        };

//        // Read K
//        int K;
//        cin >> K;
//        string dummy;
//        getline(cin, dummy); // to skip the newline after K

//        for (int i=0; i<K; i++) {
//            string line;
//            getline(cin, line);
//            // Tokenize
//            vector<string> tokens;
//            for (int j=0; j<line.size(); ) {
//                if (isspace(line[j])) {
//                    j++;
//                    continue;
//                }
//                if (line[j]=='0' || line[j]=='1') {
//                    tokens.push_back(string(1, line[j]));
//                    j++;
//                } else if (isupper(line[j])) {
//                    tokens.push_back(string(1, line[j]));
//                    j++;
//                } else if (line[j]=='(' || line[j]==')') {
//                    tokens.push_back(string(1, line[j]));
//                    j++;
//                } else if (line[j]=='~' || line[j]=='&' || line[j]=='|') {
//                    tokens.push_back(string(1, line[j]));
//                    j++;
//                } else if (line[j]=='=') {
//                    if (j+1 < line.size() && line[j+1]=='>') {
//                        tokens.push_back("=>");
//                        j += 2;
//                    } else {
//                        tokens.push_back("=");
//                        j++;
//                    }
//                } else {
//                    j++;
//                }
//            }

//            // Parse the tokens
//            // Define ASTNode struct
//            struct ASTNode {
//                enum Type { CONST0, CONST1, VAR, NOT, AND, OR, IMPL } type;
//                char var;
//                unique_ptr<ASTNode> left, right;
//                ASTNode(Type t, char c=0, unique_ptr<ASTNode> l=nullptr, unique_ptr<ASTNode> r=nullptr) 
//                    : type(t), var(c), left(move(l)), right(move(r)) {}
//            };

//            // Parser
//            int pos = 0;
//            function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_implication;
//            function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_disjunction;
//            function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_conjunction;
//            function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_negation;
//            function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_atom;
//            function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_equivalence;

//            parse_atom = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//                if (pos >= tokens.size()) return nullptr;
//                if (tokens[pos]=="0") {
//                    pos++;
//                    return make_unique<ASTNode>(ASTNode::CONST0);
//                }
//                if (tokens[pos]=="1") {
//                    pos++;
//                    return make_unique<ASTNode>(ASTNode::CONST1);
//                }
//                if (tokens[pos].size()==1 && isupper(tokens[pos][0])) {
//                    char c = tokens[pos][0];
//                    pos++;
//                    return make_unique<ASTNode>(ASTNode::VAR, c);
//                }
//                if (tokens[pos]=="(") {
//                    pos++;
//                    auto expr = parse_equivalence(tokens, pos);
//                    if (pos < tokens.size() && tokens[pos]==")") {
//                        pos++;
//                    }
//                    return expr;
//                }
//                return nullptr;
//            };

//            parse_negation = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//                if (pos < tokens.size() && tokens[pos]=="~") {
//                    pos++;
//                    auto expr = parse_negation(tokens, pos);
//                    return make_unique<ASTNode>(ASTNode::NOT, 0, move(expr), nullptr);
//                }
//                return parse_atom(tokens, pos);
//            };

//            parse_conjunction = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//                auto left = parse_negation(tokens, pos);
//                while (pos < tokens.size() && tokens[pos]=="&") {
//                    pos++;
//                    auto right = parse_negation(tokens, pos);
//                    left = make_unique<ASTNode>(ASTNode::AND, 0, move(left), move(right));
//                }
//                return left;
//            };

//            parse_disjunction = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//                auto left = parse_conjunction(tokens, pos);
//                while (pos < tokens.size() && tokens[pos]=="|") {
//                    pos++;
//                    auto right = parse_conjunction(tokens, pos);
//                    left = make_unique<ASTNode>(ASTNode::OR, 0, move(left), move(right));
//                }
//                return left;
//            };

//            parse_implication = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//                auto left = parse_disjunction(tokens, pos);
//                if (pos < tokens.size() && tokens[pos]==">") {
//                    // Check: it should be "=>", but we tokenized "=>" as "=>", so we check tokens[pos]=="=>"
//                    // However, in our tokenization, "=>" is stored as "=>", so we check for "=>"
//                    if (tokens[pos] == "=>") {
//                        pos++;
//                        auto right = parse_implication(tokens, pos);
//                        return make_unique<ASTNode>(ASTNode::IMPL, 0, move(left), move(right));
//                    }
//                }
//                return left;
//            };

//            parse_equivalence = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//                vector<unique_ptr<ASTNode>> parts;
//                parts.push_back(parse_implication(tokens, pos));
//                while (pos < tokens.size() && tokens[pos]=="=") {
//                    pos++;
//                    parts.push_back(parse_implication(tokens, pos));
//                }
//                if (parts.size() == 1) {
//                    return move(parts[0]);
//                }
//                auto make_equivalence = [](unique_ptr<ASTNode> a, unique_ptr<ASTNode> b) {
//                    auto impl1 = make_unique<ASTNode>(ASTNode::IMPL, 0, move(a), move(b));
//                    auto impl2 = make_unique<ASTNode>(ASTNode::IMPL, 0, move(b), move(a));
//                    return make_unique<ASTNode>(ASTNode::AND, 0, move(impl1), move(impl2));
//                };
//                auto res = make_equivalence(move(parts[0]), move(parts[1]));
//                for (int i=2; i<parts.size(); i++) {
//                    auto eq = make_equivalence(move(parts[i-1]), move(parts[i]));
//                    res = make_unique<ASTNode>(ASTNode::AND, 0, move(res), move(eq));
//                }
//                return res;
//            };

//            auto ast = parse_equivalence(tokens, pos);

//            // Extract variables
//            set<char> vars;
//            function<void(unique_ptr<ASTNode>&)> get_vars;
//            get_vars = [&](unique_ptr<ASTNode>& node) {
//                ASTNode* n = node.get();
//                if (n->type == ASTNode::VAR) {
//                    vars.insert(n->var);
//                } else {
//                    if (n->left) get_vars(n->left);
//                    if (n->right) get_vars(n->right);
//                }
//            };
//            get_vars(ast);

//            vector<char> var_list(vars.begin(), vars.end());
//            int num_vars = var_list.size();
//            int total_assign = 1;
//            for (int i=0; i<num_vars; i++) {
//                total_assign *= D;
//                if (total_assign > 1000000) break; // safety, but problem says Sum(Di)<=10^6
//            }

//            bool valid_flag = true;
//            // Evaluate for all assignments
//            for (int mask=0; mask<total_assign; mask++) {
//                map<char, int> assignment;
//                int tmp = mask;
//                for (int j=0; j<num_vars; j++) {
//                    assignment[var_list[j]] = tmp % D;
//                    tmp /= D;
//                }
//                // Evaluation function
//                function<int(unique_ptr<ASTNode>&, const map<char, int>&)> eval;
//                eval = [&](unique_ptr<ASTNode>& node, const map<char, int>& assignment) -> int {
//                    ASTNode* n = node.get();
//                    if (n->type == ASTNode::CONST0) {
//                        return const0;
//                    }
//                    if (n->type == ASTNode::CONST1) {
//                        return const1;
//                    }
//                    if (n->type == ASTNode::VAR) {
//                        return assignment.at(n->var);
//                    }
//                    if (n->type == ASTNode::NOT) {
//                        int a = eval(n->left, assignment);
//                        return op_imp(a, const0);
//                    }
//                    if (n->type == ASTNode::AND) {
//                        int a = eval(n->left, assignment);
//                        int b = eval(n->right, assignment);
//                        return op_and(a, b);
//                    }
//                    if (n->type == ASTNode::OR) {
//                        int a = eval(n->left, assignment);
//                        int b = eval(n->right, assignment);
//                        return op_or(a, b);
//                    }
//                    if (n->type == ASTNode::IMPL) {
//                        int a = eval(n->left, assignment);
//                        int b = eval(n->right, assignment);
//                        return op_imp(a, b);
//                    }
//                    return -1;
//                };
//                int res = eval(ast, assignment);
//                if (res != const1) {
//                    valid_flag = false;
//                    break;
//                }
//            }

//            cout << (valid_flag ? "valid" : "invalid") << endl;
//        }

//   However, note: the above ASTNode is defined inside the for-loop for formulae. That is acceptable in C++17.

//   But the lambdas capture by reference, and the ASTNode is defined locally, so it's fine.

//   Let's hope it passes.

//   Note: The sample input #1: 
//        N=1, M=0, K=6, then 6 formulae.

//   We'll run on the sample.

//   But due to the complexity, we hope it's correct.

//   One more note: the tokenization for "=>" might be confused with "=>" as two tokens? 
//        We do: if we see '=' and then '>' we push "=>", so it's one token.

//   Now, write the entire code.

//   IMPORTANT: The problem says the first token is N and M, and then M edges. 
//        After reading N, M, we read M edges.

//   Also, after reading K, we use getline for the next K lines.

//   But note: after "cin >> K", there is a newline, so we do:

//        cin >> K;
//        string dummy;
//        getline(cin, dummy); // to skip the newline

//   However, the first formula might be on the same line as K? 
//        The problem says: 
//            next line contains K
//            following K lines contain one formula each.

//        So after K, there is a newline, then the first formula.

//   So we do:

//        cin >> K;
//        vector<string> formulae(K);
//        for (int i=0; i<K; i++) {
//            getline(cin, formulae[i]);
//        }

//   But if the line after K is empty? The problem says: "the following K lines contain one formula each", so we assume non-empty lines.

//   Alternatively, we can:

//        cin >> K;
//        cin.ignore();
//        for (int i=0; i<K; i++) {
//            string line;
//            getline(cin, line);
//        }

//   Let's do that.

//   Code structure:

//        #include <bits/stdc++.h>
//        using namespace std;

//        int main() {
//            // Read N, M
//            int N, M;
//            cin >> N >> M;
//            vector<vector<int>> adj(N);
//            for (int i=0; i<M; i++) {
//                int s, t;
//                cin >> s >> t;
//                s--; t--;
//                adj[s].push_back(t);
//            }

//            // Compute reachability
//            vector<vector<bool>> reach(N, vector<bool>(N, false));
//            for (int i=0; i<N; i++) {
//                reach[i][i] = true;
//                for (int neighbor : adj[i]) {
//                    reach[i][neighbor] = true;
//                }
//            }
//            for (int k=0; k<N; k++) {
//                for (int i=0; i<N; i++) {
//                    for (int j=0; j<N; j++) {
//                        if (reach[i][k] && reach[k][j]) {
//                            reach[i][j] = true;
//                        }
//                    }
//                }
//            }

//            // Enumerate antichains
//            vector<vector<int>> antichains;
//            set<vector<int>> seen;
//            queue<vector<int>> q;
//            vector<int> empty_vec;
//            q.push(empty_vec);
//            seen.insert(empty_vec);
//            while (!q.empty()) {
//                vector<int> a = q.front(); q.pop();
//                antichains.push_back(a);
//                for (int i=0; i<N; i++) {
//                    if (find(a.begin(), a.end(), i) != a.end()) continue;
//                    bool ok = true;
//                    for (int x : a) {
//                        if (reach[x][i] || reach[i][x]) {
//                            ok = false;
//                            break;
//                        }
//                    }
//                    if (ok) {
//                        vector<int> b = a;
//                        b.push_back(i);
//                        sort(b.begin(), b.end());
//                        if (seen.find(b) == seen.end()) {
//                            seen.insert(b);
//                            q.push(b);
//                        }
//                    }
//                }
//            }

//            map<vector<int>, int> antichainToIndex;
//            for (int i=0; i<antichains.size(); i++) {
//                antichainToIndex[antichains[i]] = i;
//            }
//            int D = antichains.size();

//            // Constants
//            vector<int> all_vertices(N);
//            iota(all_vertices.begin(), all_vertices.end(), 0);
//            vector<int> max_X;
//            for (int i=0; i<N; i++) {
//                bool is_max = true;
//                for (int j=0; j<N; j++) {
//                    if (i == j) continue;
//                    if (reach[i][j]) {
//                        is_max = false;
//                        break;
//                    }
//                }
//                if (is_max) max_X.push_back(i);
//            }
//            sort(max_X.begin(), max_X.end());
//            int const0 = antichainToIndex[max_X];
//            int const1 = antichainToIndex[empty_vec];

//            // Operations
//            auto op_imp = [&](int a_idx, int b_idx) -> int {
//                const vector<int>& b_list = antichains[b_idx];
//                vector<int> res;
//                for (int x : b_list) {
//                    bool ok = true;
//                    for (int y : antichains[a_idx]) {
//                        if (reach[x][y]) {
//                            ok = false;
//                            break;
//                        }
//                    }
//                    if (ok) res.push_back(x);
//                }
//                sort(res.begin(), res.end());
//                return antichainToIndex[res];
//            };

//            auto op_and = [&](int a_idx, int b_idx) -> int {
//                vector<int> T;
//                for (int x : antichains[a_idx]) T.push_back(x);
//                for (int x : antichains[b_idx]) T.push_back(x);
//                vector<int> res;
//                for (int x : T) {
//                    bool is_max = true;
//                    for (int y : T) {
//                        if (x == y) continue;
//                        if (reach[x][y]) {
//                            is_max = false;
//                            break;
//                        }
//                    }
//                    if (is_max) res.push_back(x);
//                }
//                sort(res.begin(), res.end());
//                return antichainToIndex[res];
//            };

//            auto op_or = [&](int a_idx, int b_idx) -> int {
//                vector<int> U;
//                for (int x=0; x<N; x++) {
//                    bool from_a = false;
//                    for (int y : antichains[a_idx]) {
//                        if (reach[x][y]) {
//                            from_a = true;
//                            break;
//                        }
//                    }
//                    bool from_b = false;
//                    for (int y : antichains[b_idx]) {
//                        if (reach[x][y]) {
//                            from_b = true;
//                            break;
//                        }
//                    }
//                    if (from_a && from_b) {
//                        U.push_back(x);
//                    }
//                }
//                vector<int> res;
//                for (int x : U) {
//                    bool is_max = true;
//                    for (int y : U) {
//                        if (x == y) continue;
//                        if (reach[x][y]) {
//                            is_max = false;
//                            break;
//                        }
//                    }
//                    if (is_max) res.push_back(x);
//                }
//                sort(res.begin(), res.end());
//                return antichainToIndex[res];
//            };

//            int K;
//            cin >> K;
//            cin.ignore();
//            for (int i=0; i<K; i++) {
//                string line;
//                getline(cin, line);
//                // Tokenize
//                vector<string> tokens;
//                for (int j=0; j<line.size(); ) {
//                    if (isspace((unsigned char)line[j])) {
//                        j++;
//                        continue;
//                    }
//                    if (line[j]=='0' || line[j]=='1') {
//                        tokens.push_back(string(1, line[j]));
//                        j++;
//                    } else if (isupper((unsigned char)line[j])) {
//                        tokens.push_back(string(1, line[j]));
//                        j++;
//                    } else if (line[j]=='(' || line[j]==')') {
//                        tokens.push_back(string(1, line[j]));
//                        j++;
//                    } else if (line[j]=='~' || line[j]=='&' || line[j]=='|') {
//                        tokens.push_back(string(1, line[j]));
//                        j++;
//                    } else if (line[j]=='=') {
//                        if (j+1 < line.size() && line[j+1]=='>') {
//                            tokens.push_back("=>");
//                            j += 2;
//                        } else {
//                            tokens.push_back("=");
//                            j++;
//                        }
//                    } else {
//                        j++;
//                    }
//                }

//                // Now parse
//                struct ASTNode {
//                    enum Type { CONST0, CONST1, VAR, NOT, AND, OR, IMPL } type;
//                    char var;
//                    unique_ptr<ASTNode> left, right;
//                    ASTNode(Type t, char c=0, unique_ptr<ASTNode> l=nullptr, unique_ptr<ASTNode> r=nullptr) 
//                        : type(t), var(c), left(move(l)), right(move(r)) {}
//                };

//                int pos = 0;
//                function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_implication;
//                function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_disjunction;
//                function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_conjunction;
//                function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_negation;
//                function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_atom;
//                function<unique_ptr<ASTNode>(vector<string>&, int&)> parse_equivalence;

//                parse_atom = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//                    if (pos >= tokens.size()) return nullptr;
//                    if (tokens[pos]=="0") {
//                        pos++;
//                        return make_unique<ASTNode>(ASTNode::CONST0);
//                    }
//                    if (tokens[pos]=="1") {
//                        pos++;
//                        return make_unique<ASTNode>(ASTNode::CONST1);
//                    }
//                    if (tokens[pos].size()==1 && isupper((unsigned char)tokens[pos][0])) {
//                        char c = tokens[pos][0];
//                        pos++;
//                        return make_unique<ASTNode>(ASTNode::VAR, c);
//                    }
//                    if (tokens[pos]=="(") {
//                        pos++;
//                        auto expr = parse_equivalence(tokens, pos);
//                        if (pos < tokens.size() && tokens[pos]==")") {
//                            pos++;
//                        }
//                        return expr;
//                    }
//                    return nullptr;
//                };

//                parse_negation = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//                    if (pos < tokens.size() && tokens[pos]=="~") {
//                        pos++;
//                        auto expr = parse_negation(tokens, pos);
//                        return make_unique<ASTNode>(ASTNode::NOT, 0, move(expr), nullptr);
//                    }
//                    return parse_atom(tokens, pos);
//                };

//                parse_conjunction = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//                    auto left = parse_negation(tokens, pos);
//                    while (pos < tokens.size() && tokens[pos]=="&") {
//                        pos++;
//                        auto right = parse_negation(tokens, pos);
//                        left = make_unique<ASTNode>(ASTNode::AND, 0, move(left), move(right));
//                    }
//                    return left;
//                };

//                parse_disjunction = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//                    auto left = parse_conjunction(tokens, pos);
//                    while (pos < tokens.size() && tokens[pos]=="|") {
//                        pos++;
//                        auto right = parse_conjunction(tokens, pos);
//                        left = make_unique<ASTNode>(ASTNode::OR, 0, move(left), move(right));
//                    }
//                    return left;
//                };

//                parse_implication = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//                    auto left = parse_disjunction(tokens, pos);
//                    if (pos < tokens.size() && tokens[pos] == "=>") {
//                        pos++;
//                        auto right = parse_implication(tokens, pos);
//                        return make_unique<ASTNode>(ASTNode::IMPL, 0, move(left), move(right));
//                    }
//                    return left;
//                };

//                parse_equivalence = [&](vector<string>& tokens, int& pos) -> unique_ptr<ASTNode> {
//                    vector<unique_ptr<ASTNode>> parts;
//                    parts.push_back(parse_implication(tokens, pos));
//                    while (pos < tokens.size() && tokens[pos]=="=") {
//                        pos++;
//                        parts.push_back(parse_implication(tokens, pos));
//                    }
//                    if (parts.size() == 1) {
//                        return move(parts[0]);
//                    }
//                    auto make_equivalence = [](unique_ptr<ASTNode> a, unique_ptr<ASTNode> b) {
//                        auto impl1 = make_unique<ASTNode>(ASTNode::IMPL, 0, move(a), move(b));
//                        auto impl2 = make_unique<ASTNode>(ASTNode::IMPL, 0, move(b), move(a));
//                        return make_unique<ASTNode>(ASTNode::AND, 0, move(impl1), move(impl2));
//                    };
//                    auto res = make_equivalence(move(parts[0]), move(parts[1]));
//                    for (int i=2; i<parts.size(); i++) {
//                        auto eq = make_equivalence(move(parts[i-1]), move(parts[i]));
//                        res = make_unique<ASTNode>(ASTNode::AND, 0, move(res), move(eq));
//                    }
//                    return res;
//                };

//                auto ast = parse_equivalence(tokens, pos);

//                // Extract variables
//                set<char> vars;
//                function<void(unique_ptr<ASTNode>&)> get_vars;
//                get_vars = [&](unique_ptr<ASTNode>& node) {
//                    ASTNode* n = node.get();
//                    if (n->type == ASTNode::VAR) {
//                        vars.insert(n->var);
//                    } else {
//                        if (n->left) get_vars(n->left);
//                        if (n->right) get_vars(n->right);
//                    }
//                };
//                get_vars(ast);
//                vector<char> var_list(vars.begin(), vars.end());
//                int num_vars = var_list.size();
//                int total_assign = 1;
//                for (int j=0; j<num_vars; j++) {
//                    total_assign *= D;
//                    if (total_assign > 1000000) break;
//                }

//                bool valid_flag = true;
//                // If there are no variables, total_assign=1.
//                for (int mask=0; mask<total_assign; mask++) {
//                    map<char, int> assignment;
//                    int tmp = mask;
//                    for (int j=0; j<num_vars; j++) {
//                        assignment[var_list[j]] = tmp % D;
//                        tmp /= D;
//                    }

//                    // Evaluation function
//                    function<int(unique_ptr<ASTNode>&, const map<char, int>&)> eval;
//                    eval = [&](unique_ptr<ASTNode>& node, const map<char, int>& assignment) -> int {
//                        ASTNode* n = node.get();
//                        if (n->type == ASTNode::CONST0) {
//                            return const0;
//                        }
//                        if (n->type == ASTNode::CONST1) {
//                            return const1;
//                        }
//                        if (n->type == ASTNode::VAR) {
//                            return assignment.at(n->var);
//                        }
//                        if (n->type == ASTNode::NOT) {
//                            int a = eval(n->left, assignment);
//                            return op_imp(a, const0);
//                        }
//                        if (n->type == ASTNode::AND) {
//                            int a = eval(n->left, assignment);
//                            int b = eval(n->right, assignment);
//                            return op_and(a, b);
//                        }
//                        if (n->type == ASTNode::OR) {
//                            int a = eval(n->left, assignment);
//                            int b = eval(n->right, assignment);
//                            return op_or(a, b);
//                        }
//                        if (n->type == ASTNode::IMPL) {
//                            int a = eval(n->left, assignment);
//                            int b = eval(n->right, assignment);
//                            return op_imp(a, b);
//                        }
//                        return -1;
//                    };
//                    int res = eval(ast, assignment);
//                    if (res != const1) {
//                        valid_flag = false;
//                        break;
//                    }
//                }

//                cout << (valid_flag ? "valid" : "invalid") << '\n';
//            }

//            return 0;
//        }

//   Let's hope it works.

//   Note: The problem says the graph is acyclic, so no issue.

//   Run on sample input #1.

//   However, note: the first formula "1=0" is the first formula.

//   We'll trust the logic.

//   Due to the problem constraints, we hope it runs within time.

//   Note: The enumeration of antichains might be heavy if D is large, but D<=100, so the BFS is O(D * N) which is 100*100=10000.

//   Now, output the code.

//   IMPORTANT: The problem says the memory limit is 65536 KB, and we are storing antichains (D<=100, each antichain size <= N=100, so 100*100=10000 integers, which is 40KB), so it's fine.

//   Let's write the code.

//   One more note: The tokenization might have issues with lowercase? But the problem says variables are capital letters.

//   We use isupper with unsigned char cast to avoid issues.

//   Now, the full code.


