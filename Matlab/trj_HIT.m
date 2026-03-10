% 函数功能：轨迹定义生成函数 - 用于定义机器人末端执行器的运动轨迹
% 
% 该函数通过设置不同的轨迹参数来定义机器人的运动路径，可以生成直线、圆弧等多种轨迹类型。
% 用户可以根据需要修改相应的参数块来构建自定义的机器人运动路径。
%
% 返回值：
%   trj_type - 轨迹类型单元格数组，每个元素指定对应轨迹段的类型，如'line'(直线)、'circle'(圆弧)或'uniform'(匀速直线)
%   para_trj - 轨迹参数单元格数组，每个单元格包含对应轨迹段的参数
%   v_trj    - 速度参数单元格数组，指定各轨迹段的速度(可选)
%   a_trj    - 加速度参数单元格数组，指定各轨迹段的加速度(可选)
%
% 使用方法：
% 1. 选择需要的轨迹类型（直线/圆弧/匀速直线）
% 2. 设置轨迹的起始点和终点坐标
% 3. 配置速度、加速度和时间参数
% 4. 将配置好的参数放入对应的变量中
function [trj_type, para_trj, v_trj, a_trj] = trj_HIT
%% 初始化速度和加速度参数（默认为0，根据需要可以修改）
% v_trj = {0};  % 速度参数单元格数组
% a_trj = {0};  % 加速度参数单元格数组
%% 初始位置定义（注释中提供了不同高度架子的参考坐标）
% pos0 = [ 1.3596, 4.0342, 246.0335]; %比较低的架子
% pos0 = [ 1.4313, 4.0242, 106]; %比较高的架子 z=306
% pos0 = [-639.042; 54.8; 148.85];  % 当前使用的初始位置（单位：毫米）

%% 轨迹点定义
% 注意：所有坐标单位均为毫米

%% 初始高度位置（通常用于机器人的安全高度）
% h0 = [ 0, 0, 800];  % 初始高度点 [x, y, z]

%% 定义轨迹的起始点和终点（直线轨迹示例）
% line1 = [0, -600, 800];  % 直线段的起点 [x, y, z]
% line2 = [0, 600, 800];   % 直线段的终点 [x, y, z]

% 配置轨迹参数
% 轨迹参数格式: {t0, dt, t_total, p_start, p_end}
%  - t0: 轨迹起始时间（秒）
%  - dt: 时间步长（秒）
%  - t_total: 轨迹总时间（秒）
%  - p_start: 起始位置坐标 [x, y, z]
%  - p_end: 终止位置坐标 [x, y, z]

% para_trj1 = {0, 0.1, 12, line1, line2};  % 定义一个直线轨迹段

%% 组合所有轨迹段参数到para_trj中
% para_trj = {para_trj1};  % 如果有多个轨迹段，可以添加更多，例如：{para_trj1, para_trj2, ...}

% 定义每个轨迹段的类型
% 支持的轨迹类型：
%  - 'line': 直线轨迹（加减速可控）
%  - 'uniform': 匀速直线轨迹
%  - 'circle': 圆弧轨迹

%% 注意：trj_type中元素的数量必须与para_trj中元素的数量一致
% trj_type = {'line'};  % 当前使用直线轨迹

% =================== 轨迹示例库 ===================
% 以下是各种不同类型轨迹的示例，用户可以通过取消注释对应的代码块来使用
%
% 示例1: 带自定义速度和加速度的直线轨迹
% line1 = [500, 500, 1000];  % 定义目标点
% 
% para_trj1 = {0,0.1,10,line1,h0};  % 从h0到line1的轨迹段
% v_trj1 = [-60, -60, 0];           % 自定义速度向量 [vx, vy, vz] (mm/s)
% a_trj1 = [-72, -72, 0];           % 自定义加速度向量 [ax, ay, az] (mm/s?)
% para_trj = {para_trj1};           % 设置轨迹参数
% trj_type = {'line'};              % 设置轨迹类型为直线
% v_trj = {v_trj1};                 % 设置速度参数
% a_trj = {a_trj1};                 % 设置加速度参数

%% 示例2: 从初始位置到安全高度的直线上升轨迹
% para_trj1 = {0, 0.1, 6, pos0, h0};  % 从pos0(初始位置)到h0(安全高度)的轨迹段
% para_trj = {para_trj1};             % 设置轨迹参数
% trj_type = {'line'};                % 设置轨迹类型为直线

