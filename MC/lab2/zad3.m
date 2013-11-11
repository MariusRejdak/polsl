function dx = zad3( ~,x )
    global a;
    u = 1;
    dx = [-x(2); x(1) - 2*a*x(2) + u];
end
