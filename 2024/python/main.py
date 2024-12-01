filepath = "input/day1_input.txt"

# NOT MY SOLUTION BUT HOLY COW AT PYTHON
##################################################
# data = [*map(int, open(filepath).read().split())]
# left, right = sorted(data[0::2]), sorted(data[1::2])

# Both parts
# p1, p2 = (sum(map(lambda a, b: abs(a - b), left, right)),
#           sum(map(lambda a: a * right.count(a), left)))
# print(p1)
# print(p2)
##################################################

contents = open(filepath, "r").read().split()
contents = [*map(int, contents)]
left = sorted(contents[0::2])
right = sorted(contents[1::2])

sum= 0;

# Part One
for i in range(len(left)):
    sum += abs(left[i] - right[i])

print(sum)

# Part Two
sum2 = 0
for i in left:
    if i in right:
        sum2 += i * right.count(i)

print(sum2)
