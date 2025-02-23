"""
functions.py


"""
import numpy as np
import functions as f
np.set_printoptions(precision=3, suppress=True)


I3 = np.identity(3)
sin, cos = np.sin, np.cos
deg, rad = np.degrees, np.radians
matrix, matmul = np.matrix, np.matmul
pi, exp, sqrt = np.pi, np.exp, np.sqrt



# Ws = f.skew3(v)
## Skew-symmetric function -- returns the skew-symmetric form (3x3) of a vector
### Note that the vector must be a numpy array; ex: v = np.array( [x, y, z] )
def skew(vector):
	matrix_ = matrix([  [        0        , -1*vector.item(2),   vector.item(1) ],
						[  vector.item(2) ,         0        , -1*vector.item(0)],
						[-1*vector.item(1),   vector.item(0) ,         0        ]  ])
	return matrix_


# AdT = f.adjoint(Rsb, ps)
## Finds the adjoint of a transformation matrix T given a frame R and a vector p
### Note p must be a numpy array; ex: p = np.array( [x, y, z] )
def adjoint(R, p):
	pSkew = skew(p)
	pR = matmul(pSkew,R)
	AdT = matrix([   [ R.item(0), R.item(1), R.item(2),    0    ,    0    ,    0    ],
				   	 [ R.item(3), R.item(4), R.item(5),    0    ,    0    ,    0    ],
				   	 [ R.item(6), R.item(7), R.item(8),    0    ,    0    ,    0    ],
				   	 [pR.item(0),pR.item(1),pR.item(2),R.item(0),R.item(1),R.item(2)],
				   	 [pR.item(3),pR.item(4),pR.item(5),R.item(3),R.item(4),R.item(5)],
				   	 [pR.item(6),pR.item(7),pR.item(8),R.item(6),R.item(7),R.item(8)]  	])
	return AdT


# T = f.transform(Rsb, ps)
## Returns the transformation matrix given a frame R and a vector p
### Note p must be a numpy array; ex: p = np.array( [x, y, z] )
def transform(R, p):
	a = matrix([ [1, 0, 0, p.item(0)],
				 [0, 1, 0, p.item(1)],
				 [0, 0, 1, p.item(2)],
				 [0, 0, 0,     1    ] ])
	b = matrix([ [R.item(0), R.item(1), R.item(2), 0],
				 [R.item(3), R.item(4), R.item(5), 0],
				 [R.item(6), R.item(7), R.item(8), 0],
				 [    0    ,     0    ,     0    , 1] 	])
	T = matmul(a,b)
	T = np.where( matrix.round(T,3) == 0, 0, T )	# gets rid of negative signs in front of zeros
	return T


# Returns the Twist (6 vector) given screw pitch, screw axis,
#   rate of rotation, and point on on the screw axis to locate space frame origin
def twist(h, s_hat, theta_dot, q):
    first = s_hat * theta_dot                                       # angular velocity
    last = np.cross( -1*s_hat*theta_dot, q ) + h*s_hat*theta_dot    # linear velocity
    V = np.array( [first.item(0), first.item(1), first.item(2), last.item(0), last.item(1), last.item(2)] )
    return V


# Returns the screw S given location/position vector, screw axis direction, and screw pitch
def screw(q, s, h):
    Sw = s
    Sv = np.cross( -1*s, q ) + h * s    # angular component
    cross = np.cross(-1*s, q)
    last = cross + h*s
    S = np.array( [s.item(0), s.item(1), s.item(2), last.item(0), last.item(1), last.item(2)] )
    return S


# Returns the result of rotating an identity matrix numRotations theta degrees about rotationAxes
def rotate(numRotations, rotationAxes = [], theta = []):
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
					  		   [          0        ,            0       , 1] ]))
		else:
			print('\n\nSomething went wrong in rotate()')
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