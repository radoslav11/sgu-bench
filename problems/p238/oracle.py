import sys

def main():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    n = int(data[0])

    # We do not actually need the tree structure.
    # The number of reachable configurations depends only on n:
    # - Add a fake node representing the floor -> total nodes = n + 1.
    # - Each operation corresponds to re-rooting the tree at a neighbor (child of root).
    # - Any node can be the root by walking along edges, and each root gives a unique state.
    # Hence answer = n + 1, except for n = 1 corner case.

    if n == 1:
        print(1)
    else:
        print(n + 1)

if __name__ == "__main__":
    main()
