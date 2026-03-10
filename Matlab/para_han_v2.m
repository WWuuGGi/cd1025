function para_cdpr = para_han_v2
%% attachment points coordinates 
% base points coordinate 4 rows and 8 column
%8个绳索的固定的位置
% bp_x=2000; %暂定整体为4*4*2.5的矩形架，开题测试的是2*2*1.8的架子
% bp_y=2000; 
% bp_z_up=2500;
% bp_1=[-bp_x;  bp_y; bp_z_up; 1];
% bp_2=[-bp_x; -bp_y; bp_z_up; 1];
% bp_3=[ bp_x; -bp_y; bp_z_up; 1];
% bp_4=[ bp_x;  bp_y; bp_z_up; 1];

% %以下为清华动锚点座框架下的加装定锚点座坐标参数
% bp_1=[  1861.886;  1468.073; 2768.536; 1];
% bp_2=[ -1842.096;  1469.787; 2769.178; 1];
% bp_3=[ -1870.610; -1469.246; 2771.086; 1];
% bp_4=[  1824.558; -1468.918; 2769.590; 1];

%以下为G302锚点座坐标参数
bp_1=[  900;  900; 1670; 1];
bp_2=[ -900;  900; 1670; 1];
bp_3=[ -900; -900; 1670; 1];
bp_4=[  900; -900; 1670; 1];

bp_coor=[bp_1 bp_2 bp_3 bp_4];
% bp_coor = bp_coor + 1.0e+02 * ...
%     [-1.026355313410875   1.566216725853491   1.500559832580053  -1.038480436572918;
%   -1.322268787294585  -0.748024116581165   1.806958943897863   1.304456897122053;
%    1.117142138902443   1.513330395810867   1.389044781154068   1.241226941223504;
%    0 0 0 0;];

% bp_err = [ 9.3886   -9.0970    3.4319   -6.8908;
%           -6.3906   -5.1871    8.0963    2.0473;
%            8.6120   -9.8229    1.4483    1.3547;
%            0,0,0,0]*5;
% bp_coor = bp_coor + bp_err;


para_cdpr.bp_coor=bp_coor;
% EE attachment points coordinate 4 rows and 8 column
% ep_x_l=2.47;%ep_x_r=16.58e-3; %7.07e-3;s
% ep_y_l=2.47;%ep_y_r=51.07e-3; %7.07e-3;
% ep_z=1; %15e-3;17.25 14.45 15.85
ep_x_l=2.8;
ep_y_l=2.8;
ep_z=0.0; 

ep_1=[  ep_x_l;  ep_y_l;  ep_z; 1];
ep_2=[ -ep_x_l;  ep_y_l;  ep_z; 1];
ep_3=[ -ep_x_l; -ep_y_l;  ep_z; 1];
ep_4=[  ep_x_l; -ep_y_l;  ep_z; 1];
ep_o=[0;0;0;1];
ep_coor=[ep_1 ep_2 ep_3 ep_4];
para_cdpr.ep_coor=ep_coor;
para_cdpr.ep_o=ep_o;
M_ee=2;%kg
g=-9.81;
para_cdpr.M_ee=M_ee;
% I_xx=14.652;I_yy=9.032;I_zz=20.072;
% I_xy=-0.003;I_yz=0;I_xz=0;
% I_ee=[I_xx;I_yy;I_zz;I_xy;I_yz;I_xz];
% para.I_ee=I_ee;
para_cdpr.g=g;
%% 定义质心与局部坐标原点之间的向量
r_oc=[0;0;-80];
%r_oc=[ 0; 0; 138];
para_cdpr.r_oc=r_oc;
%% motor and winch 绞盘
dia_winch=50.0*ones(4,1);
para_cdpr.dia_winch=dia_winch;