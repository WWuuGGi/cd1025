function [f_cable,output,exitflag]=fun_min_solve(pose_ee,acce_ee,~,pose_start_end)
%UNTITLED15 此处显示有关此函数的摘要
%   此处显示详细说明
% options=optimoptions('fmincon','Display','iter','FunctionTolerance',1e-12,'OptimalityTolerance',1e-11,'MaxIterations',1000);
% options=optimoptions('fmincon','Display','final','FunctionTolerance',1e-12,'OptimalityTolerance',1e-11,'MaxIterations',1000);
%   f(1) f(2) f(3) f(4) f(5)   f(6)   f(7)    f(8)    f(9)    f(10)

% x0=[0.1;     0.1;     0;              2000;      2000;       2000;       2000;       10;      10;      10;      10];%    0.1];
% lb=[ -2.5;     -2.5;       -9;               200;       200;        200;        200;      0.1;     0.1;     0.1;     0.1];%      0];
% ub=[  2.5;      2.5;        9;             80000;     80000;      80000;      80000;       35;      35;      35;      35];%     10];
% if(pose_ee(1)==0&&pose_ee(2)==0)
%     pose_ee(1)=pose_ee(1)+1e-20;
%     pose_ee(2)=pose_ee(2)+1e-20;
% end
% [f_cable,output,exitflag]=fmincon(@(x) fun_min_slove_min(x,pose_ee),x0,[],[],[],[],lb,ub,@(x) fun_con(x,pose_ee,acce_ee,pose_start_end),options);

options = optimoptions(@lsqnonlin,'Display', 'final', 'TolFun', 1e-8, 'TolX', 1e-8);

x0=[  0.1;  0.1;   2000;      2000;       2000;       2000;       10;      10;      10;      10];%    0.1];
lb=[ -2.5; -2.5;    200;       200;        200;        200;      0.1;     0.1;     0.1;     0.1];%      0];
ub=[  2.5;  2.5;  80000;     80000;      80000;      80000;       35;      35;      35;      35];%     10];

if(pose_ee(1)==0&&pose_ee(2)==0)
    pose_ee(1)=pose_ee(1)+1e-20;
    pose_ee(2)=pose_ee(2)+1e-20;
end

[f_cable,output, residual,exitflag]=lsqnonlin(@(x) fun_con_lsqnonlin(x,pose_ee,acce_ee,pose_start_end),x0,lb,ub,options);

%fmincon

end