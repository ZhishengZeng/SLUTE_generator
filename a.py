def combinations(iterable, r):
    # combinations('ABCD', 2) --> AB AC AD BC BD CD
    # combinations(range(4), 3) --> 012 013 023 123
    pool = tuple(iterable)
    n = len(pool)
    if r > n:
        return
    indices = list(range(r))
    print(tuple(pool[i] for i in indices))
    while True:
        for i in reversed(range(r)):
            if indices[i] != i + n - r:
                break
        else:
            return
        indices[i] += 1
        for j in range(i+1, r):
            indices[j] = indices[j-1] + 1
        print(tuple(pool[i] for i in indices))

def combinations_with_replacement(iterable, r):
    # combinations_with_replacement('ABC', 2) --> AA AB AC BB BC CC
    pool = tuple(iterable)
    n = len(pool)
    if not n and r:
        return
    indices = [0] * r
    print("0",tuple(pool[i] for i in indices))
    while True:
        for i in reversed(range(r)):
            print(i,indices[i])  
            if indices[i] != n - 1:
                break
        else:
            return
        print(i, tuple(pool[i] for i in indices))    
        indices[i:] = [indices[i] + 1] * (r - i)
        print("pro",tuple(pool[i] for i in indices))

if __name__ == "__main__":
    combinations('0011222333', 1)