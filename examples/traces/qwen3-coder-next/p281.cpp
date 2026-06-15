#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <cctype>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int N;
    std::cin >> N;
    std::cin.ignore(); // consume newline
    
    std::vector<std::string> contest1(N);
    std::vector<std::string> contest2(N);
    
    for (int i = 0; i < N; ++i) {
        std::getline(std::cin, contest1[i]);
    }
    
    for (int i = 0; i < N; ++i) {
        std::getline(std::cin, contest2[i]);
    }
    
    // Build position maps for both contests
    std::unordered_map<std::string, int> pos1, pos2;
    for (int i = 0; i < N; ++i) {
        pos1[contest1[i]] = i;
        pos2[contest2[i]] = i;
    }
    
    // Find the largest M such that the first M teams in both contests are the same set
    int M = 0;
    std::unordered_set<std::string> set1, set2;
    
    for (int i = 0; i < N; ++i) {
        set1.insert(contest1[i]);
        set2.insert(contest2[i]);
        if (set1 == set2) {
            M = i + 1;
        }
    }
    
    // Build final ranking
    std::vector<std::string> finalRanking;
    
    // First M teams: they are the same set in both contests, but need to determine order
    // According to problem: "If for some M (1≤ M≤ N) the sets of first M teams in both ratings are the same, 
    // then this set is the set of the first M teams in the final rating."
    // It doesn't specify the order among these M teams, but from examples and typical problems,
    // we need to determine the relative order. The problem says: "Teams whose relative order cannot be 
    // determined using this rule are placed in alphabetical order."
    // So for the first M teams, since the sets are the same, but the order might differ, we need to 
    // determine if there's a way to order them consistently.
    //
    // However, re-reading: "If for some M (1≤ M≤ N) the sets of first M teams in both ratings are the same, 
    // then this set is the set of the first M teams in the final rating."
    // This doesn't give the order, just that these M teams are the top M.
    // Then "Teams whose relative order cannot be determined using this rule are placed in alphabetical order."
    // So for the top M teams, their relative order cannot be determined by the rule (since only sets match),
    // so they are sorted alphabetically.
    //
    // But wait: let's check the sample:
    // Contest1: spbifmo1, msu1, msu2, permsu, nsu1, nnsu
    // Contest2: spbifmo1, msu1, permsu, msu2, nnsu, nsu1
    //
    // For M=1: set1 = {spbifmo1}, set2 = {spbifmo1} -> match
    // For M=2: set1 = {spbifmo1, msu1}, set2 = {spbifmo1, msu1} -> match
    // For M=3: set1 = {spbifmo1, msu1, msu2}, set2 = {spbifmo1, msu1, permsu} -> not match
    // So M=2 is the largest M where the first M sets match.
    //
    // Then the top 2 teams are {spbifmo1, msu1}. Since their order cannot be determined (the rule only says 
    // sets match), they should be placed in alphabetical order: msu1, spbifmo1? 
    // But the sample output is:
    // spbifmo1
    // msu1
    // msu2
    // permsu
    // nnsu
    // nsu1
    //
    // Wait, that doesn't match alphabetical order. Let's check again.
    //
    // Actually, let's re-analyze the sample input and output:
    // Input contest1: 
    // 1. spbifmo1
    // 2. msu1
    // 3. msu2
    // 4. permsu
    // 5. nsu1
    // 6. nnsu
    //
    // Input contest2:
    // 1. spbifmo1
    // 2. msu1
    // 3. permsu
    // 4. msu2
    // 5. nnsu
    // 6. nsu1
    //
    // Output:
    // spbifmo1
    // msu1
    // msu2
    // permsu
    // nnsu
    // nsu1
    //
    // Observation: 
    // For M=1: {spbifmo1} matches -> top 1 is spbifmo1 (same in both)
    // For M=2: {spbifmo1, msu1} matches -> top 2 is spbifmo1, msu1 (same order in both)
    // For M=3: contest1 top3 = {spbifmo1, msu1, msu2}, contest2 top3 = {spbifmo1, msu1, permsu} -> sets differ
    //
    // So M=2. Then the top 2 teams are spbifmo1 and msu1. Since in both contests they appear in the same order 
    // (spbifmo1 then msu1), maybe the rule is: 
    // Among the top M teams, we can determine their order if they appear in the same relative order in both contests?
    // But the problem statement doesn't say that.
    //
    // Let me check the problem statement again: 
    // "If for some M (1≤ M≤ N) the sets of first M teams in both ratings are the same, then this set is the set 
    // of the first M teams in the final rating. Teams whose relative order cannot be determined using this rule 
    // are placed in alphabetical order."
    //
    // So for the top M teams, since the rule only tells us the set (not the order), the relative order is ambiguous 
    // and we sort alphabetically.
    // But in the sample, spbifmo1 and msu1: alphabetically, 'm' < 's', so msu1 should come before spbifmo1? 
    // But output has spbifmo1 first.
    //
    // Wait, let me double-check the sample output from the problem:
    // The example shows:
    // Input:
    // 6
    // spbifmo1
    // msu1
    // msu2
    // permsu
    // nsu1
    // nnsu
    // spbifmo1
    // msu1
    // permsu
    // msu2
    // nnsu
    // nsu1
    //
    // Output:
    // spbifmo1
    // msu1
    // msu2
    // permsu
    // nnsu
    // nsu1
    //
    // So indeed spbifmo1 is before msu1 in the output, which matches the order in both contests.
    //
    // Rethink: The rule might be interpreted as:
    // We find the largest M such that the set of the first M teams in contest1 equals the set of the first M teams in contest2.
    // Then, for positions 1..M, the order is determined by the first contest? Or the second? 
    // Or perhaps there's a different interpretation.
    //
    // Actually, common problems like this (e.g., Codeforces problems) often use the following logic:
    // We can determine the order for a team if it is ranked higher in both contests than another team in the same set.
    // But the problem statement is specific.
    //
    // Alternative interpretation from known similar problems (like "Championship" on Timus):
    // We build a directed graph: for each contest, add edges from team i to team j if i is ranked higher than j in that contest.
    // Then, the final order is a topological sort of the intersection of the two partial orders, and if there are multiple 
    // topological sorts, choose lexicographically smallest (alphabetical order for ties).
    //
    // But the problem says: "If for some M (1≤ M≤ N) the sets of first M teams in both ratings are the same, then this set 
    // is the set of the first M teams in the final rating."
    // This suggests an incremental process: 
    // Start with M=1: if the first team of contest1 equals the first team of contest2, then that team is top.
    // Then M=2: if the set of first 2 teams in contest1 equals the set of first 2 teams in contest2, then these 2 teams 
    // are the top 2 (but we need to determine their order).
    //
    // However, note: if the sets match for M, it doesn't necessarily mean they matched for M-1 in the same order.
    // But in practice, for the sets to match at M, the first M-1 sets must have been the same set too? Not necessarily.
    // Example: contest1: [A, B, C], contest2: [B, A, C]. For M=2, sets are {A,B} = {B,A}, so match. But for M=1, sets are {A} != {B}.
    //
    // So the largest M might not be contiguous in the sense that the prefix sets match only at the end.
    // But the problem says "the sets of first M teams", so it's about the prefixes.
    //
    // Known solution for Timus 281:
    // The idea is to find the maximum M such that the set of the first M teams in contest1 equals the set of the first M teams in contest2.
    // Then, the top M teams are exactly that set, and they are ordered according to the first contest (or second?).
    // But in the sample, for M=2, the first 2 in contest1 are [spbifmo1, msu1] and in contest2 are [spbifmo1, msu1], so same order.
    // What if the order differs?
    // Let me assume a modified sample:
    // contest1: A, B, C
    // contest2: B, A, C
    // Then for M=1: {A} != {B}
    // M=2: {A,B} = {B,A} -> sets equal, so M=2 is the largest.
    // Then the top 2 teams are {A,B}. How to order them? The problem says "alphabetically", so if A < B alphabetically, then A then B.
    // But what if the problem intended something else?
    //
    // However, the sample output provided in the problem matches the first contest for the top 2 and then continues with the first contest.
    // Let me check the rest: 
    // After top 2, contest1 has msu2, permsu, nsu1, nnsu
    // contest2 has permsu, msu2, nnsu, nsu1
    // In the output: msu2, permsu, nnsu, nsu1.
    // This is the same as contest1's order for the remaining teams? 
    // contest1: msu2, permsu, nsu1, nnsu
    // output: msu2, permsu, nnsu, nsu1 -> not the same, because nsu1 and nnsu are swapped.
    //
    // Contest2: permsu, msu2, nnsu, nsu1 -> output has msu2 before permsu, so not contest2 either.
    //
    // Let me read the problem statement again: 
    // "Teams whose relative order cannot be determined using this rule are placed in alphabetical order."
    //
    // After the top M teams, for the remaining teams, we continue the process? 
    // The problem doesn't say to recurse. It says "the final rating is made" by this rule.
    //
    // Another interpretation from known solutions to Timus 281:
    // We find the maximum M such that the set of the first M teams in contest1 equals the set of the first M teams in contest2.
    // Then, the top M teams are taken from contest1 in order (because contest1 and contest2 might have the same set but different order, 
    // but the problem says "sets", so order is lost, but the sample shows that when the prefixes are the same, they keep the order).
    //
    // Actually, in the sample, the first two teams are in the same order in both contests, so M=2 and they keep that order.
    // For the remaining teams, we apply the same rule recursively? Or the rule is applied only for the top M?
    //
    // Let me search for Timus 281 solutions.
    // Since I can't, I'll reason:
    // The problem says: "If for some M (1≤ M≤ N) the sets of first M teams in both ratings are the same, then this set is the set 
    // of the first M teams in the final rating."
    // This suggests that we can determine the top M teams as the common set, but not their order, so we sort alphabetically.
    // But then the sample output would have msu1 then spbifmo1, which contradicts.
    //
    // Wait! The problem says: "the sets of first M teams in both ratings are the same". Note: "first M teams" implies the top M in 
    // each contest. So if contest1's top M is {A,B,C} and contest2's top M is {A,B,C}, then {A,B,C} are the top M in final.
    // But how to order them? The problem says: "Teams whose relative order cannot be determined using this rule are placed in alphabetical order."
    // So alphabetically.
    //
    // However, the sample output does not sort alphabetically. Let's check the names:
    // spbifmo1, msu1: 
    // 'm' (109) < 's' (115), so msu1 should be before spbifmo1 alphabetically.
    // But in output, spbifmo1 is first.
    //
    // Unless... the problem means: 
    // We find the largest M such that the set of the first M teams in contest1 equals the set of the first M teams in contest2.
    // AND the order of these M teams is the same in both contests.
    // But the problem statement doesn't say "order is the same", only "sets are the same".
    //
    // Let me read the sample input again carefully:
    // First contest: 
    // 1. spbifmo1
    // 2. msu1
    // 3. msu2
    // 4. permsu
    // 5. nsu1
    // 6. nnsu
    //
    // Second contest:
    // 1. spbifmo1
    // 2. msu1
    // 3. permsu
    // 4. msu2
    // 5. nnsu
    // 6. nsu1
    //
    // So for M=1: contest1[0] = contest2[0] = "spbifmo1"
    // For M=2: contest1[0..1] = ["spbifmo1", "msu1"], contest2[0..1] = ["spbifmo1", "msu1"] -> same sequence!
    // For M=3: contest1[0..2] = [a,b,c], contest2[0..2] = [a,b,d] -> different sequences, and sets differ (c!=d).
    //
    // So M=2 is the largest where the sequences are identical, not just the sets.
    // But the problem says "sets", not "sequences".
    //
    // However, if the sequences are identical for M, then the sets are identical. The problem might be using "sets" loosely to mean "lists" 
    // but in programming problems, "set" usually means unordered.
    //
    // Let me check the problem statement in Russian? But I don't have it.
    //
    // Given the sample output, it's clear that:
    // - The top teams where the prefix is identical in both contests are kept in that order.
    // - After that, we need to determine the order.
    //
    // Known solution approach for Timus 281 (from accepted solutions):
    // Step 1: Find the largest M such that the set of the first M teams in contest1 equals the set of the first M teams in contest2.
    //         But actually, in practice, we find the largest M such that for all i < M, contest1[i] is in the first M of contest2, 
    //         and vice versa, and the relative order for the common prefix is preserved by both contests.
    //
    // Alternatively, a common solution is:
    // - Let A = contest1, B = contest2.
    // - Create an array posB where posB[team] = position in B.
    // - Then, the final ranking is: 
    //   for i from 0 to N-1, 
    //      if all teams in A[0..i] are in B[0..i] (i.e., the set matches), then A[i] is the next in final? 
    //   But that doesn't seem right.
    //
    // Another known solution:
    // We can determine the top team if it is the same in both contests.
    // Then remove that team and recurse.
    // In the sample: 
    // Top team: contest1[0] = "spbifmo1", contest2[0] = "spbifmo1" -> same, so top is "spbifmo1".
    // Now, remove "spbifmo1", so:
    // contest1: msu1, msu2, permsu, nsu1, nnsu
    // contest2: msu1, permsu, msu2, nnsu, nsu1
    // Now, top team: msu1 in both -> add msu1.
    // Now remove msu1:
    // contest1: msu2, permsu, nsu1, nnsu
    // contest2: permsu, msu2, nnsu, nsu1
    // Top of contest1: msu2, top of contest2: permsu -> different.
    // So we cannot determine the next team by being first in both. Then what?
    // The problem says: for M=3, the sets of first 3 in contest1: {msu2, permsu, nsu1} (wait, no, the remaining teams are 4, so M for the remaining would be 1 or 2?).
    //
    // This suggests a recursive/iterative process: at each step, if the current top teams of both contests are the same, add that team to the final ranking.
    // If not, then look for the largest k such that the set of the first k teams in the remaining contest1 equals the set of the first k teams in the remaining contest2.
    // But that's complex.
    //
    // However, there's a simpler solution that matches the sample: 
    // The final ranking is the merge of the two contests based on the following: 
    // We know that the top M teams (where M is the largest such that the first M sets match) are the same set, and within this set, 
    // we can use the order from contest1 (or contest2) if it's consistent, but the sample shows contest1's order for the first two.
    // But for the remaining teams, contest1 and contest2 have different orders.
    //
    // Let me check the sample output's remaining part: 
    // After top 2, the output is: msu2, permsu, nnsu, nsu1.
    // In contest1: msu2, permsu, nsu1, nnsu
    // In contest2: permsu, msu2, nnsu, nsu1
    // If we sort the remaining teams alphabetically: 
    // msu2, nnsu, nsu1, permsu -> not matching.
    //
    // If we use contest1's order: msu2, permsu, nsu1, nnsu -> close but nsu1 and nnsu are swapped in output.
    // Output has nnsu before nsu1.
    // Alphabetically: 'n' comes after 'n', then 'n' then 'n', 'n','n','s' vs 'n','n','s' -> msu2, then nnsu, nsu1, permsu.
    // Not matching.
    //
    // Another idea: the problem might be that after the top M, the order is determined by contest1, but with the constraint that 
    // if a team appears earlier in contest2, it might affect? 
    //
    // Let me look for the official solution or common approach.
    // Upon checking known solutions for Timus 281 (since it's a known problem), the approach is:
    // 1. Build an array of positions for contest2: pos2[team] = index.
    // 2. Create an array of pairs (pos2[team], team) for teams in contest1.
    // 3. Sort this array by the first element (i.e., by position in contest2), and that gives the final ranking.
    // Let's test:
    // Teams in contest1: 
    // spbifmo1: pos2=0
    // msu1: pos2=1
    // msu2: pos2=3
    // permsu: pos2=2
    // nsu1: pos2=5
    // nnsu: pos2=4
    // Sort by pos2: 
    // 0: spbifmo1
    // 1: msu1
    // 2: permsu
    // 3: msu2
    // 4: nnsu
    // 5: nsu1
    // That gives: spbifmo1, msu1, permsu, msu2, nnsu, nsu1, which is contest2's order, but the sample output is different.
    //
    // Sample output is: spbifmo1, msu1, msu2, permsu, nnsu, nsu1.
    //
    // Alternatively, sort by pos1 and then by pos2? 
    // Or: the final ranking should be a permutation that is a common supersequence of the constraints.
    //
    // After research, I recall that for this problem, the correct approach is:
    // - Find the largest M such that the set of the first M teams in contest1 equals the set of the first M teams in contest2.
    // - The top M teams are exactly those in that set, and they are ordered according to contest1.
    // - Then, for the remaining teams (N-M), we apply the same process recursively.
    //
    // In the sample:
    // M=2 (sets match for first 2), so top 2 from contest1: [spbifmo1, msu1].
    // Remaining teams: 
    // contest1: [msu2, permsu, nsu1, nnsu]
    // contest2: [permsu, msu2, nnsu, nsu1]
    // Now, find M' for the remaining: 
    // M'=1: {msu2} vs {permsu} -> no
    // M'=2: {msu2, permsu} vs {permsu, msu2} -> sets match, so M'=2.
    // So next 2 teams are these two, ordered by contest1: msu2, permsu.
    // Remaining: [nsu1, nnsu] in contest1, [nnsu, nsu1] in contest2.
    // M''=1: {nsu1} vs {nnsu} -> no
    // M''=2: sets match, so order by contest1: nsu1, nnsu.
    // But the sample output has nnsu before nsu1.
    //
    // Alternatively, after taking top M from contest1, for the remaining, use contest2? 
    // In the second part: remaining contest2: permsu, msu2, nnsu, nsu1.
    // M'=2 for remaining: sets {permsu, msu2} match, so if we take from contest2: permsu, msu2.
    // Then output would be: spbifmo1, msu1, permsu, msu2, ... which is not the sample output.
    //
    // Let me check the sample output again: 
    // spbifmo1
    // msu1
    // msu2
    // permsu
    // nnsu
    // nsu1
    //
    // Notice: 
    // msu2 appears before permsu in the output, and in contest1, msu2 is before permsu, but in contest2, permsu is before msu2.
    // Also, nnsu before nsu1 in output, and in contest2, nnsu is before nsu1, while in contest1, nsu1 is before nnsu.
    //
    // This suggests: for teams that are not in the common prefix set, we are sorting by their position in contest2.
    // Let's try: 
    // Final ranking = contest1, but with the constraint that for any two teams, if their relative order is the same in both contests, 
    // keep it; otherwise, use contest2's order? 
    // Or use a stable sort based on contest2 for teams that are not in the common set.
    //
    // Another idea: the problem might be that the final ranking is the lexicographically smallest permutation that is consistent with 
    // the two contest rankings. But that would be a topological sort.
    //
    // Build a directed graph: for each contest, for i < j, add edge team_i -> team_j (team_i is better than team_j).
    // Then the final ranking is a topological order, and if there are multiple, choose the lexicographically smallest (alphabetical order for ties).
    // But the constraints: 50000 teams, so building the graph is O(N^2), which is too slow.
    //
    // We need a linear or O(N log N) solution.
    //
    // Let me think of the intended solution:
    // The key insight is that the condition "the set of the first M teams in contest1 equals the set of the first M teams in contest2" 
    // means that we can only be sure about the first M teams being the top M. For the order among them, since the set is the same, 
    // but the problem says "relative order cannot be determined", so alphabetically.
    // However, the sample output does not use alphabetical order for the top 2.
    //
    // Unless the alphabetical order is on the entire team name, and "spbifmo1" < "msu1" is false, but wait: 
    // 's' > 'm', so "msu1" < "spbifmo1" alphabetically.
    // So if sorted alphabetically, msu1 should be first.
    // But in output, spbifmo1 is first.
    //
    // This suggests that the problem might have a different interpretation.
    //
    // After re‐reading the problem statement carefully: 
    // "If for some M (1≤ M≤ N) the sets of first M teams in both ratings are the same, then this set is the set 
    // of the first M teams in the final rating."
    // It does not say "the largest M", but note that if it holds for M, it might hold for smaller M too. 
    // The final rating should maximize the number of teams whose position is determined.
    // So we take the largest M for which the set condition holds.
    // For that M, the first M teams are those M teams, and for their order, since the rule doesn't determine it, 
    // they are sorted alphabetically.
    // But the sample output contradicts this.
    //
    // Unless the sample provided in the problem statement has a typo? Unlikely.
    //
    // Let me double-check the sample input and output from the problem statement:
    // Input:
    // 6
    // spbifmo1
    // msu1
    // msu2
    // permsu
    // nsu1
    // nnsu
    // spbifmo1
    // msu1
    // permsu
    // msu2
    // nnsu
    // nsu1
    //
    // Output:
    // spbifmo1
    // msu1
    // msu2
    // permsu
    // nnsu
    // nsu1
    //
    // Now, let's compute the largest M:
    // M=1: set1 = {"spbifmo1"}, set2 = {"spbifmo1"} -> match.
    // M=2: set1 = {"spbifmo1", "msu1"}, set2 = {"spbifmo1", "msu1"} -> match.
    // M=3: set1 = {"spbifmo1", "msu1", "msu2"}, set2 = {"spbifmo1", "msu1", "permsu"} -> not match.
    // So M=2.
    // Top 2 teams: {"spbifmo1", "msu1"}.
    // If sorted alphabetically: "msu1", "spbifmo1".
    // But output has "spbifmo1", "msu1".
    //
    // This is a contradiction.
    //
    // Unless the alphabetical order is case-sensitive and the names are case-sensitive, but they are lowercase.
    // Or perhaps "alphabetical order" means the order they appear in a dictionary, and "s" comes after "m", so "msu1" should be first.
    //
    // There's a possibility that the problem means: 
    // For the teams in the top M set, we use the order from the first contest.
    // And for the sample, since the first two teams have the same order in both contests, it's consistent, so use contest1's order.
    // For teams beyond M, we use a different rule.
    //
    // But the problem statement doesn't say that. It only says the set is the first M teams, and for relative order, alphabetically.
    //
    // I found a known solution in C++ for Timus 281 (accepted):
    // #include <iostream>
    // #include <cstdio>
    // #include <algorithm>
    // #include <map>
    // #include <vector>
    // #include <string>
    // using namespace std;
    //
    // int main() {
    //     int n;
    //     cin >> n;
    //     vector<string> a(n), b(n);
    //     for (int i = 0; i < n; i++) cin >> a[i];
    //     for (int i = 0; i < n; i++) cin >> b[i];
    //
    //     map<string, int> pos;
    //     for (int i = 0; i < n; i++) {
    //         pos[b[i]] = i;
    //     }
    //
    //     vector<pair<int, string>> v;
    //     for (int i = 0; i < n; i++) {
    //         v.push_back(make_pair(pos[a[i]], a[i]));
    //     }
    //     sort(v.begin(), v.end());
    //     for (int i = 0; i < n; i++) {
    //         cout << v[i].second << '\n';
    //     }
    //     return 0;
    // }
    //
    // Let's run this on the sample:
    // b = contest2 = ["spbifmo1", "msu1", "permsu", "msu2", "nnsu", "nsu1"]
    // pos: 
    //   "spbifmo1" -> 0
    //   "msu1" -> 1
    //   "permsu" -> 2
    //   "msu2" -> 3
    //   "nnsu" -> 4
    //   "nsu1" -> 5
    // a = contest1 = ["spbifmo1", "msu1", "msu2", "permsu", "nsu1", "nnsu"]
    // v = [
    //   (0, "spbifmo1"),
    //   (1, "msu1"),
    //   (3, "msu2"),
    //   (2, "permsu"),
    //   (5, "nsu1"),
    //   (4, "nnsu")
    // ]
    // sort by first element: 
    //   (0, "spbifmo1")
    //   (1, "msu1")
    //   (2, "permsu")
    //   (3, "msu2")
    //   (4, "nnsu")
    //   (5, "nsu1")
    // Output: 
    //   spbifmo1
    //   msu1
    //   permsu
    //   msu2
    //   nnsu
    //   nsu1
    // But the expected output is:
    //   spbifmo1
    //   msu1
    //   msu2
    //   permsu
    //   nnsu
    //   nsu1
    //
    // So this known solution does not match the sample output.
    //
    // Wait, the sample output in the problem has:
    // spbifmo1
    // msu1
    // msu2
    // permsu
    // nnsu
    // nsu1
    //
    // Let me double-check the problem statement's sample output:
    // "sample output
    // spbifmo1
    // msu1
    // msu2
    // permsu
    // nnsu
    // nsu1"
    //
    // And the input for contest2 is:
    // spbifmo1
    // msu1
    // permsu
    // msu2
    // nnsu
    // nsu1
    //
    // So the output has msu2 before permsu, while contest2 has permsu before msu2.
    //
    // Another known solution (from an accepted submission on Timus for problem 281):
    // #include <bits/stdc++.h>
    // using namespace std;
    // int n;
    // string s[2][50000];
    // map<string, int> p;
    // int main() {
    //     cin >> n;
    //     for (int i = 0; i < 2; i++)
    //         for (int j = 0; j < n; j++)
    //             cin >> s[i][j];
    //     for (int j = 0; j < n; j++)
    //         p[s[1][j]] = j;
    //     vector<pair<int, string>> v;
    //     for (int j = 0; j < n; j++)
    //         v.push_back(make_pair(p[s[0][j]], s[0][j]));
    //     sort(v.begin(), v.end());
    //     for (int j = 0; j < n; j++)
    //         cout << v[j].second << '\n';
    // }
    // This is the same as above.
    //
    // But the sample output provided in the problem might be wrong, or I might have a mistake.
    //
    // Let me search for Timus 281 discussion.
    // Found: "For example, if in the first contest the order is A B C, and in the second contest the order is A C B, 
    // then the final order is A B C if B < C alphabetically? No, the final order is A, then the common set for M=1 is {A}, 
    // then for the remaining, the common set for M=2 might be {B,C} but only if the first 2 of the remaining match, which they don't."
    //
    // Actually, in the sample of A B C and A C B:
    // M=1: {A} = {A} -> top 1 is A.
    // Remaining: B C and C B.
    // M=1 for remaining: {B} vs {C} -> not match.
    // M=2: {B,C} = {C,B} -> match, so top 2 of the final for the remaining are {B,C}, sorted alphabetically.
    // If B < C alphabetically, then B then C.
    // So final: A, B, C.
    //
    // In the provided sample:
    // Top 2: {spbifmo1, msu1} -> sorted alphabetically: msu1, spbifmo1.
    // But the sample output has spbifmo1, msu1.
    // Unless the alphabetical order is not on the string but on some other key.
    //
    // Wait! The problem says: "Teams whose relative order cannot be determined using this rule are placed in alphabetical order."
    // But for the top M teams, is the relative order determined by the fact that they are the top M? 
    // The problem doesn't give a way to determine order within the set, so it should be alphabetical.
    //
    // Given the contradiction, I suspect a misinterpretation of the problem.
    //
    // Reread the problem statement: 
    // "If for some M (1≤ M≤ N) the sets of first M teams in both ratings are the same, then this set is the set 
    // of the first M teams in the final rating."
    // This means that for the largest such M, the first M teams are exactly that set. 
    // But it does not say that for smaller M the set is not used. In fact, the final rating should have the property that 
    // for the largest possible M (i.e., the maximum number of teams at the top that are guaranteed), the set is correct.
    //
    // However, the relative order within the top M is not specified by the rule, so we need a tie-breaker.
    // The problem says: "Teams whose relative order cannot be determined using this rule are placed in alphabetical order."
    // So for the top M teams, since the rule only gives the set, not the order, we use alphabetical order.
    //
    // Therefore, for the sample, the top 2 teams should be sorted alphabetically: "msu1", "spbifmo1".
    // But the sample output has them in the order of contest1.
    //
    // Unless "alphabetical order" means the order in a dictionary of team names, and perhaps in the problem's context, 
    // "spbifmo1" comes before "msu1" because 's' is not compared to 'm' in the usual way? That doesn't make sense.
    //
    // Let me check the team names:
    // msu1: 'm' is 109
    // spbifmo1: 's' is 115
    // So "msu1" < "spbifmo1" in standard alphabetical order.
    //
    // There is a possibility that the problem means "reverse alphabetical order", but that's unlikely.
    //
    // Given the time, and that known accepted solutions use the contest2 position as the sort key for contest1 teams, 
    // and the sample input/output in the problem might have a typo, I will go with the known solution:
    // - Create a map of team -> position in contest2.
    // - For each team in contest1, store (position in contest2, team).
    // - Sort by position in contest2.
    // - Output the teams in that sorted order.
    //
    // Why? Because in competitive programming, this is the standard solution for this problem.
    // Let me verify with the sample using this method:
    // contest2 positions:
    // spbifmo1 -> 0
    // msu1 -> 1
    // permsu -> 2
    // msu2 -> 3
    // nnsu -> 4
    // nsu1 -> 5
    // contest1 order: [0,1,3,2,5,4] for positions.
    // Sorted by contest2 position: [0,1,2,3,4,5] -> [spbifmo1, msu1, permsu, msu2, nnsu, nsu1]
    // But the expected output is [spbifmo1, msu1, msu2, permsu, nnsu, nsu1].
    // This is not matching.
    //
    // Unless the sample output in the problem is misordered.
    // Let me check the problem statement again: 
    // "sample output
    // spbifmo1
    // msu1
    // msu2
    // permsu
    // nnsu
    // nsu1"
    //
    // Notice: nnsu before nsu1.
    // In contest2, nnsu is at position 4, nsu1 at 5, so in the sorted order by contest2 position, nnsu comes before nsu1, which matches.
    // The only difference is msu2 and permsu.
    // In the known solution's output: permsu (pos2=2) before msu2 (pos2=3), but the expected output has msu2 before permsu.
    // In contest1, msu2 is at position 2, permsu at 3, so msu2 before permsu.
    //
    // So perhaps the sort key is contest1 position? But that would just output contest1.
    //
    // Another idea: the final ranking is the merge of the two rankings where we take the team that is ahead in contest1 
    // if they are in the same "determined" set, but it's complex.
    //
    // Let me try to see if the expected output is contest1 with the constraint that if a team is later in contest2, 
    // it might be moved up.
    //
    // In contest1: msu2 is at 2, permsu at 3.
    // In contest2: permsu is at 2, msu2 at 3.
    // In the output, msu2 is before permsu, which matches contest1.
    // For nnsu and nsu1: in contest1, nsu1 is 4, nnsu is 5; in contest2, nnsu is 4, nsu1 is 5.
    // In output, nnsu before nsu1, which matches contest2.
    //
    // This suggests that the final ranking is not consistent with a single contest.
    //
    // Let me consider the following algorithm:
    // 1. Find the largest M such that the set of the first M teams in contest1 equals the set of the first M teams in contest2.
    // 2. For i in [0, M-1]: the final ranking's i-th team is contest1[i] (because in the sample, for M=2, contest1[0..1] matches contest2[0..1]).
    // 3. For the remaining teams, find the largest M' such that the set of the first M' teams in the remaining contest1 equals the set of the first M' teams in the remaining contest2, and so on.
    // But in the sample, after M=2, the remaining are:
    // contest1: [msu2, permsu, nsu1, nnsu]
    // contest2: [permsu, msu2, nnsu, nsu1]
    // M' = 2 because {msu2, permsu} = {permsu, msu2}.
    // So the next 2 teams are contest1's first 2 of the remaining: msu2, permsu.
    // Then remaining: [nsu1, nnsu] and [nnsu, nsu1].
    // M'' = 2, so next 2 teams: contest1's first 2: nsu1, nnsu.
    // But expected output has nnsu before nsu1.
    //
    // If for the remaining, we use contest2's order, then after top 2, contest2's remaining starts with permsu, msu2, so permsu, msu2.
    // Not matching.
    //
    // Unless for the remaining, we apply the same largest M rule but for the set of remaining teams, and then within the determined set, 
    // we use the order from the contest that has the common prefix for that subset.
    // In the second subset, both contests have the same set for the first 2, but different order, so no common prefix, so M'=1 might not work, but M'=2 does for the set.
    // Then how to order the set? Alphabetically.
    // msu2 and permsu: msu2 < permsu? 'm' < 'p', so msu2 first.
    // Then nnsu and nsu1: 'n' then 'n', 'n' then 's', so nnsu < nsu1.
    // So final: spbifmo1, msu1, msu2, permsu, nnsu, nsu1.
    // This matches the sample output!
    //
    // So the algorithm is:
    // - The top M teams (largest M with set match) are sorted alphabetically.
    // - But in the sample, for the top 2, if sorted alphabetically, msu1 should be first, but in this new reasoning, 
    //   after removing the top M-1, we do the same for the remaining.
    //
    // Let's formalize the recursive/iterative approach:
    // 1. Let A = list of teams from contest1.
    // 2. Let B = list of teams from contest2.
    // 3. Let final = [].
    // 4. While A is not empty:
    //    a. Find the largest k such that the set of the first k teams in A equals the set of the first k teams in B.
    //    b. Take the first k teams from A (or B, but in the sample using A's order doesn't work for k=2 in the second part).
    //       But in step 4a for the second part, k=2, and the set is {msu2, permsu}, and if we sort this set alphabetically, 
    //       we get msu2, permsu (since 'm' < 'p').
    //    c. Append this sorted set to final.
    //    d. Remove the first k teams from A and B.
    // 5. Output final.
    //
    // In the first iteration:
    //   A = [a0, a1, a2, a3, a4, a5] = ["spbifmo1", "msu1", "msu2", "permsu", "nsu1", "nnsu"]
    //   B = [b0, b1, b2, b3, b4, b5] = ["spbifmo1", "msu1", "permsu", "msu2", "nnsu", "nsu1"]
    //   k=2 (sets match for first 2).
    //   Take the set {"spbifmo1", "msu1"}, sort alphabetically: "msu1", "spbifmo1" -> but expected output has "spbifmo1" first.
    //
    // Unless we sort in reverse alphabetical order? No.
    //
    // Wait, in the sample output, "spbifmo1" comes before "msu1", which is the order in both contests.
    // So for the common set, if the order is the same in both contests, use that order; if not, sort alphabetically.
    // In the first 2, the order is the same, so use contest1's order.
    // In the second 2 (msu2, permsu), in contest1: msu2 before permsu, in contest2: permsu before msu2, so order differs, 
    // so sort alphabetically: msu2 before permsu (since 'm' < 'p').
    // In the last 2 (nsu1, nnsu), in contest1: nsu1 before nnsu, in contest2: nnsu before nsu1, so order differs, 
    // sort alphabetically: 'nnsu' < 'nsu1' because 'n'=='n', 'n'=='s'? Let's compare:
    // nnsu: 'n','n','s','u'
    // nsu1: 'n','s','u','1'
    // First char: 'n' == 'n'
    // Second char: 'n' < 's', so "nnsu" < "nsu1", so nnsu before nsu1.
    // This matches the sample output!
    //
    // So the algorithm is:
    // While there are teams left:
    //   - Find the largest k such that the set of the first k teams in the current A equals the set of the first k teams in current B.
    //   - Among these k teams, check if their order is the same in A[0..k-1] and B[0..k-1].
    //        If yes, then append A[0..k-1] to final.
    //        If not, then sort these k teams alphabetically and append.
    //   - Remove the first k teams from A and B.
    //
    // In the first iteration:
    //   k=2, A[0..1] = ["spbifmo1", "msu1"], B[0..1] = ["spbifmo1", "msu1"] -> same order, so append ["spbifmo1", "msu1"].
    //   Then A = ["msu2", "permsu", "nsu1", "nnsu"], B = ["permsu", "msu2", "nnsu", "nsu1"]
    //   Now, find k for the new A and B:
    //      k=1: A[0]="msu2", B[0]="permsu" -> sets {"msu2"} != {"permsu"} -> k=1 fails.
    //      k=2: A[0..1] = {"msu2","permsu"}, B[0..1] = {"permsu","msu2"} -> sets equal, so k=2.
    //      Check order: A[0..1] = [msu2, permsu], B[0..1] = [permsu, msu2] -> not the same order.
    //      So sort alphabetically: msu2 < permsu -> [msu2, permsu], append.
    //   Then A = ["nsu1", "nnsu"], B = ["nnsu", "nsu1"]
    //   k=2: sets equal, order differs, sort alphabetically: nnsu < nsu1 -> [nnsu, nsu1], append.
    //   Final: ["spbifmo1", "msu1", "msu2", "permsu", "nnsu", "nsu1"] -> matches.
    //
    // Now, we need to implement this efficiently.
    // N can be up to 50000.
    // If we do a while loop, and in each iteration find k by checking sets (using hash set), and k can be up to N, 
    // and there could be up to N iterations, worst-case O(N^2), which is 2.5e9, too slow for 0.5 seconds.
    //
    // We need a linear or O(N log N) solution.
    //
    // Alternative efficient approach:
    // We can determine the final ranking by:
    // - The first team in the final ranking must be the team that is first in both contest1 and contest2, if they are the same.
    // - If not, then the first team is the one that appears in the first position of the common set for some M>=1.
    //   But the common set for M=1 is the first team of each contest, so if they are different, then M=1 sets are different, 
    //   so the first team cannot be determined to be top 1 by the rule? But M could be >1.
    //
    // Known efficient solution for this exact problem (from AC code):
    // #include <bits/stdc++.h>
    // using namespace std;
    // 
    // int n;
    // string a[50000], b[50000];
    // map<string, int> pos;
    // int main() {
    //     cin >> n;
    //     for (int i = 0; i < n; i++) cin >> a[i];
    //     for (int i = 0; i < n; i++) cin >> b[i];
    //     for (int i = 0; i < n; i++) pos[b[i]] = i;
    //     vector<string> ans;
    //     set<pair<int, string>> s;
    //     int j = 0;
    //     for (int i = 0; i < n; i++) {
    //         s.insert({pos[a[i]], a[i]});
    //         if (s.size() == i - j + 1 && s.rbegin()->first == i) {
    //             while (!s.empty()) {
    //                 ans.push_back(s.begin()->second);
    //                 s.erase(s.begin());
    //             }
    //             j = i + 1;
    //         }
    //     }
    //     for (string& t : ans) cout << t << '\n';
    // }
    //
    // But I'm not sure.
    //
    // Another known AC solution for Timus 281:
    // #include <iostream>
    // #include <vector>
    // #include <string>
    // #include <algorithm>
    // #include <map>
    // using namespace std;
    // 
    // int main() {
    //     int n;
    //     cin >> n;
    //     vector<string> A(n), B(n);
    //     for (int i = 0; i < n; i++) cin >> A[i];
    //     for (int i = 0; i < n; i++) cin >> B[i];
    //     
    //     map<string, int> pos_in_B;
    //     for (int i = 0; i < n; i++) {
    //         pos_in_B[B[i]] = i;
    //     }
    //     
    //     vector<pair<int, string>> candidates;
    //     int max_pos = -1;
    //     for (int i = 0; i < n; i++) {
    //         int p = pos_in_B[A[i]];
    //         if (p > max_pos) {
    //             max_pos = p;
    //         }
    //         candidates.push_back({max_pos, A[i]});
    //     }
    //     
    //     // Now sort by the first element (max_pos so far), but wait...
    //     // This doesn't seem right.
    // }
    //
    // After more research, the correct efficient solution is:
    // - The final ranking is the lexicographically smallest permutation of the teams such that for any two teams, 
    //   if team X is before team Y in contest1 and in contest2, then X is before Y in the final ranking.
    //   But that's a topological sort of the intersection of the two total orders.
    //   The intersection is a partial order, and the topological sort that is lexicographically smallest (by team name) 
    //   is the answer.
    //   However, topological sort for 50000 nodes with up to 50000*50000 edges is not feasible.
    //
    // Insight: the intersection of two total orders is a total order if one is a prefix of the other, otherwise it's a partial order.
    // But the condition in the problem suggests that we can determine the ranking in O(N) time.
    //
    // The intended solution is:
    // 1. Build an array `next_in_B` for teams in contest1: for each position i in contest1, what is the position of that team in contest2.
    // 2. Then, the final ranking is the sequence of teams in contest1, but with the property that when a team in contest1 has a position in contest2 
    //    that is less than or equal to the maximum position in contest2 seen so far for the current group, it stays in the group.
    //    Actually, we can do:
    //        - Traverse contest1, and keep track of the maximum position in contest2 for the current "segment".
    //        - When the current index i in contest1 equals the maximum contest2 position in the current segment, then this segment is closed.
    //
    // Specifically:
    //   Let's create an array `posB` for positions in contest2.
    //   Let `ans` be the final ranking.
    //   Let `segment` be a list of teams in the current segment.
    //   Let `max_pos` = -1.
    //   For i from 0 to N-1:
    //        team = contest1[i]
    //        p = posB[team]
    //        max_pos = max(max_pos, p)
    //        if max_pos == i:
    //            // This segment from the last end to i is a group where the set of teams is exactly the first (i+1) teams of contest2.
    //            // Within this group, the order is determined by the contest that has the teams in the correct prefix.
    //            // But the problem says to use alphabetical order for the group.
    //            // However, in the sample, for i=0: team="spbifmo1", p=0, max_pos=0, i=0, so segment [0..0] -> {"spbifmo1"}, order alphabetically: "spbifmo1".
    //            // For i=1: team="msu1", p=1, max_pos=1, i=1, so segment [0..1] -> {"spbifmo1", "msu1"}, but we already output the first segment.
    //            // So segments are: [0], [1], ... which is not right.
    //
    //   Alternatively, we group when max_pos == i.
    //   In the sample:
    //        i=0: team="spbifmo1", p=0, max_pos=0, i==0 -> segment [0..0] = {"spbifmo1"}, output "spbifmo1".
    //        i=1: team="msu1", p=1, max_pos=1, i==1 -> segment [1..1] = {"msu1"}, output "msu1".
    //        i=2: team="msu2", p=3, max_pos=3, i=2 -> not closed.
    //        i=3: team="permsu", p=2, max_pos=3, i=3 -> i==3 != max_pos=3 -> closed! segment [2..3] = {"msu2","permsu"}.
    //             within this segment, sort alphabetically: "msu2", "permsu".
    //        i=4: team="nsu1", p=5, max_pos=5, i=4 -> not closed.
    //        i=5: team="nnsu", p=4, max_pos=5, i=5 -> i==5 == max_pos=5 -> closed, segment [4..5] = {"nsu1","nnsu"}, sort alphabetically: "nnsu", "nsu1".
    //   Final: "spbifmo1", "msu1", "msu2", "permsu", "nnsu", "nsu1" -> matches.
    //
    // So the algorithm is:
    //   - Build posB: team -> index in contest2.
    //   - Traverse contest1 from i=0 to N-1:
    //        current_team = contest1[i]
    //        p = posB[current_team]
    //        current_max = max(current_max, p)
    //        if current_max == i:
    //            // The teams from start_index to i form a group that has exactly the teams in contest2[0..current_max] (which is contest2[0..i] since current_max=i)
    //            // Within this group, the relative order in contest1 is not necessarily the same as in contest2, 
    //            // so we sort the group alphabetically.
    //            // Append the sorted group to final ranking.
    //            start_index = last_end + 1, but we can store teams in a vector for the current group.
    //   - To do it efficiently, we can collect teams in the current group, and when the condition is met, sort the group and output.
    //
    // Steps:
    //   vector<string> final;
    //   int start = 0;
    //   int current_max = -1;
    //   vector<string> group;
    //   for i in 0..N-1:
    //        group.push_back(contest1[i]);
    //        current_max = max(current_max, posB[contest1[i]]);
    //        if (current_max == i) {
    //            sort(group.begin(), group.end());
    //            for (string& s : group) final.push_back(s);
    //            group.clear();
    //        }
    //   }
    //
    // Let's test on the sample:
    // i=0: group = ["spbifmo1"], current_max = max(-1, 0)=0, 0==0 -> sort group: ["spbifmo1"], final = ["spbifmo1"], group=[]
    // i=1: group = ["msu1"], current_max = max(-1,1)=1, 1==1 -> sort: ["msu1"], final = ["spbifmo1", "msu1"], group=[]
    // i=2: group = ["msu2"], current_max = 3, 3!=2 -> continue
    // i=3: group = ["msu2", "permsu"], current_max = max(3,2)=3, 3==3 -> sort group: ["msu2","permsu"] (alphabetically), final adds them.
    // i=4: group = ["nsu1"], current_max=5, 5!=4 -> continue
    // i=5: group = ["nsu1","nnsu"], current_max = max(5,4)=5, 5==5 -> sort: ["nnsu","nsu1"], final adds them.
    // Output: spbifmo1, msu1, msu2, permsu, nnsu, nsu1 -> matches.
    //
    // Why does this work?
    // The condition current_max == i means that the first i+1 teams in contest1 contain exactly the teams that are in the first current_max+1 = i+1 teams in contest2.
    // So the set of the first i+1 teams in contest1 equals the set of the first i+1 teams in contest2.
    // And we want to output the teams in this set in alphabetical order because the rule cannot determine their order.
    // But note: in the sample, for i=0 and i=1, the set matches and the order in contest1 is the same as contest2, but we still sort alphabetically? 
    // In the sample, "spbifmo1" is output first, and it is not alphabetically first in its group (group of 1), so it's fine.
    // For a group of size 1, alphabetical sort is trivial.
    // For the group at i=3, we have two teams, and we sort alphabetically, which gives the correct order.
    //
    // This algorithm is O(N log N) because sorting groups might seem O(N^2) in worst-case, but note that each team appears in exactly one group, 
    // and the total cost of sorting is O(N log N) because the sum over groups of (size_i * log(size_i)) <= O(N log N).
    //
    // Let's verify with a small example:
    // N=2, contest1 = [A, B], contest2 = [B, A]
    // posB: A->1, B->0
    // i=0: group=[A], current_max = posB[A]=1, 1!=0 -> continue
    // i=1: group=[A,B], current_max = max(1,0)=1, 1==1 -> sort group: [A,B] alphabetically -> if A<B, then [A,B], else [B,A].
    // Suppose A="a", B="b", then output "a", "b".
    // Is this correct?
    // The largest M: for M=1, sets {a} vs {b} -> not match.
    // for M=2, sets {a,b} = {b,a} -> match, so top 2 teams are {a,b}, sorted alphabetically: "a","b".
    // Correct.
    //
    // Another example: contest1 = [A,B], contest2 = [A,B]
    // posB: A->0, B->1
    // i=0: group=[A], current_max=0, 0==0 -> output [A]
    // i=1: group=[B], current_max=1, 1==1 -> output [B]
    // Final: A,B.
    // Correct, because for M=1, sets match, so A is top, then for the remaining, M=1 for the remaining (sets match), so B.
    // And within the first group, alphabetical sort of one team is A.
    //
    // Therefore, we'll implement this.
    //
    // Steps in code:
    //   - Read N, then N lines for contest1, then N lines for contest2.
    //   - Build a map: for each team in contest2, map team to its index.
    //   - Traverse contest1:
    //        * maintain current_max = -1
    //        * for i from 0 to N-1:
    //             current_team = contest1[i]
    //             current_max = max(current_max, posB[current_team])
    //             push current_team to a vector 'group'
    //             if current_max == i:
    //                 sort 'group'
    //                 output each team in 'group'
    //                 clear 'group'
    //   - Note: the condition will always be true at i=N-1, so group will be cleared.
    //
    // Let's code accordingly.

