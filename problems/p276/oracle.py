S, P = map(int, input().split())

delay = P - S

if delay <= 0:
    print(0)
elif delay < 5 * 60:
    print(1)
elif delay < 15 * 60:
    print(2)
elif delay < 30 * 60:
    print(3)
else:
    print(4)
