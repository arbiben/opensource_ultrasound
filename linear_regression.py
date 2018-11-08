import csv, math
import numpy as np

class Plane:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

class Line:
    def __init__(self,x,y,z,e):
        self.x = x
        self.y = y
        self.z = z
        self.e = e

time_stamp = []
distance = []
with open('obs.csv') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_count = 0
    for row in csv_reader:
        if line_count != 0:
            time_stamp.append(row[0])
            distance.append(row[1])
        line_count+=1
time_stamp.reverse()
distance.reverse()

angle = 36
points = []
start_point = 5
v = np.array([0, math.cos(25), math.sin(25)])
A = np.array([[math.cos(angle), 0, math.sin(angle)],[0,1,0],[-math.sin(angle), 0, math.cos(angle)]])
probe_coord = []
while start_point > -5:
    probe_coord.append([0, -1, start_point])
    start_point -= 0.5
    # if len(distance) > 0:
    dis = distance.pop()
    x = probe_coord[-1][0] + (v[0]*float(dis))
    y = probe_coord[-1][1] + (v[1]*float(dis))
    z = probe_coord[-1][2] + (v[2]*float(dis))
    points.append([x,y,z])
    # print("{} {} {}".format(x*float(dis),y*float(dis),z*float(dis)))
    v = A.dot(np.array(v))
