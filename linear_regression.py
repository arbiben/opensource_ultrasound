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
while start_point >= -5:
    start_point -= 0.5
    dis = distance.pop()
    x, y, z = v[0], v[1], v[2]
    points.append([x*dis,y*dis,z*dis])
    np.array(v)
    v = A.dot(b)



'''
Ben assignmen
assume start of probe at 0,-1,5 to 0,-1,-5
every loop moves z -0.5
facing the y axis 0,1,0 
tilting angle = 25
10 samples per rev
2 revs in total between 0, -1, 5 to 0, -1, -5

time 0 = change angle = 0, cos25, sin25
time 1 = (change by 36 degree)
every loop left multiply prev vector by rotational matrix   [cos  0 sin ] (matrix on the left)
                                                        [0    1   0 ]
                                                        [-sin 0 cos]
Mv(t) * dis + 0,-1, 5-(0.5)*t
that gives us new coordinate of point at time t
at time t multiply dis by 

E stem M step 
'''
