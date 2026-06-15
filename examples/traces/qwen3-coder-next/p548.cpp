#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <stack>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // We'll store the type and value for cells 2..n (index 1..n-1 in 0-indexed arrays)
    // Also track which cells are dragons/princesses and their values
    vector<int> dragon_g(n, 0); // dragon_g[i] = gold at cell i+1 (if dragon), else 0
    vector<int> princess_b(n, 0); // princess_b[i] = beauty at cell i+1 (if princess), else 0
    vector<bool> is_dragon(n, false);
    vector<bool> is_princess(n, false);

    for (int i = 1; i < n; ++i) {
        char type;
        int val;
        cin >> type >> val;
        if (type == 'd') {
            is_dragon[i] = true;
            dragon_g[i] = val;
        } else { // 'p'
            is_princess[i] = true;
            princess_b[i] = val;
        }
    }

    // The knight starts at cell 0 (index 0) and must reach cell n-1 (index n-1).
    // When stepping on a princess at cell i (i >= 1), if dragons_killed >= princess_b[i], he must marry.

    // We want to maximize gold while ensuring that we don't get forced into marrying before reaching the last cell.
    // The last cell is a princess (guaranteed), so we must reach her with at least her beauty requirement.

    // Observation:
    // - We must kill enough dragons so that the total number of dragons killed is >= princess_b[n-1] (the last cell).
    // - But killing extra dragons may trigger earlier princesses before we reach the end.

    // Strategy: greedy with reverse thinking or simulation with priority queue.

    // Alternate approach:
    // - We can only be forced to marry at a princess cell i if up to and including that cell, we have killed >= b_i dragons.
    // - To maximize gold, we want to kill dragons with large gold values, but we must be careful not to trigger a princess too early.

    // Idea: simulate from left to right, keeping track of the minimal number of dragons we *must* kill to satisfy the final requirement,
    // and try to kill as many high-value dragons as possible while not triggering any princess before the last cell.

    // Let's define:
    // - Required: at the last princess (cell n-1, 0-indexed), we need kills >= b_{n-1}.
    // - For any princess at position i (0-indexed i, i < n-1), if we have killed >= b_i dragons up to position i, we get stuck.

    // So we must ensure that for all princesses except the last one, the cumulative dragons killed before reaching her is < b_i.
    // At the last princess, we must have >= b_i.

    // Let kills[i] be the number of dragons killed from cell 0 to cell i (exclusive? inclusive? let's define properly).

    // Actually, when we step on a cell:
    // - If it's a dragon: we can choose to kill it (adds to kill count and gold) or skip.
    // - If it's a princess: immediately after stepping on, we check: if kills >= b_i, marriage happens.

    // So kills count is checked *after* we step on the cell (but before moving on).

    // Therefore, for princess at position i (0-indexed), if we have killed k dragons in cells 0..i (i.e., before or at i),
    // and k >= b_i, then we marry at i.

    // Since we must reach the last cell (n-1), we need to avoid triggering any princess from 1 to n-2 (0-indexed indices 1 to n-2).
    // At the last princess (index n-1), we need kills >= b_{n-1}.

    // Let total_kill be the number of dragons we kill.
    // Constraints:
    // - For every princess i (0-indexed i, 1 <= i <= n-2), the number of dragons killed in cells 0..i must be < b_i.
    // - For the last princess (i = n-1), the number of dragons killed in cells 0..n-1 must be >= b_{n-1}.

    // Note: Cell 0 (index 0) is the starting cell. The problem says: "the map of n cells", knight initially at cell 1 (index 0).
    // The input describes cells from 2 to n (so indices 1 to n-1). So cell 1 (index 0) has no description — it's just the start.
    // There is nothing in cell 1, so we don't have to worry about it.

    // So:
    // - Cells with indices 0: start (no monster)
    // - Cells 1 to n-1: described in input.

    // Therefore, when we are at cell i (0-indexed, i from 0 to n-1), we have processed cells 0..i.
    // The knight moves step by step: starts at 0, then moves to 1, then 2, ..., until n-1.

    // When he steps on cell i (i>=1), if it's a dragon or princess, the event happens.

    // So the kill count is the number of dragons killed in cells 1..i (since cell 0 has nothing).

    // Let kill_count[i] = number of dragons killed in cells 1..i.

    // Constraints:
    // - For each princess at cell i (i from 1 to n-2): kill_count[i] < b_i
    // - For the last princess at cell n-1: kill_count[n-1] >= b_{n-1}

    // We want to maximize sum of gold from killed dragons.

    // We can think greedily: to satisfy the final requirement, we must kill at least b_{n-1} dragons in total.
    // But we also must not exceed b_i - 1 dragons before cell i (for each princess i < n-1).

    // So the constraints on cumulative kills at each princess position give upper bounds.

    // Let's define:
    //   L = b_{n-1} (minimum dragons needed at the end)
    //   For each princess i (i < n-1), we have: kills up to i <= b_i - 1.

    // So the total kills must be at least L, but also, the kills at any intermediate princess i must be < b_i.

    // However, note that between princesses, we can kill more dragons.

    // Let's collect all princess positions (except the last one) and sort them by position (which they are, since we traverse left to right).
    // For each princess i (i from 1 to n-2), let required_upper[i] = b_i - 1.

    // Then the total kills must satisfy:
    //   kills[princess_i] <= required_upper[i] for each i in 1..n-2
    //   kills[n-1] >= L

    // But kills[n-1] = kills[princess_k] + (dragons killed between princess_k and n-1) for the last princess.

    // Alternate approach: simulate and use a min-heap (or max-heap with negative) to make greedy choices.

    // Known technique: "minimum cost to satisfy constraints" but here we want maximum gold.

    // Insight: we want to kill dragons with high gold, but we must not accumulate too many kills too early.

    // We can use a greedy algorithm with a priority queue:

    // Idea from similar problems (e.g., "collect coins but don't exceed some thresholds"):
    //   - Traverse from left to right.
    //   - When we see a dragon, we consider killing it (push its gold into a max-heap or min-heap for possible later removal).
    //   - When we hit a princess (not last), if the current kill count is >= b_i, we have a problem. So we must have kept kill count < b_i.
    //     Therefore, if after killing some dragons we have kill count >= b_i, we need to "undo" some kills (i.e., skip some dragons we previously decided to kill)
    //     to bring the count down to b_i - 1, and we want to undo the ones with smallest gold (to maximize total gold).

    // However, in this problem, we can plan ahead: we know the constraints at each princess.

    // Proposed algorithm:
    //   Let's collect all events (princesses and dragons) and simulate.
    //   We want to ensure that at each princess i (i < n-1), the number of kills so far < b_i.
    //   We want to maximize total gold, so we want to kill as many high-value dragons as possible, but respecting the constraints.

    // Steps:
    //   - We know we must have at least L = b_{last_princess} kills at the end.
    //   - Also, at each princess i (i < n-1), we must have kills <= b_i - 1.
    //   So the minimal number of kills we must have at the end is max(L, ... ) but constrained by the intermediate thresholds.

    // Actually, the constraints might make it impossible: if at some princess i (i < n-1), even if we kill 0 dragons up to i, we are safe, but if L > (max possible kills without violating constraints), then impossible.

    // How to compute the maximum possible kills without violating constraints?
    //   Let prefix[i] = number of dragons from cell 1 to i (0-indexed: cells 1..i, i.e., indices 1 to i in our arrays).
    //   But we can choose to skip some.

    // Actually, we can reframe:
    //   Let required[i] = for the princess at cell i (i from 1 to n-2), the kill count at i must be <= b_i - 1.
    //   Let T = total kills at the end.
    //   Then T must be >= b_{n-1}.
    //   Also, for each princess i (i < n-1), the kills up to i <= b_i - 1.
    //   And the kills up to i is at most (total dragons from 1 to i) but we can skip some.

    // However, we want to maximize gold, so we want to kill the most valuable dragons that we are allowed to kill.

    // Known solution approach for this problem (it's a known CF problem):
    //   - We traverse from left to right, and whenever we meet a dragon, we add it to a list.
    //   - When we meet a princess (not last), if the current number of dragons we've collected (killed) is >= b_i, then we have to remove some dragons (the least valuable ones) until the count is b_i - 1.
    //   - But note: we are not forced to kill a dragon when we see it; we can choose to skip. However, in a greedy approach, we can assume we kill all, and then if we exceed a constraint, we remove the least valuable ones.

    // Algorithm:
    //   Let's maintain a min-heap (priority_queue<int, vector<int>, greater<int>>) of the gold values of dragons we have "decided" to kill so far.
    //   Also maintain the total gold and count.
    //   Additionally, we need to remember which dragons (cell numbers) we have chosen.

    //   But we also have to ensure that at the end, we have at least L kills. So after processing all cells up to n-1, if the count is < L, then it's impossible.

    //   However, during the simulation, when we hit a princess i (i < n-1), if count >= b_i, we remove the least valuable dragons (from our heap) until count < b_i.
    //   Why? Because we want to maximize gold, so removing the least valuable ones minimizes gold loss.

    //   Steps:
    //     1. Initialize:
    //          total_gold = 0
    //          count = 0
    //          min_heap (for gold values of selected dragons)
    //          selected_cells: vector to record which cells we selected (but we'll store the cell index and gold value)
    //     2. Iterate i from 1 to n-1 (0-indexed cells 1 to n-1):
    //          - If cell i is a dragon:
    //               push dragon_g[i] into heap, add to total_gold, count++.
    //               Also, record that we selected cell i+1 (since cell index in problem is 1-indexed: cell i in 0-indexed is cell i+1) — but note: our input for cell i (0-indexed) corresponds to cell number i+1.
    //          - If cell i is a princess:
    //               While count >= b_i (princess_b[i]):
    //                   if heap is empty -> impossible (but b_i>=1, so count>=1 and heap not empty if count>0)
    //                   pop the smallest gold from heap, subtract from total_gold, count--.
    //                   Also, we remove that cell from our selected list.
    //               Note: after this, count < b_i, so we are safe for this princess.

    //     3. After processing all cells (i = n-1), the last cell is a princess. But we don't process it the same way — because we are allowed to have count >= b_{last}.
    //        However, in the above loop, we processed up to i = n-1 (which is the last cell). But the last cell is a princess, so in step 2, when i = n-1, we would do the princess check: while count >= b_{n-1} ... but wait, we want count >= b_{n-1} at the end.

    //   Correction: we should not process the last princess as a constraint that forces removal. Instead, we only process princesses at indices 1 to n-2 (i.e., the ones before the last) in the above loop.

    //   So modify: 
    //        for i from 1 to n-2 (0-indexed) for princess removal.
    //        Then after the loop, for the last cell (i = n-1), if it's a dragon, we add it normally, and if it's a princess (which it is), we don't do removal.

    //   But note: the last cell is guaranteed to be a princess.

    //   So:
    //        for i = 1 to n-2:
    //            if dragon: add to heap
    //            if princess: while count >= b_i: pop least valuable
    //        Then for i = n-1:
    //            if dragon: add to heap (and count, gold)
    //            if princess: do nothing (but we require count >= b_{n-1} at the end)

    //   After the loop, check: if count < b_{n-1} (the last princess's beauty), then it's impossible.

    //   However, note: the last cell is a princess, so we don't add it as a dragon. So the last cell is processed as a princess, but we don't do removal. But we require count >= b_{n-1} at the moment we step on it.

    //   So after processing up to i = n-2, we then process the last cell: but the last cell is a princess, so we don't add any dragon, but we check if count >= b_{n-1}. If not, impossible.

    //   But wait: we haven't processed the last cell in the loop. So after the loop (i from 1 to n-2), we have a state for the situation just before stepping on the last cell.
    //   Then we step on the last cell: if it's a princess, we check count. So we need count >= b_{n-1}.

    //   However, we might have dragons after the last princess? No, the last cell is the last princess. So there are no cells after.

    //   Therefore, after the main loop (i=1 to n-2), we must check: if count < b_{n-1}, then output -1.

    //   But note: what if there are dragons in the last cell? The problem says: "It is guaranteed that the last cell contains a princess." So last cell is princess.

    //   So steps:
    //        Read n.
    //        Store for i=1 to n-1: type and value.
    //        Let L = princess_b[n-1] (the last cell's beauty).
    //        Initialize:
    //            priority_queue<int, vector<int>, greater<int>> min_heap;
    //            vector<int> selected; // we'll store the cell indices (1-indexed) for dragons we select.
    //            But note: we need to know which dragon corresponds to which cell to output the cell numbers.

    //        Instead, we can store in the heap not just the gold, but also the cell index? Or we can store separately.

    //        Better: use a heap of pairs (gold, cell_index) and then when popping, we know which cell to remove.

    //        Alternatively, we can store the selected cells in a set or vector, and update it when we pop from heap.

    //        Let's do:
    //            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap; 
    //            // (gold, cell_index) — cell_index in 1-indexed: for cell i (0-indexed), the cell number is i+1.
    //            int total_gold = 0;
    //            int kill_count = 0;
    //            vector<int> selected_cells; // we'll build this at the end from the heap.

    //        But note: when we remove a dragon, we remove it from the heap and subtract its gold.

    //        However, we cannot easily remove from a heap. But we are only removing from the top (smallest) when needed, so we can do:

    //        For i from 1 to n-2:
    //            if dragon: 
    //                heap.push({dragon_g[i], i+1});
    //                total_gold += dragon_g[i];
    //                kill_count++;
    //            else: // princess
    //                while (kill_count >= princess_b[i]) {
    //                    if (heap.empty()) break; // shouldn't happen if kill_count>0
    //                    auto [g, idx] = heap.top(); heap.pop();
    //                    total_gold -= g;
    //                    kill_count--;
    //                }
    //        Then after the loop, check: if kill_count < princess_b[n-1] -> print -1.
    //        But wait: what about the last cell? We haven't processed it. The last cell is a princess, so we don't add any dragon, but we require kill_count (which is the count up to cell n-2) to be at least princess_b[n-1]? 
    //        Actually, no: the last cell is cell n-1 (0-indexed), and when we step on it, we have killed dragons from cells 1 to n-2 (and possibly the last cell is a princess, so no dragon at last cell). 
    //        So the kill_count we have after processing up to n-2 is the total kills we have when we step on the last princess.

    //        Therefore, we require kill_count >= princess_b[n-1] at the end of the loop (for i=1 to n-2).

    //        However, note: the last cell is the princess, and we don't have a dragon at the last cell, so the kill_count remains as after n-2.

    //        So after the loop (i=1 to n-2), we check: if kill_count < princess_b[n-1], then output -1.

    //        But what if there is a dragon at the last cell? The problem states: "It is guaranteed that the last cell contains a princess." So no.

    //        However, let me check the problem: "It is guaranteed that the last cell contains a princess." So we are safe.

    //        But note: the last cell is cell n, which is index n-1 in 0-indexed. And we only loop i from 1 to n-2 (0-indexed), so we skip the last cell.

    //        So after processing cells 1 to n-2, we have the kill_count for cells 1..n-2. Then we step on cell n-1 (the last princess) and we require kill_count >= b_{n-1}.

    //        Therefore, we don't do anything for the last cell in the loop.

    //        However, what if there are dragons between the last princess and ...? No, the last cell is the princess.

    //        So the algorithm:
    //            Step 1: Read n.
    //            Step 2: For i=1 to n-1 (0-indexed indices), read the description.
    //            Step 3: Store princess_b for the last cell: last_princess_b = princess_b[n-1] (but note: we read it, so we have it).
    //            Step 4: 
    //                min_heap for (gold, cell_index) for dragons we have selected so far (cell_index = i+1, 1-indexed).
    //                total_gold = 0, kill_count = 0.
    //            Step 5: For i from 1 to n-2 (0-indexed, so i=1,2,...,n-2):
    //                if is_dragon[i]:
    //                    push (dragon_g[i], i+1) to heap, total_gold += dragon_g[i], kill_count++.
    //                else: // princess
    //                    while (kill_count >= princess_b[i]):
    //                        if heap is empty -> break (but then it's impossible? actually, if kill_count>=princess_b[i] and heap empty, then kill_count=0 -> contradiction, so heap shouldn't be empty if kill_count>0)
    //                        pop the top (smallest gold), subtract, kill_count--.
    //            Step 6: After the loop, if kill_count < last_princess_b:
    //                    cout << -1 << endl;
    //            Else:
    //                    Now, we have the heap containing the dragons we decided to kill. 
    //                    The number of dragons = kill_count.
    //                    The total_gold is the sum.
    //                    But note: we might have killed more dragons than necessary, but we removed the least valuable ones when forced by princesses. And we want to maximize gold, so this is optimal.
    //                    However, we haven't considered: could we skip some expensive dragons to save for later to meet the final requirement? 
    //                    But our greedy removal only happens when forced by a princess constraint. And we remove the least valuable ones first, so we keep the most valuable ones. And at the end, if we have enough for the last princess, we are good.

    //            But wait: what if we have extra dragons after the last princess? There are none. So the above is correct.

    //        However, there's a catch: the above greedy does not consider that we might want to skip a dragon earlier to avoid triggering a princess, but then we could kill it later? But there is no "later" after the princess — once you pass a princess, you can't go back. And the dragons are in fixed order. So the only choice is: at each dragon, you can choose to kill or skip, and if you skip, you lose the chance.

    //        The greedy approach of "kill all, then when constrained, remove the least valuable ones" is standard for these "scheduling with deadlines" or "knapsack with constraints" and works because:
    //            - The constraints are: at princess i, the number of kills so far must be < b_i.
            //            - We want to maximize the total gold, so we prefer high gold dragons. Removing the least valuable ones when constrained minimizes the loss.

        //        This is similar to: https://codeforces.com/problemset/problem/1579/F (but not exactly) or classic "buy and sell stock with constraints" — actually, it's like: we have to have at most x items at some points, and we want to maximize value.

        //        Known problem: "Dragons and Princesses" is a known CF problem (e.g., Codeforces Round #Pi (Div. 2), problem C). The standard solution is the greedy with heap.

        //        Let's test with the example:
        //            n=6
        //            cells: 
        //                1: start (skip)
        //                2: d 10 -> i=1 (0-indexed), dragon, gold=10 -> heap: [10], total=10, count=1
        //                3: d 12 -> i=2, dragon, heap: [10,12], total=22, count=2
        //                4: p 2 -> i=3, princess, b=2. Count=2 >=2 -> remove smallest: 10 -> heap: [12], total=12, count=1 (which is <2 -> ok)
        //                5: d 1 -> i=4, dragon, heap: [1,12], total=13, count=2
        //                6: p 2 -> last princess, we don't process in loop (loop only to i=4, which is n-2=4? n=6, n-2=4 in 0-indexed? n-2=4 -> i from 1 to 4 (0-indexed indices 1,2,3,4) — yes, because n-2 = 4 (0-indexed index for the 5th cell? no: cells 2..6: indices 1..5. n-1=5, so n-2=4 (0-indexed index 4 is the 5th cell). Then last cell is index 5.
        //                So after loop (i=1 to 4): total_gold=13, count=2.
        //                last_princess_b = 2 (from input: the last line says "p 2", so b=2).
        //                count=2 >= 2 -> valid.
        //                Then output: 13, 2, and the cell numbers: which dragons? we have heap: [1,12] -> but wait, we removed the 10 at cell2, and kept 12 (cell3) and 1 (cell5). So cells: 3 and 5 -> output "3 5", which matches.

        //            Second example:
        //                n=6
        //                d 10, d 12, p 2, d 1, p 3
        //                Process:
        //                  i=1: d10 -> heap=[10], total=10, count=1
        //                  i=2: d12 -> heap=[10,12], total=22, count=2
        //                  i=3: p2 -> count=2>=2 -> remove 10: heap=[12], total=12, count=1
        //                  i=4: d1 -> heap=[1,12], total=13, count=2
        //                Then last princess: b=3 -> count=2 < 3 -> output -1.

        //        So it matches.

        //        Implementation details:
        //            - We need to store princess_b for all princesses (for indices 1 to n-2) and for the last one.
        //            - We'll create arrays: 
        //                  vector<int> g(n, 0); // for dragons: g[i] for cell i (0-indexed), but cell0 (start) is not used.
        //                  vector<int> b(n, 0); // for princesses: b[i] for cell i (0-indexed)
        //                  vector<bool> is_d(n, false);
        //                  vector<bool> is_p(n, false);
        //            - Then, last_princess_b = b[n-1] (since last cell is princess).

        //        Steps:
        //            Read n.
        //            For i from 1 to n-1 (0-indexed i=1,2,...,n-1):
        //                read char c and int val.
        //                if c=='d': 
        //                    is_d[i]=true; g[i]=val;
        //                else:
        //                    is_p[i]=true; b[i]=val;
        //
        //            Then:
        //                priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        //                long long total_gold = 0; // but gold up to 10^4 * 2e5 = 2e9, so int might be enough? but 2e9 is near limit for int (2e9). Use long long to be safe.
        //                int kill_count = 0;
        //
        //                for (int i = 1; i <= n-2; i++) { // 0-indexed i from 1 to n-2
        //                    if (is_d[i]) {
        //                        pq.push({g[i], i+1}); // cell number = i+1
        //                        total_gold += g[i];
        //                        kill_count++;
        //                    } else { // princess
        //                        // while kill_count >= b[i]
        //                        while (kill_count >= b[i] && !pq.empty()) {
        //                            auto [gold_val, cell] = pq.top(); pq.pop();
        //                            total_gold -= gold_val;
        //                            kill_count--;
        //                        }
        //                    }
        //                }
        //
        //                if (kill_count < b[n-1]) {
        //                    cout << -1 << endl;
        //                } else {
        //                    // Now, extract the cell numbers from the heap.
        //                    vector<int> cells;
        //                    while (!pq.empty()) {
        //                        cells.push_back(pq.top().second);
        //                        pq.pop();
        //                    }
        //                    // But note: the heap doesn't preserve order, but we need increasing order of cell numbers.
        //                    // However, we are pushing in increasing i, but the heap is by gold value. So we have to sort.
        //                    sort(cells.begin(), cells.end());
        //
        //                    cout << total_gold << "\n";
        //                    cout << cells.size() << "\n";
        //                    for (int i = 0; i < cells.size(); i++) {
        //                        if (i > 0) cout << " ";
        //                        cout << cells[i];
        //                    }
        //                    if (cells.size() > 0) cout << "\n";
        //                    // If no dragons, then the third line is empty? but the problem says: "k integers", so if k=0, then third line is empty? but we output the count and then the list (which is empty). 
        //                    // However, the problem: "The third line should contain k integers", so if k=0, we output a blank line? Actually, the problem says: "print a single integer k", then "k integers", so if k=0, we output the integer 0 and then a line with nothing? 
        //                    // But the example output: 
        //                    //   13
        //                    //   2
        //                    //   3 5
        //                    // So for k=0, we output:
        //                    //   [total]
        //                    //   0
        //                    //   [and then nothing? but the third line must be printed. So we output the 0 and then an empty line? Actually, the problem says: "the third line should contain k integers", so if k=0, we don't print any integers, but we still print a newline? Or no newline? 
        //                    // However, the problem output example has three lines. So we must output three lines.
        //                    // So if k=0, we output:
        //                    //   [total]
        //                    //   0
        //                    //   [and then a newline? but no numbers] -> so we output a newline for the third line.
        //                    // But in our code: if cells.size()==0, then we output the third line as empty (but we do cout << ... and then if cells.size()>0 we output the numbers, but we need to output a newline regardless? 
        //                    // Actually, we output the third line: 
        //                    //   for (int i=0; ...) ... then if cells.size()>0, we output the numbers without trailing space, and then we don't output a newline? 
        //                    // But we should output a newline after the third line.
        //                    // However, the problem says: "The third line should contain k integers", meaning if k>0, then the integers on that line, and then a newline? 
        //                    // Standard: each line is a line. So we output:
        //                    //   first line: total
        //                    //   second line: k
        //                    //   third line: the k integers (space separated) and then newline.
        //                    // So even if k=0, we output the third line (which is empty) and then newline? Actually, the problem doesn't specify, but typically we output a newline for each line.
        //                    // In our code: 
        //                    //   cout << total_gold << "\n";
        //                    //   cout << cells.size() << "\n";
        //                    //   then for the third line: we output the numbers (if any) and then we need to output a newline? 
        //                    //   But if we do: 
        //                    //        for ... { if (i>0) cout << " "; cout << cells[i]; }
        //                    //        cout << "\n";
        //                    //   then if k=0, we output nothing and then a newline -> an empty line.
        //                    // That is acceptable.
        //                }

        //        However, note: what if there are no dragons at all? Then the heap is empty, and if the last princess requires 0 kills? But b_i>=1, so if no dragons, kill_count=0, and last princess requires b>=1 -> 0 < 1 -> impossible -> output -1.

        //        But what if the last princess has b=0? The problem says: "1 <= bi", so minimum b is 1. So we don't have to worry.

        //        Let's code accordingly.

        //        Important: use long long for total_gold? The problem says gi up to 10^4, and up to 2e5 dragons, so maximum gold = 2e5 * 10^4 = 2e9, which fits in int? (2e9 is the limit for signed int: 2^31-1 = 2.1e9). But to be safe, use long long.

        //        However, the problem says: "print a single integer" for gold, and 2e9 is within int? Actually, in C++ int is at least 16-bit, but typically 32-bit (so up to 2e9). But the problem constraints: n up to 200,000, gi up to 10,000 -> max 2e9, which is 2,000,000,000 -> fits in 32-bit signed int (max 2,147,483,647). So we can use int? But 2e9 is 2,000,000,000 < 2,147,483,647 -> yes.

        //        But to be safe, use long long.

        //        Let's do:

        //        Steps:
        //            Read n.
        //            Pre-allocate vectors of size n (index 0 to n-1).
        //            For i from 1 to n-1 (0-indexed), read the line.

        //        Note: the first line of input after n is for cell2 (which is index1).

        //        Code:

        //        Edge: n=2 -> then we have only cell1 (start) and cell2 (princess). 
        //            Then the loop for i from 1 to n-2: n-2 = 0, so the loop runs from 1 to 0 -> no iteration.
        //            Then check: kill_count (0) >= b[1]? (b[1] is the beauty of the last princess, which is the only other cell)
        //            If b[1] > 0, then 0 < b[1] -> output -1.

        //        Example: n=2, input:
        //            2
        //            p 1
        //            Then: kill_count=0, last_princess_b=1 -> 0<1 -> -1.

        //        But the knight starts at cell1, then steps to cell2 (princess). At cell2, kills=0, and 0>=1? no -> so he marries? 
        //            The problem: "if that number is greater or equal to her beauty bi, the princess considers the Knight brave enough" — so if 0>=1? false -> so she doesn't ask? 
        //            But wait: the problem says: "If that number is greater or equal to her beauty bi, the princess considers the Knight brave enough and instantly asks him to marry her." 
        //            So if 0 < 1, then she doesn't ask? Then the knight continues? But there is no next cell. 
        //            And the knight must finish at the rightmost cell (which is cell2). So he is at cell2, but the princess didn't ask? Then what? 
        //            The problem says: "He should pass all the cells one by one and finish his way at the rightmost cell". 
        //            And "if the Knight steps to the cell with a princess, she wonders ... if >= b_i, she asks -> adventure ends". 
        //            But if not, does the adventure continue? But there is no next cell. 
        //            The problem states: "the Knight loves the princess who lives in the cell number n and wants to marry her". So he must marry her. 
        //            Therefore, the only way to marry her is to have kills >= b_n. 
        //            So if he doesn't have enough kills, he cannot marry her? Then the adventure fails? 
        //            The problem says: "If the Knight can't marry his beloved princess, just print -1 in the first line of the output."

        //        So for n=2, if the last princess has b_i=1, then he needs at least 1 kill, but there are no dragons -> impossible -> -1.

        //        Therefore, our condition is correct.

        //        Let's code accordingly.

        //        Note: the problem says the last cell is a princess, so we know b[n-1] is defined.

        //        Implementation:

        //        We'll create arrays:
        //            vector<int> dragon_g(n, 0);
        //            vector<int> princess_b(n, 0);
        //            vector<bool> is_d(n, false);
        //            vector<bool> is_p(n, false);

        //        Then for i from 1 to n-1 (0-indexed indices):
        //            char c;
        //            int val;
        //            cin >> c >> val;
        //            if (c == 'd') {
        //                is_d[i] = true;
        //                dragon_g[i] = val;
        //            } else {
        //                is_p[i] = true;
        //                princess_b[i] = val;
        //            }

        //        Then:
        //            int last_b = princess_b[n-1]; // because last cell (index n-1) is princess.

        //        Then simulate for i=1 to n-2.

        //        Important: the last cell (index n-1) is not processed in the loop.

        //        Let's code.

        //        One more note: the problem says the knight is initially at cell1 (index0). So when he moves to cell2 (index1), that's the first step.

        //        We are good.

        //        Code:

        //        Note: We must output -1 if impossible.

        //        Let's write the code accordingly.

        //        We'll use:
        //            #include <bits/stdc++.h> // or specific includes

        //        But the problem says C++17, and we are allowed standard libraries.

        //        We'll use vector, queue, iostream, algorithm.

        //        Note: the heap is a min-heap for the gold value.

        //        Let's code.

        //        Edge: if n==2, then the loop runs for i from 1 to n-2 = 0 -> no iteration.

        //        Example: n=2 -> last_b = princess_b[1] (since n-1=1, 0-indexed index1).
        //        Then we check: kill_count (0) < last_b -> output -1.

        //        Correct.

        //        Now, write the code.

        //        Important: the princess_b for the last cell is stored at index n-1.

        //        We assume the input is as described.

        //        Let's go.

        //        Note: the problem says the last cell is a princess, so we don't have to worry about is_p[n-1] being false.

        //        But to be safe, we can assert, but the problem guarantees it.

        //        Code:

        //        We'll use long long for total_gold.

        //        Also, note: the heap might be empty when we try to pop? But if kill_count>=b_i and b_i>=1, then kill_count>=1, so heap should not be empty. So the condition !pq.empty() is safe.

        //        However, we have the condition: while (kill_count >= b[i] && !pq.empty()) — so safe.

        //        Let's write.

        //        Note: the cell numbers: for index i (0-indexed), the cell number is i+1.

        //        We store in the heap: (gold, i+1).

        //        Then after the loop, we extract the cell numbers and sort.

        //        Output.

        //        One more corner: if there are multiple princesses with the same constraint, we remove as needed.

        //        Code:

        //        We'll do:

        //            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        //        But note: pair<int, int>: first element is gold (so min-heap by gold), second is cell index.

        //        Yes.

        //        Let's code accordingly.

        //        IMPORTANT: the problem says "The cell numbers should be printed in the increasing order", so we sort.

        //        Now, write the full program.

        //        Note: if it's impossible, we output "-1" and that's it (one line). The problem says: "just print -1 in the first line of the output", meaning one line.

        //        So for impossible: 
        //            cout << "-1" << endl;

        //        For possible: three lines.

        //        Code structure:

        //            int n;
        //            cin >> n;
        //            // vectors of size n
        //            vector<int> dragon_g(n, 0);
        //            vector<int> princess_b(n, 0);
        //            vector<bool> is_d(n, false);
        //            vector<bool> is_p(n, false);
        //
        //            for (int i = 1; i < n; i++) {
        //                char c;
        //                int val;
        //                cin >> c >> val;
        //                if (c == 'd') {
        //                    is_d[i] = true;
        //                    dragon_g[i] = val;
        //                } else {
        //                    is_p[i] = true;
        //                    princess_b[i] = val;
        //                }
        //            }
        //
        //            long long total_gold = 0;
        //            int kill_count = 0;
        //            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        //
        //            // Process cells 1 to n-2 (0-indexed indices)
        //            for (int i = 1; i <= n-2; i++) {
        //                if (is_d[i]) {
        //                    pq.push({dragon_g[i], i+1});
        //                    total_gold += dragon_g[i];
        //                    kill_count++;
        //                } else { // princess
        //                    // Remove least valuable dragons until kill_count < princess_b[i]
        //                    while (kill_count >= princess_b[i] && !pq.empty()) {
        //                        auto [gold_val, cell] = pq.top();
        //                        pq.pop();
        //                        total_gold -= gold_val;
        //                        kill_count--;
        //                    }
        //                }
        //            }
        //
        //            // Now check the last princess: index n-1
        //            if (kill_count < princess_b[n-1]) {
        //                cout << -1 << endl;
        //            } else {
        //                vector<int> cells;
        //                while (!pq.empty()) {
        //                    cells.push_back(pq.top().second);
        //                    pq.pop();
        //                }
        //                sort(cells.begin(), cells.end());
        //                cout << total_gold << "\n";
        //                cout << cells.size() << "\n";
        //                for (int i = 0; i < cells.size(); i++) {
        //                    if (i > 0) cout << " ";
        //                    cout << cells[i];
        //                }
        //                cout << "\n";
        //            }

        //        Let's test with the examples.

        //        Example 1: n=6
        //            dragon_g: [0,10,12,0,1,0] -> but wait, cell4 is princess, so is_d[3]=false, and dragon_g[3] is unused.
        //            princess_b: [0,0,0,2,0,2] -> but we set princess_b[3]=2 (for cell4, index3) and princess_b[5]=2 (for cell6, index5).
        //
        //            Loop i from 1 to 4 (0-indexed indices 1,2,3,4):
        //                i=1: is_d[1]=true -> push (10,2), total=10, kill=1.
        //                i=2: is_d[2]=true -> push (12,3), total=22, kill=2.
        //                i=3: is_p[3]=true -> princess_b[3]=2 -> while (2>=2): pop (10,2): total=12, kill=1.
        //                i=4: is_d[4]=true -> push (1,5), total=13, kill=2.
        //            Then last_b = princess_b[5] = 2 -> 2>=2 -> valid.
        //            Then cells: from pq: [12,1] -> cells = [3,5] -> sorted: [3,5] -> output 13, 2, "3 5".

        //        Example 2: n=6, last princess b=3.
        //            After loop: kill=2, last_b=3 -> 2<3 -> output -1.

        //        It matches.

        //        Now, write the code.

        //        Note: the vector indices: princess_b[n-1] is the last element (index n-1).

        //        Let's code accordingly.

        //        One more: n=3
        //            Input:
        //                3
        //                d 10
        //                p 1
        //            Then:
        //                n=3
        //                i=1: d10 -> pq: (10,2), total=10, kill=1.
        //                Then loop: i from 1 to n-2 = 1 to 1 -> so we process i=1: it's a dragon, done.
        //                Then last_b = princess_b[2] = 1.
        //                kill_count=1 >= 1 -> valid.
        //                Then output: 10, 1, and cell 2.

        //        What if: 
        //            n=3
        //            p 2   -> at cell2 (index1)
        //            p 1   -> at cell3 (index2)
        //            Then: 
        //                i=1: princess -> princess_b[1]=2 -> kill_count=0 < 2? so no removal. Then last_b=1: 0<1 -> -1.
        //            Correct: because at cell2, kills=0 < 2 -> no problem, but then at cell3, kills=0 < 1 -> cannot marry.

        //        Another test:
        //            n=4
        //            d 5
        //            p 1
        //            p 2
        //            Process:
        //                i=1: d5 -> pq: (5,2), total=5, kill=1.
        //                i=2: p1 -> kill_count=1 >= 1 -> remove 5: total=0, kill=0.
        //                i=3: not processed in loop (loop to n-2=2, so i=1,2) -> then last_b = princess_b[3] = 2 -> kill_count=0 < 2 -> -1.
        //            But is there a solution? 
        //                If we skip the dragon at cell2, then at cell2 (princess1) we have 0 kills -> safe (0<1), then at cell3 (princess2) we have 0 kills -> 0<2 -> not enough. 
        //                If we kill the dragon at cell2: then at cell2 (princess1) we have 1 kill -> 1>=1 -> marriage at cell2, so we never reach cell4? 
        //                But the problem: the knight must pass all cells. However, when he marries at cell2, the adventure ends and he doesn't reach the last cell. 
        //                So he cannot marry the princess at cell4. 
        //                Therefore, no solution -> -1 is correct.

        //        So the algorithm is correct.

        //        Let's write the code.

        //        Note: We must be cautious about the loop bounds: for (int i = 1; i <= n-2; i++) 
        //            because n-2 is the last index we process (0-indexed), and the last cell is n-1.

        //        In C++, for n=2: n-2 = 0, so the condition i<=n-2 -> i<=0, and i starts at 1 -> loop doesn't run.

        //        Correct.

        //        Code:


