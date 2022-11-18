if __name__ == "__main__":
    enc = "giants"
    dec = "isrveawhobpnutfg"

    corr_table = {}
    print(type(corr_table))
    for i in 'abcdefghijklmnopqrstuvwxyz':
        if dec[ord(i) & 0xf] in 'giants':
            corr_table[dec[ord(i) & 0xf]] = i
            print(dec[ord(i) & 0xf], '->', i)
    
    print(''.join([corr_table[i] for i in enc]))