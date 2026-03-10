function [] = plot_robot_motion(t_vec, pose_ee, para_cdpr)
trj_x = pose_ee(1,:);
trj_y = pose_ee(2,:);
trj_z = pose_ee(3,:);
% position

figure (1)
%%
scrsz = get(0,'ScreenSize');  % ��Ϊ�˻����Ļ��С��Screensize��һ��4Ԫ������[left,bottom, width, height]
set(gcf,'Position',scrsz);    % �û�õ�screensize��������figure��position���ԣ�ʵ����󻯵�Ŀ��?

%% 
plot3(para_cdpr.bp_coor(1,:),para_cdpr.bp_coor(2,:),para_cdpr.bp_coor(3,:),'r*','Markersize',7);hold on
xlabel('X');ylabel('Y');zlabel('Z');
% view(100,30);% ֻ��ʾxzƽ��,��������ֱ�Ϊ��λ�Ǻ����ǣ�?,0��Ϊxz����90,0��Ϊyz����0,90��Ϊxy
view(5,20);
Tr_matrix=[1 0 0 pose_ee(1);0 1 0 pose_ee(2);0 0 1 pose_ee(3);0 0 0 1]*[cosd(pose_ee(6)) -sind(pose_ee(6)) 0 0;sind(pose_ee(6)) cosd(pose_ee(6)) 0 0;0 0 1 0;0 0 0 1]*[cosd(pose_ee(5)) 0 sind(pose_ee(5)) 0;0 1 0 0;-sind(pose_ee(5)) 0 cosd(pose_ee(5)) 0;0 0 0 1]*[1 0 0 0;0 cosd(pose_ee(4)) -sind(pose_ee(4)) 0;0 sind(pose_ee(4)) cosd(pose_ee(4)) 0;0 0 0 1];
a1_g=Tr_matrix*para_cdpr.ep_coor(:,1);
a2_g=Tr_matrix*para_cdpr.ep_coor(:,2);
a3_g=Tr_matrix*para_cdpr.ep_coor(:,3);
a4_g=Tr_matrix*para_cdpr.ep_coor(:,4);
e1_g=Tr_matrix*[para_cdpr.ep_coor(1,1)-150;para_cdpr.ep_coor(2,1)+150;0;1];
e2_g=Tr_matrix*[para_cdpr.ep_coor(1,2)-150;para_cdpr.ep_coor(2,2)-150;0;1];
e3_g=Tr_matrix*[para_cdpr.ep_coor(1,3)+150;para_cdpr.ep_coor(2,3)-150;0;1];
e4_g=Tr_matrix*[para_cdpr.ep_coor(1,4)+150;para_cdpr.ep_coor(2,4)+150;0;1];
e5_g=Tr_matrix*[para_cdpr.ep_coor(1,1)-150;para_cdpr.ep_coor(2,1)+150;-110;1];
e6_g=Tr_matrix*[para_cdpr.ep_coor(1,2)-150;para_cdpr.ep_coor(2,2)-150;-110;1];
e7_g=Tr_matrix*[para_cdpr.ep_coor(1,3)+150;para_cdpr.ep_coor(2,3)-150;-110;1];
e8_g=Tr_matrix*[para_cdpr.ep_coor(1,4)+150;para_cdpr.ep_coor(2,4)+150;-110;1];
e_g=[e1_g,e2_g,e3_g,e4_g,e1_g,e5_g,e6_g,e2_g,e6_g,e7_g,e3_g,e7_g,e8_g,e4_g,e8_g,e5_g];
cable_length=[para_cdpr.bp_coor(:,1),a1_g,para_cdpr.bp_coor(:,2),a2_g,para_cdpr.bp_coor(:,3),a3_g,para_cdpr.bp_coor(:,4),a4_g];
cl1=plot3(cable_length(1,1:2),cable_length(2,1:2),cable_length(3,1:2),'k');
cl2=plot3(cable_length(1,3:4),cable_length(2,3:4),cable_length(3,3:4),'k');
cl3=plot3(cable_length(1,5:6),cable_length(2,5:6),cable_length(3,5:6),'k');
cl4=plot3(cable_length(1,7:8),cable_length(2,7:8),cable_length(3,7:8),'k');
central_point=plot3(trj_x(1),trj_y(1),trj_z(1));
central_point.Marker='o';
central_point.MarkerEdgeColor='r';
central_point.MarkerFaceColor='r';
central_point.MarkerSize=6;
robot_frame=plot3(e_g(1,:),e_g(2,:),e_g(3,:),'k','Linewidth',1);
robot_fill=fill3([e5_g(1,1),e6_g(1,1),e7_g(1,1),e8_g(1,1)],...
                 [e5_g(2,1),e6_g(2,1),e7_g(2,1),e8_g(2,1)],...
                 [e5_g(3,1),e6_g(3,1),e7_g(3,1),e8_g(3,1)],'g');