%% 示例3: 三段直线组成的往返轨迹
% line1 = [ 1000, 0, 1000];  % 第一个目标点
% line2 = [-1000, 0, 1000];  % 第二个目标点
% 
% para_trj1 = {0, 0.1, 6, h0, line1};    % 第一段: 从安全高度到第一个目标点
% para_trj2 = {0, 0.1, 12, line1, line2}; % 第二段: 从第一个目标点到第二个目标点
% para_trj3 = {0, 0.1, 6, line2, h0};    % 第三段: 从第二个目标点返回安全高度
% 
% para_trj = {para_trj1, para_trj2, para_trj3};  % 组合三个轨迹段
% trj_type = {'line', 'line', 'line'};            % 所有轨迹段都是直线
% line1 = [-640, -400,  800];
% line2 = [ 640,  400,  800];
% 
% para_trj1 = {0, 0.1, 12, line1, line2};
% para_trj = {para_trj1};
% trj_type = {'line'};

%% 示例4: 圆弧轨迹
% c_start = [494, 0, 600];   % 圆弧起始点
% c_center = [0, 0, 600];    % 圆弧中心点
% 
% %% 圆弧轨迹参数格式: {t0, dt, t_total, p_start, p_center}
% %%  - t0: 轨迹起始时间（秒）
% %%  - dt: 时间步长（秒）
% %%  - t_total: 轨迹总时间（秒）
% %%  - p_start: 圆弧起始点坐标 [x, y, z]
% %%  - p_center: 圆弧中心点坐标 [x, y, z]
% 
% para_trj1 = {0, 0.1, 20, c_start, c_center};  % 定义一个圆弧轨迹段
% para_trj = {para_trj1};                        % 设置轨迹参数
% trj_type = {'circle'};                         % 设置轨迹类型为圆弧


%% 示例5: 匀速直线轨迹
% line1 = [ -780, 350, 1000];          % 目标点
% para_trj1 = {0, 0.1, 8, line1, h0};  % 从line1到h0的匀速轨迹段
% para_trj = {para_trj1};              % 设置轨迹参数
% trj_type = {'uniform'};              % 设置轨迹类型为匀速直线

%% 示例6: 五段匀速直线组成的复杂轨迹
% line1 = [ 780, -350, 1000];  % 点1: 右下角
% line2 = [-780, -350, 1000];  % 点2: 左下角
% line3 = [-780,    0, 1000];  % 点3: 左中
% line4 = [ 780,    0, 1000];  % 点4: 右中
% line5 = [ 780,  350, 1000];  % 点5: 右上角
% line6 = [-780,  350, 1000];  % 点6: 左上角
% 
% para_trj1 = {0, 0.1, 12, line1, line2};  % 右下方→左下方
% para_trj2 = {0, 0.1,  3, line2, line3};  % 左下方→左中
% para_trj3 = {0, 0.1, 12, line3, line4};  % 左中→右中
% para_trj4 = {0, 0.1,  3, line4, line5};  % 右中→右上方
% para_trj5 = {0, 0.1, 12, line5, line6};  % 右上方→左上方
% 
% para_trj = {para_trj1, para_trj2, para_trj3, para_trj4, para_trj5};  % 组合五段轨迹
% trj_type = {'uniform', 'uniform', 'uniform', 'uniform', 'uniform'};  % 所有轨迹段都是匀速直线

%%
% line1 = [   0,    0,  500];
% line2 = [ 500,  500, 1100];
% line3 = [-500,  500, 1100];
% line4 = [ 500, -500, 1100];
% line5 = [-500, -500, 1100];
% line6 = [-700,  350, 1100];
% 
% para_trj1 = {0, 0.1, 6, line1, line2};
% v_trj1 = [0, 0, 0];
% a_trj1 = [0, 0, 0];
% 
% para_trj2 = {0, 0.1, 6, line2, line3};
% v_trj2 = [ 0, 0, 0];
% a_trj2 = [ 0, 0, 0];
% 
% para_trj3 = {0, 0.1, 6, line3, line4};
% v_trj3 = [0, 0, 0];
% a_trj3 = [0, 0, 0];
% 
% para_trj4 = {0, 0.1, 6, line4, line5};
% v_trj4 = [ 0, 0, 0];
% a_trj4 = [ 0, 0, 0];
% 
% para_trj5 = {0, 0.1, 6, line5, line6};
% v_trj5 = [0, 0, 0];
% a_trj5 = [0, 0, 0];
% 
% para_trj = {para_trj1, para_trj2, para_trj3, para_trj4, para_trj5};
% trj_type = {'line','line','line','line','line'};
% v_trj = {v_trj1, v_trj2, v_trj3, v_trj4, v_trj5};
% a_trj = {a_trj1, a_trj2, a_trj3, a_trj4, a_trj5};


