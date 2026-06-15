import sys
import random
from array import array

# ------------------------------------------------------------
# Fast scanner.
# It reads the whole input once and parses tokens manually.
# This avoids creating a huge list with split().
# ------------------------------------------------------------

class FastScanner:
    def __init__(self):
        self.data = sys.stdin.buffer.read()
        self.i = 0
        self.n = len(self.data)

    def next_token(self):
        data = self.data
        n = self.n
        i = self.i

        while i < n and data[i] <= 32:
            i += 1

        j = i
        while j < n and data[j] > 32:
            j += 1

        self.i = j
        return data[i:j]

    def next_int(self):
        data = self.data
        n = self.n
        i = self.i

        while i < n and data[i] <= 32:
            i += 1

        x = 0
        while i < n and data[i] > 32:
            x = x * 10 + data[i] - 48
            i += 1

        self.i = i
        return x


# ------------------------------------------------------------
# Treap stored in compact arrays.
#
# Node index 0 means null.
#
# For node v:
#   key[v]   = starting cell of tower
#   end[v]   = ending cell of tower
#   sumv[v]  = total cubes in tower
#   left[v]  = left child
#   right[v] = right child
#   size[v]  = subtree size
#   prio[v]  = random priority
# ------------------------------------------------------------

key = array("i", [0])
end = array("i", [0])
sumv = array("q", [0])
left = array("i", [0])
right = array("i", [0])
size = array("i", [0])
prio = array("Q", [0])

# Deterministic xorshift RNG for treap priorities.
seed = 88172645463393265


def rng():
    global seed
    seed ^= (seed << 7) & ((1 << 64) - 1)
    seed ^= seed >> 9
    return seed & ((1 << 64) - 1)


def new_node(k, r, s):
    """Create a new treap node and return its index."""
    idx = len(key)

    key.append(k)
    end.append(r)
    sumv.append(s)
    left.append(0)
    right.append(0)
    size.append(1)
    prio.append(rng())

    return idx


def pull(v):
    """Recalculate subtree size of node v."""
    size[v] = 1 + size[left[v]] + size[right[v]]


sys.setrecursionlimit(2_000_000)


def split(t, k):
    """
    Split treap t into:
      a: all nodes with key <= k
      b: all nodes with key > k
    """
    if t == 0:
        return 0, 0

    if k < key[t]:
        a, b = split(left[t], k)
        left[t] = b
        pull(t)
        return a, t
    else:
        a, b = split(right[t], k)
        right[t] = a
        pull(t)
        return t, b


def merge(a, b):
    """Merge two treaps where every key in a is <= every key in b."""
    if a == 0 or b == 0:
        return a or b

    if prio[a] > prio[b]:
        right[a] = merge(right[a], b)
        pull(a)
        return a
    else:
        left[b] = merge(a, left[b])
        pull(b)
        return b


def insert(t, v):
    """Insert node v into treap t and return new root."""
    if t == 0:
        return v

    if prio[v] > prio[t]:
        a, b = split(t, key[v])
        left[v] = a
        right[v] = b
        pull(v)
        return v

    if key[v] < key[t]:
        left[t] = insert(left[t], v)
    else:
        right[t] = insert(right[t], v)

    pull(t)
    return t


def erase(t, k):
    """Erase node with key k from treap t and return new root."""
    if key[t] == k:
        return merge(left[t], right[t])

    if k < key[t]:
        left[t] = erase(left[t], k)
    else:
        right[t] = erase(right[t], k)

    pull(t)
    return t


def kth(t, k):
    """Return index of zero-based k-th node by key order."""
    while t:
        ls = size[left[t]]

        if k < ls:
            t = left[t]
        elif k == ls:
            return t
        else:
            k -= ls + 1
            t = right[t]

    return 0


def find_node(t, k):
    """Find node with exact key k."""
    while t:
        if key[t] == k:
            return t
        if k < key[t]:
            t = left[t]
        else:
            t = right[t]
    return 0


def pred_node(t, x):
    """Find node with maximum key <= x."""
    ans = 0

    while t:
        if key[t] <= x:
            ans = t
            t = right[t]
        else:
            t = left[t]

    return ans


# ------------------------------------------------------------
# Main command processing.
# ------------------------------------------------------------

def main():
    scanner = FastScanner()

    n = scanner.next_int()

    # height[x] = cubes in global cell x.
    # Use 1_000_002 so x-1 and x+1 are safe for x in [1, 10^6].
    height = array("i", [0]) * 1_000_002

    root = 0

    out = []

    def ordinal(x):
        return str(x) + "th"

    def do_put(x, c):
        nonlocal root

        # If cell already belongs to a tower, only heights and sums change.
        if height[x] > 0:
            height[x] += c
            v = pred_node(root, x)
            sumv[v] += c
            return

        # Cell becomes non-empty.
        height[x] = c

        has_left = height[x - 1] > 0
        has_right = height[x + 1] > 0

        if not has_left and not has_right:
            # New isolated tower [x, x].
            root = insert(root, new_node(x, x, c))

        elif has_left and not has_right:
            # Extend the left tower to the right.
            lt = pred_node(root, x - 1)
            end[lt] = x
            sumv[lt] += c

        elif not has_left and has_right:
            # Extend the right tower to the left.
            rt = find_node(root, x + 1)
            key[rt] = x
            sumv[rt] += c

        else:
            # Merge left tower, new cell x, and right tower.
            lt = pred_node(root, x - 1)
            rt = find_node(root, x + 1)

            end[lt] = end[rt]
            sumv[lt] += c + sumv[rt]

            root = erase(root, x + 1)

    for _ in range(n):
        cmd = scanner.next_token()

        if cmd[0] == ord("p"):
            # put x c
            x = scanner.next_int()
            c = scanner.next_int()
            do_put(x, c)

        elif cmd[0] == ord("c"):
            # cubes t
            t = scanner.next_int()
            v = kth(root, t - 1)
            out.append(f"{sumv[v]} cubes in {ordinal(t)} tower\n")

        elif cmd[0] == ord("l"):
            # length t
            t = scanner.next_int()
            v = kth(root, t - 1)
            length = end[v] - key[v] + 1
            out.append(f"length of {ordinal(t)} tower is {length}\n")

        elif cmd[1] == ord("o"):
            # towers
            out.append(f"{size[root]} towers\n")

        elif cmd[1] == ord("p"):
            # tput t x c
            t = scanner.next_int()
            x = scanner.next_int()
            c = scanner.next_int()

            v = kth(root, t - 1)
            cell = key[v] + x - 1

            height[cell] += c
            sumv[v] += c

        else:
            # tcubes t x
            t = scanner.next_int()
            x = scanner.next_int()

            v = kth(root, t - 1)
            cell = key[v] + x - 1

            out.append(
                f"{height[cell]} cubes in {ordinal(x)} column of {ordinal(t)} tower\n"
            )

    sys.stdout.write("".join(out))


if __name__ == "__main__":
    main()
