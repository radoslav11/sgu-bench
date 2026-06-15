# Read three lines of input, each with three integers
grid = [list(map(int, input().split())) for _ in range(3)]

# For each row in the grid:
#  - Sort the row, then take row[1], the middle element
medians = []
for row in grid:
    row.sort()        # Sorts in non-decreasing order
    medians.append(row[1])

# Now we have three medians; sort them and pick the middle one
medians.sort()
print(medians[1])
