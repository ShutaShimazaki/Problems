from cs50 import get_float


def Change_float():
    while True:
        n = get_float("Change owed (dollars) ")
        if n > 0:
            break

    return float(n)
    

def main():
    dollars = Change_float()
    cents = 100 * dollars
    coins = 0
    
    if cents > 0:
        while cents >= 25:
            cents = cents - 25
            coins += 1
            
        while cents >= 10:
            cents = cents - 10
            coins += 1
        
        while cents >= 5:
            cents = cents - 5
            coins += 1
        
        while cents >= 1:
            cents = cents - 1
            coins += 1
        
        print(f"{coins}")    
    
    else:
        print("0")


main()        