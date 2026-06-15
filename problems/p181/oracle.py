import sys


def main():
    data = sys.stdin.read().split()
    A, alpha, beta, gamma, M, k = map(int, data)

    if k == 0:
        print(A)
        return

    def f(x):
        return (alpha * x * x + beta * x + gamma) % M

    first = [-1] * M
    x = A % M
    steps = 0

    while k > 0 and first[x] == -1:
        first[x] = steps
        steps += 1
        x = f(x)
        k -= 1

    if k == 0:
        print(x)
        return

    cycle_start = first[x]
    cycle_len = steps - cycle_start

    k %= cycle_len

    for _ in range(k):
        x = f(x)

    print(x)


if __name__ == "__main__":
    main()
