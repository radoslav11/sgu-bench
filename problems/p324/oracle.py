import sys

def is_letter(c: str) -> bool:
    """Return True if c is an ASCII Latin letter."""
    return ('a' <= c <= 'z') or ('A' <= c <= 'Z')

def dash_string(k: int) -> str:
    """
    Convert k consecutive '-' into a valid dash representation.

    k=0 -> ""
    k=1 -> "-" (hyphen; only valid between two words)
    k>=2 -> combination of short dash " -- " (2 hyphens) and optionally one long dash " --- " (3 hyphens)
    """
    if k == 0:
        return ""
    if k == 1:
        return "-"

    # Build a canonical representation.
    # If k is odd, use one long dash first; else use one short dash first.
    if k % 2 == 1:
        res = " --- "
        k -= 3
    else:
        res = " -- "
        k -= 2

    # Remaining k is even; each short dash contributes 2 hyphens.
    while k > 0:
        res += "-- "
        k -= 2

    return res

def solve_one(line: str) -> str:
    """
    Solve for one quoted input line.
    Return either 'error' or a quoted formatted string.
    """
    line = line.rstrip("\r\n")

    # Validate and strip surrounding quotes
    if len(line) >= 2 and line[0] == '"' and line[-1] == '"':
        text = line[1:-1]
    else:
        # Invalid input format per statement; treat as impossible
        return "error"

    words = []   # extracted words
    dashes = []  # dash counts before/between/after words

    dash_count = 0
    i = 0

    # Parse into words and dash counts, ignoring spaces
    while i < len(text):
        c = text[i]
        if c == ' ':
            i += 1
        elif c == '-':
            dash_count += 1
            i += 1
        elif is_letter(c):
            # store dash count before this word
            dashes.append(dash_count)
            dash_count = 0

            # read the full word
            j = i
            while j < len(text) and is_letter(text[j]):
                j += 1
            words.append(text[i:j])
            i = j
        else:
            # any other character is invalid
            return "error"

    # dash count after last word
    dashes.append(dash_count)

    # If there are no words, the whole string is only spaces and hyphens
    if not words:
        if dash_count == 1:
            return "error"              # single hyphen cannot stand alone
        elif dash_count == 0:
            return '""'                 # empty after formatting
        else:
            return '"' + dash_string(dash_count) + '"'

    n = len(words)

    # Single hyphen cannot be at start or end (needs words on both sides)
    if dashes[0] == 1 or dashes[n] == 1:
        return "error"

    out_parts = []

    # leading dashes
    if dashes[0] > 0:
        out_parts.append(dash_string(dashes[0]))

    # first word
    out_parts.append(words[0])

    # between words
    for j in range(1, n):
        k = dashes[j]
        if k == 0:
            out_parts.append(" ")
        elif k == 1:
            out_parts.append("-")
        else:
            out_parts.append(dash_string(k))
        out_parts.append(words[j])

    # trailing dashes
    if dashes[n] > 0:
        out_parts.append(dash_string(dashes[n]))

    return '"' + "".join(out_parts) + '"'

def main():
    data = sys.stdin.read().splitlines()
    if not data:
        return
    t = int(data[0].strip())
    ans = []
    # Next t lines are the quoted texts (may be empty inside quotes, but line exists)
    for idx in range(1, 1 + t):
        ans.append(solve_one(data[idx] if idx < len(data) else ""))
    sys.stdout.write("\n".join(ans))

if __name__ == "__main__":
    main()
