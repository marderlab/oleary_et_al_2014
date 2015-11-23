function d = deriv(y,dx,order)

x = 1:length(y);
sy = spline(x,y);

if nargin == 2
    dsy = fnder(sy);
else
    dsy = fnder(sy,order);
end

d = fnval(dsy,x)/dx;

end



