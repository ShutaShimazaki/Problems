from cs50 import get_int


def main():
    
    h = Height_int()
    for i in range(h):
        
        for k in range(h - i - 1):
            print(" ", end="")
            k = k + 1
        
        for j in range(i + 1):
            print("#", end="")
            j = j + 1
        
        print()
    
    i = i + 1
    
        
def Height_int():
    while (True):
        n = get_int("Height number")
        if n > 0 and n < 9:
            break
    return n


main()

