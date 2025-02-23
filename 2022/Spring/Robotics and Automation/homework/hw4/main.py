"""
main.py


"""
import numpy as np
import functions as f
from numpy.linalg import norm
np.set_printoptions(precision=3, suppress=True)


I3 = np.identity(3)
sin, cos, pi = np.sin, np.cos, np.pi



########## NUMBER 1 ##########
print('\n------------------------------\n')
q = np.array( [1, 3, 6] )
s = np.array( [3, 2, 1] )
s = s / norm(s)
h = 4

S = f.screw(q, s, h)
print('> Number 1:')
print(f'\nS  =  {S}\n')
print('------------------------------\n')
##############################




########## NUMBER 2 ##########
print('\n------------------------------\n')
Tbc = np.matrix([  [1, 0, 0, 5],
                   [0, 1, 0, 0],
                   [0, 0, 1, 0],
                   [0, 0, 0, 1]  ])

Pa = np.array( [-1, 1, 0] )
Pc = np.array( [-5, 0, 0] )
Pd = np.array( [ 0, 0, 2] )

Rdc = f.rotate(2, ['x', 'z'], [180, 270])

Tad = f.transform(I3, Pa)
Tdc = f.transform(Rdc, Pd)
Tcb = f.transform(I3, Pc)
Tdb = np.matmul(Tdc, Tcb)
Tab = np.matmul(Tad, Tdb)
print('> Number 2:')
print(f'\nTab  =  \n{Tab}\n')

Vb = np.array( [1, 2, 2, 0, 0, 0] )
Vb.shape = (6,1)
Adj_Tcb = f.adjoint(I3, Pc)

Vc = Adj_Tcb * Vb
Vc.shape = (1,6)
print(f'Vc  =  {Vc}')
print('\n------------------------------\n')
##############################




########## NUMBER 3 ##########
print('\n------------------------------\n')
q = np.array( [-2, 1, 0] )
s = np.array( [ 0, 0, 1] )
h = 0
theta_dot = 7

Va = f.twist(h, s, theta_dot, q)

print('> Number 3:')
print(f'\nVa  =  {Va}')
print('\n------------------------------\n')
##############################




########## NUMBER 4 ##########
print('\n------------------------------\n')
q = np.array( [0, 3, 0] )
s = np.array( [0, cos(pi/4), sin(pi/4)] )
h = 5
theta_dot = 2

Sc = f.screw(q, s, h)
Vc = f.twist(h, s, theta_dot, q)

print('> Number 4:')
print(f'\nSc  =  {Sc}\n')
print(f'Vc  =  {Vc}')
print('\n------------------------------\n\n')
##############################

