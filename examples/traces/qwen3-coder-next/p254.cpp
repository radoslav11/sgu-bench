#include <iostream>
#include <vector>
using namespace std;

struct Node {
    int val;
    Node* prev;
    Node* next;
    Node(int v) : val(v), prev(nullptr), next(nullptr) {}
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, Q;
    cin >> N >> Q;
    
    if (N == 0) {
        return 0;
    }
    
    // Create circular doubly linked list
    vector<Node*> nodes(N + 1);
    for (int i = 1; i <= N; i++) {
        nodes[i] = new Node(i);
    }
    
    for (int i = 1; i <= N; i++) {
        int prev_idx = (i == 1) ? N : i - 1;
        int next_idx = (i == N) ? 1 : i + 1;
        nodes[i]->prev = nodes[prev_idx];
        nodes[i]->next = nodes[next_idx];
    }
    
    Node* current = nodes[1];
    bool clockwise = true;
    int last_removed = 0;
    
    for (int step = 0; step < N; step++) {
        // Find Q-th node in current direction
        Node* to_remove = current;
        for (int i = 0; i < Q - 1; i++) {
            if (clockwise) {
                to_remove = to_remove->next;
            } else {
                to_remove = to_remove->prev;
            }
        }
        
        // Record the removed value
        last_removed = to_remove->val;
        
        // Remove to_remove from the list
        to_remove->prev->next = to_remove->next;
        to_remove->next->prev = to_remove->prev;
        
        // Move to next node clockwise from removed position
        current = to_remove->next;
        
        // Update direction based on current node's parity
        if (current->val % 2 == 1) {
            clockwise = true;
        } else {
            clockwise = false;
        }
        
        // Delete the removed node (optional, but good practice)
        delete to_remove;
    }
    
    cout << last_removed << endl;
    
    // Clean up remaining nodes
    for (int i = 1; i <= N; i++) {
        delete nodes[i];
    }
    
    return 0;
}
