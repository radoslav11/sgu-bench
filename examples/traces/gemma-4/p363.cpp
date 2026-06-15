#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

/**
 * Problem Analysis:
 * - There are M baggage room windows (queues).
 * - N people arrive at specified times k_i and require t_i time to be serviced.
 * - A person joins the queue with the minimum number of people.
 * - If there's a tie in the number of people, the queue with the smallest index is chosen.
 * - A person's servicing time starts once all people before them in that queue have finished.
 * - If a person arrives at the same time someone else's servicing ends, they wait for
 *   the preceding people to leave the queue before making their own queue choice.
 * - A person is considered part of the queue from their arrival time until their
 *   servicing time is complete.
 *
 * Simulation Strategy:
 * - Use a deque for each queue to store the finish times of the people currently in it.
 * - For each arriving person at time k_i:
 *   1. Iterate through all queues and remove any person whose finish time f is <= k_i.
 *   2. Identify the queue with the minimum number of people currently in it.
 *   3. If multiple queues have the same minimum size, choose the one with the smallest index.
 *   4. Calculate the person's finish time:
 *      - If the chosen queue is empty, finish_time = k_i + t_i.
 *      - If the chosen queue is not empty, finish_time = (last person's finish time in that queue) + t_i.
 *   5. Add the person's finish time to the chosen queue.
 *   6. Output the queue index (1-based) and the finish time.
 *
 * Time Complexity: O(N * M), which is at most 10,000 operations.
 * Space Complexity: O(N + M).
 */

int main() {
    // Faster input/output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    // queues[j] stores the finish times of people currently in queue j.
    // Since we add people such that finish times are strictly increasing,
    // the smallest finish time will always be at the front.
    vector<deque<int>> queues(M);

    for (int i = 0; i < N; ++i) {
        int k, t;
        if (!(cin >> k >> t)) break;

        // Step 1: Process all people who have finished by the time the new person arrives.
        // The problem states that if someone has just finished, the newcomer 
        // waits until they leave before choosing their queue.
        for (int j = 0; j < M; ++j) {
            while (!queues[j].empty() && queues[j].front() <= k) {
                queues[j].pop_front();
            }
        }

        // Step 2 & 3: Find the queue with the minimum current number of people.
        int min_size = N + 1;
        int chosen_queue_idx = -1;
        for (int j = 0; j < M; ++j) {
            int current_size = static_cast<int>(queues[j].size());
            if (current_size < min_size) {
                min_size = current_size;
                chosen_queue_idx = j;
            }
        }

        // Step 4: Calculate when the person will finish.
        int finish_time;
        if (queues[chosen_queue_idx].empty()) {
            // Queue is empty, person starts servicing immediately at arrival time.
            finish_time = k + t;
        } else {
            // Queue is not empty. Since we've popped all finish times <= k, 
            // the last person's finish time must be > k.
            finish_time = queues[chosen_queue_idx].back() + t;
        }

        // Step 5: Update the chosen queue.
        queues[chosen_queue_idx].push_back(finish_time);

        // Step 6: Output the result for the ith person.
        cout << (chosen_queue_idx + 1) << " " << finish_time << "\n";
    }

    return 0;
}

