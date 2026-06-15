#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/**
 * Performs addition of two large numbers represented as strings.
 * @param a The first large number as a string.
 * @param b The second large number as a string.
 * @return The sum of a and b as a string.
 */
string add(const string& a, const string& b) {
    string res = "";
    int i = (int)a.size() - 1, j = (int)b.size() - 1, carry = 0;
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry + (i >= 0 ? a[i--] - '0' : 0) + (j >= 0 ? b[j--] - '0' : 0);
        res.push_back(sum % 10 + '0');
        carry = sum / 10;
    }
    reverse(res.begin(), res.end());
    return res;
}

/**
 * Compares two large numbers represented as strings.
 * Returns true if a is strictly greater than b.
 * @param a The first large number as a string.
 * @param b The second large number as a string.
 * @return True if a > b, false otherwise.
 */
bool isGreater(const string& a, const string& b) {
    if (a.size() != b.size()) return a.size() > b.size();
    return a > b;
}

/**
 * Utility function to remove leading zeros from a string representation of a number.
 * @param s The input string.
 * @return A string with leading zeros removed.
 */
string clean(string s) {
    size_t first = s.find_first_not_of('0');
    if (string::npos == first) {
        return "0";
    }
    return s.substr(first);
}

int main() {
    // Speed up I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;
    vector<string> s(N);
    for (int i = 0; i < N; ++i) {
        string input;
        cin >> input;
        s[i] = clean(input);
    }

    // Sort segments by their numerical value.
    // Since they are represented as strings, first sort by length,
    // then lexicographically for strings of the same length.
    sort(s.begin(), s.end(), [](const string& a, const string& b) {
        if (a.size() != b.size()) return a.size() < b.size();
        return a < b;
    });

    // A triangle can be formed by three segments if and only if 
    // the sum of the two smaller segments is strictly greater than the largest segment.
    // When sorted, we only need to check consecutive triplets to find if any triangle exists.
    // This is based on the property that if any triplet (s[i], s[j], s[k]) with i < j < k 
    // satisfies s[i] + s[j] > s[k], then the triplet (s[k-2], s[k-1], s[k]) 
    // will also satisfy s[k-2] + s[k-1] >= s[i] + s[j] > s[k].
    for (int i = 0; i <= N - 3; ++i) {
        // Check the condition: s[i] + s[i+1] > s[i+2]
        if (isGreater(add(s[i], s[i+1]), s[i+2])) {
            cout << s[i] << " " << s[i+1] << " " << s[i+2] << endl;
            return 0;
        }
    }

    // If no such triplet is found, print "0 0 0" as specified.
    cout << "0 0 0" << endl;
    return 0;
}

