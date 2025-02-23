"""
main.py


"""
import numpy as np
import functions as f
r = np.radians



########## NUMBER 1 ##########

M = np.matrix([ [1, 0, 0,  0 ],     # Home position (orientation of end-effector {b} in the fixed frame {f})
                [0, 1, 0,  0 ],
                [0, 0, 1, 200],
                [0, 0, 0,  1 ] ])

S1 = np.array(  [0, 1, 0, 0,   0 , 0]  )     # Joint 1 Screw
S2 = np.array(  [0, 1, 0, 0, -100, 0]  )     # Joint 2 Screw
S3 = np.array(  [0, 1, 0, 0, -150, 0]  )     # Joint 3 Screw

JointAngle1, JointAngle2, JointAngle3 = r(30), r(30), r(30)     # Joint angles (converted to radians)

T = f.RBMPoE('space', M, [S1, S2, S3], [JointAngle1,JointAngle2,JointAngle3])

f.PrintMatrix(T, '#1', 3)     # ANSWER
# print('\n\n'); print(T)

##############################





########## NUMBER 2 ##########

I3 = np.identity(3)
b, L1, L2, L3 = 0.05, 0.1, 0.1, 0.1     # Relevant lengths (in meters)

M = f.Transform(I3, np.array([0, L2+L3, b+L1]))     # Home position (orientation of end-effector {ee} in the fixed frame {0})

JointAngle1, JointAngle2, JointAngle3 = r(45), r(35), r(-45)     # Joint angles (converted to radians)

s1 = np.array( [0, 0, 1] )     # Joint 1 screw axis
s2 = np.array( [1, 0, 0] )     # Joint 2 screw axis
s3 = np.array( [1, 0, 0] )     # Joint 3 screw axis

q1 = np.array( [0,  0,  0  ] )     # Joint 1 axis location wrt {0}
q2 = np.array( [0,  0, b+L1] )     # Joint 2 axis location wrt {0}
q3 = np.array( [0, L2, b+L1] )     # Joint 3 axis location wrt {0}

h = 0     # Joints are taken to be revolute

S1 = f.Screw3(q1, s1, h)     # Joint 1 Screw
S2 = f.Screw3(q2, s2, h)     # Joint 2 Screw
S3 = f.Screw3(q3, s3, h)     # Joint 3 Screw

T = f.RBMPoE('space', M, [S1, S2, S3], [JointAngle1, JointAngle2, JointAngle3])

f.PrintMatrix(T, '#2', 3)     # ANSWER
# print('\n\n'); print(T)

##############################






########## NUMBER 3 ##########

I3 = np.identity(3)
b, L1, L2, L3, L4 = 0.0098, 0.2755, 0.41, 0.2073, 0.1     # Relevant lengths (in meters)

M = f.Transform(I3, np.array([0, L1+L2+L3+L4, -b]))     # Home position (orientation of end-effector {b} in the fixed frame {s})

JointAngle1, JointAngle2, JointAngle3, JointAngle4 = r(45), r(30), r(-30), r(35)     # Joint angles (converted to radians)

s1 = np.array( [0, 1, 0] )     # Joint 1 screw axis
s2 = np.array( [0, 0, 1] )     # Joint 2 screw axis
s3 = np.array( [0, 0, 1] )     # Joint 3 screw axis
s4 = np.array( [0, 1, 0] )     # Joint 4 screw axis

q1 = np.array( [0,   0,    0] )     # Joint 1 axis location wrt {s}
q2 = np.array( [0,  L1,    0] )     # Joint 2 axis location wrt {s}
q3 = np.array( [0, L1+L2, -b] )     # Joint 3 axis location wrt {s}
q4 = np.array( [0,   0,   -b] )     # Joint 4 axis location wrt {s}

h = 0     # Joints are taken to be revolute

S1 = f.Screw3(q1, s1, h)     # Joint 1 Screw
S2 = f.Screw3(q2, s2, h)     # Joint 2 Screw
S3 = f.Screw3(q3, s3, h)     # Joint 3 Screw
S4 = f.Screw3(q4, s4, h)     # Joint 4 Screw

T = f.RBMPoE('space', M, [S1, S2, S3, S4], [JointAngle1, JointAngle2, JointAngle3, JointAngle4])

f.PrintMatrix(T, '#3', 3)     # ANSWER
# print('\n\n'); print(T)

##############################

