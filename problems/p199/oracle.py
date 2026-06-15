
import sys
import bisect

def main():
    input = sys.stdin.readline
    n = int(input())
    members = []
    for i in range(n):
        s, b = map(int, input().split())
        members.append((s, b, i+1))  # store 1-based index

    # 1) Sort by strength asc, beauty desc when strengths tie
    members.sort(key=lambda x: (x[0], -x[1]))

    # Arrays for LIS
    lis = []          # will store tail beauties
    lis_idx = []      # will store which member index ends that LIS length
    prev = [-1] * n   # back-pointer to reconstruct path

    # 2) Build LIS on beauty dimension
    for i, (s, b, orig) in enumerate(members):
        # find place in lis for b
        pos = bisect.bisect_left(lis, b)
        if pos == len(lis):
            # extending the LIS
            lis.append(b)
            lis_idx.append(i)
        else:
            # replace to get smaller tail
            lis[pos] = b
            lis_idx[pos] = i

        # set back-pointer: if pos > 0, link to end of previous length
        if pos > 0:
            prev[i] = lis_idx[pos-1]
        else:
            prev[i] = -1

    # 3) Reconstruct the LIS sequence of original indices
    length = len(lis)
    seq = []
    # start from the last index in lis_idx
    idx = lis_idx[-1]
    while idx != -1:
        seq.append(members[idx][2])  # original index
        idx = prev[idx]
    seq.reverse()

    # 4) Output
    print(len(seq))
    print(*seq)

if __name__ == "__main__":
    main()
