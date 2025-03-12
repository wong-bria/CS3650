def collatz(n):
  return f(n,1)

def f(n,x):
  if n == 1:
    return x
  elif n % 2 == 0:  # if n is even
    return f(n//2, x+1)  # '//' is integer division in Python
  elif n % 2 == 1:  # if n is odd (and greater than 1)
    return f(3*n+1, x+1)

for i in range(1, 100):
  print("collatz(" + str(i) + "): " + str(collatz(i)))
