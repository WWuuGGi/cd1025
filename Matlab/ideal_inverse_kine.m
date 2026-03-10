%function [ideal_cl,flag_jaco] = ideal_inverse_kine(para_cdpr,pose_ee)
function [ideal_cl,m_jaco] = ideal_inverse_kine(para_cdpr,pose_ee)
% ideal inverse kinematics: given the pose of the end-effector, get the cable
% lengths. Also check the jacobian matrix.
[jaco,ideal_cl]=M_jaco(para_cdpr,pose_ee);
m_jaco=jaco;
% if the rank of jacobian matrix is less than 6,the position is singular       
if rank(jaco)<3
    flag_jaco=0;
else
    flag_jaco=1;
end
end