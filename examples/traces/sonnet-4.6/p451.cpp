#include<bits/stdc++.h>
using namespace std;
typedef vector<int> VI;
typedef vector<VI> VVI;

VVI expandRel(const string& r);
VVI combine(const VVI& a, const VVI& b){
    VVI res;
    for(auto&x:a)for(auto&y:b){VI t=x;for(int v:y)t.push_back(v);res.push_back(t);}
    return res;
}

// Each person represented as position in family tree
// father=go up via father edge, mother=go up via mother edge
// son=go down to a son, daughter=go down to a daughter
// husband/wife = spouse (0 cost)
// 
// For degree computation: model the family tree
// A person has unique father and mother (no remarriage)
// spouse is unique
// 
// Represent a person as a "symbolic" node
// Track as a tree path from a root
// 
// Actually: track position as (node_id) in a dynamically built family tree
// Use a graph and BFS

// Let's assign coordinates:
// Each person = node
// Moving father: go to father node (create if needed)
// Moving mother: go to mother node
// Moving son: create a new son node (but sons can be shared... no, in different expansions they're different)
// 
// For a single path, we build a mini family tree and compute distance

// Represent person as integer ID, build adjacency with weights

struct FamilyTree {
    // node -> {father, mother, husband/wife, children_sons[], children_daughters[]}
    // For sons/daughters going "down", each step creates a new unique child
    // But husband shares father/mother with wife? No - no marriages between relatives
    // 
    // Actually for computing distance in ONE specific path:
    // We just need the graph of nodes visited and their connections
    
    map<int, int> father_of, mother_of, spouse_of;
    int next_id = 1;
    
    // Returns new unique node
    int newNode(){ return next_id++; }
};

// For a given path, simulate and build the graph, then BFS from start to end
// 
// Nodes: start=0, then each step creates/references nodes
// Edges: parent-child (weight 1), spouse (weight 0)

