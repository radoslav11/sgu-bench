import sys

def solve():
    data = sys.stdin.read().strip().split()
    n = int(data[0])
    s = data[1]
    m = 2 * n - 1

    # fence: current simulated fence contents
    # target: canonical palindrome for current odd length
    fence = [' '] * m
    target = [' '] * m

    q_count = 0
    c_count = 0

    actions = []  # list of (a,b), 1-indexed swaps, or (0,0) pass

    def add_letter(ch: str):
        nonlocal q_count, c_count
        if ch == 'Q':
            q_count += 1
        else:
            c_count += 1

    # Initial move: He writes s[0], Qc must pass
    fence[0] = s[0]
    add_letter(s[0])
    actions.append((0, 0))

    # Process stages i = 1..n-1 (each stage adds two letters)
    for i in range(1, n):
        c_even = s[2 * i - 1]
        c_odd = s[2 * i]

        fence[2 * i - 1] = c_even
        fence[2 * i] = c_odd

        add_letter(c_even)
        add_letter(c_odd)

        # --- build canonical target palindrome for length 2*i+1 ---

        # center forced by which count is odd
        target[i] = 'Q' if (q_count & 1) else 'C'

        rem_q = q_count
        rem_c = c_count

        # remove one instance of the center letter
        if rem_q & 1:
            rem_q -= 1
        else:
            rem_c -= 1

        # alternating region length (even number)
        alt = min(rem_q, rem_c)

        # fill alternating QC... from outside in, symmetrically
        j = 0
        while j < alt:
            target[j] = 'Q'
            target[2 * i - j] = 'Q'
            target[j + 1] = 'C'
            target[2 * i - j - 1] = 'C'
            j += 2

        # fill the remaining symmetric middle area with the majority letter
        filler = 'Q' if rem_q > rem_c else 'C'
        diff = abs(rem_q - rem_c)  # even

        # each step places 2 filler letters (one on each side)
        j = alt
        while diff > 0:
            target[j] = filler
            target[2 * i - j] = filler
            j += 1
            diff -= 2

        # --- find mismatches in prefix [0..2*i] ---
        mismatches = []
        for pos in range(0, 2 * i + 1):
            if fence[pos] != target[pos]:
                mismatches.append(pos + 1)  # 1-indexed for output

        if len(mismatches) > 4:
            sys.stdout.write("He\n")
            return

        # schedule two actions (even then odd turn)
        if len(mismatches) == 0:
            actions.append((0, 0))
            actions.append((0, 0))
        elif len(mismatches) == 2:
            actions.append((0, 0))
            actions.append((mismatches[0], mismatches[1]))
        else:  # len == 4
            actions.append((mismatches[0], mismatches[1]))
            actions.append((mismatches[2], mismatches[3]))

        # commit: after these two turns, fence becomes target on this prefix
        for pos in range(0, 2 * i + 1):
            fence[pos] = target[pos]

    # Output actions
    out = ["Qc"]
    out.extend(f"{a} {b}" for (a, b) in actions)
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    solve()
