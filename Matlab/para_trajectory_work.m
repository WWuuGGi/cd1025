function [t_vec_G, pose_ee,cl1] = para_trajectory_work(p_start,p_end)
% p_start=[1400;700;500];
% p_end=[1200;700;500];
para_cdpr=para_han_v2;
M_ee=para_cdpr.M_ee;
if(p_start(1)<-1200)
    x_pre=-700:-10:-1200; y_pre=p_start(2); z_pre=p_start(3);
    p_1200=[-1200;p_start(2);p_start(3)];
else
    x_pre=700:10:1200; y_pre=p_start(2); z_pre=p_start(3);
    p_1200=[1200;p_start(2);p_start(3)];
end
i_num=1;r_oc_ideal=[0;0;0];
for i=1:51
    pose_ee0=[x_pre(i);y_pre;z_pre];
    [fx,~,exitflag]=fun_find_pos(pose_ee0,r_oc_ideal);
    if((exitflag==1||exitflag==2||exitflag==3||exitflag==4||exitflag==5))
%         f_cable_test(:,i_num)=fx;
        pose_ee_test(:,i_num)=[pose_ee0;fx(1:3,:)];
%         f_flag_test(:,i_num)=exitflag;
        i_num=i_num+1;
    else
        break;
    end
end
f=5:0.005:7.6;
for k=1:520
    [fx0,~,~]=fun_find_acce(p_1200,r_oc_ideal,f(k));
    if(norm(norm(fx0(2))-15)<12e-3) 
        f_a=f(k);break;
    end
%     fff(:,k)=fx0;
end
%%%正式计算
%第一段
a1=f_a/M_ee;
[pose_ee1,t1,v01,k1]=cal_speed3(p_start,a1);
b1=polyfit(t1(1:end-1),pose_ee1(1,:),9);%?多项式系数
t_1=t1(1):0.04:t1(end);
clear pose_ee1
size1=size(t_1,2);
pose_new1=zeros(6,size1);
for i1=1:size1
    pose_ee1(1,i1)=b1*[t_1(i1)^9 t_1(i1)^8 t_1(i1)^7 t_1(i1)^6 t_1(i1)^5 t_1(i1)^4 t_1(i1)^3 t_1(i1)^2 t_1(i1) 1]';
    pose_ee1(2,i1)=p_start(2);
    pose_ee1(3,i1)=p_start(3);
    a1_ee(1,i1)=k1*(pose_ee1(1,i1)-p_start(1));a1_ee(2,i1)=0;a1_ee(3,i1)=0;
    [fx1,~,~]=fun_min_solve(pose_ee1(:,i1),a1_ee(:,i1),r_oc_ideal,p_end);
    pose_new1(:,i1)=[pose_ee1(:,i1);fx1(1:3)];
end
% plot(x_pre(1:36),f_cable_test(12,:))
%%第二段
[pose_ee2,t2,v02,k2]=cal_speed2(p_1200,pose_ee_test(1:3,end),a1,v01,t1(:,end));
% plot(t2(1:50),pose_ee2(1,1:50),'*');hold on
% plot(t_1,pose_ee1(1,:),'*');
b2=polyfit(t2(1:end-1),pose_ee2(1,:),5);%多项式系数
t_2=t2(1):0.04:t2(end);
clear pose_ee2
size2=size(t_2,2);
pose_new2=zeros(6,size2);
for i2=1:size2
    pose_ee2(1,i2)=b2*[t_2(i2)^5 t_2(i2)^4 t_2(i2)^3 t_2(i2)^2 t_2(i2) 1]';
    pose_ee2(2,i2)=p_start(2);
    pose_ee2(3,i2)=p_start(3);
    a2_ee(1,i2)=k2*(pose_ee2(1,i2)-pose_ee_test(1,end));a2_ee(2,i2)=0;a2_ee(3,i2)=0;
    [fx2,~,~]=fun_min_solve(pose_ee2(:,i2),a2_ee(:,i2),r_oc_ideal,p_end);
    pose_new2(:,i2)=[pose_ee2(:,i2);fx2(1:3)];
end
%%%第三段匀速到x=0
[pose_ee3,t3,a3_ee]=cal_speed1(pose_ee_test(1:3,end),v02,t2(:,end));
size3=size(t3,2);
pose_new3=zeros(6,size3);
for i3=1:size3
    [fx3,~,~]=fun_min_solve(pose_ee3(:,i3),a3_ee(:,i3),r_oc_ideal,p_end);
    pose_new3(:,i3)=[pose_ee3(:,i3);fx3(1:3)];
end
%%%%对称前面几段
pose_half=[pose_new1,pose_new2,pose_new3];
t_half=[t_1,t_2,t3];
a_ee_half=[a1_ee,a2_ee,a3_ee];
t_forget=2*norm(pose_new3(1,end))/norm(1000*v02);

size_half=size(t_half,2);t4=fliplr(t_half);
pose_endtostart=fliplr(pose_half);
a4_ee=fliplr(-a_ee_half);
pose_ee4=[-pose_endtostart(1,:);pose_endtostart(2:3,:)];
pose_other_half=zeros(6,size_half);
for i4=1:size_half
    [fx3,~,~]=fun_min_solve(pose_ee4(:,i4),a4_ee(:,i4),r_oc_ideal,p_end);
    pose_other_half(:,i4)=[pose_ee4(:,i4);fx3(1:3)];
end
t_cal=t_half(:,end)+t_forget;t_other_half=zeros(1,size_half);
t_other_half=t_cal;
for i5=1:size_half-1
    t_other_half(:,i5+1)=t4(i5)-t4(i5+1)+t_cal;
    t_cal=t_other_half(:,i5+1);
end
%%%%%输出位姿和时间
t_vec_G=[t_half,t_other_half];
pose_ee=[pose_half,pose_other_half];
cl1=[];
end