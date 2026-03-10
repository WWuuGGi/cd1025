function [pose_ee,t,a_ee]=cal_speed1(p_start,v0,t0)
vec=-(p_start(1)/norm(p_start(1)));
% pose_x=p_start(1):vec*10:0;
% v0=-1.5;t0=1.2;
% p_start=[850;0;0];
t_max=norm(p_start(1))/(1000*norm(v0));
t=t0:0.04:(t0+t_max);
pose_t=0:0.04:t_max;
size_t=size(pose_t,2);
% t_sum=t0;
pose_ee=zeros(3,size_t);a_ee=zeros(3,size_t);
for i=1:size_t
%     t(:,i)=t_sum+(10*1e-3)/norm(v0);
%     pose_ee(:,i)=[pose_x(:,i);p_start(2);p_start(3)];
%     t_sum=t(:,i);
      pose_ee(1,i)=p_start(1)+1000*v0*pose_t(i);
      pose_ee(2,i)=p_start(2);
      pose_ee(3,i)=p_start(3);
end

end