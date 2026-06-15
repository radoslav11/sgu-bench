import sys

def strip_spaces(s: str) -> str:
    # Match the C++ behavior: remove only normal spaces, not tabs.
    return s.replace(" ", "")

def parse_substr_args(s: str):
    # s is inside substr(...), with spaces already removed:
    # "name,offset" or "name,offset,count"
    comma1 = s.find(",")
    name = s[:comma1]
    rest = s[comma1 + 1:]

    comma2 = rest.find(",")
    if comma2 == -1:
        return name, int(rest), 0, False
    offset = int(rest[:comma2])
    count = int(rest[comma2 + 1:])
    return name, offset, count, True

def get_substr(st: str, offset: int, count: int, has_count: bool) -> str:
    # Implements the statement's substring semantics
    n = len(st)
    start = offset if offset >= 0 else n + offset

    if not has_count:
        end = n
    elif count >= 0:
        end = start + count
    else:
        end = n + count  # leave -count chars off the end

    return st[start:end]

def main():
    data = sys.stdin.read().splitlines()
    if not data:
        return

    # First line: N M
    first = data[0].split()
    n = int(first[0])
    m = int(first[1])

    # Collect all non-blank lines after the first line (blank = only whitespace)
    all_lines = []
    for line in data[1:]:
        if any(c not in " \t\r" for c in line):
            all_lines.append(line)

    init_lines = all_lines[:n]
    prog_lines = all_lines[n:n+m]

    # Variable table; missing variables are empty strings
    vars = {}

    # Parse initializations: $name = "value";
    for line in init_lines:
        # Find variable name starting at '$'
        d = line.find("$")
        i = d
        name_chars = []
        while i < len(line) and (line[i].isalnum() or line[i] in "$_"):
            name_chars.append(line[i])
            i += 1
        name = "".join(name_chars)

        # Extract text between the first and last double quote
        q1 = line.find('"')
        q2 = line.rfind('"')
        vars[name] = line[q1 + 1:q2]

    out_lines = []

    for raw in prog_lines:
        line = strip_spaces(raw)
        if not line or line == ";":
            continue

        if line.startswith("print("):
            # content between print( and );
            content = line[6:-2]

            if content.startswith("substr("):
                inner = content[7:-1]
                name, offset, count, has_count = parse_substr_args(inner)
                st = vars.get(name, "")
                out_lines.append(get_substr(st, offset, count, has_count))
            else:
                out_lines.append(vars.get(content, ""))

        elif line.startswith("substr("):
            # LHS: substr(...)=...
            close = line.find(")")
            lhs_inner = line[7:close]                 # inside lhs substr(...)
            rhs = line[close + 2:-1]                  # after ")=" up to ';'

            name1, offset1, count1, has_count1 = parse_substr_args(lhs_inner)

            # Evaluate RHS expression to a string
            if rhs.startswith("substr("):
                rhs_inner = rhs[7:-1]
                name2, offset2, count2, has_count2 = parse_substr_args(rhs_inner)
                new_value = get_substr(vars.get(name2, ""), offset2, count2, has_count2)
            else:
                new_value = vars.get(rhs, "")

            # Replace substring in vars[name1]
            st = vars.get(name1, "")
            nlen = len(st)
            start = offset1 if offset1 >= 0 else nlen + offset1

            if not has_count1:
                end = nlen
            elif count1 >= 0:
                end = start + count1
            else:
                end = nlen + count1

            vars[name1] = st[:start] + new_value + st[end:]

        else:
            # Assignment: name1=...
            eq = line.find("=")
            lhs = line[:eq]
            rhs = line[eq + 1:-1]  # strip trailing ';'

            if rhs.startswith("substr("):
                rhs_inner = rhs[7:-1]
                name2, offset2, count2, has_count2 = parse_substr_args(rhs_inner)
                vars[lhs] = get_substr(vars.get(name2, ""), offset2, count2, has_count2)
            else:
                vars[lhs] = vars.get(rhs, "")

    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    main()
