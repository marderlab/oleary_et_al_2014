function tidyfig()

hf = gcf;
%set(hf,'fontsize',13);

ha = get(hf,'children');

for i=1:length(ha)
    
    hp = get(ha(i),'children');
    hx = get(ha(i),'xlabel');
    hy = get(ha(i),'ylabel');
    ht = get(ha(i),'title');
    
    set(hx,'fontsize',16);
    set(hy,'fontsize',16);
    set(ht,'fontsize',16);
    
    set(ha(i),'fontsize',14);
    
    for j=1:length(hp)
        
        set(hp,'linewidth',3);
    end
end

end
