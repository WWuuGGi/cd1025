%%根据动力学得到的惯性力与x的关系可以得到加速度a与x的关系。假定在小段内为匀加速
%%先按不加惯性力，角度变化到5°，然后逐渐增加惯性力使角度缓慢上升至15°(斜率比不加小)
%0*
%  *         
%5  *         
%       *      
%          *    
%             *  
%                *
%                   *
%15°                   *
%%%%%%%%%%%%%%%%%%%%%%%%%x
function [pose_ee,t,v0,k]=cal_speed3(p_start,a1)
% t=zeros(1,(norm(p_start(1))-1200));pose_ee=zeros(3,(norm(p_start(1))-1200));a_ee=zeros(3,(norm(p_start(1))-1200));
v0=0;t_sum=0;j=1;
if(p_start(1)<-1200)
    p_stop=-1200;
    p_length=norm(p_start(1)-p_stop);
    k=(a1*1000)/p_length;
    for i=0:1:(p_length-1)
    x=(p_start(1)+0.5)/1000+i*1*1e-3;
    syms t_vec
    assume(0<t_vec<0.8);
    equ=v0*t_vec+0.5*k*(x-p_start(1)/1000)*t_vec^2==1*1e-3;
    t_vec=vpa(solve(equ,t_vec));
    t_store(:,i+1)=double(t_vec);
%     if(t_store(1,i+1)>0&&t_store(1,i+1)<0.7)
%         t(:,i+1)=t_sum+t_store(1,i+1);
        v0=v0+k*(x-p_start(1)/1000)*t_store(1,i+1);
%     elseif(t_store(2,i+1)>0&&t_store(2,i+1)<0.7)
%         t(:,i+1)=t_sum+t_store(2,i+1);
%         v0=v0+k*(x-p_start(1)/1000)*t_store(2,i+1);
%     else
%         t(:,i+1)=0;
%     end   
%     t_sum=t(:,i+1);
%     v0=v0+k*(x-p_start/1000)*t(:,i+1);
%     pose_ee(:,i+1)=[x*1000;p_start(2);p_start(3)];
%     a_ee(:,i+1)=1000*[k*(x-p_start(1)/1000);0;0];
    result(:,i+1)=mod(x*1000-0.5,-10);
%     if(result(:,i+1)>-1e-11||result(i+1)<-9.99)
        t(:,i+2)=t_sum+t_store(1,i+1);
        pose_ee(:,i+1)=[x*1000-0.5;p_start(2);p_start(3)];
%         a_ee(:,i+1)=1000*[k*(x-p_start(1)/1000);0;0];
        a_ee(:,i+1)=[k*(x*1000-0.5-p_start(1));0;0];
%         j=j+1;
%     end
    t_sum=t_sum+t_store(1,i+1);
    end
elseif(p_start(1)>1200)
    p_stop=1200;
    p_length=norm(p_start(1)-p_stop);
    k=(norm(a1)*1000)/p_length;
    for i=0:1:(p_length-1)
    x=(p_start(1)-0.5)/1000-i*1*1e-3;
    syms t_vec
    assume(0<t_vec<0.8);
    equ=v0*t_vec+0.5*k*(x-p_start(1)/1000)*t_vec^2==-1*1e-3;
    t_vec=vpa(solve(equ,t_vec));
    t_store(:,i+1)=double(t_vec);
%     if(t_store(1,i+1)>0&&t_store(1,i+1)<0.7)
%         t(:,i+1)=t_sum+t_store(1,i+1);
        v0=v0+k*(x-p_start(1)/1000)*t_store(1,i+1);
%     elseif(t_store(2,i+1)>0&&t_store(2,i+1)<0.7)
%         t(:,i+1)=t_sum+t_store(2,i+1);
%         v0=v0+k*(x-p_start(1)/1000)*t_store(2,i+1);
%     else
%         t(:,i+1)=0;
%     end
%     t_sum=t(:,i+1);
%     v0=v0+k*(x-p_start/1000)*t(:,i+1);
%     pose_ee(:,i+1)=[x*1000;p_start(2);p_start(3)];
%     a_ee(:,i+1)=1000*[k*(x-p_start(1)/1000);0;0];
    result(:,i+1)=mod(x*1000+0.5,10);
%     if(result(:,i+1)<1e-11||result(i+1)>9.99)
        t(:,i+2)=t_sum+t_store(1,i+1);
        pose_ee(:,i+1)=[x*1000+0.5;p_start(2);p_start(3)];
        a_ee(:,i+1)=[k*(x*1000+0.5-p_start(1));0;0];
        j=j+1;
%     end
    t_sum=t_sum+t_store(1,i+1);
    end
else
end


end