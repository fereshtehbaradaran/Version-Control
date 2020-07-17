def diff(obj1, low1, high1, obj2, low2, high2, i, j):
    global bb
    len_Obj1 = high1 - low1
    len_Obj2 = high2 - low2
    len_Sum = len_Obj1 + len_Obj2
    len_Min = 2 * min(len_Obj1, len_Obj2) + 2
    if len_Obj1 > 0 and len_Obj2 > 0:
        g = [0] * len_Min
        p = [0] * len_Min
        for h in range(0, len_Sum//2 + len_Sum%2 + 1):
            for r in range(0, 2):
                if r == 0:
                    c = g
                    d = p
                    o = 1
                    m = 1
                else:
                    c = p
                    d = g
                    o = 0
                    m = -1
                for k in range(2*max(0,h-len_Obj2)-h, h-2*max(0,h-len_Obj1)+1, 2):
                    if (k==-h or (k!=h and c[(k-1)%len_Min] < c[(k+1)%len_Min])):
                        a = c[(k+1)%len_Min]
                    else:
                        a = c[(k-1)%len_Min] + 1
                    b = a - k
                    s = a
                    t = b
                    while a<len_Obj1 and b<len_Obj2 and obj1[(1-o)*len_Obj1+m*a+(o-1)] == obj2[(1-o)*len_Obj2+m*b+(o-1)]:
                        a += 1
                        b += 1
                    c[k%len_Min] = a
                    z = len_Obj1 - len_Obj2 - k
                    if len_Sum%2 == o and o-h <= z and z <= h-o and c[k%len_Min] + d[z%len_Min] >= len_Obj1:
                        if o == 1:
                            D = 2*h-1
                            x = s
                            y = t
                            u = a
                            v = b
                        else:
                            D = 2 * h
                            x = len_Obj1 - a
                            y = len_Obj2 - b
                            u = len_Obj1 - s
                            v = len_Obj2 - t
                        
                        if D > 1 or (x != u and y != v):
                            diff(obj1[0:x], 0, x, obj2[0:y], 0, y, i, j)
                            diff(obj1[u:len_Obj1], u, len_Obj1, obj2[v:len_Obj2], v, len_Obj2, i+u, j+v)
                        elif len_Obj2 > len_Obj1:
                            diff([], 0, 0, obj2[len_Obj1:len_Obj2], len_Obj1, len_Obj2, i+len_Obj1, j+len_Obj1)
                        elif len_Obj2 < len_Obj1:
                            diff(obj1[len_Obj2:len_Obj1], len_Obj2, len_Obj1, [], 0, 0, i+len_Obj2, j+len_Obj2)
                        return
    elif len_Obj1 > 0:
        for n in range(0, len_Obj1):
            bF.write("%c %d\n" % ('-', i+n))
    else:
        for n in range(0, len_Obj2):
            bF.write("%c %d %d\n" % ('+', i, bb[j+n]))
    return

firstFilePath = input()
secondFilePath = input()
fileOfChangesPath = input()

a = open(firstFilePath, "rb").read()
bb = open(secondFilePath, "rb").read()
bF = open(fileOfChangesPath, "w")
diff(a,0,len(a),bb,0,len(bb),0,0)
bF.close()