import sys


def main():
    data = sys.stdin.buffer.read().split()
    n, k = int(data[0]), int(data[1])
    queries = [int(x) for x in data[2:2 + k]]

    # generated[i] = 1 iff i = j + digitsum(j) for some j <= n.
    limit = n + 64 + 1
    generated = bytearray(limit + 1)
    small = [0] * 1000
    for i in range(1, 1000):
        small[i] = small[i // 10] + i % 10
    ds_high = 0
    for base in range(0, n + 1, 1000):
        if base:
            ds_high = small[(base // 1000) % 1000] + small[base // 1000000]
        top = min(999, n - base)
        for r in range(0 if base else 1, top + 1):
            generated[base + r + ds_high + small[r]] = 1

    want = set(queries)
    answer_at = {}
    cnt = 0
    for i in range(1, n + 1):
        if not generated[i]:
            cnt += 1
            if cnt in want:
                answer_at[cnt] = i

    out = [str(cnt), " ".join(str(answer_at[q]) for q in queries)]
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
