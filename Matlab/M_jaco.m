function [jaco,ideal_cl,vector_ee,Tr_matrix] = M_jaco(para_cdpr,pose_ee)
% calculate the jacobian matrix for a given EE pose 
%%对于给定末端的执行器的位置 计算雅克比逆矩阵
%%末端执行器的固定点的坐标系，原来的坐标系
a1_e=para_cdpr.ep_coor(:,1);
a2_e=para_cdpr.ep_coor(:,2);
a3_e=para_cdpr.ep_coor(:,3);
a4_e=para_cdpr.ep_coor(:,4);
% a5_e=para_cdpr.ep_coor(:,5);
% a6_e=para_cdpr.ep_coor(:,6);
% a7_e=para_cdpr.ep_coor(:,7);
% a8_e=para_cdpr.ep_coor(:,8);
ao_e=para_cdpr.ep_o;

%%基座的坐标系
b1_g=para_cdpr.bp_coor(:,1);
b2_g=para_cdpr.bp_coor(:,2);
b3_g=para_cdpr.bp_coor(:,3);
b4_g=para_cdpr.bp_coor(:,4);
% b5_g=para_cdpr.bp_coor(:,5);
% b6_g=para_cdpr.bp_coor(:,6);
% b7_g=para_cdpr.bp_coor(:,7);
% b8_g=para_cdpr.bp_coor(:,8);
%Input the coordinate transformation matrix
%%给定末端执行器的姿态变换
Tr_matrix=[1 0 0 pose_ee(1);0 1 0 pose_ee(2);0 0 1 pose_ee(3);0 0 0 1]*...
    [cosd(pose_ee(6)) -sind(pose_ee(6)) 0 0;...
     sind(pose_ee(6)) cosd(pose_ee(6)) 0 0;...
     0 0 1 0;0 0 0 1]*...
    [cosd(pose_ee(5)) 0 sind(pose_ee(5)) 0;...
     0 1 0 0;...
     -sind(pose_ee(5)) 0 cosd(pose_ee(5)) 0;...
     0 0 0 1]*...
    [1 0 0 0;...
     0 cosd(pose_ee(4)) -sind(pose_ee(4)) 0;...
     0 sind(pose_ee(4)) cosd(pose_ee(4)) 0;...
     0 0 0 1];%旋转不可控，就假定其为无转动的平动，所以pose_ee(4\5\6)都是0.
%Calculate the coordinate (global coordinate system) of the attachment 
%%%points in the EE 末端执行器上的点的坐标（更新），全局坐标系下  gTe*eA=gA
a1_g=Tr_matrix*a1_e; 
a2_g=Tr_matrix*a2_e;
a3_g=Tr_matrix*a3_e;
a4_g=Tr_matrix*a4_e;
% a5_g=Tr_matrix*a5_e;
% a6_g=Tr_matrix*a6_e;
% a7_g=Tr_matrix*a7_e;
% a8_g=Tr_matrix*a8_e;
ao_g=Tr_matrix*ao_e;

%vector of EE in global coordinate system (from the mass center to the
%%%attachment point)固定点的矢量
vector_ee1=a1_g(1:3)-ao_g(1:3); 
vector_ee2=a2_g(1:3)-ao_g(1:3);
vector_ee3=a3_g(1:3)-ao_g(1:3);
vector_ee4=a4_g(1:3)-ao_g(1:3);
% vector_ee5=a5_g(1:3)-ao_g(1:3);
% vector_ee6=a6_g(1:3)-ao_g(1:3);
% vector_ee7=a7_g(1:3)-ao_g(1:3);
% vector_ee8=a8_g(1:3)-ao_g(1:3);
vector_ee=[vector_ee1 vector_ee2 vector_ee3 vector_ee4];% ...
       %    vector_ee5 vector_ee6 vector_ee7 vector_ee8];
%distance (cable length) vectorvector of relevant attachment points
%%%(from EE to the base) 从末端执行器到基座
vector_cable1=b1_g(1:3)-a1_g(1:3); 
vector_cable2=b2_g(1:3)-a2_g(1:3);
vector_cable3=b3_g(1:3)-a3_g(1:3);
vector_cable4=b4_g(1:3)-a4_g(1:3);
% vector_cable5=b5_g(1:3)-a5_g(1:3);
% vector_cable6=b6_g(1:3)-a6_g(1:3);
% vector_cable7=b7_g(1:3)-a7_g(1:3);
% vector_cable8=b8_g(1:3)-a8_g(1:3);
% ideal cable length
ideal_length_cable1=norm(vector_cable1);
ideal_length_cable2=norm(vector_cable2);
ideal_length_cable3=norm(vector_cable3);
ideal_length_cable4=norm(vector_cable4);
% ideal_length_cable5=norm(vector_cable5);
% ideal_length_cable6=norm(vector_cable6);
% ideal_length_cable7=norm(vector_cable7);
% ideal_length_cable8=norm(vector_cable8);
ideal_cl=[ideal_length_cable1;ideal_length_cable2;ideal_length_cable3;...
          ideal_length_cable4];%ideal_length_cable5;ideal_length_cable6;...
%           ideal_length_cable7;ideal_length_cable8];
% unit cable vector
unit_vector_cable1=vector_cable1/ideal_length_cable1;
unit_vector_cable2=vector_cable2/ideal_length_cable2;
unit_vector_cable3=vector_cable3/ideal_length_cable3;
unit_vector_cable4=vector_cable4/ideal_length_cable4;
% unit_vector_cable5=vector_cable5/ideal_length_cable5;
% unit_vector_cable6=vector_cable6/ideal_length_cable6;
% unit_vector_cable7=vector_cable7/ideal_length_cable7;
% unit_vector_cable8=vector_cable8/ideal_length_cable8;
% jacobian matrix
jaco_trans=[unit_vector_cable1 unit_vector_cable2...
            unit_vector_cable3 unit_vector_cable4;...
            cross(vector_ee1,unit_vector_cable1)...
            cross(vector_ee2,unit_vector_cable2)...
            cross(vector_ee3,unit_vector_cable3)...
            cross(vector_ee4,unit_vector_cable4)];
jaco=jaco_trans';