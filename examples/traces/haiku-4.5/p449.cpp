#include<bits/stdc++.h>
using namespace std;

struct Node {
    vector<int> children; // indices of child nodes
    vector<int> objects;  // leaf objects if this is a leaf
    int id;
};

int nodeCount = 0;
map<set<int>, int> nodeMap;
vector<Node> nodes;

int getOrCreateNode(set<int> objs) {
    if (nodeMap.find(objs) != nodeMap.end()) {
        return nodeMap[objs];
    }
    int id = nodeCount++;
    nodes.push_back(Node());
    nodes[id].id = id;
    nodeMap[objs] = id;
    return id;
}

vector<int> getOrder(int nodeId);

vector<int> getOrder(int nodeId) {
    Node& node = nodes[nodeId];
    
    if (node.objects.size() > 0) {
        // Leaf node
        return node.objects;
    }
    
    if (node.children.size() == 0) {
        return {};
    }
    
    // Get orders for all children
    vector<pair<int, vector<int>>> childOrders;
    for (int child : node.children) {
        vector<int> order = getOrder(child);
        int minObj = *min_element(order.begin(), order.end());
        childOrders.push_back({minObj, order});
    }
    
    // Sort children by their minimum element (lexicographically smallest)
    sort(childOrders.begin(), childOrders.end());
    
    // Concatenate the orders
    vector<int> result;
    for (auto& p : childOrders) {
        for (int obj : p.second) {
            result.push_back(obj);
        }
    }
    return result;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, Q;
    cin >> N >> M >> Q;
    
    // Initialize leaf nodes
    for (int i = 1; i <= N; i++) {
        set<int> s = {i};
        int id = getOrCreateNode(s);
        nodes[id].objects.push_back(i);
    }
    
    vector<pair<int, set<int>>> splits; // (Y, set of representatives)
    
    for (int i = 0; i < M; i++) {
        int Y, L;
        cin >> Y >> L;
        set<int> representatives;
        for (int j = 0; j < L; j++) {
            int v;
            cin >> v;
            representatives.insert(v);
        }
        splits.push_back({Y, representatives});
    }
    
    // Build hierarchy from splits
    set<int> allObjects;
    for (int i = 1; i <= N; i++) {
        allObjects.insert(i);
    }
    
    map<int, set<int>> groups; // rep -> actual objects in group
    for (int i = 1; i <= N; i++) {
        groups[i] = {i};
    }
    
    // Process splits in order (smallest Y first)
    sort(splits.begin(), splits.end());
    
    for (auto& split : splits) {
        set<int> reps = split.second;
        set<int> merged;
        
        for (int rep : reps) {
            for (int obj : groups[rep]) {
                merged.insert(obj);
            }
        }
        
        int newRep = *merged.begin();
        groups[newRep] = merged;
        
        // Create node for this group
        int nodeId = getOrCreateNode(merged);
        nodes[nodeId].children.clear();
        for (int rep : reps) {
            nodes[nodeId].children.push_back(getOrCreateNode(groups[rep]));
        }
    }
    
    // Find root (all objects)
    int rootId = getOrCreateNode(allObjects);
    vector<int> finalOrder = getOrder(rootId);
    
    for (int i = 0; i < Q; i++) {
        int pos;
        cin >> pos;
        cout << finalOrder[pos - 1] << "\n";
    }
    
    return 0;
}
