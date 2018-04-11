import numpy as np
 
 # pz is stationary distribution
def zcorrelation(n,pz):
    # should be (ncomb = len(pz)) == pow(2,n)
    corr = np.zeros((n,n))
    marg = np.zeros((n,n,4)) # marginalization table
    
    if n==1:
        corr[0,0]=1
        return corr
    
    ## calculate powers of 2
    #pows = np.zeros(n)
    #pows[0]=1
    #for i in xrange(1,n):
        #pows[i] = pows[i-1]*2
    ## invert pows to match binary coding
    #pows=pows[::-1]
    
    #prepare all combinations of n-2 elements
    size = np. ones(n)*2
    allcomb = np.indices(size).reshape(len(size), -1).T

    for ind, comb in enumerate(allcomb):
        # repeat for each combination (upper triangular-exluding selfcorrelation)
        p = pz[ind]
        for i in xrange(n):
            for j in xrange(i+1,n):
                par = comb[i]*2+comb[j]
                marg[i,j,par]+=p
    # calculate corr for each upper triangle pair
    for i in xrange(n):
        for j in xrange(i+1,n):
            dolje = (marg[i,j,2]+marg[i,j,3])*(marg[i,j,0]+marg[i,j,1])*(marg[i,j,0]+marg[i,j,2])*(marg[i,j,1]+marg[i,j,3])
            corr[i,j]=(marg[i,j,3]*marg[i,j,0]-marg[i,j,2]*marg[i,j,1])/np.sqrt(dolje)

    # mirror the correlation from upper triangle to bottom one
    for i in xrange(n):
        for j in xrange(i+1,n):
            corr[j,i]=corr[i,j]
    return corr