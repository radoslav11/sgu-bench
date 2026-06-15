import sys
input = sys.stdin.readline


def solve_case():
    n = int(input())
    a = list(map(int, input().split()))

    up_left = [0] * n
    down_left = [0] * n
    up_right = [0] * n
    down_right = [0] * n

    for i in range(1, n):
        if a[i] > a[i - 1]:
            up_left[i] = up_left[i - 1] + 1
        if a[i] < a[i - 1]:
            down_left[i] = down_left[i - 1] + 1

    for i in range(n - 2, -1, -1):
        if a[i] > a[i + 1]:
            up_right[i] = up_right[i + 1] + 1
        if a[i] < a[i + 1]:
            down_right[i] = down_right[i + 1] + 1

    max_hill = 0
    max_dale = 0

    for i in range(n):
        if up_left[i] > 0 and up_right[i] > 0:
            height = min(up_left[i], up_right[i])
            if height > max_hill:
                max_hill = height
        if down_left[i] > 0 and down_right[i] > 0:
            depth = min(down_left[i], down_right[i])
            if depth > max_dale:
                max_dale = depth

    print(max_hill, max_dale)


def main():
    T = int(input())
    for _ in range(T):
        solve_case()


if __name__ == "__main__":
    main()
