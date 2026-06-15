import sys

def count_configurations(a):
    """
    Given a[i] constraints (second column numbers), return how many
    binary mine assignments m[0..n-1] satisfy the Minesweeper equations.
    """
    n = len(a)

    def check(first):
        """
        Try fixing m[0] = first (0/1) and derive the rest.
        Return True if we end with a valid binary assignment.
        """
        # Special case: one row, the only neighbor in column 1 is itself
        if n == 1:
            return a[0] == first

        # m_prev = m[i-1], m_cur = m[i]
        m0 = first

        # From a[0] = m[0] + m[1] -> m1 = a[0] - m0
        m1 = a[0] - m0
        if m1 not in (0, 1):
            return False

        m_prev, m_cur = m0, m1

        # For i = 1..n-2: a[i] = m[i-1] + m[i] + m[i+1]
        # => m[i+1] = a[i] - m[i] - m[i-1]
        for i in range(1, n - 1):
            m_next = a[i] - m_cur - m_prev
            if m_next not in (0, 1):
                return False
            m_prev, m_cur = m_cur, m_next

        # Now m_prev = m[n-2], m_cur = m[n-1]
        # Bottom equation: a[n-1] = m[n-2] + m[n-1]
        return a[n - 1] == m_prev + m_cur

    # Try both possibilities for m[0] and count those that work
    return int(check(0)) + int(check(1))


def main():
    data = list(map(int, sys.stdin.read().split()))
    n = data[0]
    a = data[1:1+n]
    print(count_configurations(a))

if __name__ == "__main__":
    main()
