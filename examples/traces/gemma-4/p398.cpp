#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem: Friends of Friends
 * The task is to find users who are "friends of friends" for a given user x.
 * A user c is a friend of a friend of x if:
 * 1. c is not x.
 * 2. c is not a direct friend of x.
 * 3. There exists some user b such that b is a friend of x and c is a friend of b.
 * 
 * Constraints:
 * N <= 50, x <= N.
 * Time limit: 0.25s (very generous for N=50 in C++).
 * Memory limit: 65536 KB.
 */

int main() {
    // Faster I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, x;
    // Read N (total users) and x (the user to process)
    if (!(cin >> N >> x)) return 0;

    // Use an adjacency matrix to store the friendship relation
    // Since N is small (up to 50), an adjacency matrix is very efficient.
    bool adj[55][55];
    for (int i = 0; i < 55; ++i) {
        for (int j = 0; j < 55; ++j) {
            adj[i][j] = false;
        }
    }

    // Read the friendship list for each user
    for (int i = 1; i <= N; ++i) {
        int d;
        if (!(cin >> d)) break;
        for (int j = 0; j < d; ++j) {
            int friend_id;
            cin >> friend_id;
            // Friendship is symmetric
            adj[i][friend_id] = true;
            adj[friend_id][i] = true;
        }
    }

    // Find the friends of friends of user x
    vector<int> fof;
    for (int c = 1; c <= N; ++c) {
        // Condition 1: c must not be user x
        // Condition 2: c must not be a direct friend of x
        if (c == x || adj[x][c]) continue;

        bool is_fof = false;
        // Condition 3: there exists a b such that b is a friend of x and c is a friend of b
        for (int b = 1; b <= N; ++b) {
            if (adj[x][b] && adj[b][c]) {
                is_fof = true;
                break;
            }
        }

        if (is_fof) {
            fof.push_back(c);
        }
    }

    // Output the result
    // The problem requires the list to be in increasing order.
    sort(fof.begin(), fof.end());

    // Print the count of friends of friends
    cout << fof.size() << "\n";
    // Print the friends of friends separated by space
    for (int i = 0; i < (int)fof.size(); ++i) {
        if (i > 0) cout << " ";
        cout << fof[i];
    }
    // Add a trailing newline if there was any output in the second line
    if (!fof.empty()) {
        cout << "\n";
    }

    return 0;
}

