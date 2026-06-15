import sys

def lower(s: str) -> str:
    """ASCII-lowercase version for case-insensitive comparisons."""
    return s.lower()

def split_words(s: str) -> list[str]:
    """Split sentence into words by whitespace."""
    # Input guarantees single spaces between words, but split() is fine.
    return s.split()

def solve() -> None:
    # Read entire input (may include newlines due to wrapping in files)
    lines = sys.stdin.read().splitlines()

    # Concatenate lines with spaces between them (but avoid double spaces)
    text_parts = []
    for line in lines:
        if not line:
            # Problem statements typically won't contain empty lines,
            # but if they do, treating them as separators is safest.
            continue
        text_parts.append(line)
    text = " ".join(text_parts)

    # Helpers for trigger words (case-insensitive match)
    def is_not_word(w: str) -> bool:
        w = lower(w)
        return w in ("can", "may", "must", "should")

    def is_no_word(w: str) -> bool:
        w = lower(w)
        return w in ("is", "are")

    def is_article(w: str) -> bool:
        w = lower(w)
        return w in ("a", "the")

    # Split into sentences by '.'
    # The dot is not part of words; we will re-add it on output.
    sentences = []
    cur = []
    for ch in text:
        if ch == ".":
            sentences.append("".join(cur))
            cur = []
        else:
            cur.append(ch)

    results: list[list[str]] = []

    # Process each sentence independently, scanning word positions left-to-right
    for sent in sentences:
        words = split_words(sent)
        n = len(words)

        for i in range(n):
            if not (is_not_word(words[i]) or is_no_word(words[i])):
                continue

            out = words[: i + 1]  # copy up to and including i

            if is_no_word(words[i]):
                out.append("no")
                start = i + 1
                # If next word is an article, remove it by skipping
                if start < n and is_article(words[start]):
                    start += 1
                out.extend(words[start:])
            else:
                out.append("not")
                out.extend(words[i + 1 :])

            results.append(out)

    # Print results
    sys.stdout.write(str(len(results)) + "\n")
    for words in results:
        sys.stdout.write(" ".join(words) + ".\n")

if __name__ == "__main__":
    solve()
