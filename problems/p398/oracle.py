def main():
    import sys
    data = sys.stdin.read().split()
    it = iter(data)

    # Read number of users n and target user x
    n = int(next(it))
    x = int(next(it))

    # Build adjacency sets for each user
    # friends[i] is a set of direct friends of i
    friends = [set() for _ in range(n+1)]
    for i in range(1, n+1):
        d = int(next(it))
        for _ in range(d):
            f = int(next(it))
            friends[i].add(f)
            friends[f].add(i)

    result = []
    # Examine each candidate user u
    for u in range(1, n+1):
        if u == x:
            continue                  # skip x itself
        if u in friends[x]:
            continue                  # skip direct friends of x

        # Check if u shares any mutual friend with x
        # i.e., intersection of friends[x] and friends[u] is non-empty
        if friends[x].intersection(friends[u]):
            result.append(u)

    # Output count and sorted list
    result.sort()
    print(len(result))
    if result:
        print(*result)

if __name__ == "__main__":
    main()
