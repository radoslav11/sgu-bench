import sys

def opponent(c: str) -> str:
    """Return the competitor letter."""
    return 'B' if c == 'A' else 'A'

def main() -> None:
    # Read input lines; they may contain trailing newlines/spaces.
    data = [line.strip() for line in sys.stdin if line.strip() != ""]
    init = data[0]          # 'A' or 'B'
    s = data[1]             # final filled stands string

    # Stack of pending nonterminals (empty stands to be resolved).
    # We represent S_A as 'A' and S_B as 'B'.
    stack = [init]

    # Predictive parsing / simulation:
    for ch in s:
        # If there is no pending empty stand to produce the next filled stand,
        # the string cannot be generated.
        if not stack:
            print("NO")
            return

        top = stack.pop()  # Get the nonterminal we must expand/match now

        if ch == top:
            # Expand:
            # S_A -> 'A' S_B S_A
            # S_B -> 'B' S_A S_B
            #
            # We already consumed the leading terminal (ch),
            # so we must still generate: S_opponent(top) then S_top.
            # Using a stack, push in reverse order so opponent is processed first.
            stack.append(top)
            stack.append(opponent(top))
        else:
            # Must be the terminal production:
            # S_A -> 'B'  or  S_B -> 'A'
            # We consume ch and push nothing.
            # If ch is neither top nor opponent(top), it would be invalid,
            # but since alphabet is only A/B, it's implicitly handled.
            pass

    # After producing all characters, all nonterminals must be resolved.
    print("YES" if not stack else "NO")

if __name__ == "__main__":
    main()
