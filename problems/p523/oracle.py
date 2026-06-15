import sys


def main():
    data = sys.stdin.read().split()
    n, cur = int(data[0]), int(data[1])
    pending = list(map(int, data[2 : 2 + n]))

    tgt = pending[0]
    ans = []
    while pending:
        if cur in pending:
            ans.append(cur)
            was_target = cur == tgt
            pending.remove(cur)
            if not pending:
                break
            if was_target:
                tgt = pending[0]
        if cur < tgt:
            cur += 1
        elif cur > tgt:
            cur -= 1

    print(" ".join(map(str, ans)))


if __name__ == "__main__":
    main()
