def process_bhtml(s):
    """
    Parse the BHTML string s and return the rendered text:
      <UP>...</UP> => uppercase
      <DOWN>...</DOWN> => lowercase
    Nested tags are allowed; innermost takes precedence.
    """
    i = 0
    n = len(s)
    stack = []       # will store 'UP' or 'DOWN'
    output = []      # list of characters for the result

    while i < n:
        # Check for each possible tag at position i
        if s.startswith("<UP>", i):
            stack.append("UP")
            i += 4
        elif s.startswith("</UP>", i):
            # must match the last pushed
            stack.pop()
            i += 5
        elif s.startswith("<DOWN>", i):
            stack.append("DOWN")
            i += 6
        elif s.startswith("</DOWN>", i):
            stack.pop()
            i += 7
        else:
            # normal character
            c = s[i]
            if stack and stack[-1] == "UP":
                output.append(c.upper())
            elif stack and stack[-1] == "DOWN":
                output.append(c.lower())
            else:
                output.append(c)
            i += 1

    return "".join(output)

if __name__ == "__main__":
    import sys
    data = sys.stdin.read().strip()
    # only one line of input
    print(process_bhtml(data))
