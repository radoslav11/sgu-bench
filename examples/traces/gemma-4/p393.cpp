#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/**
 * Problem: Bergamot Problem
 * The objective is to partition N letters into K buttons such that no two words 
 * in a given vocabulary of M two-letter words share the same sequence of two buttons.
 * The number of buttons K is to be minimized.
 * 
 * Constraints:
 * N <= 13, M <= 50
 * 
 * Approach:
 * Since N is small (up to 13), we can use backtracking to explore all possible 
 * partitions of the N letters into K buttons. The number of ways to partition 
 * a set of 13 elements is given by the 13th Bell number, B_13 = 27,644,437.
 * For each partition, we check if it's valid by ensuring all button sequences 
 * (button_of_first_letter, button_of_last_letter) for the M words are distinct.
 * To prune the search, we check for violations as soon as both letters of a word 
 * are assigned to buttons.
 */

struct Word {
    int u, v;
};

int N, M;
vector<Word> words;
vector<int> words_ending_at[13];
int assignment[13];
int best_assignment[13];
bool used_pairs[13][13];
int min_K = 14;

struct AddedPair {
    int b1, b2;
};
AddedPair added_pairs[50];

/**
 * backtrack explores all possible assignments of letters to buttons.
 * letter_idx: index of the current letter being assigned (from 0 to N-1).
 * num_buttons: number of buttons currently being used in the assignment.
 */
void backtrack(int letter_idx, int num_buttons) {
    // Pruning: if we've already found a valid placement with fewer buttons, stop.
    if (num_buttons >= min_K) return;

    // Base case: all N letters have been assigned.
    if (letter_idx == N) {
        min_K = num_buttons;
        for (int i = 0; i < N; ++i) best_assignment[i] = assignment[i];
        return;
    }

    // Try assigning the current letter to an existing button or a new one.
    // b is the index of the button to assign current letter to.
    for (int b = 0; b <= num_buttons; ++b) {
        // If we try to add a new button, check if we exceed the number of letters.
        if (b == num_buttons && num_buttons == N) continue;

        assignment[letter_idx] = b;
        int next_num_buttons = (b == num_buttons) ? num_buttons + 1 : num_buttons;
        
        // Pruning: if adding this button makes the count >= our current minimum, skip.
        if (next_num_buttons >= min_K) continue;

        bool ok = true;
        int added_count = 0;
        // Check all words that consist of letters already assigned (up to current letter_idx).
        for (int word_idx : words_ending_at[letter_idx]) {
            int b1 = assignment[words[word_idx].u];
            int b2 = assignment[words[word_idx].v];
            if (used_pairs[b1][b2]) {
                ok = false;
                break;
            }
            used_pairs[b1][b2] = true;
            added_pairs[added_count++] = {b1, b2};
        }

        if (ok) {
            backtrack(letter_idx + 1, next_num_buttons);
        }

        // Backtrack: reset the used_pairs state for the parent call.
        for (int i = 0; i < added_count; ++i) {
            used_pairs[added_pairs[i].b1][added_pairs[i].b2] = false;
        }
    }
}

int main() {
    // Speed up standard input/output.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> M)) return 0;

    for (int i = 0; i < M; ++i) {
        string s;
        cin >> s;
        int u = s[0] - 'a';
        int v = s[1] - 'a';
        words.push_back({u, v});
        // Store the word in the list for the letter that is assigned last.
        words_ending_at[max(u, v)].push_back(i);
    }

    // Initialize used_pairs matrix.
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 13; ++j) {
            used_pairs[i][j] = false;
        }
    }

    // Start the backtracking process.
    backtrack(0, 0);

    // Output the minimal number of buttons K.
    cout << min_K << "\n";

    // Group letters by their assigned button to output the placement.
    vector<vector<char>> buttons(min_K);
    for (int i = 0; i < N; ++i) {
        buttons[best_assignment[i]].push_back(i + 'a');
    }

    // Output each button's assigned letters.
    for (int i = 0; i < min_K; ++i) {
        for (size_t j = 0; j < buttons[i].size(); ++j) {
            cout << buttons[i][j];
        }
        cout << "\n";
    }

    return 0;
}