int computeDegree(const VI& path) {
    // Build family graph
    // node 0 = A
    // follow path to get node C
    // BFS with 0-1 weights (Dijkstra or 0-1 BFS)
    
    map<int,int> father_of, mother_of, spouse_of;
    // children: for each node, list of (child_node, type) where type=son/daughter
    map<int, vector<pair<int,int>>> children_of;
    
    int next_id = 1;
    auto getFather = [&](int n) -> int {
        if(father_of.count(n)) return father_of[n];
        int f = next_id++;
        father_of[n] = f;
        children_of[f].push_back({n, 0}); // son-type? no, need to track
        return f;
    };
    auto getMother = [&](int n) -> int {
        if(mother_of.count(n)) return mother_of[n];
        int m = next_id++;
        mother_of[n] = m;
        children_of[m].push_back({n, 1});
        return m;
    };
    auto getSpouse = [&](int n) -> int {
        if(spouse_of.count(n)) return spouse_of[n];
        int s = next_id++;
        spouse_of[n] = s;
        spouse_of[s] = n;
        return s;
    };
    auto newSon = [&](int n) -> int {
        int s = next_id++;
        children_of[n].push_back({s, 0});
        // s's parents: n and n's spouse (if exists)
        // Actually son's father or mother is n
        // We'll say n is father if n is male... but we don't track gender here
        // Hmm, this is getting complicated
        // Let's just track "child_of" as one parent
        father_of[s] = n; // approximate: parent
        return s;
    };
    // This approach is getting messy. Let me think differently.
    
    // Actually for degree computation, we just need the graph distance.
    // The graph has edges: (person, father) weight 1, (person, mother) weight 1,
    //                       (person, spouse) weight 0
    // Children are just inverse of parent edges.
    // 
    // For a path, we track which nodes are visited and what edges exist between them.
    // Then BFS/Dijkstra from node 0 to final node.
    
    // Let me represent: each node has father, mother, spouse (all optional)
    // and children (sons and daughters)
    // 
    // When we traverse "father", we go to a father node (created if not exists)
    // When we traverse "son", we go to a NEW son node (each "son" step = distinct child)
    // BUT: two different paths might share a son if they converge... 
    // For ONE path evaluation, each "son"/"daughter" step creates a fresh node.
    
    // Actually for ONE path (sequence of moves), we just follow them linearly.
    // The graph we build only needs to support BFS between start and end.
    // 
    // Key insight: along the path, when we go "up" (father/mother) and then "down" (son/daughter),
    // the down-child might be the same as someone we passed through.
    // But "no marriages between relatives" and "no cycles" means...
    // 
    // Actually the distance IS just the path length ignoring spouse edges?
    // No wait - we might go up then down differently.
    
    // Let me think about the example:
    // A's son's mother's mother's son -> distance 2
    // A -> son (down, +1) -> son's mother = A's wife (0) -> wife's mother (up, +1) -> mother's son
    // wife's mother's son... this could be A's wife's brother, or it could be A's wife herself?
    // If mother's son = wife herself (same person as A's wife), then distance = 0 from A's wife = 0+1=1 from A? No wait.
    // 
    // Hmm, A's son's mother = A's wife (since son's mother is either A's wife if A is father, 
    // or son's mother could be a different woman if... no, the son has exactly one mother).
    // son's mother = A's wife (since A is the father of the son, the son's mother = A's wife)
    // Then A's wife's mother's son = A's wife's brother, OR A's wife herself.
    // If A's wife's mother's son = A's wife, that's fine (she's a daughter of her mother, son of her father... wait no, son means male).
    // Hmm, A's wife's mother's son = A's wife's brother (or A's wife if she were male, but she's female... 
    // actually gender isn't tracked in this problem explicitly)
    // 
    // The problem says "no same-sex marriages", so we know gender from context.
    // A's wife is female. Wife's mother's son = wife's brother.
    // Distance from A to wife's brother: A-wife(0)-wife's mother(1)-wife's brother(1) = 2. That matches!
    
    // OK so the graph distance approach should work. Let me implement it properly.
    
    // Node representation: I'll track the "path" as a sequence of up/down/spouse moves
    // and build a small graph.
    
    // Actually, let me use a cleaner representation.
    // Each node is an integer. I'll track:
    // - parent_f[v] = father of v
    // - parent_m[v] = mother of v  
    // - spouse[v] = spouse of v
    // - sons[v], daughters[v] = lists of children
    
    // When we go "father" from v: create/get father node
    // When we go "son" from v: create a new son (unless v already has a son from a previous step in this path... 
    //   but for a single linear path, we just create new)
    // 
    // Wait but for BFS we need to know ALL edges of each node, not just the ones on the path.
    // If we create a son for v, that son's father is v. So there's an edge (son, v) in the graph.
    // Also son has a mother: v's spouse (if exists).
    
    // Let me just track the nodes and edges as we go:
    
    map<int, int> par_f, par_m, sp;
    map<int, vector<int>> adj; // adjacency with weights in separate map
    map<pair<int,int>, int> edge_weight;
    
    auto addEdge = [&](int u, int v, int w) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        edge_weight[{u,v}] = w;
        edge_weight[{v,u}] = w;
    };
    
    auto getOrCreateFather = [&](int n) -> int {
        if(par_f.count(n)) return par_f[n];
        int f = next_id++;
        par_f[n] = f;
        addEdge(n, f, 1);
        return f;
    };
    auto getOrCreateMother = [&](int n) -> int {
        if(par_m.count(n)) return par_m[n];
        int m = next_id++;
        par_m[n] = m;
        addEdge(n, m, 1);
        return m;
    };
    auto getOrCreateSpouse = [&](int n) -> int {
        if(sp.count(n)) return sp[n];
        int s = next_id++;
        sp[n] = s;
        sp[s] = n;
        addEdge(n, s, 0);
        return s;
    };
    
    int cur = 0;
    int final_node = -1;
    
    for(int move : path) {
        int next = -1;
        if(move == 0) { // father
            next = getOrCreateFather(cur);
        } else if(move == 1) { // mother
            next = getOrCreateMother(cur);
        } else if(move == 2) { // son
            // Create new son. Son's parents: cur and cur's spouse.
            int s = next_id++;
            par_f[s] = cur; // assuming cur is father... but what if cur is female?
            // Hmm, gender matters. Let's ignore for now and just say cur is one parent.
            addEdge(s, cur, 1);
            // If cur has spouse, add edge to spouse
            if(sp.count(cur)) {
                int spouse_node = sp[cur];
                par_m[s] = spouse_node; // or par_f, doesn't matter for degree
                addEdge(s, spouse_node, 1);
            }
            next = s;
        } else if(move == 3) { // daughter
            int d = next_id++;
            addEdge(d, cur, 1);
            if(sp.count(cur)) {
                addEdge(d, sp[cur], 1);
            }
            next = d;
        } else if(move == 4) { // husband
            next = getOrCreateSpouse(cur);
        } else if(move == 5) { // wife
            next = getOrCreateSpouse(cur);
        }
        cur = next;
    }
    
    final_node = cur;
    
    if(final_node == 0) return 0;
    
    // BFS (0-1 BFS since weights are 0 or 1)
    map<int, int> dist;
    deque<int> dq;
    dist[0] = 0;
    dq.push_back(0);
    
    while(!dq.empty()) {
        int v = dq.front(); dq.pop_front();
        if(!adj.count(v)) continue;
        for(int u : adj[v]) {
            int w = edge_weight[{v,u}];
            int nd = dist[v] + w;
            if(!dist.count(u) || dist[u] > nd) {
                dist[u] = nd;
                if(w == 0) dq.push_front(u);
                else dq.push_back(u);
            }
        }
    }
    
    return dist.count(final_node) ? dist[final_node] : 1000;
}

