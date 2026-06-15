import sys


def parse(s: str) -> str:
    # Stack entries are tuples: (closing_tag, auto_close)
    # auto_close=True means this script was of the form ^x / _x (single char),
    # so it should be closed immediately after emitting the next atom.
    close_stack = []
    out_parts = []

    i = 0
    n = len(s)
    while i < n:
        c = s[i]

        # '{' starts a grouped script content (^{...} or _{...})
        # Do not output braces.
        if c == '{':
            if close_stack:
                # If a script has just been opened, it is not single-character anymore.
                closing, _ = close_stack[-1]
                close_stack[-1] = (closing, False)
            i += 1
            continue

        # '}' ends a grouped script: output closing tag
        if c == '}':
            if close_stack:
                closing, _ = close_stack.pop()
                out_parts.append(closing)
            i += 1
            continue

        # Superscript/subscript opening
        if c == '^' or c == '_':
            if c == '^':
                out_parts.append("<sup>")
                close_stack.append(("</sup>", True))
            else:
                out_parts.append("<sub>")
                close_stack.append(("</sub>", True))
            i += 1
            continue

        # Letters: italicize maximal run
        if c.isalpha():
            out_parts.append("<i>")
            j = i
            while j < n and s[j].isalpha():
                out_parts.append(s[j])
                j += 1
            out_parts.append("</i>")
            i = j
        # Operators: surround with &nbsp;
        elif c in "+-*/":
            out_parts.append("&nbsp;")
            out_parts.append(c)
            out_parts.append("&nbsp;")
            i += 1
        # Digits, parentheses, etc.
        else:
            out_parts.append(c)
            i += 1

        # Auto-close any single-character scripts after emitting one atom
        while close_stack and close_stack[-1][1] is True:
            closing, _ = close_stack.pop()
            out_parts.append(closing)

    return "".join(out_parts)


def sanitize_line(line: str) -> str:
    # Keep only relevant characters; this removes '$' and whitespace automatically.
    allowed = set("+-*/^_{}()")
    res = []
    for ch in line:
        if ch.isalnum() or ch in allowed:
            res.append(ch)
    return "".join(res)


def main():
    out = []
    for line in sys.stdin:
        line = line.rstrip("\n")
        s = sanitize_line(line)
        out.append(parse(s))
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
