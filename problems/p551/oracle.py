import sys


def main():
    data = sys.stdin.read().split()
    n, t1, t2 = map(int, data)

    def finished(x):
        return x // t1 + x // t2

    low, high = 0, n * min(t1, t2)
    ret = high
    while low <= high:
        mid = (low + high) // 2
        if finished(mid) >= n:
            ret = mid
            high = mid - 1
        else:
            low = mid + 1

    total = finished(ret)
    last_time = ret

    if ret % t1 != 0:
        finishA = ret + (t1 - ret % t1)
        last_time = max(last_time, finishA)
        total += 1

    if ret % t2 != 0:
        finishB = ret + (t2 - ret % t2)
        last_time = max(last_time, finishB)
        total += 1

    print(total, last_time)


if __name__ == "__main__":
    main()
