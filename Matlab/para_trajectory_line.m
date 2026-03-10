function [t_vec_G, pose_ee, cl, cf] = para_trajectory_line(t_start,t_step,t_end,p_start,p_end)
%生成中间过程中的位姿
t_vec_G=t_start:t_step:t_end;
min_x=p_start(1);
min_y=p_start(2);
min_z=p_start(3);
max_x=p_end(1);
max_y=p_end(2);
max_z=p_end(3);
%% trajectory generator 轨迹生产
t_T=t_end-t_start; %总行走时间
t_vec=0:t_step:t_T;%数组
t_num=size(t_vec,2); %可以用length
%% trajectory straight line: 直线轨迹 单位m
% 采用五次多项式计算
a_x=(max_x-min_x);% x轴的变化量  Variation 
a_y=(max_y-min_y);% y轴的变化量
a_z=(max_z-min_z);% z轴的变化量
ai0_x=min_x; ai3_x=(10*a_x)/t_T.^3; ai4_x=(-15*a_x)/t_T.^4; ai5_x=(6*a_x)/t_T.^5;
ai0_y=min_y; ai3_y=(10*a_y)/t_T.^3; ai4_y=(-15*a_y)/t_T.^4; ai5_y=(6*a_y)/t_T.^5;
ai0_z=min_z; ai3_z=(10*a_z)/t_T.^3; ai4_z=(-15*a_z)/t_T.^4; ai5_z=(6*a_z)/t_T.^5;
trj_x=ai0_x+ai3_x*t_vec.^3+ai4_x*t_vec.^4+ai5_x*t_vec.^5;
trj_y=ai0_y+ai3_y*t_vec.^3+ai4_y*t_vec.^4+ai5_y*t_vec.^5;
trj_z=ai0_z+ai3_z*t_vec.^3+ai4_z*t_vec.^4+ai5_z*t_vec.^5;

atrj_x=6*ai3_x*t_vec+12*ai4_x*t_vec.^2+20*ai5_x*t_vec.^3;
atrj_y=6*ai3_y*t_vec+12*ai4_y*t_vec.^2+20*ai5_y*t_vec.^3;
atrj_z=6*ai3_z*t_vec+12*ai4_z*t_vec.^2+20*ai5_z*t_vec.^3;
r_oc_ideal=[0;0;0];
%%求始末位置的姿态角，方便后面进行规划*****************************************
pose_start=[trj_x(1);trj_y(1);trj_z(1);trj_x(1);trj_y(1);trj_z(1)];
acce_start=[atrj_x(1);atrj_y(1);atrj_z(1);0;0;0];
[start_angle,~,~]=fun_min_solve(pose_start(4:6),acce_start,r_oc_ideal,pose_start);
min_anglex=start_angle(1);min_angley=start_angle(2);min_anglez=start_angle(3);

pose_end=[trj_x(1);trj_y(1);trj_z(1);trj_x(t_num);trj_y(t_num);trj_z(t_num)];
acce_end=[atrj_x(t_num);atrj_y(t_num);atrj_z(t_num);0;0;0];
[end_angle,~,~]=fun_min_solve(pose_end(4:6),acce_end,r_oc_ideal,pose_end);
max_anglex=end_angle(1);max_angley=end_angle(2);max_anglez=end_angle(3);
% 采用五次多项式计算
%%这块是用来求角加速度的
a_angle_x=(max_anglex-min_anglex);% x轴角度的变化量  Variation 
a_angle_y=(max_angley-min_angley);% y轴角度的变化量
a_angle_z=(max_anglez-min_anglez);% z轴角度的变化量
ai0_anglex=min_anglex; ai3_anglex=(10*a_angle_x)/t_T.^3; ai4_anglex=(-15*a_angle_x)/t_T.^4; ai5_anglex=(6*a_angle_x)/t_T.^5;
ai0_angley=min_angley; ai3_angley=(10*a_angle_y)/t_T.^3; ai4_angley=(-15*a_angle_y)/t_T.^4; ai5_angley=(6*a_angle_y)/t_T.^5;
ai0_anglez=min_anglez; ai3_anglez=(10*a_angle_z)/t_T.^3; ai4_anglez=(-15*a_angle_z)/t_T.^4; ai5_anglez=(6*a_angle_z)/t_T.^5;

alp_anglex=6*ai3_anglex*t_vec+12*ai4_anglex*t_vec.^2+20*ai5_anglex*t_vec.^3;%角加速度，但是这里没用，因为加不加角加速度影响不是很大
alp_angley=6*ai3_angley*t_vec+12*ai4_angley*t_vec.^2+20*ai5_angley*t_vec.^3;
alp_anglez=6*ai3_anglez*t_vec+12*ai4_anglez*t_vec.^2+20*ai5_anglez*t_vec.^3;
trj_Ax=zeros(1,t_num);
trj_Ay=zeros(1,t_num);
trj_Az=zeros(1,t_num);
%%********************************************************************* 
cl=zeros(4,t_num);
cf=zeros(4,t_num);
pose_start_end=[trj_x(1);trj_y(1);trj_z(1);trj_x(t_num);trj_y(t_num);trj_z(t_num)];
for a_num=1:t_num
                 pose_ee_old=[trj_x(1,a_num);trj_y(1,a_num);trj_z(1,a_num)];
                 acce_ee=[atrj_x(1,a_num);atrj_y(1,a_num);atrj_z(1,a_num);alp_anglex(1,a_num);alp_angley(1,a_num);alp_anglez(1,a_num)];%加速度集合
                 [f_cable_ideal,fff,f_flag]=fun_min_solve(pose_ee_old,acce_ee,r_oc_ideal,pose_start_end);
                 trj_Ax(a_num)=f_cable_ideal(1);
                 trj_Ay(a_num)=f_cable_ideal(2);
%                  trj_Az(a_num)=f_cable_ideal(3);
%                  cl(:,a_num)=f_cable_ideal(4:7);
%                  cf(:,a_num)=f_cable_ideal(8:11);
                 trj_Az(a_num)=0;
                 cl(:,a_num)=f_cable_ideal(3:6);
                 cf(:,a_num)=f_cable_ideal(7:10);
                 flgg(:,a_num)=f_flag;
end
%%
pose_ee=[trj_x;trj_y;trj_z;trj_Ax;trj_Ay;trj_Az];
aaa=1;
end