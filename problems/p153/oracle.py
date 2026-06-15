import sys

def winner(n, moves):
    # moves: sorted list of allowed removal sizes, including 1
    # dp[i] = 1 if winning, 0 if losing for the player to move at size i.
    # Base: dp[0] = 1 (opponent just lost by taking last), dp[1] = 0.

    pos_of_state = {}   # state_mask -> first index i
    dp = [1, 0]         # initial dp[0], dp[1]

    if n <= 1:
        return dp[n]

    limit = min(n, (1 << 13) - 1)
    for i in range(2, limit + 1):
        win = 0
        for s in moves:
            if s > i:
                break
            if dp[i - s] == 0:
                win = 1
                break
        dp.append(win)

        # Build 10-bit signature of dp[i], dp[i-1], ..., dp[i-9]
        st = 0
        for b in range(10):
            if i - b < 0 or dp[i - b] == 1:
                st |= 1 << b

        if st not in pos_of_state:
            pos_of_state[st] = i
        else:
            first_i = pos_of_state[st]
            cycle_len = i - first_i
            idx = first_i + ((n - first_i) % cycle_len)
            return dp[idx]

    return dp[n]

def main():
    data = sys.stdin.read().strip().split()
    t = int(data[0])
    ptr = 1
    out = []
    for _ in range(t):
        n = int(data[ptr]); ptr += 1
        m = int(data[ptr]); ptr += 1
        p = list(map(int, data[ptr:ptr+m]))
        ptr += m
        moves = sorted(set(p + [1]))
        w = winner(n, moves)
        out.append("FIRST PLAYER MUST WIN" if w else "SECOND PLAYER MUST WIN")
    print("\n".join(out))

if __name__ == "__main__":
    main()
