import sys


def pow3(k):
    """Return 3^k."""
    result = 1

    for _ in range(k):
        result *= 3

    return result


def format_side(coins):
    """Format one pan of the balance, e.g. [1, 2, 3] -> '1+2+3'."""
    return "+".join(map(str, coins))


class Node:
    """
    A node in the decision tree.

    A node is either:
    - a leaf, storing the fake coin, or
    - an internal node, storing one weighing and its branches.
    """

    def __init__(self, lhs=None, rhs=None, fake=None):
        # Coins on left pan.
        self.lhs = lhs

        # Coins on right pan.
        self.rhs = rhs

        # Fake coin number if this is a leaf.
        self.fake = fake

        # Branch for left pan lighter.
        self.less = None

        # Branch for balance.
        self.equal = None

        # Branch for left pan heavier.
        self.greater = None

    def print_tree(self, level, out):
        """Recursively print the decision tree."""

        indent = "  " * level

        # Leaf node: print answer.
        if self.fake is not None:
            out.append(f"{indent}fake {self.fake}")
            return

        # Internal node: print weighing.
        out.append(
            f"{indent}weigh {format_side(self.lhs)} vs {format_side(self.rhs)}"
        )

        # Print only possible branches.

        if self.less is not None:
            out.append(f"{indent}case <:")
            self.less.print_tree(level + 1, out)

        if self.equal is not None:
            out.append(f"{indent}case =:")
            self.equal.print_tree(level + 1, out)

        if self.greater is not None:
            out.append(f"{indent}case >:")
            self.greater.print_tree(level + 1, out)

        out.append(f"{indent}end")


