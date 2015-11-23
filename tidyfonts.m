function tidyfonts(sz)

if nargin<1
    sz=16;
end

hf = gcf;
%set(hf,'fontsize',13);

ha = get(hf,'children');

for i=1:length(ha)
    
    hp = get(ha(i),'children');
    hx = get(ha(i),'xlabel');
    hy = get(ha(i),'ylabel');
    hz = get(ha(i),'zlabel');
    ht = get(ha(i),'title');
    
    set(hx,'fontsize',sz);
    set(hy,'fontsize',sz);
    set(hz,'fontsize',sz);
    set(ht,'fontsize',sz);
    %set(hp,'fontsize',sz);
    
    set(ha(i),'fontsize',sz*0.8);
    
end

end
