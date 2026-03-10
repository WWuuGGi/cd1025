clear;
clc;
i=1;x=0;y=0;z=0;
for t=0:0.05:2
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    z=z+500/40;i=i+1;
    t_plot(i)=t;
end
x=0;y=0;z=500;
for t=0:0.05:2
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    x=x+(-1500/40);y=y+700/40;i=i+1;
    t_plot(i)=t+2;
end
x=-1500;y=700;z=500;
for t=0:0.05:3
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    x=x+(3000/60);i=i+1;
    t_plot(i)=t+4;
end
x=1500;y=700;z=500;
for t=0:0.05:0.5
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    y=y+(-140/10);i=i+1;
    t_plot(i)=t+7;
end
%
x=1500;y=560;z=500;
for t=0:0.05:3
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    x=x+(-3000/60);i=i+1;
    t_plot(i)=t+7.5;
end
x=-1500;y=560;z=500;
for t=0:0.05:0.5
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    y=y+(-140/10);i=i+1;
    t_plot(i)=t+10.5;
end
%%
x=-1500;y=420;z=500;
for t=0:0.05:3
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    x=x+(3000/60);i=i+1;
    t_plot(i)=t+11;
end
x=1500;y=420;z=500;
for t=0:0.05:0.5
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    y=y+(-140/10);i=i+1;
    t_plot(i)=t+14;
end
%
x=1500;y=280;z=500;
for t=0:0.05:3
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    x=x+(-3000/60);i=i+1;
    t_plot(i)=t+14.5;
end
x=-1500;y=280;z=500;
for t=0:0.05:0.5
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    y=y+(-140/10);i=i+1;
    t_plot(i)=t+17.5;
end
%%
x=-1500;y=140;z=500;
for t=0:0.05:3
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    x=x+(3000/60);i=i+1;
    t_plot(i)=t+18;
end
x=1500;y=140;z=500;
for t=0:0.05:0.5
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    y=y+(-140/10);i=i+1;
    t_plot(i)=t+21;
end
%
x=1500;y=0;z=500;
for t=0:0.05:3
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    x=x+(-3000/60);i=i+1;
    t_plot(i)=t+21.5;
end
x=-1500;y=0;z=500;
for t=0:0.05:0.5
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    y=y+(-140/10);i=i+1;
    t_plot(i)=t+24.5;
end
%%
x=-1500;y=-140;z=500;
for t=0:0.05:3
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    x=x+(3000/60);i=i+1;
    t_plot(i)=t+25;
end
x=1500;y=-140;z=500;
for t=0:0.05:0.5
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    y=y+(-140/10);i=i+1;
    t_plot(i)=t+27;
end
%
x=1500;y=-280;z=500;
for t=0:0.05:3
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    x=x+(-3000/60);i=i+1;
    t_plot(i)=t+27.5;
end
x=-1500;y=-280;z=500;
for t=0:0.05:0.5
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    y=y+(-140/10);i=i+1;
    t_plot(i)=t+30.5;
end
%%
x=-1500;y=-420;z=500;
for t=0:0.05:3
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    x=x+(3000/60);i=i+1;
    t_plot(i)=t+31;
end
x=1500;y=-420;z=500;
for t=0:0.05:0.5
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    y=y+(-140/10);i=i+1;
    t_plot(i)=t+34;
end
%
x=1500;y=-560;z=500;
for t=0:0.05:3
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    x=x+(-3000/60);i=i+1;
    t_plot(i)=t+34.5;
end
x=-1500;y=-560;z=500;
for t=0:0.05:0.5
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    y=y+(-140/10);i=i+1;
    t_plot(i)=t+37.5;
end
%%
x=-1500;y=-700;z=500;
for t=0:0.05:3
    pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
    x=x+(3000/60);i=i+1;
    t_plot(i)=t+38;
end
% x=1500;y=-700;z=500;
% for t=0:0.05:0.5
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     y=y+(-70/10);i=i+1;
%     t_plot(i)=t+41;
% end
% %
% x=1500;y=-70;z=500;
% for t=0:0.05:3
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     x=x+(-3000/60);i=i+1;
%     t_plot(i)=t+41.5;
% end
% x=-1500;y=-70;z=500;
% for t=0:0.05:0.5
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     y=y+(-70/10);i=i+1;
%     t_plot(i)=t+42;
% end
% %%
% x=-1500;y=-140;z=500;
% for t=0:0.05:3
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     x=x+(3000/60);i=i+1;
%     t_plot(i)=t+42.5;
% end
% x=1500;y=-140;z=500;
% for t=0:0.05:0.5
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     y=y+(-70/10);i=i+1;
%     t_plot(i)=t+45.5;
% end
% %
% x=1500;y=-210;z=500;
% for t=0:0.05:3
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     x=x+(-3000/60);i=i+1;
%     t_plot(i)=t+46;
% end
% x=-1500;y=-210;z=500;
% for t=0:0.05:0.5
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     y=y+(-70/10);i=i+1;
%     t_plot(i)=t+49;
% end
% %%
% x=-1500;y=-280;z=500;
% for t=0:0.05:3
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     x=x+(3000/60);i=i+1;
%     t_plot(i)=t+49.5;
% end
% x=1500;y=-280;z=500;
% for t=0:0.05:0.5
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     y=y+(-70/10);i=i+1;
%     t_plot(i)=t+52.5;
% end
% %
% x=1500;y=-350;z=500;
% for t=0:0.05:3
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     x=x+(-3000/60);i=i+1;
%     t_plot(i)=t+53;
% end
% x=-1500;y=-350;z=500;
% for t=0:0.05:0.5
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     y=y+(-70/10);i=i+1;
%     t_plot(i)=t+56;
% end
% %%
% x=-1500;y=-420;z=500;
% for t=0:0.05:3
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     x=x+(3000/60);i=i+1;
%     t_plot(i)=t+56.5;
% end
% x=1500;y=-420;z=500;
% for t=0:0.05:0.5
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     y=y+(-70/10);i=i+1;
%     t_plot(i)=t+59.5;
% end
% %
% x=1500;y=-490;z=500;
% for t=0:0.05:3
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     x=x+(-3000/60);i=i+1;
%     t_plot(i)=t+60;
% end
% x=-1500;y=-490;z=500;
% for t=0:0.05:0.5
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     y=y+(-70/10);i=i+1;
%     t_plot(i)=t+63;
% end
% %%
% x=-1500;y=-560;z=500;
% for t=0:0.05:3
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     x=x+(3000/60);i=i+1;
%     t_plot(i)=t+63.5;
% end
% x=1500;y=-560;z=500;
% for t=0:0.05:0.5
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     y=y+(-70/10);i=i+1;
%     t_plot(i)=t+64;
% end
% %
% x=1500;y=-630;z=500;
% for t=0:0.05:3
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     x=x+(-3000/60);i=i+1;
%     t_plot(i)=t+64.5;
% end
% x=-1500;y=-630;z=500;
% for t=0:0.05:0.5
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     y=y+(-70/10);i=i+1;
%     t_plot(i)=t+67.5;
% end
% %%
% x=-1500;y=-700;z=500;
% for t=0:0.05:3
%     pose_ee(1,i)=x;pose_ee(2,i)=y;pose_ee(3,i)=z;
%     x=x+(3000/60);i=i+1;
%     t_plot(i)=t+68;
% end

plot3([-1200,1200,1200,-1200,-1200],[700,700,-700,-700,700],[500,500,500,500,500],'-','Linewidth',2);
hold on;


scrsz = get(0,'ScreenSize');
set(gcf,'Position',scrsz); 
view(-1,80);

for i0=1:length(pose_ee(1,:))-1
    hp = plot3(pose_ee(1,i0:i0+1),pose_ee(2,i0:i0+1),pose_ee(3,i0:i0+1));hold on;
    hp.LineStyle='-';
    hp.Color='k';
    hp.Marker='.';
    hp.MarkerSize=6;
    hp.MarkerEdgeColor='r';
    hp.MarkerFaceColor='r';
    pause(0.01);
    axis([-1800,1800,-800,800,0,600]);
end



%plot_robot_motion(t_plot, pose_plot, para_cdpr);