"""
main.py


"""
import numpy as np
import functions as f
from numpy.linalg import inv
np.set_printoptions(precision=3, suppress=True)

I3 = np.identity(3)
pi, d = np.pi, np.degrees



### NUMBER 1

q = np.array( [2, 4, 1] )           #
s = np.array( [pi/4, pi/6, pi/6] )  #
h = 4                               # GIVEN
theta = pi/3                        #

Sb = f.Screw3(q, s, h)              # ANSWER
T = f.RBMexponential(Sb, theta)     # ANSWER

print('\n\n----------------------------------------\n')
print(f'NUMBER 1: \n\nSb = {Sb}\n\nT\' = \n\n{T}')
print('\n----------------------------------------\n\n')





### NUMBER 2

Rsa = f.rotate2(2, ['x', 'y'], [d(pi/6), d(pi/3)])  #
Pa = np.array( [0, 2, 3] )                          # GIVEN

theta, Sw, Sv = f.RBMmatrixLog2(Rsa, Pa)            # ANSWER (theta)
Sa = f.Screw4(Sw, Sv)                               # ANSWER

print('\n\n----------------------------------------\n')
print(f'NUMBER 2: \n\nSa = {Sa}\n\n𝜃 = {round(theta,3)}')
print('\n----------------------------------------\n\n')





### NUMBER 3

rh = np.array( [ 0.15, 0, 0 ] )     # Distance from {h} origin to {a} origin
fh = np.array( [ 0, -10*0.1, 0] )   # Force acting at distance rh due to mass of apple
Fh = f.wrench(rh, fh)               # Wrench at frame {h}

rf = np.array( [ 0.1, 0, 0 ] )      # Distance from {f} origin to {h} origin
ff = np.array( [ 0, -10*0.5, 0 ] )  # Force acting at distance rf due to mass of hand
Ff1 = f.wrench(rf, ff)              # Wrench at frame {f}

Rfh = I3                            # Rotation frame of {h} from perspective of {f}
Pf = np.array( [ 0.1, 0, 0 ] )      # Translation vector from {f} origin to {h} origin
Tfh = f.Transform(Rfh, Pf)          # Transformation matrix representing these two

Thf = inv(Tfh)                      # Transformation matrix of {f} from perspective of {h}
AdThf = f.Adjoint2(Thf)             # Adjoint of this matrix

Fh.shape = (6, 1)

Ff2 = (AdThf.T) * Fh                # Wrench in frame {f} due to wrench in frame {h}
Ff2.shape = (1, 6)

Ff = Ff1 + Ff2                      # Total Wrench in {f} is the combination of both Wrenches

print('\n\n----------------------------------------\n')
print(f'NUMBER 3: \n\nFf = {Ff}')
print('\n----------------------------------------\n\n')





### NUMBER 4

Ltotal = 0.35 + 0.41 + 0.41 + 0.136     # Whole length of arm
Rsb = I3                                # Rotation of {b} from persepctive of {s}
Ps = np.array( [ 0, 0, Ltotal ] )       # Translation vector from origin of {s} to origin of {b}

Fb = np.array( [0, 0, 0, 10, 5, 0] )    # no moments at {b} since forces act there

fb = np.array( [10, 5, 0] )             # force at {b}
rb = np.array( [0, 0, Ltotal] )         # distance from {s} to {b}
Fs = f.wrench(rb, fb)

print('\n\n----------------------------------------\n')
print(f'NUMBER 4: \n\nFb = {Fb}\n\nFs = {Fs}')
print('\n----------------------------------------\n\n')

