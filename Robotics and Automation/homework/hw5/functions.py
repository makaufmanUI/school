"""
functions.py


"""
import numpy as np
from numpy.linalg import norm


I3 = np.identity(3)
matrix = np.matrix
rad = np.radians
sin, cos, acos = np.sin, np.cos, np.arccos


# Cotangent function (cos/sin)
def cot(theta):
	val = np.cos(theta) / np.sin(theta)
	return val


# Returns the screw S given location/position vector, screw axis direction, and screw pitch
def Screw3(q, s, h):
    if norm(s) > 1:
        s = s / norm(s)
    Sw = s
    Sv = np.cross( -1*s, q ) + h * s
    cross = np.cross(-1*s, q)
    last = cross + h*s
    S = np.array( [s.item(0), s.item(1), s.item(2), last.item(0), last.item(1), last.item(2)] )
    return S


# Returns the Screw given the two components Sw and Sv
def Screw4(Sw, Sv):
    S = np.array( [Sw.item(0), Sw.item(1), Sw.item(2), Sv.item(0), Sv.item(1), Sv.item(2)] )
    return S


# Returns the Wrench given a distance vector r and force vector f
def wrench(r, f):
    m = np.cross(r, f)
    F = np.array( [m.item(0), m.item(1), m.item(2), f.item(0), f.item(1), f.item(2)] )
    return F


# Returns the transformation matrix given a frame R and a vector p
def Transform(R, p):
	a = matrix([ [1,0,0,p.item(0)],
				 [0,1,0,p.item(1)],
				 [0,0,1,p.item(2)],
				 [0,0,0,    1    ] ])
	b = matrix([ [R.item(0),R.item(1),R.item(2),0],
				 [R.item(3),R.item(4),R.item(5),0],
				 [R.item(6),R.item(7),R.item(8),0],
				 [    0    ,    0    ,    0    ,1] 	])
	T = np.matmul(a,b)
	T = np.where( matrix.round(T,3) == 0, 0, T )	# gets rid of negative signs in front of zeros
	return T


# Returns the Adjoint of a transformation matrix
def Adjoint2(T):
	R = np.matrix([  [T.item(0), T.item(1), T.item(2)],
					 [T.item(4), T.item(5), T.item(6)],
					 [T.item(8), T.item(9), T.item(10)]  ])
	P = np.array( [T.item(3), T.item(7), T.item(11)] )
	pSkew = skew3(P)
	pR = np.matmul(pSkew,R)
	AdT = matrix([   [ R.item(0), R.item(1), R.item(2),    0    ,    0    ,    0    ],
				   	 [ R.item(3), R.item(4), R.item(5),    0    ,    0    ,    0    ],
				   	 [ R.item(6), R.item(7), R.item(8),    0    ,    0    ,    0    ],
				   	 [pR.item(0),pR.item(1),pR.item(2),R.item(0),R.item(1),R.item(2)],
				   	 [pR.item(3),pR.item(4),pR.item(5),R.item(3),R.item(4),R.item(5)],
				   	 [pR.item(6),pR.item(7),pR.item(8),R.item(6),R.item(7),R.item(8)]  	])
	return AdT


# Returns the rigid-body exponential given the Screw and theta
def RBMexponential(S, theta):
    Sw = np.array( [S.item(0), S.item(1), S.item(2)] )
    Sv = np.array( [S.item(3), S.item(4), S.item(5)] )
    Sv.shape = (3,1)
    skewSw = skew3(Sw)
    e = np.identity(3) + sin(theta)*skewSw + (1 - cos(theta))*(skewSw)**2
    g = np.identity(3)*theta + (1-cos(theta))*skewSw + (theta-sin(theta))*(skewSw**2)
    GSv = g*Sv
    T = np.matrix([    [e.item(0), e.item(1), e.item(2), GSv.item(0)],
                       [e.item(3), e.item(4), e.item(5), GSv.item(1)],
                       [e.item(6), e.item(7), e.item(8), GSv.item(2)],
                       [    0    ,     0    ,     0    ,      1     ]    ])
    return T


# Rigid-body matrix log (only condition 3)
def RBMmatrixLog2(R, p):

	### CONDITION 3 ###
    trR = np.trace(R)
    a = 1/2 * (trR - 1)
    theta = acos(a)
    
    b = R - R.T
    c = 2 * sin(theta)
    Sw = (1 / c) * b
	###################

    Ginv = (1/theta)*I3 - (1/2)*Sw + ((1/theta) - (1/2)*cot(theta/2))*(Sw**2)

    p.shape = (3,1)
    Sv = Ginv * p
    Sw = unskew(Sw)

    return theta, Sw, Sv


# Skew-symmetric function -- returns the skew-symmetric form (3x3) of a vector
def skew3(vector):
	matrix_ = np.matrix([  [        0        , -1*vector.item(2),   vector.item(1) ],
						   [  vector.item(2) ,         0        , -1*vector.item(0)],
						   [-1*vector.item(1),   vector.item(0) ,         0        ]  ])
	return matrix_


## Vector rebuilder function -- pulls out the x,y,z components of the skew-symmetric matrix and returns the 3 vector
### 1 Input:  any skew-symmetric matrix
def unskew(skewMatrix):
	x = skewMatrix.item(7)
	y = skewMatrix.item(2)
	z = skewMatrix.item(3)
	vector = np.array( [x, y, z] )
	# colVector.shape = (3,1)
	return vector


# Rotates the space frame, {s}, theta radians about rotationAxes to get new frame
## 3 Inputs:  number of rotations to do, the axes the rotations are done on, how far each rotation goes
def rotate2(numRotations, rotationAxes = [], theta = []):
	R = []
	for i in range (numRotations):
		if rotationAxes[i] == 'x':
			R.append(matrix([  [1,        0       ,             0         ],
					  		   [0, cos(rad(theta[i])), -sin(rad(theta[i]))],
					  		   [0, sin(rad(theta[i])),  cos(rad(theta[i]))]  ]))
		elif rotationAxes[i] == 'y':
			R.append(matrix([  [ cos(rad(theta[i])), 0, sin(rad(theta[i]))],
					  		   [          0        , 1,         0         ],
					  		   [-sin(rad(theta[i])), 0, cos(rad(theta[i]))]  ]))
		elif rotationAxes[i] == 'z':
			R.append(matrix([  [ cos(rad(theta[i])), -sin(rad(theta[i])), 0],
					  		   [ sin(rad(theta[i])),  cos(rad(theta[i])), 0],
					  		   [          0        ,            0       , 1]  ]))
		else:
			print('\n\nSomething went wrong.')
			return
	if numRotations < 3:
		Rsb = np.dot(R[0], R[1])
	else:
		for i in range (numRotations):
			if i == 0 or i == 1:
				Rsb = np.dot(R[0], R[1])
			else:
				Rsb = np.dot(Rsb, R[i])
	return Rsb

