import sys
import numpy as np
import scipy.linalg
import time

def derA1b(n,W,b,i): # im indecies of weight_im
    ncomb = pow(2,n)
    d = np.zeros(ncomb)
    comb = np.zeros(n)

    for ind in xrange(ncomb-1):
        if comb[i] == 0:
            u = np.sum(W[i,:]*comb) + b[i]
            d[ind]= -np.exp(u)
        br = n-1
        while comb[br] == 1:
            comb[br]=0
            br-=1
        comb[br]=1

    return np.diag(d)

def derA1(n,W,b,i,m): # im indecies of weight_im
    ncomb = pow(2,n)
    d = np.zeros(ncomb)
    comb = np.zeros(n)

    for ind in xrange(ncomb-1):
        if comb[i] == 0 and comb[m] == 1:
            u = np.sum(W[i,:]*comb) + b[i]
            d[ind]= -np.exp(u)
        br = n-1
        while comb[br] == 1:
            comb[br]=0
            br-=1
        comb[br]=1

    return np.diag(d)

def constructA1(n,W,b):
    """
    Construct A with only A0 elements, so that you have only exp elements on diagonal
    """
    ncomb = pow(2,n)
    D = np.zeros(ncomb)
    D[ncomb-1]=1.
    comb = np.zeros(n)

    for ind in xrange(ncomb-1):
        ##na diagonali napisi -br el of 1s
        #br = 0
        #for i in xrange(n):
            #if comb[i] == 1:
                #br+=1
        #D[ind]=-br
        
        s = 0
        for i in xrange(n):
            if comb[i] == 0:
                u = np.sum(W[i,:]*comb) + b[i]
                s+= np.exp(u)
        D[ind] += -s
        br = n-1
        while comb[br] == 1:
            comb[br]=0
            br-=1
        comb[br]=1

    # create explicitly A
    A = np.zeros((ncomb,ncomb))
    
    # write D
    for i in xrange(ncomb):
      A[i,i]=D[i]
   
   # write ones at specific positions
    for i in xrange(n):
        startd = pow(2,i)
        ngrupa= pow(2,n-i-1)
        #print "diag",i
        for j in xrange(ngrupa):
            #print "   group",j
            for k in xrange(startd):
                #print "        el",k
                A[2*j*startd+k,startd+2*j*startd+k]=1
                #print "        poz",2*j*(i+1)+k,startd+2*j*(i+1)+k
    #write last row with ones
    A[-1,:]=1
    return A

def constructA0(n,W,b):
    """
    Construct A with only A1 elements, so that you have exp elements outside of diagonal
    """
    ncomb = pow(2,n)
    comb = np.zeros(n)

    # create explicitly A
    A = np.zeros((ncomb,ncomb))
    
    for ind in xrange(ncomb):
        #na diagonali napisi -br el of 1s
        br = 0
        for i in xrange(n):
            if comb[i] == 1:
                br+=1
        A[ind,ind]=-br
        #write off diagonal elements
        for i in xrange(n):
            if comb[i] == 1:
                combZ=comb.copy()
                combZ[i]=0
                u = np.sum(W[i,:]*combZ) + b[i]
                broj = ind-pow(2,n-i-1)
                A[ind,broj] = np.exp(u)
        br = n-1
        while comb[br] == 1:
            comb[br]=0
            br-=1
        comb[br]=1
    #write ones to first row
    A[0,:]=1
    return A

def derAb(n,W,b,i): # im indecies of weight_im
    ncomb = pow(2,n)
    A = np.zeros((ncomb,ncomb))
    comb = np.zeros(n)

    for ind in xrange(ncomb-1):
        if comb[i] == 0:
            u = np.sum(W[i,:]*comb) + b[i]
            A[ind,ind]= -np.exp(u)
            A[ind+int(pow(2,n-i-1)),ind]= np.exp(u)
        br = n-1
        while comb[br] == 1:
            comb[br]=0
            br-=1
        comb[br]=1
    
    # set last row to zeros
    A[-1,:]=0
    return A
        
