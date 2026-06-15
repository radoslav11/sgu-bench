import sys

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n, m = map(int, (next(it), next(it)))  # number of sequences, queries

    seq = []     # list of sequences (as lists)
    has = []     # list of sets for fast membership

    # Read n sequences
    for _ in range(n):
        k = int(next(it))
        s = []
        sset = set()
        for __ in range(k):
            x = int(next(it))
            s.append(x)
            sset.add(x)
        seq.append(s)
        has.append(sset)

    # Process m queries
    out = []
    for _ in range(m):
        l = int(next(it))
        query = [int(next(it)) for __ in range(l)]

        matches = []
        for i in range(n):
            valid = True
            sset = has[i]
            # Check each term in query
            for x in query:
                if x > 0:
                    # required element
                    if x not in sset:
                        valid = False
                        break
                else:
                    # forbidden element
                    if -x in sset:
                        valid = False
                        break
            if valid:
                matches.append(i)

        # Collect output for this query
        out.append(str(len(matches)))
        for i in matches:
            s = seq[i]
            out.append(str(len(s)) + ' ' + ' '.join(map(str, s)))

    # Print all at once
    print('\n'.join(out))

if __name__ == "__main__":
    main()
