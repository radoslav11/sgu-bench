def next_sequence(s):
    n = len(s)
    i = n - 2
    while i >= 0 and s[i:i + 2] == "()":
        i -= 2
    if i < 0:
        return "No solution"
    while i >= 0 and s[i] == ')':
        i -= 1
    if i < 0:
        return "No solution"
    prefix = list(s[:i])
    prefix.append(')')
    open_cnt = prefix.count('(')
    close_cnt = prefix.count(')')
    half = n // 2
    rem_open = half - open_cnt
    rem_close = half - close_cnt
    suffix = ['('] * rem_open + [')'] * rem_close
    return "".join(prefix + suffix)


if __name__ == "__main__":
    import sys
    s = sys.stdin.readline().strip()
    print(next_sequence(s))
