#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

/**
 * Problem Analysis:
 * - There are n objects, each with up to 4 properties of the form "key=value".
 * - There are m queries, each with up to 4 properties of the form "key=value".
 * - For each query, find the number of objects containing all specified properties.
 * - Key and value lengths are up to 5.
 * - Total properties per object is small (k <= 4), allowing for the precomputation 
 *   of all non-empty subsets of properties for each object.
 * - Each subset can be represented uniquely as a sorted, concatenated string.
 * 
 * Complexity:
 * - Precomputing subsets: O(n * 2^k * k * L) where L is the avg property length.
 * - Answering queries: O(m * l * L) where l is the query property count.
 * - Total complexity: Roughly 7.5 * 10^5 insertions and 10^5 lookups in an unordered_map.
 * - Time limit: 3 seconds, which is sufficient for this complexity in C++.
 * - Memory: 256 MB, which is enough for ~7.5 * 10^5 string keys and counts.
 */

using namespace std;

int main() {
    // Fast I/O for competitive programming.
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    // Use an unordered_map to store the counts of each unique subset of properties.
    unordered_map<string, int> counts;
    // Pre-reserve space for the unordered_map to minimize rehashing.
    // n * (2^4 - 1) = 50,000 * 15 = 750,000.
    counts.reserve(750000);

    for (int i = 0; i < n; ++i) {
        int k;
        cin >> k;
        vector<string> props(k);
        for (int j = 0; j < k; ++j) {
            cin >> props[j];
        }
        // Sort properties alphabetically to ensure the subset's string representation is unique.
        sort(props.begin(), props.end());

        // Generate all possible 2^k - 1 non-empty subsets of the object's properties.
        for (int mask = 1; mask < (1 << k); ++mask) {
            string subset = "";
            for (int j = 0; j < k; ++j) {
                // If the j-th property is included in the current subset.
                if ((mask >> j) & 1) {
                    if (!subset.empty()) {
                        // Using a non-alphanumeric character as a separator to distinguish properties.
                        subset += ";";
                    }
                    subset += props[j];
                }
            }
            counts[subset]++;
        }
    }

    int m;
    if (!(cin >> m)) return 0;
    while (m--) {
        int l;
        cin >> l;
        vector<string> q_props(l);
        for (int j = 0; j < l; ++j) {
            cin >> q_props[j];
        }
        // Sort query properties alphabetically to match the format of precomputed subsets.
        sort(q_props.begin(), q_props.end());
        
        string query = "";
        for (int j = 0; j < l; ++j) {
            if (!query.empty()) {
                query += ";";
            }
            query += q_props[j];
        }

        // Find the precomputed count for this property subset.
        auto it = counts.find(query);
        if (it != counts.end()) {
            cout << it->second << "\n";
        } else {
            cout << "0\n";
        }
    }

    return 0;
}

