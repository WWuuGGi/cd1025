function [t_vec_G, pose_ee] = para_trajectory_circle(t_start,t_step,t_end,p_start,p_center,p_end)% start point and center point
t_vec_G=t_start:t_step:t_end;
start_x=p_start(1);
start_y=p_start(2);
start_z=p_start(3);
c_x=p_center(1);%圆心
c_y=p_center(2);
c_z=p_center(3);
end_x=p_end(1);
end_y=p_end(2);
end_z=p_end(3);
%%
A_start = angle((start_x-c_x) + (start_y-c_y)*1i);% -pi~pi
A_end   = angle((end_x-c_x)+(end_y-c_y)*1i);%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if(A_end>A_start)
        A_end=A_end;
else
        A_end=A_end+2*pi;
end
A_change=0;
W_motion=(A_end-A_start)/(t_end-t_start);%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%角速度
c_radius=norm([start_x,start_y,start_z]-[c_x,c_y,c_z]);% radius
for i_cir=1:3
   if(i_cir==1)%第一段里走加速段，加速到预期速度
       t_T_start=t_step;
       t_vec_start=0:0.1:t_T_start;
       t_num_start=size(t_vec_start,2);
       thetaf_start=(A_end-A_start)/((t_end-t_start)/t_step);
       d_thetaf_start=W_motion;
       t_a0_start=0;t_a1_start=0;t_a2_start=0;%三个为0的项，所以没在式子中列出来
       t_a3_start=(20*thetaf_start-0-8*d_thetaf_start*t_T_start)/(2*t_T_start.^3);
       t_a4_start=(-30*thetaf_start+14*d_thetaf_start*t_T_start)/(2*t_T_start.^4);
       t_a5_start=(12*thetaf_start-6*d_thetaf_start*t_T_start)/(2*t_T_start.^5);
%        t_alpha_start=4.8*pi*t_vec_start.^3+(-12.8*pi*t_vec_start.^4)+9.6*pi*t_vec_start.^5;%五次多项式，起始位置为0，终止位置的速度为需要的速度
       t_alpha_start=t_a3_start*t_vec_start.^3+(t_a4_start*t_vec_start.^4)+t_a5_start*t_vec_start.^5;%五次多项式，起始位置为0，终止位置的速度为需要的速度
       t_alpha_start=A_start+t_alpha_start;
       A_change=t_alpha_start(:,end);
   elseif(i_cir==2)%中间匀速段，这块把这一大段分成很多很多个小直线段，大概以0.1s为时间间隔
       t_T_mid=t_end-2*t_T_start;
       t_vec_mid=0:0.1:t_T_mid;
       t_num_mid=size(t_vec_mid,2);
%        t_alpha_mid=(pi/5)*t_vec_mid+A_change;
       t_alpha_mid=W_motion*t_vec_mid+A_change;
   else%最后这段是减速段
       A_change=t_alpha_mid(:,end);
       t_T_end=t_step;
       t_vec_end=0:0.1:t_T_end;
       t_num_end=size(t_vec_end,2);
       thetaf_end=(A_end-A_start)/((t_end-t_start)/t_step);
       d_theta0_end=W_motion;
       t_a0_end=0;
       t_a1_end=d_theta0_end;
       t_a2_end=0;
       t_a3_end=(20*thetaf_end-12*d_theta0_end*t_T_end)/(2*t_T_end.^3);
       t_a4_end=(-30*thetaf_end+16*d_theta0_end*t_T_end)/(2*t_T_end.^4);
       t_a5_end=(12*thetaf_end-6*d_theta0_end*t_T_end)/(2*t_T_end.^5);
%        t_alpha_end=0.2*pi*t_vec_end+3.2*pi*t_vec_end.^3+(-11.2*pi*t_vec_end.^4)+9.6*pi*t_vec_end.^5;%五次多项式，起始位置为目标速度，终止位置的速度为0
       t_alpha_end=t_a1_end*t_vec_end+t_a3_end*t_vec_end.^3+(t_a4_end*t_vec_end.^4)+t_a5_end*t_vec_end.^5;%五次多项式，起始位置为目标速度，终止位置的速度为0
       t_alpha_end=A_change+t_alpha_end;
   end
end
t_alpha=[t_alpha_start t_alpha_mid(:,2:end) t_alpha_end(:,2:end)];
t_num=t_num_start+t_num_mid-1+t_num_end-1;
t_vec_G=[t_vec_start t_vec_mid(:,2:end)+t_T_start t_vec_start(:,2:end)+t_T_start+t_T_mid];
%求出x和y和z的坐标以后，代入静/动力学求出倾角大小
trj_x=c_radius*cos(t_alpha) + c_x;
trj_y=c_radius*sin(t_alpha) + c_y;
trj_z=0*ones(1,t_num) + c_z;
trj_Ax=zeros(1,t_num);
trj_Ay=zeros(1,t_num);
trj_Az=zeros(1,t_num);

r_oc_ideal=[0;0;0];
for a_num=1:t_num
                 pose_ee_old=[trj_x(1,a_num);trj_y(1,a_num);trj_z(1,a_num)];
                 e_pose=(p_center'-pose_ee_old)/norm(pose_ee_old-p_center');%加速度方向
                 acce_ee=W_motion.^2*(c_radius)*e_pose;%加速度集合
                 [f_cable_ideal,fff,f_flag]=fun_min_solve(pose_ee_old,acce_ee,r_oc_ideal,p_end);
                 trj_Ax(a_num)=f_cable_ideal(1);
                 trj_Ay(a_num)=f_cable_ideal(2);
                 trj_Az(a_num)=f_cable_ideal(3);
%                  cl1(:,a_num)=f_cable_ideal(4:7);
% eee(:,a_num)=e_pose;
end
% plot3(eee(1,:),eee(2,:),eee(3,:),'*');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% %t_T是一个时间间隔内的总时长，t_vec
% % t_T=t_step;
% t_T=10;
% t_vec=0:t_step:t_T;
% t_num=size(t_vec,2);
% %%
% %t_alpha=12*pi*(t/t_T)^5-30*pi*(t/t_T)^4+20*pi*(t/t_T)^3; % 0~2pi
% t_alpha =12*pi*(t_vec/t_T).^5-30*pi*(t_vec/t_T).^4+20*pi*(t_vec/t_T).^3;%//机器人轨迹规划里的五次多项凑出来的，目的是为了让圆轨迹在初始和末位置的速度和加速度为0，平稳无冲击
% trj_x=c_radius*cos(t_alpha+A_start) + c_x;
% trj_y=c_radius*sin(t_alpha+A_start) + c_y;
% trj_z=0*ones(1,t_num) + c_z;
% trj_Ax=zeros(1,t_num);
% trj_Ay=zeros(1,t_num);
% trj_Az=zeros(1,t_num);
% % tt=0:0.1:0.5;
% % plot(tt,t_alpha_end);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
pose_ee=[trj_x;trj_y;trj_z;trj_Ax;trj_Ay;trj_Az];
