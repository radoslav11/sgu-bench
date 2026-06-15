import sys

def add_to_basis(x, basis):
    """
    Try to insert integer x into the XOR-basis.
    'basis' is a dict mapping bit-position -> basis vector.
    """
    # Process bits from high to low
    for bit in reversed(range(61)):  # bits 0..60
        if not (x >> bit) & 1:
            continue
        if bit not in basis:
            # Found an empty slot: store x here
            basis[bit] = x
            return
        # Eliminate this bit using existing basis vector
        x ^= basis[bit]
    # If x reduces to 0, it's dependent and we discard it

def get_max_xor(basis):
    """
    Given a basis, greedily build the maximum XOR.
    """
    ans = 0
    # Try to improve ans by XORing with each basis vector in descending bit-order
    for bit in sorted(basis.keys(), reverse=True):
        candidate = ans ^ basis[bit]
        if candidate > ans:
            ans = candidate
    return ans

def main():
    data = sys.stdin.read().split()
    n = int(data[0])
    nums = list(map(int, data[1:]))

    basis = {}
    # Build the basis
    for x in nums:
        add_to_basis(x, basis)

    # Print the maximum subset XOR
    print(get_max_xor(basis))

if __name__ == "__main__":
    main()
