 
import matplotlib.pyplot as plt
import numpy as np
import numpy.linalg as alg

import plot_results as pr
import solve_general as sg
import graph_shapes as gs

import simulation2 as sim
import functions as fun

#np.random.seed(2)

n = 2 # 8sec za n=15,  2^(n-15)*8

maxB = -1 # max bias
minB = 1 # min bias
maxW = 2 # max weights
minW = -2 # min weights

#W,b = gs.create_full_conn(n)
#W,b = gs.create_ring(n)
#W,b = gs.create_chain(n)
#W,b = gs.create_star(n)

#W = gs.set_random(W,minW,maxW)
#b = gs.set_random(b,minB,maxB)

W = np.array([[0,np.log(3)],
              [np.log(3),0]])
b = np.array([np.log(2),np.log(2)])
# set diagonal elements to 0
for i in xrange(n):
    for j in xrange(n):
        if i==j:
            W[i,i]=0
            
print W
print b


tau = 10 # ms
simtime = 10000000# ms = 100s 10000000
simp, spikes = sim.simulate(n,W,b,tau, simtime)
print "simulated prob", simp

A = sg.constructA(n,W,b)
iA = alg.inv(A)
p = iA[:,-1] # current prob distribution
print "calculated prob:", p

#corr = fun.zcorrelation(n,p)
#print corr
#print "max", corr.max()

fig = plt.figure(figsize=(6,3))
ax = fig.add_subplot(111)
pr.comparebars(ax,n, simp, p)


print "1",simp[1]+simp[3]
print "2",simp[2]+simp[3]
#fig = plt.figure()
#ax1 = fig.add_subplot(121)
#ax1.imshow(W, interpolation='nearest')
#ax2 = fig.add_subplot(122)
#ax2.imshow(corr, interpolation='nearest')

plt.show()