% --------------------------
% 1. 批量定义路径点（按顺序排列）
% --------------------------
% waypoints = [
% -500, -500,  0;    % line1（起点）
% 500,  500, 500;  % line2
% % -500, 500, 1100;  % line3
% % 500, -500, 1100;  % line4
% % -500, -500, 1100; % line5
% % -700, 350, 1100   % line6（终点）
% ];


waypoints = readmatrix("valid_sample_points_4.xlsx");
waypoints = waypoints(:,1:49);
waypoints = [[0;0;0],waypoints];
waypoints = waypoints';


% num_points = 59;

% x = -500 + 1000*rand(num_points,1);
% y = -500 + 1000*rand(num_points,1);
% z =  300 +  800*rand(num_points,1);

% --------------------------
% 2. 批量定义每段轨迹的时间参数
%    格式：[t_start, t_step, t_total]
%    （若每段时间相同，可简化为统一值）
% --------------------------
t_step = 0.1;       % 时间间隔（所有段共用）
t_total_per_segment = 6;  % 每段轨迹总时长（所有段共用）
num_segments = size(waypoints, 1) - 1;  % 路段数量 = 路径点数量 - 1
t_start = 0;

% % 计算每段的起始时间（上一段结束后立即开始）
% t_start = zeros(1, num_segments);
% for i = 2:num_segments
% t_start(i) = t_start(i-1) + t_total_per_segment;
% end

% --------------------------
% 3. 批量定义每段的速度和加速度
%    （若每段相同，可简化为统一值；若不同，按顺序定义）
% --------------------------
% 示例：所有段速度为[0,0,0]，加速度为[0,0,0]
v = zeros(3, num_segments);  % 3行（x,y,z），n列（n段）
a = zeros(3, num_segments);  % 同上

% 若某段需要不同速度，可单独修改，例如：
% v(:, 2) = [100, 0, 50];  % 第2段x方向速度100，z方向50

% --------------------------
% 4. 自动生成轨迹参数
% --------------------------
para_trj = cell(1, num_segments);  % 存储所有段的参数
v_trj = cell(1, num_segments);     % 存储所有段的速度
a_trj = cell(1, num_segments);     % 存储所有段的加速度

for i = 1:num_segments
% 生成当前段的参数：{t_start, t_step, t_total, 起点, 终点}
para_trj{i} = {t_start, t_step,t_total_per_segment,waypoints(i, :),waypoints(i+1, :)};

% 速度和加速度（转为行向量，与轨迹参数对应）
v_trj{i} = v(:, i)';
a_trj{i} = a(:, i)';
end

% 轨迹类型：所有段均为直线（'line'）
trj_type = repmat({'line'}, 1, num_segments);


% line1 = [ 700,    0, 600];
% line2 = [-700,    0, 600];
% para_trj1 = {0,0.1,10,line1,h0};
% para_trj = {para_trj1};
% trj_type = {'line'};
% para_trj1 = {0,0.1,16,line1,line2};
% v_trj1 = [-100, 0, 0];
% a_trj1 = [-80,  0, 0];
% 
% para_trj2 = {0, 0.1, 16, line2, line1};
% v_trj2 = [100, 0, 0];
% a_trj2 = [ 80, 0, 0];
% 
% para_trj = {para_trj1, para_trj2};
% trj_type = {'line', 'line'};
% v_trj = {v_trj1, v_trj2};
% a_trj = {a_trj1, a_trj2};

% para_trj1 = {0,0.1,10,h0,line1};
% v_trj1 = [100, 0, 15];
% a_trj1 = [80,  0, 10];
% para_trj = {para_trj1};
% trj_type = {'line'};
% v_trj = {v_trj1};
% a_trj = {a_trj1};

% % 折线轨迹
% poly1 = [ 1000,    0, 1000];
% poly2 = [  500,  700, 1200];
% poly3 = [    0, -700,  800];
% poly4 = [ -500,  700, 1200];
% poly5 = [-1000,    0, 1000];
% 
% para_trj1 = {0, 0.1, 6,    h0, poly1};
% para_trj2 = {0, 0.1, 4, poly1, poly2};
% para_trj3 = {0, 0.1, 8, poly2, poly3};
% para_trj4 = {0, 0.1, 8, poly3, poly4};
% para_trj5 = {0, 0.1, 4, poly4, poly5};
% para_trj6 = {0, 0.1, 6, poly5,    h0};
% 
% para_trj = {para_trj1, para_trj2, para_trj3, para_trj4, para_trj5, para_trj6};
% trj_type = {'line', 'line', 'line', 'line', 'line', 'line'};

