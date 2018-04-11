
import numpy as np

def frates(n,W,b):
    # calculate in each combination firing rate for each neuron
    size = np.ones(n)*2
    allcomb = np.indices(size).reshape(len(size), -1).T
    comb = np.zeros(n)
    ncomb = len(allcomb)
    frs = np.zeros((n,ncomb)) # firing rates for each neuron in each combination
    
    for ind in xrange(ncomb-1):
        for i in xrange(n):
            if comb[i] == 0:
                u = np.sum(W[i,:]*comb) + b[i]
                frs[i,ind]=np.exp(u)
        br = n-1
        while comb[br] == 1:
            comb[br]=0
            br-=1
        comb[br]=1
    
    #print "frs",frs
    # sum of all firing rates for each combination
    fr = sum(frs)
    # correct last row which has all zeros
    fr[ncomb-1]=1
    # normalize all firing rates
    for i in xrange(n):
        frs[i,:]/=fr
    #print "frs normalized",frs
    # sum up probabilities for neuron
    for ind in xrange(ncomb-1):
        for i in xrange(1,n):
            frs[i,ind]+=frs[i-1,ind]
    #print "frs additive",frs
    return fr,frs


def currentfrates(n,W,b,comb):
    frs = np.zeros(n) # firing rates for each neuron in each combination
    for i in xrange(n):
        if comb[i] == 0:
            frs[i] = np.exp(np.sum(W[i,:]*comb) + b[i])
    fr = sum(frs)
    # normalize all firing rates
    frs/=fr
    # sum up probabilities for neuron
    for i in xrange(1,n):
        frs[i]+=frs[i-1]
    return fr,frs


def rewindbuff(buff,ibuff,ps,pe):
    #rewind
    nbuff = np.zeros(len(buff))
    nibuff = np.zeros(len(ibuff))
    #print "rewind buff", buff
    #print "rewind ibuff", ibuff
    #print ps,pe
    for i in xrange(ps,pe+1):
        nbuff[i-ps]=buff[i] # no overlap is possible due to big buffer
        nibuff[i-ps]=ibuff[i]
    pe=pe-ps
    ps=0
    #print "rewind buff", nbuff
    #print "rewind ibuff", ibuff
    #print ps,pe
    return nbuff,nibuff,ps,pe
    
