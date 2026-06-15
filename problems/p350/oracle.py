import math
import sys


def main():
    data = sys.stdin.read().split()
    m = int(data[0])
    Bs = list(map(int, data[1:]))

    n = int((1 + math.isqrt(1 + 8 * m)) // 2)

    O = set(Bs)

    ans = [0]
    pedo = [0]
    L = {0}

    for b in Bs:
        if b in L:
            continue

        valid = True
        for y in ans:
            if (b ^ y) not in O:
                valid = False
                break
        if not valid:
            continue

        old_pedo = list(pedo)
        for s in old_pedo:
            t = b ^ s
            if t not in L:
                L.add(t)
                pedo.append(t)

        ans.append(b)
        if len(ans) == n:
            break

    print(" ".join(map(str, ans)))


if __name__ == "__main__":
    main()