def build_unknown(suspects, known_good):
    """
    Build a decision tree for an unknown-direction state.

    suspects:
        Coins that may be fake, with unknown direction.

    known_good:
        Coins known to be genuine.
    """

    sz = len(suspects)

    # No suspects means this branch is impossible.
    if sz == 0:
        return None

    # One suspect means that coin must be fake.
    if sz == 1:
        return Node(fake=suspects[0])

    # If we have known genuine coins, compare suspects against them.
    if known_good:
        k = 0

        # Capacity of an unknown-direction state with genuine references:
        # (3^k + 1) / 2
        while (pow3(k) + 1) // 2 < sz:
            k += 1

        # Number of suspects to weigh against genuine coins.
        b = min(pow3(k - 1), sz)

        # Suspects placed on the scale.
        weighed = suspects[:b]

        # Suspects left off the scale.
        remaining = suspects[b:]

        # Genuine coins used as references.
        good_on_scale = known_good[:b]

        # Create weighing node.
        node = Node(weighed, good_on_scale)

        # If suspect side is lighter, fake is among weighed suspects,
        # and it is lighter.
        node.less = build_signed(weighed, [], known_good)

        # If suspect side is heavier, fake is among weighed suspects,
        # and it is heavier.
        node.greater = build_signed([], weighed, known_good)

        # If balance, weighed suspects are genuine.
        if remaining:
            new_good = known_good + weighed
            node.equal = build_unknown(remaining, new_good)

        return node

    # Otherwise, this is an unknown state with no known genuine coins.

    k = 0

    # Initial-state capacity:
    # (3^k - 1) / 2
    while (pow3(k) - 1) // 2 < sz:
        k += 1

    # Number of suspects on each pan.
    a = (pow3(k - 1) - 1) // 2

    # Ensure the first weighing is meaningful.
    a = max(a, 1)

    # Leave at least one coin off-scale when possible.
    a = min(a, (sz - 1) // 2)

    # Split suspects into left pan, right pan, and off-scale.
    left = suspects[:a]
    right = suspects[a:2 * a]
    off = suspects[2 * a:]

    # Create weighing node.
    node = Node(left, right)

    # If left pan is lighter:
    # - fake may be a light coin on the left,
    # - or a heavy coin on the right.
    node.less = build_signed(left, right, off)

    # If left pan is heavier:
    # - fake may be a light coin on the right,
    # - or a heavy coin on the left.
    node.greater = build_signed(right, left, off)

    # If equal, the fake coin is among the off-scale coins,
    # and all weighed coins are known genuine.
    if off:
        new_good = left + right
        node.equal = build_unknown(off, new_good)

    return node


def build_signed(light, heavy, known_good):
    """
    Build a decision tree for a signed state.

    light:
        Coins that, if fake, must be lighter.

    heavy:
        Coins that, if fake, must be heavier.

    known_good:
        Known genuine coins.
    """

    L = len(light)
    H = len(heavy)
    total = L + H

    # Impossible branch.
    if total == 0:
        return None

    # Only one possible coin remains.
    if total == 1:
        if light:
            return Node(fake=light[0])
        else:
            return Node(fake=heavy[0])

    # Special case: two suspects.
    if total == 2:
        # Both suspects are heavy.
        if L == 0:
            node = Node([heavy[0]], [heavy[1]])

            # If left is lighter, right heavy-suspect is fake.
            node.less = Node(fake=heavy[1])

            # If left is heavier, left heavy-suspect is fake.
            node.greater = Node(fake=heavy[0])

            return node

        # Both suspects are light.
        elif H == 0:
            node = Node([light[0]], [light[1]])

            # If left is lighter, left light-suspect is fake.
            node.less = Node(fake=light[0])

            # If left is heavier, right light-suspect is fake.
            node.greater = Node(fake=light[1])

            return node

        # One light-suspect and one heavy-suspect.
        else:
            # Use a known genuine coin as reference.
            node = Node([light[0]], [known_good[0]])

            # If light-suspect is lighter than genuine, it is fake.
            node.less = Node(fake=light[0])

            # If they balance, the heavy-suspect must be fake.
            node.equal = Node(fake=heavy[0])

            return node

    # General signed case.

    # Target size for < and > branches.
    s = (total + 1) // 3

    # Try to use as many heavy suspects as possible on both pans.
    h = min(s, H // 2)

    # Fill remaining slots with light suspects.
    l = s - h

    # If not enough light suspects are available for both pans,
    # reduce light count and increase heavy count.
    if 2 * l > L:
        l = L // 2
        h = s - l

    # Left and right pans.
    left_pan = []
    right_pan = []

    # Put h heavy-suspects on the left pan.
    left_pan.extend(heavy[:h])

    # Put l light-suspects on the left pan.
    left_pan.extend(light[:l])

    # Put h heavy-suspects on the right pan.
    right_pan.extend(heavy[h:2 * h])

    # Put l light-suspects on the right pan.
    right_pan.extend(light[l:2 * l])

    # If left pan is heavier:
    # possible suspects are heavy suspects on left and light suspects on right.
    gt_heavy = heavy[:h]
    gt_light = light[l:2 * l]

    # If balance:
    # possible suspects are off-scale signed suspects.
    eq_light = light[2 * l:]
    eq_heavy = heavy[2 * h:]

    # If left pan is lighter:
    # possible suspects are light suspects on left and heavy suspects on right.
    lt_light = light[:l]
    lt_heavy = heavy[h:2 * h]

    # Create weighing node.
    node = Node(left_pan, right_pan)

    # Recurse on the three possible outcomes.
    node.less = build_signed(lt_light, lt_heavy, known_good)
    node.equal = build_signed(eq_light, eq_heavy, known_good)
    node.greater = build_signed(gt_light, gt_heavy, known_good)

    return node


def main():
    # Read n.
    data = sys.stdin.read().strip().split()
    n = int(data[0])

    # Coins are numbered from 1 to n.
    coins = list(range(1, n + 1))

    # Compute optimal number of weighings.
    w = 0
    while (pow3(w) - 1) // 2 < n:
        w += 1

    # Build output.
    out = []

    # First line.
    out.append(f"need {w} weighings")

    # Build and print the decision tree.
    root = build_unknown(coins, [])
    root.print_tree(0, out)

    # Write final answer.
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
