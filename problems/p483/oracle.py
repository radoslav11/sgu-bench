import sys


def solve() -> None:
    data = sys.stdin.read().strip().split()
    n = int(data[0])
    h = int(data[1])

    pos = (h + 1) * (h + 1)
    if pos < 1:
        pos = 1

    step = h + 1

    front = []
    back = [pos]

    while pos + step <= n:
        pos += step
        if pos <= n:
            front.append(pos)

        pos += step
        if pos <= n:
            back.append(pos)

        step += 1

    front.reverse()

    ans = front + back
    sys.stdout.write(" ".join(map(str, ans)) + "\n")


if __name__ == "__main__":
    solve()
