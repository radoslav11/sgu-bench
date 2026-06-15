#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int N;
    std::cin >> N;

    std::vector<long long> freq(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> freq[i];
    }

    // For Huffman coding, we need to build the tree and sum up
    // (frequency * depth) for each leaf.
    // Instead of building the tree explicitly, we can use a priority queue
    // and track the contribution to the total bit count.

    // However, note: the problem says characters are given from most rare to most frequent (i.e., non-decreasing order).

    // We'll use a min-heap (priority queue) for the frequencies.
    std::priority_queue<long long, std::vector<long long>, std::greater<long long>> pq;

    // Initialize the priority queue with all frequencies
    for (long long f : freq) {
        pq.push(f);
    }

    // We'll use a queue for merged nodes (this is the "two-queue" optimization for Huffman)
    std::queue<long long> merged;

    long long total_bits = 0;

    // We need N-1 merges to build the tree. But we want to compute the total encoded length,
    // which is the sum over all leaves of (frequency * depth).
    // Alternative approach: during merging, when we combine two nodes with weights a and b,
    // the new node has weight a+b, and this a+b will be added again in future merges
    // exactly (depth contribution) times. Actually, the total encoded length equals
    // the sum of all internal node weights (excluding the root? no, including all merges).
    //
    // Specifically, in Huffman coding, the total encoded length = sum of all internal nodes'
    // weights. Why? Each time two nodes are merged, their combined frequency is added to the
    // total cost, and this happens for each level above the leaf. So the total bit count is
    // the sum of all the internal node weights (which is the same as the sum of all merges).

    // Example: frequencies [1,1,4]
    // Step 1: merge 1 and 1 -> cost = 2, new node = 2
    // Step 2: merge 2 and 4 -> cost = 6, new node = 6
    // Total = 2 + 6 = 8.

    // So we can just compute the sum of all merge costs.

    while (pq.size() + merged.size() > 1) {
        long long first = -1, second = -1;

        // Get the smallest two elements from the two queues
        auto pick = [&]() -> long long {
            if (pq.empty()) {
                long long val = merged.front(); merged.pop();
                return val;
            } else if (merged.empty()) {
                long long val = pq.top(); pq.pop();
                return val;
            } else {
                if (pq.top() <= merged.front()) {
                    long long val = pq.top(); pq.pop();
                    return val;
                } else {
                    long long val = merged.front(); merged.pop();
                    return val;
                }
            }
        };

        first = pick();
        second = pick();

        long long combined = first + second;
        total_bits += combined;
        merged.push(combined);
    }

    std::cout << total_bits << '\n';

    return 0;
}