robot_fill2=fill3([e1_g(1,1),e5_g(1,1),e6_g(1,1),e2_g(1,1)],...
                  [e1_g(2,1),e5_g(2,1),e6_g(2,1),e2_g(2,1)],...
                  [e1_g(3,1),e5_g(3,1),e6_g(3,1),e2_g(3,1)],'y');
robot_fill3=fill3([e2_g(1,1),e6_g(1,1),e7_g(1,1),e3_g(1,1)],...
                  [e2_g(2,1),e6_g(2,1),e7_g(2,1),e3_g(2,1)],...
                  [e2_g(3,1),e6_g(3,1),e7_g(3,1),e3_g(3,1)],'y');
robot_fill4=fill3([e3_g(1,1),e7_g(1,1),e8_g(1,1),e4_g(1,1)],...
                  [e3_g(2,1),e7_g(2,1),e8_g(2,1),e4_g(2,1)],...
                  [e3_g(3,1),e7_g(3,1),e8_g(3,1),e4_g(3,1)],'y');
robot_fill5=fill3([e4_g(1,1),e8_g(1,1),e5_g(1,1),e1_g(1,1)],...
                  [e4_g(2,1),e8_g(2,1),e5_g(2,1),e1_g(2,1)],...
                  [e4_g(3,1),e8_g(3,1),e5_g(3,1),e1_g(3,1)],'y');
robot_fill6=fill3([e4_g(1,1),e3_g(1,1),e2_g(1,1),e1_g(1,1)],...
                  [e4_g(2,1),e3_g(2,1),e2_g(2,1),e1_g(2,1)],...
                  [e4_g(3,1),e3_g(3,1),e2_g(3,1),e1_g(3,1)],'g');
% hp = plot3(trj_x(1:2),trj_y(1:2),trj_z(1:2),'Color','b','LineStyle','-');
hp=animatedline('Color','b','LineStyle','-');
% hp.LineStyle='-';
% hp.Color='b';
%hp.Marker='.';
%hp.MarkerSize=6;
% hp.MarkerEdgeColor='r';
% hp.MarkerFaceColor='r';
axis equal
grid on
xlim([min(para_cdpr.bp_coor(1,:)),max(para_cdpr.bp_coor(1,:))])
ylim([min(para_cdpr.bp_coor(2,:)),max(para_cdpr.bp_coor(2,:))])
zlim([0,max(para_cdpr.bp_coor(3,:))+100])
pause(0.5)

