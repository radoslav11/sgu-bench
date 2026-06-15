#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>

using namespace std;

/**
 * Problem Summary:
 * Given preference lists for N men and N women, find a perfect assignment
 * such that no pair is "unstable". An unstable pair is a man and woman 
 * who are not matched but prefer each other to their current partners.
 * This is the definition of a Stable Marriage Problem.
 * The Gale-Shapley algorithm can solve this in O(N^2) time.
 * 
 * N is up to 800. The total number of strings in the input is about 2*N^2,
 * which is approximately 1.28 million. With N=800, O(N^2) is well within 1 second.
 * Memory limit is 64 MB, so we must be careful with string storage.
 */

int main() {
    // Speed up standard I/O
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    // Step 1: Read men's preference lists and store names.
    // We store names in a vector of vectors to manage memory better.
    vector<string> man_names(N);
    vector<vector<string>> man_prefs_names(N, vector<string>(N));
    for (int i = 0; i < N; ++i) {
        cin >> man_names[i];
        for (int j = 0; j < N; ++j) {
            cin >> man_prefs_names[i][j];
        }
    }

    // Step 2: Read women's preference lists and store names.
    vector<string> woman_names(N);
    vector<vector<string>> woman_prefs_names(N, vector<string>(N));
    for (int i = 0; i < N; ++i) {
        cin >> woman_names[i];
        for (int j = 0; j < N; ++j) {
            cin >> woman_prefs_names[i][j];
        }
    }

    // Step 3: Map names to integer IDs (0 to N-1).
    map<string, int> man_to_id, woman_to_id;
    for (int i = 0; i < N; ++i) man_to_id[man_names[i]] = i;
    for (int i = 0; i < N; ++i) woman_to_id[woman_names[i]] = i;

    // Step 4: Convert string preferences to integer preferences and ranks.
    // man_prefs[m][rank] = w_id
    vector<vector<int>> man_prefs(N, vector<int>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            man_prefs[i][j] = woman_to_id[man_prefs_names[i][j]];
        }
    }
    // Free memory from string preference lists as soon as they are converted to ints.
    man_prefs_names.clear();
    man_prefs_names.shrink_to_fit();

    // woman_ranks[w][m_id] = rank of man m_id in woman w's preference list.
    vector<vector<int>> woman_ranks(N, vector<int>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int m_id = man_to_id[woman_prefs_names[i][j]];
            woman_ranks[i][m_id] = j;
        }
    }
    woman_prefs_names.clear();
    woman_prefs_names.shrink_to_fit();

    // Step 5: Gale-Shapley algorithm for stable matching.
    vector<int> woman_matched_to(N, -1);
    vector<int> man_matched_to(N, -1);
    vector<int> next_proposal(N, 0);
    queue<int> free_men;
    for (int i = 0; i < N; ++i) {
        free_men.push(i);
    }

    while (!free_men.empty()) {
        int m = free_men.front();
        free_men.pop();

        // If man m has not proposed to every woman yet.
        if (next_proposal[m] < N) {
            int w = man_prefs[m][next_proposal[m]];
            next_proposal[m]++;

            if (woman_matched_to[w] == -1) {
                // Woman is free, matching (m, w)
                woman_matched_to[w] = m;
                man_matched_to[m] = w;
            } else {
                int m_prime = woman_matched_to[w];
                // Check if woman w prefers the new man m over current partner m_prime.
                if (woman_ranks[w][m] < woman_ranks[w][m_prime]) {
                    // Woman w accepts m, and m_prime becomes free.
                    woman_matched_to[w] = m;
                    man_matched_to[m] = w;
                    man_matched_to[m_prime] = -1;
                    free_men.push(m_prime);
                } else {
                    // Woman w rejects m, m stays free to propose to his next choice.
                    free_men.push(m);
                }
            }
        }
    }

    // Step 6: Verify and Output.
    bool possible = true;
    for (int i = 0; i < N; ++i) {
        if (man_matched_to[i] == -1) {
            possible = false;
            break;
        }
    }

    if (possible) {
        cout << "YES" << "\n";
        for (int i = 0; i < N; ++i) {
            cout << man_names[i] << " " << woman_names[man_matched_to[i]] << "\n";
        }
    } else {
        cout << "NO" << "\n";
    }

    return 0;
}