def derA(n,W,b,i,m): # im indecies of weight_im
    ncomb = pow(2,n)
    A = np.zeros((ncomb,ncomb))
    comb = np.zeros(n)

    for ind in xrange(ncomb-1):
        if comb[i] == 0 and comb[m] == 1:
            u = np.sum(W[i,:]*comb) + b[i]
            A[ind,ind]= -np.exp(u)
            A[ind+int(pow(2,n-i-1)),ind]=np.exp(u)
        br = n-1
        while comb[br] == 1:
            comb[br]=0
            br-=1
        comb[br]=1
    
    # set last row to zeros
    A[-1,:]=0
    return A

def constructA(n,W,b):
    """
    Construct A with A0 and A1 elements, correct!
    """
    ncomb = pow(2,n)
    comb = np.zeros(n)

    # create explicitly A
    A = np.zeros((ncomb,ncomb))
    
    for ind in xrange(ncomb):
        #na diagonali napisi -br el of 1s
        br = 0
        for i in xrange(n):
            if comb[i] == 1:
                br+=1
        A[ind,ind]=-br
        #write all other elemtns: diagonal and off diagonal elements
        s=0
        for i in xrange(n):
            if comb[i] == 1:
                combZ=comb.copy()
                combZ[i]=0
                u = np.sum(W[i,:]*combZ) + b[i]
                broj = ind-pow(2,n-i-1)
                A[ind,broj] = np.exp(u)
            else:
                u = np.sum(W[i,:]*comb) + b[i]
                s+= np.exp(u)
        A[ind,ind] -= s
        br = n-1
        while comb[br] == 1:
            comb[br]=0
            br-=1
        comb[br]=1

    # write ones at specific positions
    for i in xrange(n):
        startd = pow(2,i)
        ngrupa= pow(2,n-i-1)
        #print "diag",i
        for j in xrange(ngrupa):
            #print "   group",j
            for k in xrange(startd):
                #print "        el",k
                A[2*j*startd+k,startd+2*j*startd+k]=1
                #print "        poz",2*j*(i+1)+k,startd+2*j*(i+1)+k
    # to correctly set the problem last row (111...) should be overwritten with ones!
    A[-1,:]=1
    return A

def solve_inverse(n,W,b):
    ########
    # solution for small number of neurons, up to 15 probably (12 needs 11sec)
    ########

    #define b and W and number of neurons
    #n = 2 # 8sec za n=15,  2^(n-15)*8

    ncomb = pow(2,n)

    D = np.zeros(ncomb)
    D[ncomb-1]=1.

    pows = np.zeros(n)
    pows[0]=1
    for i in xrange(1,n):
        pows[i] = pows[i-1]*2
    # invert pows to match binary coding
    pows=pows[::-1]
    comb = np.zeros(n)
    ind = 0

    size = np.ones(n)*2
    allcomb = np.indices(size).reshape(len(size), -1).T
    sallcomb = sum(allcomb.T,0)

    for ind in xrange(ncomb-1):
        s = 0
        for i in xrange(n):
            if comb[i] == 0:
                u = np.sum(W[i,:]*comb) + b[i]
                s+= np.exp(u)
        D[ind] = -s
        br = n-1
        while comb[br] == 1:
            comb[br]=0
            br-=1
        comb[br]=1

    # calculating l (normalization const)
    l = np.zeros(ncomb)
    l[0]=1/D[0]

    # for each level (number of active neurons), not zero
    for i in xrange(1,n+1):
        # create all combinations
        combs = allcomb[sallcomb==i]
        for comb in combs:
            # get index of it
            ind = int(sum(pows*comb))
            s = 0
            for i in xrange(n):
                if comb[i] == 1:
                    broj = ind-pows[i]
                    s+= l[broj]
            l[ind]=(1-s)/D[ind]

    #now calculate probabilities!
    # calculating p 
    p = np.zeros(ncomb)
    p[-1]=1/l[-1]

    # for each level (number of active neurons), not zero
    for i in xrange(1,n+1):
        # create all combinations
        combs = allcomb[sallcomb==n-i]
        for comb in combs:
            # get index of it
            ind = int(sum(pows*comb))
            s = 0
            for i in xrange(n):
                if comb[i] == 0:
                    broj = ind+pows[i]
                    s+= p[broj]
            p[ind]=-s/D[ind]

    return p, D, l
