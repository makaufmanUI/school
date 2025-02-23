"""
main.py


"""
import numpy as np
from numpy.linalg import inv

sin = np.sin
cos = np.cos
rad = np.radians


def rotate(name, numRotations, rotationAxes = [], rotationAngles = []):
	R = []
	for i in range(numRotations):
		if rotationAxes[i] == 'x':
			R.append(np.matrix([  [1, 0, 0], [0, cos(rad(rotationAngles[i])), -sin(rad(rotationAngles[i]))], [0, sin(rad(rotationAngles[i])), cos(rad(rotationAngles[i]))]  ]))
		elif rotationAxes[i] == 'y':
			R.append(np.matrix([  [cos(rad(rotationAngles[i])), 0, sin(rad(rotationAngles[i]))], [0, 1, 0], [-sin(rad(rotationAngles[i])), 0, cos(rad(rotationAngles[i]))]  ]))
		elif rotationAxes[i] == 'z':
			R.append(np.matrix([  [cos(rad(rotationAngles[i])), -sin(rad(rotationAngles[i])), 0], [sin(rad(rotationAngles[i])), cos(rad(rotationAngles[i])), 0], [0, 0, 1]  ]))
		else:
			print('\n\nSomething went wrong.')
			return
	if numRotations < 3:
		R2 = np.dot(R[0], R[1])
	else:
		for i in range(numRotations):
			if i == 0 or i == 1:
				R2 = np.dot(R[0], R[1])
			else:
				R2 = np.dot(R2, R[i])
	print('>> ' + name + ' = \n')
	print(np.matrix.round(R2, 3))
	return R2


def inverse(name, R):
	Rinv = inv(R)
	print('\n>> ' + name + ' = \n')
	print(np.matrix.round(Rinv,3))
	return Rinv



print('\n\n\n========== NUMBER 2 ==========\n')

Rsa = rotate('Rsa', 2, ['x', 'z'], [45, 90])

Ras = inverse('Ras', Rsa)

print('\n==============================\n\n')



print('========== NUMBER 3 ==========\n')

Rsb = rotate('Rsb', 3, ['z', 'x', 'y'], [60, 30, 90])

Rbs = inverse('Rbs', Rsb)

print('\n==============================\n\n')



print('========== NUMBER 4 ==========\n')

Rab = np.matmul(Ras, Rsb)
print('>> Rab = \n\n' + str(np.matrix.round(Rab,3)))

Rba = inverse('Rba', Rab)

print('\n==============================\n\n\n')