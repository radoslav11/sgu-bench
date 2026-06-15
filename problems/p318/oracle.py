import sys


def main():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)

    n = int(next(it))
    m = int(next(it))

    resource_users = [[] for _ in range(n + 1)]

    for u in range(1, m + 1):
        k = int(next(it))
        for _ in range(k):
            r = int(next(it))
            resource_users[r].append(u)

    unique_groups = set()
    for r in range(1, n + 1):
        if resource_users[r]:
            users = tuple(sorted(resource_users[r]))
            unique_groups.add(users)

    print(len(unique_groups))


if __name__ == "__main__":
    main()
