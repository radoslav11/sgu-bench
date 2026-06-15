But this is for diameter where dummy not present and no removed. Works.

Use dist as long long; init -1 meaning unvisited; but distances are positive so fine. For src dist 0.

Argmax: iterate nodes 1..n.

For dp graph DFS from rootC: similar but include pseudo edges and skip removed, also produce order & children.

Let me code one DFS that also records order and children:
