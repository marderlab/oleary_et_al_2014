% bp = [period, freq, DC, SloAmp, SpikeHeight]
function bp = burstParams(v,dt,thr)

st = spikeTimes(v,thr);

ISI = diff(st);
mid = (max(ISI) + min(ISI))/2;
IBI = (ISI(ISI >= mid));
first_spikes = ([0 diff((ISI < mid))] > 0);
first_spike_times = st(first_spikes);

if length(first_spike_times) < 5
    %error('not enough bursts (< 5)');
    bp = [NaN, NaN, NaN, NaN, NaN];
    return;
end

period = mean(diff(first_spike_times));
DC = (period - mean(IBI))/period;
fastISI = mean(ISI(ISI < mid));

% everything up to here in samples (not time units)

%%%%%%%%%% find threshold
seglength = floor(mean(IBI));

thrvals = zeros(length(first_spike_times-2)-1,1);
mins = thrvals;

for i=2:length(first_spike_times)
    try
        vseg = v(first_spike_times(i)-seglength:first_spike_times(i));

        mins(i-1) = min(vseg);

        k = deriv(vseg,1e-6,2)./((1+deriv(vseg,1e-6).^2).^1.5);

        %plot(vseg);
        %hold on;
        %plot(k);

        thridx = findhump(k,'backwards','');
        thrvals(i-1) = vseg(thridx);
    catch
        'error'
        bp = nan(1,5);
        return;
    end
end

%%%%%%%%%%% find spike heights

spheight = zeros(length(st)-1,1);

for i=1:length(st)-2
    %i
    %length(st)
    vseg = v(st(i):st(i) + floor(fastISI));
    maxidx = findhump(vseg,'forwards','');
    if ~isempty(maxidx)
        spheight(i) = vseg(maxidx);
    else
        spheight(i) = NaN;
    end
end

%plot(v,'b');
%hold on;
%plot(st(1:end-1),spheight,'*k');
%plot(first_spike_times,mean(spheight),'*r');

slomax = mean(thrvals);
sh = mean(spheight) - slomax;
sloamp = slomax - mean(mins);

%plot(v,'b');
%v(v>slomax) = slomax;
%hold on;
%plot(v,'r');

freq = 1000/(dt*fastISI);

bp = [dt*period freq DC sloamp sh];

end

function s = spikeTimes(v,t)

s = find(diff(v >= t) > 0);
end


