function [f_cable,output,exitflag]=fun_find_pos(pose_ee,~)

% options=optimoptions('fmincon','Display','iter','FunctionTolerance',1e-12,'OptimalityTolerance',1e-11,'MaxIterations',1000);
options=optimoptions('fmincon','Display','final','FunctionTolerance',1e-12,'OptimalityTolerance',1e-11,'MaxIterations',1000);
%   f(1) f(2) f(3) f(4) f(5)   f(6)   f(7)    f(8)    f(9)    f(10)

x0=[0.1;   0.1;   0.1;    2000;    2000;    2000;    2000;    10;    10;    10;    10;    0];
lb=[-15;   -15;    -7;     200;     200;     200;     200;     1;     1;     1;     1;    0];
ub=[ 15;    15;     7;   40000;   40000;   40000;   40000;   100;   100;   100;   100;    0];
[f_cable,output,exitflag,]=fmincon(@(x) fun_min_slove_min(x,pose_ee),x0,[],[],[],[],lb,ub,@(x) fun_position(x,pose_ee),options);

end