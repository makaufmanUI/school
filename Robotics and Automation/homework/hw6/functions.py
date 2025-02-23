"""
functions.py


"""
import numpy as np
import pandas as pd
from numpy.linalg import norm



def PrintMatrix(object, name, decimals):
	"""Prints a numpy matrix in a nice format using a Pandas DataFrame

	Args:
		object (𝑛𝑝.𝑚𝑎𝑡𝑟𝑖𝑥): Numpy matrix to be printed

		name (𝑠𝑡𝑟𝑖𝑛𝑔): Name of object

		decimals (𝑖𝑛𝑡): Decimal precision for output
	"""
	if decimals == 0: pd.options.display.float_format = '{:,.0f}'.format
	if decimals == 1: pd.options.display.float_format = '{:,.1f}'.format
	if decimals == 2: pd.options.display.float_format = '{:,.2f}'.format
	if decimals == 3: pd.options.display.float_format = '{:,.3f}'.format

	numRows, numCols = np.shape(object)[0], np.shape(object)[1]
	rows, cols = [], []
	for i in range(numRows):
		rows.append('   ')
	for i in range(numCols):
		cols.append('   ')
	df = pd.DataFrame(object, index = rows, columns = cols)
	
	print(f'\n {name} = {df}\n')



def RBMPoE(frame, M, S = [], theta = []):
	"""Performs the RBM Product of Exponentials to express the final configuration
	of a robot given the frame you want this configuration expressed in (frame), a Home Position (M),
	a list of Screws for each joint where 𝜃 ≠ 0 ( [ S₁ , S₂ , ... ] ), and a list of angles corresponding to those joints ( [ 𝜃₁ , 𝜃₂ , ... ] )


	Args:
		frame (𝑠𝑡𝑟𝑖𝑛𝑔): The frame the final configuration is to be expressed in (either '𝑠𝑝𝑎𝑐𝑒' or '𝑏𝑜𝑑𝑦'). Determines whether the Home Position (M) is 𝑟𝑖𝑔𝘩𝑡 or 𝑙𝑒𝑓𝑡 multiplied, respectively.

		M (𝑛𝑝.𝑚𝑎𝑡𝑟𝑖𝑥): Matrix ( 4 x 4 ) describing the Home Position of the robot (the orientation of the end-effector wrt the base). Combination of a rotation matrix 𝑅 and a location vector 𝑝.

		S (𝑛𝑝.𝑎𝑟𝑟𝑎𝑦 𝑙𝑖𝑠𝑡): Screw ( 1 x 6 ) vectors for each joint where 𝜃 ≠ 0

		theta (𝑖𝑛𝑡/𝑓𝑙𝑜𝑎𝑡 𝑙𝑖𝑠𝑡): Rotation angles of the joints where 𝜃 ≠ 0, in 𝑟𝑎𝑑𝑖𝑎𝑛𝑠

	Returns:
		T (𝑛𝑝.𝑚𝑎𝑡𝑟𝑖𝑥): Final configuration (transformation matrix) ( 4 x 4 )
	"""
	exponentials = []			# empty list to store each rigid-body exponential result
	for i in range(len(S)):
		exponentials.append( RBMexponential(S[i], theta[i]) )	# append exponentials to list
	
	product = np.identity(4)				    # initialize the product of exponentials as identity matrix
	for i in range(len(exponentials)):
		product = product * exponentials[i]		# compute the product of exponentials
	
	if frame == 'space':
		T = product * M			# right-multiply by M if expressed in space frame
		T = np.where( np.matrix.round(T,3) == 0, 0, T )	  # gets rid of negative signs in front of zeros
		return T
	
	elif frame == 'body':
		T = M * product			# left-multiply by M if expressed in body frame
		T = np.where( np.matrix.round(T,3) == 0, 0, T )	  # gets rid of negative signs in front of zeros
		return T