# simulation for given network in continuos time with rectangular EPSP
def simulate(n,W,b,tau, simtime):
    # create allfiringrate, firingrate, counter
    ncomb = int(pow(2,n))

    allFR, fp = frates(n,W,b)
    counter = np.zeros(ncomb)
    
    #print allFR
    #print fp
    # calculate powers of 2
    pows = np.zeros(n)
    pows[0]=1
    for i in xrange(1,n):
        pows[i] = pows[i-1]*2
    # invert pows to match binary coding
    pows=pows[::-1]
    
    ps = 0# ps, pe pointer star and end to buffer
    pe = 0
    maxbuff = 1000000 # spikes in principle
    buff = np.zeros(maxbuff)# buffer holds stop times
    ibuff = np.zeros(maxbuff)# buffer which holds info which neuron spiked
    
    # keep info of all spikes
    spikes = []
    for i in xrange(n):
        spikes.append([])
    
    #simulation
    t=0
    state = 0 # current state in decade sys   
    broj=0
    brsp=0
    #while t<simtime and broj<10:
    while t<simtime:
        broj+=1
        if state==ncomb-1: #if last state, everyone spiked, then no new spike can be produced
            #print broj,"),t=",t, "the last state", state
            ps+=1
            ns = buff[ps]
            #if ns-t<0:
               #print "wwhaaaat?"            
            counter[state]+=(ns-t)
            #print "    counter[",state,"]=",counter[state]
            #new state?
            ni = ibuff[ps]
            state-=pows[ni]
            #print "    neuron",ni," stopped"
            #print "    new state", state
            t=buff[ps]
            continue
        # end of while loop in that case
        #fr = allFR[state]
        fr = 1000./tau*allFR[state]
        #print broj,"),t=",t, "state", state, "rate", fr
        # determine next spike
        #s = np.random.expovariate(fr)
        s=np.random.exponential(1000/fr)# gives time in ms
        #s=np.random.exponential(100/fr)
        #print "  next spajk in",s, "(at",t+s,")"
        #print "  ps==pe", ps, pe
        if pe==ps: # no limit, if this is the case neuron tau period is over
        # this is not true only for start, so first spike should be done outside the loop
            #print "    yes", "pe+1",pe+1            
            if pe==maxbuff-1:
                buff,ibuff,ps,pe=rewindbuff(buff,ibuff,ps,pe)
            pe+=1
            buff[ps] = t+s
            buff[pe] = t+s+tau
            #print "     buff[ps,pe]", buff[ps:pe+1]
            # duration is s always
            counter[state]+=s # the only problem is last spike which could go over simtime
            #print "    counter[",state,"]=",counter[state]
            #counter[state]+=min(s,simtime-t)
            # determine who spiked
            r = np.random.rand()            
            ni = 0
            suma = 0
            for i in xrange(n):
                #print "      i", i
                #print "      if r<fp", r<fp[i,state]
                if r<fp[i,state]:
                    ni=i
                    break
            #print "     neuron",ni," spiked(r=",r,")"
            ibuff[pe]=ni
            brsp+=1
            #neuron ni spiked so save spike time
            spikes[ni].append(int(t+s)) # we are interested only in discretized version
            #next state
            # neurons tau period is over, so change the state
            # ili je gotov period tau ili je neuron spajkao (samo za t=0 true)
            state = state+pows[ni]
            #print "    new state", state
                        
        else: # if we need to check for next stop and change the firing rate
            #print "    no"
            ns = buff[ps+1]
            #print "    next stop", ns
            #if spike occures inbetween ct and ns then accept it:
            #print "    spike valid?"
            if ns-t>s:
                #print "      yes", ns-t,">",s
                buff[ps]=t+s                                            
                if pe==maxbuff-1:
                   buff,ibuff,ps,pe=rewindbuff(buff, ibuff,ps,pe)
                pe+=1
                buff[pe]=t+s+tau
                #print "       buff[ps,pe]", buff[ps:pe+1]
                counter[state]+=s
                #print "    counter[",state,"]=",counter[state]
                # determine who spiked
                r = np.random.rand()
                ni = 0
                suma = 0
                for i in xrange(n):
                    if r<fp[i,state]:
                        ni=i
                        break
                #print "     neuron",ni," spiked"
                ibuff[pe]=ni
                brsp+=1
                #neuron ni spiked so save spike time
                spikes[ni].append(int(t+s)) # we are interested only in discretized version
                #new state
                state+=pows[ni]
                #print "    new state", state
            else: # discard spike
                #print "      no", ns-t,">",s
                ps+=1
                counter[state]+=(ns-t)
                #if ns-t<0:
                   #print "wwhaaaat?"
                #print "    counter[",state,"]=",counter[state]
                # new state?
                ni = ibuff[ps]
                state-=pows[ni]
                #print "    neuron",ni," stopped"
                #print "    new state", state
            
        #end else
        t=buff[ps]
    #print "counter",counter
    #print "broj",broj
    #print "state",state
    #print buff[:20]
    #print ibuff[:20]
    #print ps,pe

    #print broj, brsp, t
    # convert counter to probabilities
    scounter = sum(counter)
    #print scounter
    pcounter = counter/scounter
    #print "simprobabilities",pcounter
    return pcounter, spikes
            
            