%% 示例7: 从安全高度到初始位置的下降轨迹
% para_trj1 = {0, 0.1, 10, h0, pos0};  % 从h0(安全高度)到pos0(初始位置)的轨迹段
% para_trj = {para_trj1};              % 设置轨迹参数
% trj_type = {'line'};                 % 设置轨迹类型为直线

%直线轨迹
% a1=[0,0,500];
% a2=[0,-1000,500];
% a3=[600,-1000,700];
% a4=[600,500,700];
% a5=[-600,500,700];
% a6=[0,0,500];
% 
% para_trj1={0,0.1,5,b0,a1};
% para_trj2={0,0.1,4,a1,a2};
% para_trj3={0,0.1,3,a2,a3};
% para_trj4={0,0.1,5,a3,a4};
% para_trj5={0,0.1,5,a4,a5};
% para_trj6={0,0.1,3,a5,a6};
% 
% para_trj={para_trj1,para_trj2,para_trj3,para_trj4,para_trj5,para_trj6};
% trj_type={'line','line','line','line','line','line'};

%% 示例8: 包含直线和圆弧的复杂混合轨迹
% a1=[0,0,500];    % 中心点
% a2=[0,500,500];  % 右侧点
% a3=[0,800,500];  % 右上点
% a4=[0,-500,500]; % 左侧点
% a5=[0,-250,500]; % 左中点
% 
% %% 注意: b0可能是另一个起点，但在当前文件中未定义
% para_trj1={0,0.1,5,b0,a1};         % 直线: b0→中心点
% para_trj2={0,0.1,3,a1,a2};         % 直线: 中心点→右侧点
% para_trj3={0,1,10,a2,a1,a2};       % 圆弧: 右侧点→中心点→右侧点 (形成半圆弧)
% para_trj4={0,0.1,2,a2,a3};         % 直线: 右侧点→右上点
% para_trj5={0,1,15,a3,a1,a3};       % 圆弧: 右上点→中心点→右上点 (形成大弧度弧)
% para_trj6={0,0.1,4,a3,a1};         % 直线: 右上点→中心点
% para_trj7={0,0.1,3,a1,a4};         % 直线: 中心点→左侧点
% para_trj8={0,1,8,a4,a5,a4};        % 圆弧: 左侧点→左中点→左侧点 (形成小弧度弧)
% para_trj9={0,0.1,3,a4,a1};         % 直线: 左侧点→中心点
% 
% %% 组合九段轨迹，交替使用直线和圆弧
% para_trj={para_trj1,para_trj2,para_trj3,para_trj4,para_trj5,para_trj6,para_trj7,para_trj8,para_trj9};
% trj_type={'line','line','circle','line','circle','line','line','circle','line'};

% =================== 构建自定义路径指南 ===================
% 如何构建你想要的路径：
% 
% 1. 直线路径构建：
%    - 定义起点和终点坐标 [x, y, z] (单位: 毫米)
%    - 设置时间参数 {t0, dt, t_total} (单位: 秒)
%    - 设置轨迹类型为 'line' 或 'uniform'
%    - 可选：设置速度和加速度参数
%
% 2. 圆弧路径构建：
%    - 定义圆弧起始点和中心点坐标 [x, y, z] (单位: 毫米)
%    - 设置时间参数 {t0, dt, t_total} (单位: 秒)
%    - 设置轨迹类型为 'circle'
%
% 3. 复杂路径构建：
%    - 将路径分解为多个直线段和圆弧段
%    - 确保相邻轨迹段的端点坐标一致，形成连续路径
%    - 将所有轨迹段参数按顺序放入 para_trj 单元格数组
%    - 对应地，将所有轨迹类型放入 trj_type 单元格数组
%    - 确保 para_trj 和 trj_type 的元素数量一致
%
% 4. 参数调整技巧：
%    - t_total (总时间) 影响运动速度，时间越短速度越快
%    - dt (时间步长) 影响轨迹点密度，步长越小轨迹越平滑
%    - 对于复杂路径，合理设置各段的时间参数以获得平稳过渡
%    - 可以通过调整 v_trj 和 a_trj 来精确控制各段的运动学特性
%
% 注意事项：
% - 所有坐标单位均为毫米
% - 确保定义的路径在机器人的工作空间范围内
% - 对于圆弧轨迹，中心点决定了圆弧的半径和方向

