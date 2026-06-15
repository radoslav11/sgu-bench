import sys


def main():
    data = sys.stdin.read().split()
    pos = 0
    k = int(data[pos])
    pos += 1
    out = []
    for _ in range(k):
        n = int(data[pos])
        pos += 1
        a = [int(x) for x in data[pos:pos + n]]
        pos += n

        # The expression is a1 + a1*a2 + ... ; it is 0 iff a1 == 0,
        # otherwise its digital root is its value mod 9 (with 0 -> 9).
        if a[0] == 0:
            out.append(0)
            continue
        ans = 0
        s = 1
        for v in a:
            s = s * (v % 9) % 9
            ans = (ans + s) % 9
        out.append(9 if ans == 0 else ans)

    print("\n".join(map(str, out)))


if __name__ == "__main__":
    main()
