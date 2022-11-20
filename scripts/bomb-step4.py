def func4(p):
    if p < 2:
        return 1
    else:
        return func4(p - 1) + func4(p - 2)

if __name__ == "__main__":
    i = 0
    while True:
        if func4(i) == 55:
            print(i)
            break
        i += 1