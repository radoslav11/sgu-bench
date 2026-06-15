#include <iostream>
#include <algorithm>

using namespace std;

/**
 * Problem Summary:
 * Petya and Vasya need to write at least n solutions.
 * Petya takes t1 time per solution, Vasya takes t2 time per solution.
 * Both start their first solution at time 0.
 * When a person finishes a solution at time t, they check how many solutions
 * (from both Petya and Vasya) have been completed at or before time t.
 * If the total number of ready solutions is strictly less than n, they
 * start writing their next solution. Once they start, they must finish it.
 *
 * Goal:
 * Find the total number of solutions completed (m) and the time when the
 * last solution was finished (f).
 *
 * Approach:
 * We can model the number of solutions Petya and Vasya will complete.
 * Let p be the number of solutions Petya completes.
 * Petya finishes his k-th solution at time k * t1.
 * After completing the k-th solution, he starts the (k+1)-th solution if:
 * (number of Petya's finished solutions) + (number of Vasya's finished solutions) < n
 * (k) + floor((k * t1) / t2) < n
 *
 * Similarly for Vasya:
 * v is the number of solutions Vasya completes.
 * Vasya finishes his k-th solution at time k * t2.
 * He starts the (k+1)-th if (k) + floor((k * t2) / t1) < n.
 *
 * Since they both start their first solution at t=0, p and v must be at least 1.
 * 
 * Complexity:
 * Time: O(n), which is well within the 1-second limit for n = 5000.
 * Space: O(1).
 */

int main() {
    // Optimizing I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long n, t1, t2;
    if (!(cin >> n >> t1 >> t2)) return 0;

    // Calculate the number of solutions Petya will complete.
    // Petya always finishes at least one solution because they both start at time 0.
    long long p = 1;
    while (p + (p * t1) / t2 < n) {
        p++;
    }

    // Calculate the number of solutions Vasya will complete.
    // Vasya always finishes at least one solution because they both start at time 0.
    long long v = 1;
    while (v + (v * t2) / t1 < n) {
        v++;
    }

    // Total solutions m = p + v.
    // The time of the latest solution is the maximum of Petya's last finishing time and Vasya's.
    long long m = p + v;
    long long f = max(p * t1, v * t2);

    cout << m << " " << f << endl;

    return 0;
}

