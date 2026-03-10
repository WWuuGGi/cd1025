function [f_cable,output,exitflag]=fun_find_acce(pose_ee,~,ff)

% options=optimoptions('fmincon','Display','iter','FunctionTolerance',1e-12,'OptimalityTolerance',1e-11,'MaxIterations',1000);
options=optimoptions('fmincon','Display','final','FunctionTolerance',1e-12,'OptimalityTolerance',1e-11,'MaxIterations',1000);
%   f(1) f(2) f(3) f(4) f(5)   f(6)   f(7)    f(8)    f(9)    f(10)
% if(pose_ee(1)>0)
%    angle_ub=-14.99999;
%    angle_lb=-15;
%    angle_x0=-15;
% else
%    angle_ub=15;
%    angle_lb=14.99999;
%    angle_x0=15;
% end
x0=[0.1;   0.1;   0.1;    2000;    2000;    2000;    2000;    10;    10;    10;    10];
lb=[-15;   -50;    -7;     200;     200;     200;     200;     1;     1;     1;     1];
ub=[ 15;    50;     7;   40000;   40000;   40000;   40000;   100;   100;   100;   100];
[f_cable,output,exitflag,]=fmincon(@(x) fun_min_slove_min(x,pose_ee),x0,[],[],[],[],lb,ub,@(x) fun_find(x,pose_ee,ff),options);

end