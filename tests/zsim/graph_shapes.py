 
import numpy as np

# create different structures

def set_const(M,c):
    return M*c

def set_random(M, minM, maxM):
    dim = len(M.shape)
    if dim==1: # vector
        r = M*(np.random.rand(M.shape[0])*(maxM-minM)+minM)
    if dim==2: # vector
        r = M*(np.random.rand(M.shape[0],M.shape[1])*(maxM-minM)+minM)
    return r

def set_randomn(M, minM, maxM):
    dim = len(M.shape)
    if dim==1: # vector
        r = np.maximum(np.minimum(M*np.random.randn(M.shape[0]),maxM),minM)
    if dim==2: # vector
        r = np.maximum(np.minimum(M*np.random.randn(M.shape[0],M.shape[0]),maxM),minM)
    return r


def create_full_conn(n):
    b = np.ones((n,1))
    W = np.ones((n,n))
    return W,b

def create_chain(n):
    b = np.ones((n,1))
    W = np.zeros((n,n))

    for i in xrange(n-1):
        W[1+i,i] = 1
    return W,b

def create_ring(n):

    b = np.ones((n,1))
    W = np.zeros((n,n))

    for i in xrange(n-1):
        W[1+i,i] = 1
    W[0,n-1] = 1
    return W,b

# create star shape network with n neurons = 1 in the middle and n-1 around
# the hub one goes at the position 0!
def create_star(n):
    b = np.ones((n,1))
    W = np.zeros((n,n))

    for i in xrange(n-1):
        W[1+i,0] = 1
    return W,b
    
def create_feedforward_reccurent(n_in, n_out,maxW, maxB):
    # n_in number of incomming neurons, n_out output neurons with recurrent connections, to all input
    b_in = np.ones((n_in,1))
    b_out = np.ones((n_out,1))
    W = np.zeros((n_in+n_out,n_in+n_out))

    for i in xrange(n-1):
        W[1+i,0] = 1
    return W,b