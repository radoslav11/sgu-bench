import sys
from functools import cmp_to_key

def compare(a, b):
    # Return negative if a should come before b
    if a + b < b + a:
        return -1
    elif a + b > b + a:
        return 1
    else:
        return 0

def main():
    data = sys.stdin.read().split()
    # First token is N, following are the N strings
    n = int(data[0])
    strings = data[1:]

    # Sort using the custom comparator
    strings.sort(key=cmp_to_key(compare))

    # Print the joined result
    sys.stdout.write(''.join(strings))

if __name__ == "__main__":
    main()