for i0=1:length(trj_x)
    Tr_matrix=[1 0 0 pose_ee(1,i0);0 1 0 pose_ee(2,i0);0 0 1 pose_ee(3,i0);0 0 0 1]*[cosd(pose_ee(6,i0)) -sind(pose_ee(6,i0)) 0 0;sind(pose_ee(6,i0)) cosd(pose_ee(6,i0)) 0 0;0 0 1 0;0 0 0 1]*[cosd(pose_ee(5,i0)) 0 sind(pose_ee(5,i0)) 0;0 1 0 0;-sind(pose_ee(5,i0)) 0 cosd(pose_ee(5,i0)) 0;0 0 0 1]*[1 0 0 0;0 cosd(pose_ee(4,i0)) -sind(pose_ee(4,i0)) 0;0 sind(pose_ee(4,i0)) cosd(pose_ee(4,i0)) 0;0 0 0 1];
    a1_g=Tr_matrix*para_cdpr.ep_coor(:,1);
    a2_g=Tr_matrix*para_cdpr.ep_coor(:,2);
    a3_g=Tr_matrix*para_cdpr.ep_coor(:,3);
    a4_g=Tr_matrix*para_cdpr.ep_coor(:,4);
   e1_g=Tr_matrix*[para_cdpr.ep_coor(1,1)-150;para_cdpr.ep_coor(2,1)+150;0;1];
    e2_g=Tr_matrix*[para_cdpr.ep_coor(1,2)-150;para_cdpr.ep_coor(2,2)-150;0;1];
    e3_g=Tr_matrix*[para_cdpr.ep_coor(1,3)+150;para_cdpr.ep_coor(2,3)-150;0;1];
    e4_g=Tr_matrix*[para_cdpr.ep_coor(1,4)+150;para_cdpr.ep_coor(2,4)+150;0;1];
    e5_g=Tr_matrix*[para_cdpr.ep_coor(1,1)-150;para_cdpr.ep_coor(2,1)+150;-110;1];
    e6_g=Tr_matrix*[para_cdpr.ep_coor(1,2)-150;para_cdpr.ep_coor(2,2)-150;-110;1];
    e7_g=Tr_matrix*[para_cdpr.ep_coor(1,3)+150;para_cdpr.ep_coor(2,3)-150;-110;1];
    e8_g=Tr_matrix*[para_cdpr.ep_coor(1,4)+150;para_cdpr.ep_coor(2,4)+150;-110;1];
    e_g=[e1_g,e2_g,e3_g,e4_g,e1_g,e5_g,e6_g,e2_g,e6_g,e7_g,e3_g,e7_g,e8_g,e4_g,e8_g,e5_g];
    cable_length=[para_cdpr.bp_coor(:,1),a1_g,para_cdpr.bp_coor(:,2),a2_g,para_cdpr.bp_coor(:,3),a3_g,para_cdpr.bp_coor(:,4),a4_g];
    set(cl1,'Xdata',cable_length(1,1:2),'Ydata',cable_length(2,1:2),'Zdata',cable_length(3,1:2));
    set(cl2,'Xdata',cable_length(1,3:4),'Ydata',cable_length(2,3:4),'Zdata',cable_length(3,3:4)); 
    set(cl3,'Xdata',cable_length(1,5:6),'Ydata',cable_length(2,5:6),'Zdata',cable_length(3,5:6));
    set(cl4,'Xdata',cable_length(1,7:8),'Ydata',cable_length(2,7:8),'Zdata',cable_length(3,7:8));
    set(central_point,'Xdata',trj_x(i0),'Ydata',trj_y(i0),'Zdata',trj_z(i0));
    set(robot_frame,'Xdata',e_g(1,:),'Ydata',e_g(2,:),'Zdata',e_g(3,:));
    set(robot_fill,'Xdata',[e5_g(1,1),e6_g(1,1),e7_g(1,1),e8_g(1,1)],...
                   'Ydata',[e5_g(2,1),e6_g(2,1),e7_g(2,1),e8_g(2,1)],...
                   'Zdata',[e5_g(3,1),e6_g(3,1),e7_g(3,1),e8_g(3,1)]);
    set(robot_fill2,'Xdata',[e1_g(1,1),e5_g(1,1),e6_g(1,1),e2_g(1,1)],...
                    'Ydata',[e1_g(2,1),e5_g(2,1),e6_g(2,1),e2_g(2,1)],...
                    'Zdata',[e1_g(3,1),e5_g(3,1),e6_g(3,1),e2_g(3,1)]);
    set(robot_fill3,'Xdata',[e2_g(1,1),e6_g(1,1),e7_g(1,1),e3_g(1,1)],...
                    'Ydata',[e2_g(2,1),e6_g(2,1),e7_g(2,1),e3_g(2,1)],...
                    'Zdata',[e2_g(3,1),e6_g(3,1),e7_g(3,1),e3_g(3,1)]);
    set(robot_fill4,'Xdata',[e3_g(1,1),e7_g(1,1),e8_g(1,1),e4_g(1,1)],...
                    'Ydata',[e3_g(2,1),e7_g(2,1),e8_g(2,1),e4_g(2,1)],...
                    'Zdata',[e3_g(3,1),e7_g(3,1),e8_g(3,1),e4_g(3,1)]);
    set(robot_fill5,'Xdata',[e4_g(1,1),e8_g(1,1),e5_g(1,1),e1_g(1,1)],...
                    'Ydata',[e4_g(2,1),e8_g(2,1),e5_g(2,1),e1_g(2,1)],...
                    'Zdata',[e4_g(3,1),e8_g(3,1),e5_g(3,1),e1_g(3,1)]);
    set(robot_fill6,'Xdata',[e4_g(1,1),e3_g(1,1),e2_g(1,1),e1_g(1,1)],...
                    'Ydata',[e4_g(2,1),e3_g(2,1),e2_g(2,1),e1_g(2,1)],...
                    'Zdata',[e4_g(3,1),e3_g(3,1),e2_g(3,1),e1_g(3,1)]);
%     addpoints(hp,'Xdata',trj_x(i0:i0+1),'Ydata',trj_y(i0:i0+1),'Zdata',trj_z(i0:i0+1));
    addpoints(hp,trj_x(i0),trj_y(i0),trj_z(i0)); 
    drawnow;
%     hp = plot3(trj_x(i0:i0+1),trj_y(i0:i0+1),trj_z(i0:i0+1));
%     hp.LineStyle='-';
%     hp.Color='b';
% %     hp.Marker='.';
% %     hp.MarkerSize=6;
%     hp.MarkerEdgeColor='r';
%     hp.MarkerFaceColor='r';
    if i0<length(trj_x)
        pause(t_vec(i0+1)-t_vec(i0));
    else
        pause(t_vec(i0)-t_vec(i0-1));
    end
end
end
%}