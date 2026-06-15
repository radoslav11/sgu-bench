import sys
data = sys.stdin.read().split()
# Parse input
n = int(data[0])
# Store levels as tuples (W, L, P)
levels = []
ptr = 1
for _ in range(n):
    W = int(data[ptr]); L = int(data[ptr+1]); P = int(data[ptr+2])
    ptr += 3
    levels.append((W, L, P))

best_cost = float('inf')
best_start = 0

# Try each starting level s
for s in range(n):
    w = 0       # accumulated water
    cost = 0    # total paid cost starting at s

    # Cascade simulation from s to n-1
    for i in range(s, n):
        W, L, P = levels[i]
        w += W
        if w <= L:
            # if it wouldn't leak automatically, pay cost
            cost += P
        # if w > L, it leaks automatically, no cost
        if w > 15000:
            # further L's are ≤ 15000, so all will leak automatically
            break

    # Keep the best starting point
    if cost < best_cost:
        best_cost = cost
        best_start = s

# Re-simulate from best_start to list the paid levels
w = 0
result = []
for i in range(best_start, n):
    W, L, P = levels[i]
    w += W
    if w <= L:
        # we paid at this level
        result.append(str(i+1))  # convert to 1-based index

# Print results, one index per line
sys.stdout.write("\n".join(result))

