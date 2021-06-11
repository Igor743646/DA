from random import *
import math, sys

def randomPoint(pp=(0, 0), a=100, b=100):
    return (uniform(pp[0], pp[0]+a), uniform(pp[1], pp[1]+b))

def area(a, b, c):
    return (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]);

def crossed1(a, b, c, d):
    if (a > b): 
        a, b = b, a
    if (c > d):
        c, d = d, c
    return max(a, c) <= min(b, d)

def crossed(a, b, c, d):
    return crossed1(a[0], b[0], c[0], d[0]) \
       and crossed1(a[1], b[1], c[1], d[1]) \
       and (area(a, b, c) * area(a, b, d) < 0) \
       and (area(c, d, a) * area(c, d, b) < 0)

def isCrossed(massive_of_edges):
    for i in range(len(massive_of_edges) - 1):
        for j in range(i+1, len(massive_of_edges)):
            a, b = massive_of_edges[i], massive_of_edges[j]
            if crossed(a[0], a[1], b[0], b[1]):
                return True
    return False

def randomFigure(count_of_vertexes=4, pp=(0, 0), a=100, b=100):
    result = [randomPoint(pp, a, b)]
    edges = []
    
    for i in range(count_of_vertexes - 1):
        rand_point = randomPoint(pp, a, b)
        # while (isCrossed(edges + [[result[-1], rand_point]])):
        #     rand_point = randomPoint(pp, a, b)
        edges.append([result[-1], rand_point])
        result.append(rand_point)
        
    # if (isCrossed(edges + [[result[0], result[-1]]])):
    #     return randomFigure(count_of_vertexes, pp, a, b)
    
    edges.append([result[0], result[-1]])
    return result, edges

def yy(x1, x2, y1, y2, x):
    if (x2-x1) == 0:
        return (2**128-1) * (x-x1) + y1
    return ((y2-y1)/(x2-x1)) * (x-x1) + y1

def inPolygon(x, y, xp, yp):
    c=0
    for i in range(len(xp)):

        if (xp[i-1] == xp[i] == x and (yp[i-1] <= y <= yp[i] or yp[i] <= y <= yp[i-1])):
            return True

        if (xp[i-1] <= x <= xp[i] or xp[i] <= x <= xp[i-1]) and yy(xp[i-1], xp[i], yp[i-1], yp[i], x) == y:
            return True

        if (((yp[i]<=y and y<yp[i-1]) or (yp[i-1]<=y and y<yp[i])) and 
            (x > (xp[i-1] - xp[i]) * (y - yp[i]) / (yp[i-1] - yp[i]) + xp[i])): c = 1 - c    
    return bool(c)


try:
    number_of_tests = int(sys.argv[1])
except:
    number_of_tests = 10

for n in range(number_of_tests):

    test_file = open(f"tests/test{n}.txt", 'w')
    test_points_file = open(f"tests/test_points{n}.txt", 'w')
    ans_file = open(f"tests/test_ans{n}.txt", 'w')

    number_of_vertexes = n+3
    print(number_of_vertexes)
    fig_mas = [randomFigure(number_of_vertexes, pp=(0, 0), a=100, b=100)]
   
    # for i in range(3):
    #     number_of_vertexes = randint(4, 15)
    #     fig_mas.append(randomFigure(number_of_vertexes, pp=(100*i, 0), a=100, b=100))
        
    # for i in range(3):
    #     number_of_vertexes = randint(4, 15)
    #     fig_mas.append(randomFigure(number_of_vertexes, pp=(100*i, 100), a=100, b=100))

    test_file.write(f"{len(fig_mas)}\n")
    for fig in fig_mas:
        test_file.write(f"{len(fig[0])}\n")
        for p in fig[0]:
            test_file.write(f"{p[0]} {p[1]}\n")

    rp_mas = [randomPoint((0, 0), 300, 200) for _ in range(100)]
    for rp in rp_mas:
        test_points_file.write(f"{rp[0]} {rp[1]}\n")
        in_or_not = -1

        num_of_figure = 1
        for fig in fig_mas:
            if inPolygon(rp[0], rp[1], [x[0] for x in fig[0]], [y[1] for y in fig[0]] ):
                in_or_not = num_of_figure
                break
            num_of_figure += 1

        ans_file.write(f"{in_or_not}\n")

    test_file.close()
    ans_file.close()