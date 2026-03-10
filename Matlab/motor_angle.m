function [RA_motor,flag_jaco,CL_ideal] = motor_angle(para_cdpr, t_vec, pose_ee)
%% cable lengths calculation during a trajectory
% loops
t_num=length(t_vec);
trj_x = pose_ee(1,:);
trj_y = pose_ee(2,:);
trj_z = pose_ee(3,:);
trj_Ax = pose_ee(4,:);
trj_Ay = pose_ee(5,:);
trj_Az = pose_ee(6,:);
CL_ideal=zeros(4,t_num); %绳索的理想长度
RA_motor=zeros(4,t_num); %旋转角度 Rotate Angle
%flag_jaco=zeros(1,t_num);% 雅克比
for i_t=1:t_num
% x,y,z coordinate
    xx_g=trj_x(i_t); 
    yy_g=trj_y(i_t); 
    zz_g=trj_z(i_t);
    Ax_g=trj_Ax(i_t);
    Ay_g=trj_Ay(i_t);
    Az_g=trj_Az(i_t);
    p_ee=[xx_g;yy_g;zz_g;Ax_g;Ay_g;Az_g]; %p_ee=[0;0;2.5;0;0;0];
% inverse kinematics
    [c_L,f_jaco] = ideal_inverse_kine(para_cdpr,p_ee);
    if i_t==1
     c_L0=c_L;
    end
    CL_ideal(:,i_t)=c_L;
    RA_motor(:,i_t)=2*(c_L-c_L0)./para_cdpr.dia_winch*180/pi;%绞盘转过的角度
    flag_jaco=f_jaco;
end
end
%% plot and check
%{
% cable length
figure(4)
plot(t_vec,CL_ideal(1,:)-c_L0(1),t_vec,CL_ideal(2,:)-c_L0(2),...
    t_vec,CL_ideal(3,:)-c_L0(3),t_vec,CL_ideal(4,:)-c_L0(4),...
    t_vec,CL_ideal(5,:)-c_L0(5),t_vec,CL_ideal(6,:)-c_L0(6),...
    t_vec,CL_ideal(7,:)-c_L0(7),t_vec,CL_ideal(8,:)-c_L0(8))
xlabel('time (s)'); ylabel('cable length (m)');
% check motor speed
V_M=(RA_motor(:,2:end)-RA_motor(:,1:end-1))./repmat((t_vec(2:end)-t_vec(1:end-1)),8,1)/360*60;
figure(5)
plot(t_vec(2:end),V_M(1,:),t_vec(2:end),V_M(2,:),...
    t_vec(2:end),V_M(3,:),t_vec(2:end),V_M(4,:),...
    t_vec(2:end),V_M(5,:),t_vec(2:end),V_M(6,:),...
    t_vec(2:end),V_M(7,:),t_vec(2:end),V_M(8,:))
xlabel('time (s)'); ylabel('motor speed (rpm)')
%}