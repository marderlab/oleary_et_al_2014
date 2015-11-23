function spiketimes = spikeTimes(Vm, thr, dt, t1, t2)
    
    if nargin == 5
        Vm = Vm(floor(t1/dt):floor(t2/dt));
        spiketimes = dt*find(diff(Vm > thr)>0);
    elseif nargin == 3
        spiketimes = dt*find(diff(Vm > thr)>0);
    else
        spiketimes = find(diff(Vm > thr)>0);
    end

end

