import sys


def main():
    data = sys.stdin.read().split()
    n = int(data[0])
    cities = []
    for i in range(n):
        x = int(data[1 + 2 * i])
        p = int(data[2 + 2 * i])
        cities.append((x, p))
    cities.sort()

    total = sum(p for _, p in cities)
    acc = 0
    for x, p in cities:
        acc += p
        if 2 * acc >= total:
            print(f"{x:.5f}")
            return


if __name__ == "__main__":
    main()
