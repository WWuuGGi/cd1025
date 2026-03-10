clear;
clc;

load("cl.mat");
load("cl_1.mat");

% nums = size(cl_total,2);
% 
% pose0 = [0;0;0];
% bp1_nom = [ 1000; 900; 1670];
% bp2_nom = [-1000; 900; 1670];
% bp3_nom = [-1000;-900; 1670];
% bp4_nom = [ 1000;-900; 1670];
% bp_nom = [bp1_nom, bp2_nom, bp3_nom, bp4_nom];
% 
% % 锚点座实际值与理论值的偏差（偏差范围-100-100，随机生成）
% % bp_err = [-98.0395,  68.6426,  84.4664,  62.9447;
% %            54.1908, -91.4680, -24.3627,  81.1584;
% %            40.8679,  45.9026, -55.1445, -74.6026;];
% % bp_err = 10*ones(3,4);
% % bp_err = [-98.0395,  68.6426,  84.4664,  62.9447;
% %            54.1908, -91.4680, -24.3627,  81.1584;
% %            40.8679,  45.9026, -55.1445, -74.6026;]*0.1;
% bp_err = [ 9.3886   -9.0970    3.4319   -6.8908;
%           -6.3906   -5.1871    8.0963    2.0473;
%            8.6120   -9.8229    1.4483    1.3547;]*5;
% bp_act = bp_err + bp_nom;
% points_act = zeros(3,1);
% points_flag = zeros(3,1);
% 
% for i = 1:nums
%     [points_act(:, i), points_flag(i)] = ...
%     fun_Direct_Kine_3_3_CDPR(bp_act, cl_total(1:3,i), pose_ee(1:3, i), pose0);
% end
% % [pose_act, resnom, fval, exitflag] = lsqnonlin(@(x) fun_Eq_Direct_Kine_3_3_CDPR(x,pose0, bp, l), x0, lb, ub, options); 

% plot3(points_act(1,:),points_act(2,:),points_act(3,:));
% figure(1);
% hold on; grid on;
% plot3(pose_ee(1,:),pose_ee(2,:),pose_ee(3,:))
err = cl_total_1 - cl_total;
figure(1);
plot(1:61,cl_total_1(1,:));
hold on;
plot(1:61,cl_total_1(2,:));
plot(1:61,cl_total_1(3,:));
plot(1:61,cl_total_1(4,:));


plot(1:61,cl_total(1,:),'--');
hold on;
plot(1:61,cl_total(2,:),'--');
plot(1:61,cl_total(3,:),'--');
plot(1:61,cl_total(4,:),'--');

figure(3);
plot(1:61,err(1,:));
hold on;
plot(1:61,err(2,:));
plot(1:61,err(3,:));
plot(1:61,err(4,:));