def RBMexponential(S, theta):
    """Computes the rigid-body exponential (4x4) given a Screw and rotation angle.

	Args:
		S (𝑛𝑝.𝑎𝑟𝑟𝑎𝑦): Screw ( 1 x 6 )

		theta (𝑖𝑛𝑡/𝑓𝑙𝑜𝑎𝑡): Distance through which the frame rotates about 𝑠̂, in 𝑟𝑎𝑑𝑖𝑎𝑛𝑠
	
	Returns:
		T (𝑛𝑝.𝑚𝑎𝑡𝑟𝑖𝑥): Final configuration (transformation matrix) ( 4 x 4 )
    """
    Sw = np.array( [S.item(0), S.item(1), S.item(2)] )
    Sv = np.array( [S.item(3), S.item(4), S.item(5)] )

	### CASE 1 (screw pitch is finite) ###
    if abs(1 - norm(Sw)) < 0.05:
        Sv.shape = (3,1)
        skewSw = skew3(Sw)
        e = np.identity(3) + np.sin(theta)*skewSw + (1 - np.cos(theta))*(skewSw)**2
        g = np.identity(3)*theta + (1-np.cos(theta))*skewSw + (theta-np.sin(theta))*(skewSw**2)
        GSv = g*Sv
        T = np.matrix([ [e.item(0), e.item(1), e.item(2), GSv.item(0)],
                        [e.item(3), e.item(4), e.item(5), GSv.item(1)],
                        [e.item(6), e.item(7), e.item(8), GSv.item(2)],
                        [    0    ,     0    ,     0    ,      1     ]    ])
	
	### CASE 2 (screw pitch is infinite) ###
    else:
        SvTheta = Sv * theta
        T = np.matrix([ [  1  ,  0  ,  0  ,  SvTheta.item(0)  ],
                        [  0  ,  1  ,  0  ,  SvTheta.item(1)  ],
                        [  0  ,  0  ,  1  ,  SvTheta.item(2)  ],
                        [  0  ,  0  ,  0  ,         1         ]    ])
    return T



def Transform(R, p):
	"""Generates the transformation matrix describing a particular rotation and translation

	Args:
		R (𝑛𝑝.𝑚𝑎𝑡𝑟𝑖𝑥): Rotation frame ( 3 x 3 )
		p (𝑛𝑝.𝑎𝑟𝑟𝑎𝑦): Vector from origin-to-origin ( 1 x 3 )

	Returns:
		T (𝑛𝑝.𝑚𝑎𝑡𝑟𝑖𝑥): Transformation matrix ( 4 x 4 )
	"""
	a = np.matrix([ [1,0,0,p.item(0)],
				    [0,1,0,p.item(1)],
				    [0,0,1,p.item(2)],
				    [0,0,0,    1    ] ])
	b = np.matrix([ [R.item(0),R.item(1),R.item(2),0],
				    [R.item(3),R.item(4),R.item(5),0],
				    [R.item(6),R.item(7),R.item(8),0],
				    [    0    ,    0    ,    0    ,1] 	])
	T = np.matmul(a,b)
	T = np.where( np.matrix.round(T,3) == 0, 0, T )	    # gets rid of negative signs in front of zeros
	return T



def Screw3(q, s, h):
	"""Returns the Screw given an location/position vector, screw axis, and screw pitch

	Args:
		q (𝑛𝑝.𝑎𝑟𝑟𝑎𝑦): Location/position vector from reference origin to the screw axis

		𝑠̂ (𝑛𝑝.𝑎𝑟𝑟𝑎𝑦): Screw axis ( 3-vector, [ x, y, z ] )

		h (𝑖𝑛𝑡): Screw pitch

	Returns:
		S (𝑛𝑝.𝑎𝑟𝑟𝑎𝑦): The corresponding Screw
	"""
	if norm(s) > 1:
		s = s / norm(s)

	cross = np.cross(-1*s, q)
	last = cross + h*s
	S = np.array( [s.item(0), s.item(1), s.item(2), last.item(0), last.item(1), last.item(2)] )
	return S



def skew3(vector):
	"""Skew-symmetric function -- returns the skew-symmetric matrix of a column/row vector

	Args:
		vector (𝑛𝑝.𝑎𝑟𝑟𝑎𝑦): Vector to be skewed ( 1 x 3 )

	Returns:
		𝑛𝑝.𝑚𝑎𝑡𝑟𝑖𝑥: The resulting skew-symmetric matrix ( 3 x 3 )
	"""
	matrix_ = np.matrix([   [        0        , -1*vector.item(2),   vector.item(1) ],
						    [  vector.item(2) ,         0        , -1*vector.item(0)],
						    [-1*vector.item(1),   vector.item(0) ,         0        ]  ])
	return matrix_

