import sys

def build_kmp_automaton(pattern: str):
    """
    Build KMP prefix-function and automaton transitions for alphabet {a,b}.
    Returns: go, where go[state][c] gives next state
             state in [0..m], c in {0,1} for 'a','b'
    """
    m = len(pattern)

    # Prefix-function pi[i] = length of longest proper prefix of pattern
    # which is also a suffix of pattern[:i+1]
    pi = [0] * m
    for i in range(1, m):
        j = pi[i - 1]
        while j > 0 and pattern[j] != pattern[i]:
            j = pi[j - 1]
        if pattern[j] == pattern[i]:
            j += 1
        pi[i] = j

    # Automaton transitions
    go = [[0, 0] for _ in range(m + 1)]
    for state in range(m + 1):
        for c in range(2):
            ch = 'a' if c == 0 else 'b'
            if state < m and pattern[state] == ch:
                # Successful extension
                go[state][c] = state + 1
            elif state == 0:
                # Can't fallback further
                go[state][c] = 0
            else:
                # Fallback and reuse transition
                go[state][c] = go[pi[state - 1]][c]

    return go

def solve():
    data = sys.stdin.read().strip().split()
    it = iter(data)

    n = int(next(it))        # number of nonterminals
    s = next(it)             # pattern S
    m = len(s)

    # Read grammar definitions.
    # We store each definition as a list of tokens:
    #   -1 = 'a', -2 = 'b', >=0 = referenced nonterminal index
    defs = []
    for _ in range(n):
        L = int(next(it))
        cur = []
        for _ in range(L):
            tok = next(it)
            if tok == 'a':
                cur.append(-1)
            elif tok == 'b':
                cur.append(-2)
            else:
                cur.append(int(tok) - 1)  # convert 1-based to 0-based
        defs.append(cur)

    # KMP automaton
    go = build_kmp_automaton(s)

    # dp[k][state] = (count, end_state)
    # In Python, big integers are built-in, so count is just int.
    dp_count = [[0] * (m + 1) for _ in range(n)]
    dp_state = [[0] * (m + 1) for _ in range(n)]

    # Compute DP in increasing order of nonterminal index (DAG order)
    for k in range(n):
        for start_state in range(m + 1):
            cur_state = start_state
            cnt = 0

            # Stream through definition tokens, composing results
            for elem in defs[k]:
                if elem < 0:
                    # Terminal
                    c = 0 if elem == -1 else 1
                    cur_state = go[cur_state][c]
                    if cur_state == m:
                        cnt += 1
                else:
                    # Nonterminal: add its internal matches and jump end state
                    cnt += dp_count[elem][cur_state]
                    cur_state = dp_state[elem][cur_state]

            dp_count[k][start_state] = cnt
            dp_state[k][start_state] = cur_state

    # Answer: start from nonterminal N with KMP state 0
    print(dp_count[n - 1][0])

if __name__ == "__main__":
    solve()
