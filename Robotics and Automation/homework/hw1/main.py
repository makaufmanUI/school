"""
main.py


"""
m = {
	'2d': 3,
	'3d': 6
}

joint_dof = {
	'h': 1,
	'r': 1,
	'p': 1,
	'u': 2,
	's': 3,
	'c': 2,
	'helical': 1,
	'revolute': 1,
	'prismatic': 1,
	'universal': 2,
	'spherical': 3,
	'cylindrical': 2
}


# Grubler function -- calculates DOF given input of m, N, J, and joints (either as a list of strings or ints)
def grubler(M, N, J, joints = [], *args):
	fi = []
	sum_fi = 0

	# Check formatting of joints list, handle whichever is given
	if type(joints[0]) is str:
		for i in range(len(joints)):
			fi.append(joint_dof[joints[i]])
	elif type(joints[0]) is int:
		for i in range(len(joints)):
			fi.append(joints[i])
	else:
		print('Improperly formatted joints argument.')
		return

	# Sum freedom of each joint
	for i in range(len(fi)):
		sum_fi += fi[i]
	
	# Check formatting of m, handle whichever is given
	if type(M) is str:
		M = m[M]
	elif type(M) is int:
		pass
	else:
		print('Invalid input m.')
		return
	
	# Calculate degrees of freedom and return it
	DOF = M*(N - 1 - J) + sum_fi
	return DOF


# number 1 -- 2 DOF
DOF = grubler('2d', 5, 5, joints = ['r', 'r', 'r', 'r', 'r'])
print('\nNumber 1 = ' + str(DOF))

# number 2 -- 1 DOF
DOF = grubler('2d', 6, 7, joints = ['r', 'r', 'r', 'r', 'r', 'r', 'r'])
print('\nNumber 2 = ' + str(DOF))

# number 3 -- 1 DOF
DOF = grubler('2d', 6, 7, joints = ['r', 'r', 'r', 'r', 'r', 'r', 'r'])
print('\nNumber 3 = ' + str(DOF))

# number 4 -- 0 DOF
DOF = grubler('2d', 7, 9, joints = ['r', 'r', 'r', 'r', 'r', 'r', 'r', 'p', 'r'])
print('\nNumber 4 = ' + str(DOF))

# number 5 -- 1 DOF
DOF = grubler('2d', 6, 7, joints = ['r', 'r', 'r', 'r', 'r', 'r', 'r'])
print('\nNumber 5 = ' + str(DOF))

