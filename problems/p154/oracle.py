import sys


def eval_zeros(x):
    total = 0
    power = 5
    while power <= x:
        total += x // power
        power *= 5
    return total


def find_min_n(q):
    low, high = 1, 2 * 10**12
    answer = -1
    while low <= high:
        mid = (low + high) // 2
        if eval_zeros(mid) >= q:
            answer = mid
            high = mid - 1
        else:
            low = mid + 1
    return answer


def main():
    data = sys.stdin.read().strip()
    if not data:
        return
    q = int(data)
    n = find_min_n(q)
    if n == -1 or eval_zeros(n) != q:
        print("No solution.")
    else:
        print(n)


if __name__ == "__main__":
    main()
