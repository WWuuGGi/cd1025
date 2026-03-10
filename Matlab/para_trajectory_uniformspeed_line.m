function [t_vec_G, pose_ee,cl1] = para_trajectory_uniformspeed_line(t_start,t_step,t_end,p_start,p_end)
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

v_x=a_x/t_T;
v_y=a_y/t_T;
v_z=a_z/t_T;

trj_x=v_x*t_vec+min_x;%
trj_y=v_y*t_vec+min_y;
trj_z=v_z*t_vec+min_z;

atrj_x=0*t_vec;
atrj_y=0*t_vec;
atrj_z=0*t_vec;
r_oc_ideal=[0;0;0];
%%求始末位置的姿态角，方便后面进行规划*****************************************
% 采用五次多项式计算
%%这块是用来求角加速度的
alp_anglex=0*t_vec;%角加速度，但是这里没用，因为加不加角加速度影响不是很大
alp_angley=0*t_vec;
alp_anglez=0*t_vec;
trj_Ax=zeros(1,t_num);
trj_Ay=zeros(1,t_num);
trj_Az=zeros(1,t_num);
%%********************************************************************* 
cl1=zeros(4,t_num);
pose_start_end=[trj_x(1);trj_y(1);trj_z(1);trj_x(t_num);trj_y(t_num);trj_z(t_num)];
for a_num=1:t_num
                 pose_ee_old=[trj_x(1,a_num);trj_y(1,a_num);trj_z(1,a_num)];
                 acce_ee=[atrj_x(1,a_num);atrj_y(1,a_num);atrj_z(1,a_num);alp_anglex(1,a_num);alp_angley(1,a_num);alp_anglez(1,a_num)];%加速度集合
                 [f_cable_ideal,fff,f_flag]=fun_min_solve(pose_ee_old,acce_ee,r_oc_ideal,pose_start_end);
                 trj_Ax(a_num)=f_cable_ideal(1);
                 trj_Ay(a_num)=f_cable_ideal(2);
                 trj_Az(a_num)=f_cable_ideal(3);
                 cl1(:,a_num)=f_cable_ideal(4:7);
end
%%
pose_ee=[trj_x;trj_y;trj_z;trj_Ax;trj_Ay;trj_Az];
end