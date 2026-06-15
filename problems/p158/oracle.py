import sys


def main():
    data = sys.stdin.read().split()
    pos = 0
    l = int(data[pos]); pos += 1
    m = int(data[pos]); pos += 1
    p = [int(data[pos + i]) for i in range(m)]
    pos += m
    n = int(data[pos]); pos += 1
    d = [0] + [int(data[pos + i]) for i in range(n - 1)]

    p2 = [x * 2 for x in p]
    d2 = [x * 2 for x in d]
    max_s = 2 * l - d2[-1]

    best_sum = -1
    best_s = 0
    for s in range(max_s + 1):
        total = 0
        j = 0
        for i in range(m):
            target = p2[i] - s
            while j + 1 < n and 2 * target > d2[j] + d2[j + 1]:
                j += 1
            total += abs(target - d2[j])
        if total > best_sum:
            best_sum = total
            best_s = s

    def half(x):
        return str(x // 2) if x % 2 == 0 else f"{x // 2}.5"

    print(half(best_s), half(best_sum))


if __name__ == "__main__":
    main()
