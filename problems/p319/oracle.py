import sys
from bisect import bisect_left

# We'll implement the same sweep-line + map logic.
# Python doesn't have a built-in ordered map with O(log N) predecessor queries,
# so we implement it using:
# - a sorted list of keys (ys)
# - a dict mapping y -> rect_id
#
# However, maintaining a sorted list with inserts/deletes is O(N) each,
# which is too slow for N=60000 in worst case.
#
# Therefore, we should use an external balanced tree structure.
# On Codeforces, Python solutions typically use "sortedcontainers" (not allowed),
# so a practical pure-Python approach is to use PyPy + "bisect" with arrays
# only if constraints allow. Here they don't.
#
# To keep this correct and efficient in Python without external libs,
# we can use a Treap (randomized BST) to support:
# - insert(key, value)
# - erase(key)
# - lower_bound(key) and predecessor(key)
# all in expected O(log N).

import random
random.seed(0)

class TreapNode:
    __slots__ = ("key", "val", "prio", "left", "right")
    def __init__(self, key, val):
        self.key = key
        self.val = val
        self.prio = random.randint(1, 1 << 30)
        self.left = None
        self.right = None

def rotate_right(p):
    q = p.left
    p.left = q.right
    q.right = p
    return q

def rotate_left(p):
    q = p.right
    p.right = q.left
    q.left = p
    return q

def treap_insert(root, key, val):
    """Insert or overwrite key with val."""
    if root is None:
        return TreapNode(key, val)
    if key == root.key:
        root.val = val
        return root
    if key < root.key:
        root.left = treap_insert(root.left, key, val)
        if root.left.prio > root.prio:
            root = rotate_right(root)
    else:
        root.right = treap_insert(root.right, key, val)
        if root.right.prio > root.prio:
            root = rotate_left(root)
    return root

def treap_erase(root, key):
    """Erase key if present."""
    if root is None:
        return None
    if key < root.key:
        root.left = treap_erase(root.left, key)
        return root
    if key > root.key:
        root.right = treap_erase(root.right, key)
        return root

    # key == root.key: remove this node by rotating it down
    if root.left is None:
        return root.right
    if root.right is None:
        return root.left
    # both children exist: rotate the higher-priority child up
    if root.left.prio > root.right.prio:
        root = rotate_right(root)
        root.right = treap_erase(root.right, key)
    else:
        root = rotate_left(root)
        root.left = treap_erase(root.left, key)
    return root

def treap_lower_bound(root, key):
    """Return node with smallest node.key >= key, or None."""
    cur = root
    ans = None
    while cur is not None:
        if cur.key >= key:
            ans = cur
            cur = cur.left
        else:
            cur = cur.right
    return ans

def treap_predecessor(root, key):
    """Return node with largest node.key < key, or None."""
    cur = root
    ans = None
    while cur is not None:
        if cur.key < key:
            ans = cur
            cur = cur.right
        else:
            cur = cur.left
    return ans

def main():
    data = sys.stdin.buffer.read().split()
    it = iter(data)

    n = int(next(it))
    w = int(next(it))
    h = int(next(it))

    # rects[i] = (x1, y1, x2, y2), with rects[0] as the canvas
    rects = [(0, 0, w, h)]
    for _ in range(n):
        x1 = int(next(it)); y1 = int(next(it))
        x2 = int(next(it)); y2 = int(next(it))
        if x1 > x2: x1, x2 = x2, x1
        if y1 > y2: y1, y2 = y2, y1
        rects.append((x1, y1, x2, y2))

    # Build sweep events: (x, y, id)
    events = []
    for i in range(1, n + 1):
        x1, y1, x2, y2 = rects[i]
        events.append((x1, y1, i))  # open boundary y1
        events.append((x1, y2, i))  # open boundary y2
        events.append((x2, 0, i))   # close
    events.sort()

    # Treap stores active boundaries: key=y, val=rect_id
    root = None
    root = treap_insert(root, 0, 0)
    root = treap_insert(root, h, 0)

    parent = [0] * (n + 1)

    for x, y, v in events:
        x1, y1, x2, y2 = rects[v]

        if x == x1:
            # On the bottom boundary event compute the parent
            if y == y1:
                pred = treap_predecessor(root, y1)
                # pred must exist because we always have key=0
                below = pred.val
                below_x1, below_y1, below_x2, below_y2 = rects[below]

                # Same boundary disambiguation as C++:
                # if predecessor key equals below's top, use below's parent
                if below_y2 == pred.key:
                    parent[v] = parent[below]
                else:
                    parent[v] = below

            # Insert boundary y -> v (both y1 and y2)
            root = treap_insert(root, y, v)

        else:
            # close: remove both boundaries
            root = treap_erase(root, y1)
            root = treap_erase(root, y2)

    # Compute facet areas using add-to-self, subtract-from-parent trick
    area = [0] * (n + 1)
    area[0] = w * h

    for i in range(1, n + 1):
        x1, y1, x2, y2 = rects[i]
        a = (x2 - x1) * (y2 - y1)
        area[i] += a
        area[parent[i]] -= a

    area.sort()
    sys.stdout.write(" ".join(map(str, area)) + "\n")

if __name__ == "__main__":
    main()
