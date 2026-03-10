function [t_vec_G, pose_ee] = para_trj_circle_new(t_start,t_step,t_end,p_start,p_center)
t_vec_G=t_start:t_step:t_end;
start_x=p_start(1);
start_y=p_start(2);
start_z=p_start(3);
c_x=p_center(1);%圆心
c_y=p_center(2);
c_z=p_center(3);

A_start = angle((start_x-c_x) + (start_y-c_y)*1i);% 初始位置角度 -pi~pi
c_radius=norm([start_x,start_y,start_z]-[c_x,c_y,c_z]);% radius

t_T=t_end-t_start;
t_vec=0:t_step:t_T;
t_num=size(t_vec,2);

W_uniform = 2*pi/t_T; %设置匀速运动段的速度

%% 加速段：时长为1/5T，五次多项式规划，初速为0，末速为W_uniform，始末加速度均为0
T_start = t_T/5;
t_vec_start = 0:t_step:T_start;
thetaf_start = 2*pi/5; %加速段运动的角度，即从0~pi/6

%五次多项式系数
a0 = 0; a1 = 0; a2 = 0;
a3 = (20*thetaf_start - 8*W_uniform*T_start) / (2*T_start^3);
a4 = (-30*thetaf_start + 14*W_uniform*T_start) / (2*T_start^4);
a5 = (12*thetaf_start - 6*W_uniform*T_start) / (2*T_start^5);

t_theta_start = a0 + a1*t_vec_start + a2*t_vec_start.^2 + a3*t_vec_start.^3 + a4*t_vec_start.^4 + a5*t_vec_start.^5;
t_theta_start = A_start + t_theta_start;
t_dtheta_start = a1 + 2*a2*t_vec_start + 3*a3*t_vec_start.^2 + 4*a4*t_vec_start.^3 + 5*a5*t_vec_start.^4;
t_ddtheta_start = 2*a2 + 6*a3*t_vec_start + 12*a4*t_vec_start.^2 + 20*a5*t_vec_start.^3;

%% 匀速段：时长为3/5T,速度为W_uniform
A_uniform = t_theta_start(:,end); %匀速段的起始角度
T_uniform = t_T*3/5;
t_vec_uniform = 0:t_step:T_uniform;
t_num_uniform = size(t_vec_uniform, 2);

t_theta_uniform = W_uniform * t_vec_uniform + A_uniform;
t_dtheta_uniform = W_uniform * ones(1, t_num_uniform);
t_ddtheta_uniform = zeros(1, t_num_uniform);

%% 减速段：时长为1/5T，五次多项式规划，初速为W_uniform，末速为0，始末加速度均为0
A_end = t_theta_uniform(:, end); %减速段的起始角度
T_end = t_T/5;
t_vec_end = 0:t_step:T_end;
thetaf_end = 2*pi/5;

%五次多项式系数
a0 = 0; a1 = W_uniform; a2 = 0;
a3 = (20*thetaf_end - 12*W_uniform*T_end) / (2*T_end^3);
a4 = (-30*thetaf_end + 16*W_uniform*T_end) / (2*T_end^4);
a5 = (12*thetaf_end - 6*W_uniform*T_end) / (2*T_end^5);

t_theta_end = a0 + a1*t_vec_end + a2*t_vec_end.^2 + a3*t_vec_end.^3 + a4*t_vec_end.^4 + a5*t_vec_end.^5;
t_theta_end = A_end + t_theta_end;
t_dtheta_end = a1 + 2*a2*t_vec_end + 3*a3*t_vec_end.^2 + 4*a4*t_vec_end.^3 + 5*a5*t_vec_end.^4;
t_ddtheta_end = 2*a2 + 6*a3*t_vec_end + 12*a4*t_vec_end.^2 + 20*a5*t_vec_end.^3;

%% 汇总
t_theta = [t_theta_start t_theta_uniform(2:end) t_theta_end(2:end)]; %角度轨迹
t_dtheta = [t_dtheta_start t_dtheta_uniform(2:end) t_dtheta_end(2:end)]; %角速度轨迹
t_ddtheta = [t_ddtheta_start t_ddtheta_uniform(2:end) t_ddtheta_end(2:end)]; %角加速度轨迹

%机器人位姿
trj_x = c_radius*cos(t_theta) + c_x;
trj_y = c_radius*sin(t_theta) + c_y;
trj_z = zeros(1, t_num) + c_z;
trj_Ax=zeros(1,t_num);
trj_Ay=zeros(1,t_num);
trj_Az=zeros(1,t_num);

pose_ee = [trj_x; trj_y; trj_z];
r_pose = (p_center' - pose_ee) / norm(p_center' - pose_ee); %指向圆心矢量，用于计算向心加速度
e_pose = zeros(3, t_num); %切向矢量，用于计算切向加速度
for ii = 1:t_num
    e_pose(:,ii) = cross(r_pose(:,ii), [0;0;1]);
end

acc_n = t_dtheta.^2*c_radius.*r_pose; %向心加速度
acc_t = t_ddtheta*c_radius.*e_pose; %切向加速度
acc_ee = acc_n + acc_t; %总加速度

r_oc_ideal=[0;0;0]; %实际没用，用于补齐函数参数
pose_start_end=[trj_x(1);trj_y(1);trj_z(1);trj_x(t_num);trj_y(t_num);trj_z(t_num)]; %实际没用，用于补齐函数参数
for ii = 1:t_num
    [f_cable_ideal,fff,f_flag]=fun_min_solve(pose_ee(:,ii),acc_ee(:,ii),r_oc_ideal,pose_start_end);
    trj_Ax(ii)=f_cable_ideal(1);
    trj_Ay(ii)=f_cable_ideal(2);
    trj_Az(ii)=f_cable_ideal(3);
end

pose_ee=[trj_x;trj_y;trj_z;trj_Ax;trj_Ay;trj_Az];

