#include <cstdio>
#include <algorithm>
#include <random>

using namespace std;

/**
 * Problem: 187. Twist and whirl - want to cheat
 * Task: Given N thimbles (initially 1 to N), perform M range reversal operations.
 * Constraints: N <= 130,000, M <= 2,000. Time limit: 0.25 sec. Memory limit: 4096 KB.
 * 
 * Approach: Use a Treap with lazy propagation to handle range reversals efficiently.
 * A Treap can perform split, merge, and range reversal operations in O(log N) time.
 * The total time complexity will be O((N + M) log N), which is well within the tight 0.25s limit.
 * Memory complexity is O(N), which fits within the 4 MB limit.
 */

const int MAXN = 130005;

struct Node {
    int val, priority, size, left, right;
    bool rev;
} tree[MAXN];

mt19937 rng(1337);

// Update the size of the subtree rooted at t
inline void update_size(int t) {
    if (t) {
        tree[t].size = 1 + tree[tree[t].left].size + tree[tree[t].right].size;
    }
}

// Propagate the lazy reversal flag to children
inline void push(int t) {
    if (t && tree[t].rev) {
        tree[t].rev = false;
        swap(tree[t].left, tree[t].right);
        if (tree[t].left) tree[tree[t].left].rev ^= 1;
        if (tree[t].right) tree[tree[t].right].rev ^= 1;
    }
}

// Merge two Treaps l and r
int merge(int l, int r) {
    if (!l || !r) return l | r;
    if (tree[l].priority > tree[r].priority) {
        push(l);
        tree[l].right = merge(tree[l].right, r);
        update_size(l);
        return l;
    } else {
        push(r);
        tree[r].left = merge(l, tree[r].left);
        update_size(r);
        return r;
    }
}

// Split Treap t into two Treaps l and r such that l contains the first k nodes
void split(int t, int k, int &l, int &r) {
    if (!t) {
        l = r = 0;
        return;
    }
    push(t);
    int left_sz = tree[tree[t].left].size;
    if (left_sz < k) {
        l = t;
        split(tree[t].right, k - left_sz - 1, tree[t].right, r);
        update_size(l);
    } else {
        r = t;
        split(tree[t].left, k, l, tree[t].left);
        update_size(r);
    }
}

// Perform in-order traversal to print the final sequence
void print_treap(int t) {
    if (!t) return;
    push(t);
    print_treap(tree[t].left);
    printf("%d ", tree[t].val);
    print_treap(tree[t].right);
}

int main() {
    int N, M;
    if (scanf("%d %d", &N, &M) != 2) return 0;

    int root = 0;
    // Initialize the Treap with values 1 to N
    for (int i = 1; i <= N; ++i) {
        tree[i].val = i;
        tree[i].priority = (int)rng();
        tree[i].size = 1;
        tree[i].left = tree[i].right = 0;
        tree[i].rev = false;
        root = merge(root, i);
    }

    // Perform M range reversal operations
    for (int i = 0; i < M; ++i) {
        int p, q;
        if (scanf("%d %d", &p, &q) != 2) break;
        int t1, t2, t3;
        // Split the treap into three parts: [1, p-1], [p, q], and [q+1, N]
        split(root, q, t1, t3);
        split(t1, p - 1, t1, t2);
        // Apply the lazy reversal flag to the segment [p, q]
        if (t2) tree[t2].rev ^= 1;
        // Merge parts back together
        root = merge(merge(t1, t2), t3);
    }

    // Output the result
    print_treap(root);
    printf("\n");

    return 0;
}

