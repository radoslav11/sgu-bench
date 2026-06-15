import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    nums = list(map(int, data[1:1 + n]))

    M = 1 << 20
    used = bytearray(M)
    for x in nums:
        used[x] = 1

    answer = 1
    for d in range(1, M):
        cnt = 0
        for j in range(d, M, d):
            if used[j]:
                cnt += 1
                if cnt >= 2:
                    if d > answer:
                        answer = d
                    break
    print(answer)


if __name__ == "__main__":
    main()
