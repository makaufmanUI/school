"""
functions.py


"""
import math
import numpy as np
from scipy.linalg import expm
np.set_printoptions(precision=3, suppress=True)


pi = np.pi
sin = np.sin
cos = np.cos
acos = np.arccos
rad = np.radians
deg = np.degrees
matrix = np.matrix
matmul = np.matmul
identity = np.identity


# Matrix rotation function -- returns the final frame configuration after rotations
def rotate(numRotations, axes = [], angles = []):
    tmp = []
    for i in range(numRotations):
        if axes[i] == 'x':
            tmp.append(matrix([  [1,         0          ,          0          ],
                                      [0, cos(rad(angles[i])), -sin(rad(angles[i]))],
                                      [0, sin(rad(angles[i])),  cos(rad(angles[i]))]        ]))
        elif axes[i] == 'y':
            tmp.append(matrix([  [ cos(rad(angles[i])), 0, sin(rad(angles[i]))],
                                      [           0        , 1,         0          ],
                                      [-sin(rad(angles[i])), 0, cos(rad(angles[i]))]      ]))
        elif axes[i] == 'z':
            tmp.append(matrix([  [cos(rad(angles[i])), -sin(rad(angles[i])), 0],
                                      [sin(rad(angles[i])),  cos(rad(angles[i])), 0],
                                      [           0       ,             0       , 1]      ]))
        else:
            print('\n\nSomething went wrong.')
            return

    if numRotations < 3:
        R = tmp[0] * tmp[1]
    else:
        for i in range(numRotations):
            if i == 0 or i == 1:
                R = tmp[0] * tmp[1]
            else:
                R = R * tmp[i]
    return R


# Vector rebuilder function -- pulls out the components
#   of the skew-symmetric matrix and returns the 3 vector
def unskew(skewMatrix):
    x = skewMatrix.item(7)
    y = skewMatrix.item(2)
    z = skewMatrix.item(3)
    vector = np.array( [x, y, z] )
    return vector


# Checks if a given matrix is an identity matrix -- returns True or False
#   (has some tolerance built in for imprecise floating point numbers) (used in matrixLog())
def checkIdentity(R):
    a_ = R.item(0)
    e_ = R.item(4)
    i_ = R.item(8)
    a = True  if  abs(1 - a_)<0.001  else  False
    e = True  if  abs(1 - e_)<0.001  else  False
    i = True  if  abs(1 - i_)<0.001  else  False
    ones = True  if   a and e and i  else  False

    b_ = R.item(1)
    c_ = R.item(2)
    d_ = R.item(3)
    f_ = R.item(5)
    g_ = R.item(6)
    h_ = R.item(7)
    b = True  if  abs(0 - b_)<0.001  else  False
    c = True  if  abs(0 - c_)<0.001  else  False
    d = True  if  abs(0 - d_)<0.001  else  False
    f = True  if  abs(0 - f_)<0.001  else  False
    g = True  if  abs(0 - g_)<0.001  else  False
    h = True  if  abs(0 - h_)<0.001  else  False
    zeros = True  if   b and c and d and f and g and h  else  False

    if ones and zeros:
        return True
    else:
        return False


# Returns the angular velocity vector given some rotation frame R
#   (also returns theta and the exponential coordinates)
def matrixLog(R):
    if checkIdentity(R):
        print('\n>> R is the identity matrix  --  wHat is undefined.',
            'Returning wHat as a 3vector with zeros.\n\n')
        wHat = np.array( [0, 0, 0] )
        return wHat

    else:
        trace_R = np.trace(R)

        if round(trace_R, 3) + 1 == 0:
            print('\n>> trace(R) = -1  --  Theta = pi.')
            print('>> wHat returned as a list of 3 vectors.',
                '(note: if wHat is a solution, then so is -wHat)')

            theta = np.pi
            theta2 = deg(theta)
            firstTerm1 = 1 / (np.sqrt( 2 * (1 + R.item(8)) ))
            firstTerm2 = 1 / (np.sqrt( 2 * (1 + R.item(4)) ))
            firstTerm3 = 1 / (np.sqrt( 2 * (1 + R.item(0)) ))
            vector1 = np.array( [R.item(2), R.item(5), 1 + R.item(8)] )
            vector2 = np.array( [R.item(1), 1 + R.item(4), R.item(7)] )
            vector3 = np.array( [1 + R.item(0), R.item(3), R.item(6)] )
            wHat1 = firstTerm1 * vector1
            wHat2 = firstTerm2 * vector2
            wHat3 = firstTerm3 * vector3
            expCoordinates1 = wHat1 * theta
            expCoordinates2 = wHat2 * theta
            expCoordinates3 = wHat3 * theta

            wHat = [wHat1, wHat2, wHat3]
            return wHat

        else:
            theta = acos( (1/2) * (trace_R - 1) )
            theta2 = deg(theta)
            wHat_skew = (  ( 1/(2 * sin(theta)) ) * ( R - R.T ) )
            wHat = unskew(wHat_skew)
            expCoordinates = wHat * theta

            results = [wHat, theta, theta2, expCoordinates]    # theta in radians, theta2 in degrees
            return results


# Skew-symmetric function -- returns the skew-symmetric
#   matrix of a column vector ( (np array) input )
def skew(vector):
    skewMatrix = matrix([  [        0         , -1*vector.item(2),   vector.item(1)  ],
                              [  vector.item(2)  ,         0        , -1*vector.item(0) ],
                              [ -1*vector.item(1),   vector.item(0) ,         0         ]  ])
    return skewMatrix


# Rodrigues formula  --  R = I + sin(theta)[w] + (1 - cos(theta)[w]^2)
#  input theta and the skew-symmetric form of w_hat; returns rotation matrix
def rodrigues(theta, Ws):
    I = identity(3)
    Ws2 = matmul(Ws, Ws)
    R = I + sin(theta)*Ws + (1 - cos(theta)) * (Ws2)
    return R


# Returns the matrix exponential of exponential coordinates (as a 3 vector)
def matrixExp(exp_coords):
    skewed = skew(exp_coords)
    R = expm(skewed)
    return R

