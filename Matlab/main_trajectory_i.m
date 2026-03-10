% -------------------------------------------------------------------------
%% parameters
clear
clc
close all
para_cdpr=para_han_v2;  % para_lib_3D_s1
%% trajectory i
%  [trj_type, para_trj, v_trj, a_trj] = trj_HIT;%弄一条直线几条直线组成一个平面矩形
[trj_type, para_trj] = trj_HIT;
RA_motor_end=zeros(4,1);
RA_motor_total=[];
pose_plot=[];
cl_total=[];
cf_total=[];
t_plot=[];
t_num = 0:0.1:36;
% delete("data_1110.txt");
% delete("data_pose_1110.txt");
% delete("data_rise.txt");
% delete("data_pose_rise.txt");

delete("data_1117_4.txt");
delete("data_pose_1117_4.txt");

for i_trj=1:size(para_trj,2)
%     [t_vec, pose_ee] = para_trajectory(para_trj{i_trj},trj_type{i_trj}, v_trj{i_trj}, a_trj{i_trj});
    [t_vec, pose_ee, cl, cf] = para_trajectory(para_trj{i_trj},trj_type{i_trj});
    [RA_motor,flag_jaco,cl_ideal] = motor_angle(para_cdpr, t_vec, pose_ee);
%     RA_motor = RA_motor + repmat (RA_motor_end,1,size(RA_motor,2));
    RA_motor_total = [RA_motor_total, RA_motor];
%     RA_motor_end = RA_motor(:,end);

    save_text(RA_motor, t_vec, i_trj);
    save_text_pose(pose_ee(4:5,:), t_vec, i_trj);

%     save_text_rise(RA_motor, t_vec, i_trj);
%     save_text_pose_rise(pose_ee(4:5,:), t_vec, i_trj);

%     plot_trajectory(t_vec, pose_ee, para_cdpr.bp_coor) % plot the trajectory
    pose_plot=[pose_plot,pose_ee];
    t_plot=[t_plot,t_vec];
    cl_total=[cl_total,cl];
    cf_total=[cf_total,cf];
end
% plot(pose_plot(1,151:263),pose_plot(5,151:263));
% xlabel('x');
% ylabel('theta');
% title('倾斜角度与位置的关系');

% save("cl.mat","cl_total");

plot(t_plot, pose_plot(6,:));
% plot_robot_motion(t_plot, pose_plot, para_cdpr);



