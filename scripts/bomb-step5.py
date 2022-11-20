import string

if __name__ == "__main__":
    e = ""
    r = "isrveawhobpnutfg"
    for c in "giants":
        for i in string.ascii_lowercase:
            if r[ord(i) & 0xf] == c:
                e += i
                break
    print(e)
