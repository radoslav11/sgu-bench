#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<tuple<int, int, int>> pairs(n); // k, a, original_index
    for(int i = 0; i < n; i++){
        int k, a;
        cin >> k >> a;
        pairs[i] = make_tuple(k, a, i);
    }
    
    // Sort by auxiliary key
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j){
        return get<1>(pairs[i]) < get<1>(pairs[j]);
    });
    
    vector<int> parent(n, 0);
    vector<int> left(n, 0);
    vector<int> right(n, 0);
    
    int root = order[0] + 1; // 1-indexed
    
    // Insert nodes in order of increasing auxiliary key
    for(int idx = 1; idx < n; idx++){
        int new_node = order[idx] + 1; // 1-indexed
        int new_k = get<0>(pairs[order[idx]]);
        
        // Find position in BST and connect
        int curr = root;
        int par = 0;
        
        while(curr != 0){
            int curr_k = get<0>(pairs[curr - 1]);
            par = curr;
            
            if(new_k < curr_k){
                if(left[curr - 1] == 0){
                    left[curr - 1] = new_node;
                    parent[new_node - 1] = curr;
                    break;
                }
                curr = left[curr - 1];
            } else {
                if(right[curr - 1] == 0){
                    right[curr - 1] = new_node;
                    parent[new_node - 1] = curr;
                    break;
                }
                curr = right[curr - 1];
            }
        }
    }
    
    // Verify the tree
    // Check BST property and heap property
    function<bool(int, int, int)> verify = [&](int node, int minK, int maxK) -> bool {
        if(node == 0) return true;
        
        int k = get<0>(pairs[node - 1]);
        int a = get<1>(pairs[node - 1]);
        
        if(k <= minK || k >= maxK) return false;
        
        // Check heap property with children
        if(left[node - 1] != 0){
            int child_a = get<1>(pairs[left[node - 1] - 1]);
            if(a >= child_a) return false;
        }
        if(right[node - 1] != 0){
            int child_a = get<1>(pairs[right[node - 1] - 1]);
            if(a >= child_a) return false;
        }
        
        return verify(left[node - 1], minK, k) && verify(right[node - 1], k, maxK);
    };
    
    if(!verify(root, INT_MIN, INT_MAX)){
        cout << "NO\n";
        return 0;
    }
    
    cout << "YES\n";
    for(int i = 0; i < n; i++){
        cout << parent[i] << " " << left[i] << " " << right[i] << "\n";
    }
    
    return 0;
}
