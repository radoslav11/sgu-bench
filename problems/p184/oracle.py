import sys

P, M, C, K, R, V = map(int, sys.stdin.read().split())
print(min(P // K, M // R, C // V))
