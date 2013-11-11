function qv = qfun(kk)
    global a
    global b
    global bb
    global cc
    a = kk(1);
    b = kk(2);
    [t x y] = sim('uk', [0 5]);
    qv = (y(end,1)-bb)^2 + (y(end,2)-cc)^2;

    plot(t, y);
    hold on;
    plot(5, bb, 'or');
    plot(5, cc, 'og');

end