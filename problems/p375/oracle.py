import sys


def main():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    n = int(data[0])

    if n % 2 == 0:
        print("No solution")
        return

    ops = []
    while n > 1:
        a = (n - 1) // 2
        b = (n + 1) // 2

        if a % 2 == 1:
            ops.append(2)
            n = a
        else:
            ops.append(1)
            n = b

    ops.reverse()

    print(len(ops))
    if ops:
        print(" ".join(map(str, ops)))


if __name__ == "__main__":
    main()
