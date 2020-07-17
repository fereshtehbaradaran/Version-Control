def apply_bridge(bridge, obj1, target):
    index = 0
    for z in bridge.readlines():
        e = [x for x in z[:-1].split()]
        while int(e[1]) > index:
            target.write((chr(obj1[index])).encode('charmap'))
            index += 1
        if int(e[1]) == index:
            if e[0] == '+':
                target.write((chr(int(e[2]))).encode('charmap'))
            else:
                index += 1
    while index < len(obj1):
        target.write((chr(obj1[index])).encode('charmap'))
        index += 1
        
firstFilePath = input()
fileOfChangespath = input()
newFilePath = input()

a = open(firstFilePath, "rb").read()
bF = open(fileOfChangespath, "r")
c = open(newFilePath, "wb")
apply_bridge(bF, a, c)
c.close()
bF.close()