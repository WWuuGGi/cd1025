function [t_vec_G, pose_ee, cl, cf] = para_trajectory(para_trj, trj_type)
t_start=para_trj{1};
t_step=para_trj{2};
t_end=para_trj{3};
p_start=para_trj{4};
p_end_center=para_trj{5};
switch trj_type
    case 'line'
%         [t_vec_G, pose_ee] = para_trjScurve_line(t_start,t_step,t_end,p_start,p_end_center, v, a);
        [t_vec_G, pose_ee, cl, cf] = para_trajectory_line(t_start,t_step,t_end,p_start,p_end_center);
    case 'circle'
        [t_vec_G, pose_ee] = para_trj_circle_new(t_start,t_step,t_end,p_start,p_end_center);
%         p_end=para_trj{6};
%         [t_vec_G, pose_ee] = para_trajectory_circle(t_start,t_step,t_end,p_start,p_end_center,p_end);
    case 'helix'
        acb=1;
    case 'uniform'%ÔÈËÙÖ±Ïß
        [t_vec_G, pose_ee] = para_trajectory_uniformspeed_line(t_start,t_step,t_end,p_start,p_end_center);
    case 'work'
%         p_start=para_trj{4};
%         p_end_center=para_trj{5};
        [t_vec_G, pose_ee] = para_trajectory_work(p_start,p_end_center);
end