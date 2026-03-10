function [t_vec_G, pose_ee] = para_trajectory_cycle(t_start,t_step,t_end,p_start,p_center)% start point and center point
t_vec_G=t_start:t_step:t_end;
start_x=p_start(1);
start_y=p_start(2);
start_z=p_start(3);
c_x=p_center(1);
c_y=p_center(2);
c_z=p_center(3);
%%
A_start = angle((start_x-c_x) + (start_y-c_y)*1i);% -pi~pi
c_radius=norm([start_x,start_y,start_z]-[c_x,c_y,c_z]);% radius
%%
t_T=t_end-t_start;
t_vec=0:t_step:t_T;
t_num=size(t_vec,2);
%%
%t_alpha=12*pi*(t/t_T)^5-30*pi*(t/t_T)^4+20*pi*(t/t_T)^3; % 0~2pi
t_alpha =12*pi*(t_vec/t_T).^5-30*pi*(t_vec/t_T).^4+20*pi*(t_vec/t_T).^3;
trj_x=c_radius*cos(t_alpha+A_start) + c_x;
trj_y=c_radius*sin(t_alpha+A_start) + c_y;
trj_z=0*ones(1,t_num) + c_z;
trj_Ax=zeros(1,t_num);
trj_Ay=zeros(1,t_num);
trj_Az=zeros(1,t_num);

r_oc_ideal=[0;0;0];

for a_num=1:t_num
                 pose_ee_old=[trj_x(1,a_num);trj_y(1,a_num);trj_z(1,a_num)];
                 acce_ee=[atrj_x(1,a_num);atrj_y(1,a_num);atrj_z(1,a_num);alp_anglex(1,a_num);alp_angley(1,a_num);alp_anglez(1,a_num)];%加速度集合
                 [f_cable_ideal,fff,f_flag]=fun_min_solve(pose_ee_old,acce_ee,r_oc_ideal,pose_start_end);
                 trj_Ax(a_num)=f_cable_ideal(1);
                 trj_Ay(a_num)=f_cable_ideal(2);
                 trj_Az(a_num)=f_cable_ideal(3);
                 cl(:,a_num)=f_cable_ideal(4:7);
                 force(:,a_num)=f_cable_ideal(8:11);
                 flgg(:,a_num)=f_flag;
end
%%
pose_ee=[trj_x;trj_y;trj_z;trj_Ax;trj_Ay;trj_Az];