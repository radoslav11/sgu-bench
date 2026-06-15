import sys
from collections import deque, defaultdict


def count_inversions(arr):
    n = len(arr)
    temp = [0] * n
    inv = 0

    width = 1
    while width < n:
        for l in range(0, n, 2 * width):
            mid = min(l + width, n)
            r = min(l + 2 * width, n)

            i, j, k = l, mid, l
            while i < mid and j < r:
                if arr[i] <= arr[j]:
                    temp[k] = arr[i]
                    i += 1
                else:
                    temp[k] = arr[j]
                    j += 1
                    inv += mid - i
                k += 1

            while i < mid:
                temp[k] = arr[i]
                i += 1
                k += 1
            while j < r:
                temp[k] = arr[j]
                j += 1
                k += 1

            arr[l:r] = temp[l:r]

        width *= 2

    return inv


def solve(s: str) -> int:
    n = len(s)

    pos = defaultdict(deque)
    for i, ch in enumerate(s):
        pos[ch].append(i)

    cnt_odd = sum((len(d) % 2) for d in pos.values())
    if cnt_odd != (n % 2):
        return -1

    p = [0] * n
    used = [False] * n

    left_final = 0
    right_final = n - 1

    for r in range(n - 1, -1, -1):
        if used[r]:
            continue

        used[r] = True
        ch = s[r]

        if len(pos[ch]) == 1:
            p[n // 2] = r
            pos[ch].pop()
            continue

        l = pos[ch][0]
        pos[ch].popleft()
        pos[ch].pop()

        used[l] = True

        p[left_final] = l
        p[right_final] = r
        left_final += 1
        right_final -= 1

    return count_inversions(p)


if __name__ == "__main__":
    s = sys.stdin.readline().strip()
    print(solve(s))