VVI expandRel(const string& r) {
    // Returns list of possible basic-move sequences
    // 0=father,1=mother,2=son,3=daughter,4=husband,5=wife
    
    if(r == "father") return {{0}};
    if(r == "mother") return {{1}};
    if(r == "son") return {{2}};
    if(r == "daughter") return {{3}};
    if(r == "husband") return {{4}};
    if(r == "wife") return {{5}};
    
    if(r == "brother") return {{0,2},{1,2}}; // father's son or mother's son
    if(r == "sister") return {{0,3},{1,3}};
    if(r == "grandfather") return {{0,0},{1,0}}; // father's father or mother's father
    if(r == "grandmother") return {{0,1},{1,1}};
    if(r == "grandson") return {{2,2},{3,2}};
    if(r == "granddaughter") return {{2,3},{3,3}};
    
    // uncle = father's brother or mother's brother
    // brother = father's son or mother's son
    // so uncle = father's father's son, father's mother's son, mother's father's son, mother's mother's son
    if(r == "uncle") {
        // father's brother or mother's brother
        VVI bro = expandRel("brother");
        VVI father_bro, mother_bro;
        for(auto& b : bro) {
            VI fb = {0}; for(int x:b) fb.push_back(x); father_bro.push_back(fb);
            VI mb = {1}; for(int x:b) mb.push_back(x); mother_bro.push_back(mb);
        }
        VVI res = father_bro;
        for(auto& x : mother_bro) res.push_back(x);
        return res;
    }
    if(r == "aunt") {
        // father's sister or mother's sister
        VVI sis = expandRel("sister");
        VVI res;
        for(auto& s : sis) {
            VI fs = {0}; for(int x:s) fs.push_back(x); res.push_back(fs);
            VI ms = {1}; for(int x:s) ms.push_back(x); res.push_back(ms);
        }
        return res;
    }
    if(r == "nephew") {
        // brother's son or sister's son
        VVI bro = expandRel("brother");
        VVI sis = expandRel("sister");
        VVI res;
        for(auto& b : bro) { VI t=b; t.push_back(2); res.push_back(t); }
        for(auto& s : sis) { VI t=s; t.push_back(2); res.push_back(t); }
        return res;
    }
    if(r == "niece") {
        VVI bro = expandRel("brother");
        VVI sis = expandRel("sister");
        VVI res;
        for(auto& b : bro) { VI t=b; t.push_back(3); res.push_back(t); }
        for(auto& s : sis) { VI t=s; t.push_back(3); res.push_back(t); }
        return res;
    }
    return {};
}

int main(){
    string line;
    getline(cin, line);
    
    // Parse: "C is A's rel1's rel2's ..."
    // or "C is A"
    
    // Remove "C is A"
    // Find "is A" then parse remaining "'s rel" parts
    
    // Find position of "is A"
    size_t pos = line.find("is A");
    string rest = line.substr(pos + 4); // after "is A"
    
    // rest = "'s rel1's rel2's ..." or ""
    vector<string> rels;
    size_t i = 0;
    while(i < rest.size()) {
        // expect "'s "
        if(rest.substr(i, 2) == "'s") {
            i += 3; // skip "'s "
            // find next "'s" or end
            size_t j = rest.find("'s", i);
            string rel;
            if(j == string::npos) rel = rest.substr(i);
            else rel = rest.substr(i, j-i);
            rels.push_back(rel);
            i = (j == string::npos) ? rest.size() : j;
        } else {
            i++;
        }
    }
    
    if(rels.empty()) {
        cout << "0 0" << endl;
        return 0;
    }
    
    // Build all possible paths
    VVI paths = expandRel(rels[0]);
    for(int k = 1; k < (int)rels.size(); k++) {
        VVI next_exp = expandRel(rels[k]);
        paths = combine(paths, next_exp);
    }
    
    int mn = INT_MAX, mx = INT_MIN;
    for(auto& p : paths) {
        int d = computeDegree(p);
        mn = min(mn, d);
        mx = max(mx, d);
    }
    
    cout << mx << " " << mn << endl;
    return 0;
}
