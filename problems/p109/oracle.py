def magic_trick(n):
    turns = []

    line1 = [n]
    for i in range(n):
        for j in range(n):
            if i + j > n:
                line1.append(i * n + j + 1)
    turns.append(line1)

    k = n + 1
    if k % 2 == 0:
        k += 1

    for step in range(n):
        current = [k]
        diag_sum = n - step
        if step == 0:
            i, j = 1, n - 1
        else:
            i, j = 0, diag_sum
        while 0 <= j < n and 0 <= i < n:
            current.append(i * n + j + 1)
            i += 1
            j -= 1
        turns.append(current)
        k += 2

    return turns


def main():
    import sys

    n = int(sys.stdin.read().split()[0])
    for line in magic_trick(n):
        print(" ".join(map(str, line)))


if __name__ == "__main__":
    main()
