function idx = findhump(y,direction,type)

y = reshape(y,length(y),1);

if strcmp(direction,'backwards')
    y = flipud(y);
end

if strcmp(type,'trough')
    y = -y;
end

dy = deriv(y,1);

idx = find((diff(dy<0)>0),1,'first');

if strcmp(direction,'backwards')
    idx = length(y) - idx + 1;
end

end
