%{
Code for generating Fig 3 of O'Leary, Williams, Franci % Marder (2014) Neuron
This code is for use as-is: no warranty or support provided!
You need mex configured to compile C++ to run this code

Copyright Timothy O'Leary 2014
Please cite/attribute if used
%}

clc;
close all;
clear;
mex int_control_compartment.cpp
mex run_comp.cpp

rang = 2.0e-1;

nplot = 20;

dt = 0.1;
taug = 1.0e2;
taum = 10*taug;
tstop = 600*taug;
ms_per_samp = tstop/1000;
e_na = 30;
e_leak = -50;

% example 1: bursting cell
target = 7.0;
simparams = [taug,taum,target,...
    15.0,...    %Na
    0.18,...    %CaT
    0.22,...    %CaS
    2.0,...     %KA
    8.0,...     %KCa
    5.0,...     %Kd
    0.08];      %H
%}

gs_end = zeros(nplot,8);
gs_bef = zeros(nplot,8);

for i=1:nplot
    
    gbar_leak = 0.001+rand*0.199;
    gbar_na = rand*rang;
    gbar_kd = rand*rang;
    gbar_ka = rand*rang;
    gbar_kca = rand*rang;
    gbar_cas = rand*rang;
    gbar_cat = rand*rang;
    gbar_h = rand*rang;
    gs = [gbar_na, gbar_cat, gbar_cas, gbar_ka, gbar_kca, gbar_kd, gbar_h, gbar_leak];
    v = int_control_compartment([dt tstop floor(ms_per_samp/dt) e_leak],[gs e_leak e_na -80 -20],simparams);
    gs_end(i,:) = v(3:10,end)';
    gs_bef(i,:) = gs;
    
    t = linspace(1/taug,tstop/taug,length(v));
    labl = {'g_{Na}', 'g_{CaT}', 'g_{CaS}', 'g_{KA}', 'g_{KCa}', 'g_{Kdr}', 'g_{H}'};

    tracecol = [0.2 0.2 0.3];
    if i <= nplot-1
        load plotcolours;
        colrs = brighten(colrs,0.7);
        linw = 1.0;
    else
        load plotcolours;
        linw = 2.0;
    end
    set(gcf,'Renderer','Zbuffer');
    
    hold on;
    set(gca,'colororder',colrs);
    plot(t,v(3:9,:),'linewidth',linw);
    set(gca,'yscale','log','xscale','log');
    box off;
    axis tight;
    drawnow;
end

set(gca,'ytick',logspace(-2,2,3),'xminortick','on','yminortick','on');
xlabel('time/tau_g');
ylabel('conductance (\muS)');
tidyfonts(19);
sizefig(700,400);

%%%%%%%%%%%%%%%%%%%%%%%%%%% example traces
figure;

nexamp = 7;
H = tight_subplot(nexamp,2,0.06,0.05,0.05);

tstop = 2e3;
bparam = zeros(nplot,5);
vparam = zeros(nplot,3);

for i=1:nplot
    vb = run_comp([dt tstop 1 e_leak],[gs_bef(i,:) e_leak e_na -80 -20]);
    t = linspace(0,tstop/(1e3),length(vb));
    I = 1+mod(i,nexamp);
    plot(H(2*I-1),t,vb(1,:),'color',tracecol,'linewidth',2);
    set(H(2*I-1), 'ylim', [-70 30], 'ytick', [-50 0], 'xlim', [0.0 1.0], 'xcolor', [1 1 1], 'box', 'off');
    
    va = run_comp([dt tstop 1 e_leak],[gs_end(i,:) e_leak e_na -80 -20]);
    v = va(1,length(va)/4:end);
    t = linspace(0,3*tstop/4,length(v));
    st = spikeTimes(v,-10,0.1);
    ISI = diff(st);
    IBI = range(ISI);
    first_spikes = ([0 diff((ISI < IBI/2))] > 0);
    first_spike_times = st(first_spikes);
    if length(first_spike_times) < 2
        xlim = [tstop/2 tstop/2+500];
    else
        xlim = [first_spike_times(2)-50 first_spike_times(2)+450];
    end
    plot(H(2*I),t,v,'color',tracecol,'linewidth',2);
    set(H(2*I), 'ylim', [-70 30], 'ytick', [-50 0], 'xlim', xlim, 'xcolor', [1 1 1], 'box', 'off');
    %[period, freq, DC, SloAmp, SpikeHeight]
    bparam(i,:) = burstParams(v,dt,-20);
    vparam(i,:) = [median(v) min(v) max(v)];
end

tidyfonts(19);
sizefig(800,700);

%%%%%% correlation plot
figure;
set(0,'DefaultAxesColorOrder',colrs(1:end,:));
[H,AX,BigAx,P,PAx] = plotmatrix_spaced(gs_end(:,1:7));
for i=1:7
    cla(PAx(i));
    set(PAx(i),'box','off','xcolor',[1 1 1],'ycolor',[1 1 1]);
    for j=1:7
        set(AX(i,j),'ticklength',[0.025 0.025]);
        if i<=j
            cla(AX(i,j));
            set(AX(i,j),'xcolor',[1 1 1],'ycolor',[1 1 1]);
            set(AX(i,j),'box','off');
        end
    end
end
title('Maximal conductances');
tidyfonts(18);
sizefig(900,600);

%%%%%% correlation plot of properties
set(0,'DefaultAxesColorOrder',colrs(4:end,:));
figure;
[H,AX,BigAx,P,PAx] = plotmatrix_spaced(bparam);
for i=1:5
    cla(PAx(i));
    set(PAx(i),'box','off','xcolor',[1 1 1],'ycolor',[1 1 1]);
    for j=1:5
        set(AX(i,j),'ticklength',[0.025 0.025]);
        if i==j
            cla(AX(i,j));
            set(AX(i,j),'xcolor',[1 1 1],'ycolor',[1 1 1]);
            set(AX(i,j),'box','off');
        end
    end
end
title('Intrinsic properties');
tidyfonts(18);
sizefig(800,600);
