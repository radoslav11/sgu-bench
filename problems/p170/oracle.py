import sys
sys.setrecursionlimit(10000)

def read_input():
    """Reads two lines from stdin: strings a and b."""
    a = sys.stdin.readline().strip()
    b = sys.stdin.readline().strip()
    return a, b

def get_positions(s):
    """
    Returns two lists:
    - pos: all indices where s[i] == '+'
    - neg: all indices where s[i] == '-'
    """
    pos, neg = [], []
    for i, ch in enumerate(s):
        if ch == '+':
            pos.append(i)
        else:
            neg.append(i)
    return pos, neg

def count_inversions(arr):
    """
    Counts inversions in arr using a merge sort variant in O(n log n).
    Returns the inversion count.
    """
    def merge_sort(a, l, r):
        if l >= r:
            return 0
        m = (l + r) // 2
        inv = merge_sort(a, l, m) + merge_sort(a, m+1, r)
        temp = []
        i, j = l, m+1
        while i <= m and j <= r:
            if a[i] <= a[j]:
                temp.append(a[i])
                i += 1
            else:
                temp.append(a[j])
                j += 1
                inv += (m - i + 1)
        # collect remains
        while i <= m:
            temp.append(a[i]); i += 1
        while j <= r:
            temp.append(a[j]); j += 1
        # write back
        for k in range(l, r+1):
            a[k] = temp[k-l]
        return inv

    # Work on a copy so as not to destroy original
    return merge_sort(list(arr), 0, len(arr)-1)

def main():
    a, b = read_input()
    # Get plus/minus positions in both strings
    pos_a, neg_a = get_positions(a)
    pos_b, neg_b = get_positions(b)

    # If counts differ, impossible
    if len(pos_a) != len(pos_b) or len(neg_a) != len(neg_b):
        print(-1)
        return

    # We want to map the ith '+' in a to the ith '+' in b, etc.
    # We'll reverse the target lists to pop from the end efficiently.
    pos_b.reverse()
    neg_b.reverse()

    # Build the permutation: for each character in a, record its target index in b
    perm = []
    for ch in a:
        if ch == '+':
            perm.append(pos_b.pop())
        else:
            perm.append(neg_b.pop())

    # Answer: inversion count of this perm
    print(count_inversions(perm))

if __name__ == "__main__":
    main()
