function [f2,f_eq]=fun_find(fx,pose_ee,ff)
%UNTITLED16 此处显示有关此函数的摘要
%   此处显示详细说明
para_cdpr=para_han_v2;
M_ee=para_cdpr.M_ee;
ep_o=para_cdpr.ep_o;
gz=para_cdpr.g;
bp=para_cdpr.bp_coor;
ep=para_cdpr.ep_coor;


r_oc=[0.181;0.507;-24.768];

pose_ee1=[pose_ee(1);pose_ee(2);pose_ee(3);fx(1);fx(2);fx(3)];
T_trans=fun_Trans_e_G(pose_ee1);
vec_cable=bp(:,1:4)-T_trans*ep(:,1:4);

unit_vec_cable1=vec_cable(1:3,1)/norm(vec_cable(1:3,1));
unit_vec_cable2=vec_cable(1:3,2)/norm(vec_cable(1:3,2));
unit_vec_cable3=vec_cable(1:3,3)/norm(vec_cable(1:3,3));
unit_vec_cable4=vec_cable(1:3,4)/norm(vec_cable(1:3,4));

f_eq3=norm(bp(:,1)-T_trans*ep(:,1))-fx(4);
f_eq4=norm(bp(:,2)-T_trans*ep(:,2))-fx(5);
f_eq5=norm(bp(:,3)-T_trans*ep(:,3))-fx(6);
f_eq6=norm(bp(:,4)-T_trans*ep(:,4))-fx(7);

f_cable_vec=[fx(8)*unit_vec_cable1 fx(9)*unit_vec_cable2 fx(10)*unit_vec_cable3 fx(11)*unit_vec_cable4];
r1=T_trans*ep(:,1)-T_trans*ep_o;
r2=T_trans*ep(:,2)-T_trans*ep_o;
r3=T_trans*ep(:,3)-T_trans*ep_o;
r4=T_trans*ep(:,4)-T_trans*ep_o;
r_ee_vec=[r1 r2 r3 r4];
r_oc_G=T_trans*[r_oc;1]-T_trans*ep_o;
%%
unit_acce_vec=([pose_ee(1);pose_ee(2);pose_ee(3)]-[0;pose_ee(2);pose_ee(3)])...
              /norm([pose_ee(1);pose_ee(2);pose_ee(3)]-[0;pose_ee(2);pose_ee(3)]);

f_eq1=sum(f_cable_vec,2)+M_ee*[0;0;gz]+ff*unit_acce_vec;

f_eq2=sum(cross(r_ee_vec(1:3,:),f_cable_vec),2)+M_ee*cross(r_oc_G(1:3,1),[0;0;gz])+ff*cross(r_oc_G(1:3,1),unit_acce_vec);
%+fx(13)*cross(r_oc_G(1:3,1),[0;1;0]);
f_eq=[f_eq1;f_eq2;f_eq3;f_eq4;f_eq5;f_eq6];
f2=[];
end