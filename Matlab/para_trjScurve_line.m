%S形曲线规划，v匀速运动的速度，a最大加速度
function [t_vec_G, pose_ee] = para_trjScurve_line(t_start, t_step, t_end, p_start, p_end, v, a)
%生成中间过程中的位姿
t_vec_G=t_start:t_step:t_end;
x_start=p_start(1);
y_start=p_start(2);
z_start=p_start(3);
x_end=p_end(1);
y_end=p_end(2);
z_end=p_end(3);

v_x = v(1);
v_y = v(2);
v_z = v(3);
a_x = a(1);
a_y = a(2);
a_z = a(3);

t_T=t_end-t_start; %总行走时间
t_vec=0:t_step:t_T;%数组
t_num=size(t_vec,2); %可以用length

[trj_x, trj_vx, trj_ax] = trjScurve(t_start, t_step, t_end, x_start, x_end, v_x, a_x);
[trj_y, trj_vy, trj_ay] = trjScurve(t_start, t_step, t_end, y_start, y_end, v_y, a_y);
[trj_z, trj_vz, trj_az] = trjScurve(t_start, t_step, t_end, z_start, z_end, v_z, a_z);
r_oc_ideal = [0;0;0];
pose_start_end=[trj_x(1);trj_y(1);trj_z(1);trj_x(t_num);trj_y(t_num);trj_z(t_num)];

trj_Ax=zeros(1,t_num);
trj_Ay=zeros(1,t_num);
trj_Az=zeros(1,t_num);

for i = 1:t_num
    pose_ee_num = [trj_x(1,i); trj_y(1,i); trj_z(1,i)];
    acce_ee_num = [trj_ax(1,i); trj_ay(1,i); trj_az(1,i)];
    [f_cable_ideal,fff,f_flag]=fun_min_solve(pose_ee_num,acce_ee_num,r_oc_ideal,pose_start_end);
    trj_Ax(i) = f_cable_ideal(1);
    trj_Ay(i) = f_cable_ideal(2);
    trj_Az(i) = f_cable_ideal(3);
end

pose_ee=[trj_x;trj_y;trj_z;trj_Ax;trj_Ay;trj_Az];
end