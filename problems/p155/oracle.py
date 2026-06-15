import sys

sys.setrecursionlimit(10**7)


def main():
    data = sys.stdin.read().split()
    n = int(data[0])
    nodes = []
    idx = 1
    for i in range(n):
        k = int(data[idx])
        a = int(data[idx + 1])
        idx += 2
        nodes.append((k, a, i))
    nodes.sort(key=lambda x: x[0])

    parent = [-1] * n
    leftC = [-1] * n
    rightC = [-1] * n
    stack = []

    for k, a_val, orig in nodes:
        last = -1
        while stack and stack[-1][1] > a_val:
            last = stack.pop()[2]
        if last != -1:
            parent[last] = orig
            leftC[orig] = last
        if stack:
            parent[orig] = stack[-1][2]
            rightC[stack[-1][2]] = orig
        stack.append((k, a_val, orig))

    out = ["YES"]
    for i in range(n):
        p = 0 if parent[i] < 0 else parent[i] + 1
        l = 0 if leftC[i] < 0 else leftC[i] + 1
        r = 0 if rightC[i] < 0 else rightC[i] + 1
        out.append(f"{p} {l} {r}")
    print("\n".join(out))


if __name__ == "__main__":
    main()