def simulateSTDP(n,W,b,tau,simtime): #tau
    """
    Simulate in contionus time and apply learning through STDP
    """

    ncomb = int(pow(2,n))

    #allFR, fp = frates(n,W,b)
    counter = np.zeros(ncomb)

    # calculate powers of 2
    pows = np.zeros(n)
    pows[0]=1
    for i in xrange(1,n):
        pows[i] = pows[i-1]*2
    # invert pows to match binary coding
    pows=pows[::-1]
    
    ps = 0# ps, pe pointer star and end to buffer
    pe = 0
    maxbuff = 1000000 # spikes in principle
    buff = np.zeros(maxbuff)# buffer holds stop times
    ibuff = np.zeros(maxbuff)# buffer which holds info which neuron spiked
    
    # keep info of all spikes
    spikes = []
    spikesC = []
    for i in xrange(n):
        spikes.append([])
        spikesC.append([])
    
    #learning with STDP
    Ap = 0.1 # amplitude for positive part
    An = 0.1 # amplitude for negative part
    tp = 5. # decay const for positive part
    tn = 5. # decay const for negative part
    lastnspikes = 10
    scalling = 0.06
    
    #simulation
    t=0
    state = 0 # current state in decade sys   
    comb = np.zeros((n,1))
    broj=0
    brsp=0
    while t<simtime:
        broj+=1
        if state==ncomb-1: #if last state, everyone spiked, then no new spike can be produced
            ps+=1
            ns = buff[ps]
            counter[state]+=(ns-t)
            ni = ibuff[ps]
            state-=pows[ni]
            comb[ni]=0
            t=buff[ps]
            continue
        # end of while loop in that case
        #fr = allFR[state]
        #print fp[:,state]
        fr,fp = currentfrates(n,W,b,comb)
        #print "fr",fr
        #print "fr",fp
        # determine next spike
        s=np.random.exponential(tau/fr)# gives time in ms
        if pe==ps: # no limit, if this is the case neuron tau period is over
            if pe==maxbuff-1:
                buff,ibuff,ps,pe=rewindbuff(buff,ibuff,ps,pe)
            pe+=1
            buff[ps] = t+s
            buff[pe] = t+s+tau
            # duration is s always
            counter[state]+=s # the only problem is last spike which could go over simtime
            # determine who spiked
            r = np.random.rand()
            ni = 0
            suma = 0
            for i in xrange(n):
                #if r<fp[i,state]:
                if r<fp[i]:
                    ni=i
                    break
            ibuff[pe]=ni
            brsp+=1
            #neuron ni spiked so save spike time
            spikes[ni].append(int(t+s)) # we are interested only in discretized version
            spikesC[ni].append(t+s) # we also need real time for STDP
            #learing - current assumtion, all to all connectivity
            # no dependance on current weight
            #first consider ni neuron as postsynaptic
            for i in xrange(n):
                W[ni,i]-=scalling*W[ni,i]
            for i in xrange(n):
                if i != ni:
                    ispiked = len(spikesC[i])
                    for j in xrange(min(lastnspikes, ispiked)):
                        spiketime = spikesC[i][-1-j]
                        W[ni,i]+=Ap*np.exp(-(t+s-spiketime)/tp)
            #now consider ni neuron as presynaptic
            for i in xrange(n):
                if i != ni:
                    ispiked = len(spikesC[i])
                    for j in xrange(min(lastnspikes, ispiked)):
                        spiketime = spikesC[i][-1-j]
                        W[i,ni]-=An*np.exp(-(t+s-spiketime)/tn)
            #next state
            # neurons tau period is over, so change the state
            # ili je gotov period tau ili je neuron spajkao (samo za t=0 true)
            state+=pows[ni]
            comb[ni]=1
        else: # if we need to check for next stop and change the firing rate
            ns = buff[ps+1]
            #if spike occures inbetween ct and ns then accept it:
            if ns-t>s:
                buff[ps]=t+s                                            
                if pe==maxbuff-1:
                   buff,ibuff,ps,pe=rewindbuff(buff, ibuff,ps,pe)
                pe+=1
                buff[pe]=t+s+tau
                counter[state]+=s
                # determine who spiked
                r = np.random.rand()
                ni = 0
                suma = 0
                for i in xrange(n):
                    #if r<fp[i,state]:
                    if r<fp[i]:
                        ni=i
                        break
                ibuff[pe]=ni
                brsp+=1
                #neuron ni spiked so save spike time
                spikes[ni].append(int(t+s)) # we are interested only in discretized version
                spikesC[ni].append(t+s) # we also need real time for STDP
                #learing - current assumtion, all to all connectivity
                # no dependance on current weight
                #first consider ni neuron as postsynaptic
                for i in xrange(n):
                    if i != ni:
                        ispiked = len(spikesC[i])
                        for j in xrange(min(lastnspikes, ispiked)):
                            spiketime = spikesC[i][-1-j]
                            W[ni,i]+=Ap*np.exp(-(t+s-spiketime)/tp)
                #now consider ni neuron as presynaptic
                for i in xrange(n):
                    if i != ni:
                        ispiked = len(spikesC[i])
                        for j in xrange(min(lastnspikes, ispiked)):
                            spiketime = spikesC[i][-1-j]
                            W[i,ni]-=An*np.exp(-(t+s-spiketime)/tn)
                #new state
                state+=pows[ni]
                comb[ni]=1
            else: # discard spike
                ps+=1
                #print state
                counter[state]+=(ns-t)
                # new state?
                ni = ibuff[ps]
                state-=pows[ni]
                comb[ni]=0
        #end else
        t=buff[ps]
    #print "counter",counter
    #print "broj",broj
    #print "state",state
    #print buff[:20]
    #print ibuff[:20]
    #print ps,pe

    #print broj, brsp, t
    # convert counter to probabilities
    scounter = sum(counter)
    #print scounter
    pcounter = counter/scounter
    #print "simprobabilities",pcounter
    return pcounter, spikes




































