import sys


def main():
    sys.set_int_max_str_digits(100000)
    n, m = map(int, sys.stdin.read().split())

    # a = number of length-t walks from the capital ending at the capital,
    # b = number of length-t walks from the capital ending anywhere on the
    # rim (all n rim towns aggregated). One step: a' = b (each rim town has
    # one edge to the capital), b' = n * a + 2 * b (capital fans out to all
    # n rim towns; a rim town reaches two rim neighbours).
    a, b = 1, 0
    for _ in range(m):
        a, b = b, n * a + 2 * b
    print(a)


if __name__ == "__main__":
    main()
