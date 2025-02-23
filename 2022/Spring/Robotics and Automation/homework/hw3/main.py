"""
main.py


"""
import numpy as np
import functions as f

rad = np.radians
deg = np.degrees



print('\n========== NUMBER 1 ==========\n')

Rsb = f.rotate(3, ['z', 'x', 'y'], [45, 60, 30])
print('>> Rsb = \n\n' + str(Rsb))

expCoordinates = f.matrixLog(Rsb)[3]
print('\n>> Exp. coordinates =  \n\n',
				str(expCoordinates))

Ws = np.array( [1, 2, 3] )
Ws.shape = (3,1)
Wb = (Rsb.T) * Ws
print('\n>> Wb = \n\n' + str(Wb))

print('\n==============================\n')



print('\n========== NUMBER 2 ==========\n')

W_hat = np.array( [0.267, 0.535, 0.802] )
theta = rad(45)
expCoordinates = W_hat * theta
print('>> Exp. coordinates =  \n\n',
			  str(expCoordinates))

R = f.rodrigues(theta, f.skew(W_hat))
print('\n>> Rotation matrix = \n\n' + str(R))


print('\n==============================\n')



print('\n========== NUMBER 3 ==========\n')

R = f.rotate(3, ['y', 'z', 'x'], [90, 180, 90])
W_hat = f.matrixLog(R)[0]
print('>> W hat =  \n\n', str(W_hat))

theta = f.matrixLog(R)[1]
print('\n>> Theta =  ' + str(round(theta,3)) + ' radians')
print('         =  ' + str(round(deg(theta),3)) + ' degrees')

expCoordinates = W_hat * theta
print('\n>> Exp. coordinates =  \n\n',
				str(expCoordinates))

print('\n==============================\n')



print('\n========== NUMBER 4 ==========\n')

expCoordinates = np.array( [1, 2, 1] )

R = f.matrixExp(expCoordinates)
print('>> Rotation matrix =\n\n', str(R))

print('\n==============================\n')

