import sys

def main():
    data = sys.stdin.read().split()
    # First token is K, second is N, then N phone numbers
    k = int(data[0])
    n = int(data[1])
    # Initialize counts for first digits '1'..'9'
    counts = {str(d): 0 for d in range(1, 10)}
    # Process each phone number
    for i in range(n):
        num = data[2 + i]
        first = num[0]
        counts[first] += 1

    # Start with 2 reserved pages
    total_pages = 2
    # For each nonempty group, compute how many pages are needed
    for d in counts:
        cnt = counts[d]
        if cnt > 0:
            # Ceil division: (cnt + k - 1) // k
            pages_for_group = (cnt + k - 1) // k
            total_pages += pages_for_group

    # Output the result
    print(total_pages)

if __name__ == "__main__":
    main()
