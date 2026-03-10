%%根据动力学得到的惯性力与x的关系可以得到加速度a与x的关系。假定在小段内为匀加速
%%先按不加惯性力，角度变化到15°，然后逐渐增加惯性力使角度保持15°
%0*
%  *         
%   *         
%    *         
%     *         
%      *         
%       *         
%        *         
%15°      **************         
%%%%%%%%%%%%%%%%%%%%%%%%%x
function [pose_ee,t,v1,k]=cal_speed2(p_start,p_end,a1,v0,t0)
% p_start=[1200;700;500];
% p_end=[850;700;500];
% v0=-1;
% t0=0;a1=3.33;
p_length=norm(p_start(1)-p_end(1));
% t_store=zeros(1,500);t=zeros(1,p_length);
% pose_ee=zeros(3,p_length);a_ee=zeros(3,p_length);
v1=v0;t_sum=t0;t(:,1)=t0;
% j=1;

if(p_start(1)<0)
    k=(1000*a1)/(p_start(1)-p_end(1));
    for i=0:1:p_length%%%%%%
    x=p_start(1)/1000+i*1e-3;%%%%%
    syms t_vec
    assume(0<t_vec<0.5);
    equ=v1*t_vec+0.5*k*(x-p_end(1)/1000)*t_vec^2==1*1e-3;%%%%%
    t_vec=vpa(solve(equ,t_vec));
    t_store(:,i+1)=double(t_vec);
    v1=v1+k*(x-p_end(1)/1000)*t_store(1,i+1);
%     t(:,i+1)=t_sum+t_store(1,i+1);
%     t_sum=t(:,i+1);
%     pose_ee(:,i+1)=[x*1000;p_start(2);p_start(3)];
%     a_ee(:,i+1)=1000*[k*(x-p_end(1)/1000);0;0];
    result(i+1)=mod(1000*x,-10);
%     if(result(i+1)>-1e-6||result(i+1)<-9.99)
        pose_ee(:,i+1)=[x*1000;p_start(2);p_start(3)];
        a_ee(:,i+1)=1000*[k*(x-p_end(1)/1000);0;0];
        t(:,i+2)=t_sum+t_store(1,i+1);
%         j=j+1;
%     end
    t_sum=t_sum+t_store(:,i+1);
    end
elseif(p_start(1)>0)
    k=(-1000*a1)/(p_start(1)-p_end(1));
    for i=0:1:p_length%%%%%
    x=p_start(1)/1000-i*1e-3;%%%%%
    syms t_vec
    assume(0<t_vec<0.5);
    equ=v1*t_vec+0.5*k*(x-p_end(1)/1000)*t_vec^2==-1*1e-3;%%%%
    t_vec=vpa(solve(equ,t_vec));
    t_store(:,i+1)=double(t_vec);
    v1=v1+k*(x-p_end(1)/1000)*t_store(1,i+1);
%     t(:,i+1)=t_sum+t_store(1,i+1);
%     t_sum=t(:,i+1);
%     pose_ee(:,i+1)=[x*1000;p_start(2);p_start(3)];
%     a_ee(:,i+1)=1000*[k*(x-p_end(1)/1000);0;0];
    result(i+1)=mod(1000*x,10);
%     if(result(i+1)<1e-6||result(i+1)>9.99)
        pose_ee(:,i+1)=[x*1000;p_start(2);p_start(3)];
        a_ee(:,i+1)=1000*[k*(x-p_end(1)/1000);0;0];
        t(:,i+2)=t_sum+t_store(1,i+1);
%         j=j+1;
%     end
    t_sum=t_sum+t_store(:,i+1);
    end
else
end

end