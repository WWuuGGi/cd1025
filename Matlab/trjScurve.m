function [x, vx, ax] = trjScurve(t_start, t_step, t_end, x_start, x_end, v, a)
Tf = t_end - t_start;
t_vec = 0:t_step:Tf;
t_num = size(t_vec, 2);

%速度加速度归一化
v = v / (x_end - x_start);
a = a / (x_end - x_start);
v = abs(v);
a = abs(a);

%参数检验
[TF,V,A,J,T] = SCurvePara(Tf, v, a);

s = zeros(1,t_num);
x = zeros(1,t_num);
vx = zeros(1,t_num);
ax = zeros(1,t_num);

for i = 1:t_num
    s(i) = SCurveScaling(t_vec(i),V,A,J,T,TF);
    x(i) = x_start + s(i) * (x_end - x_start);
    if i>1
        vx(i-1) = (s(i) - s(i-1))*sign(x_end - x_start) / t_step;
    end
    if i>2
        ax(i-2) = (vx(i-1) - vx(i-2)) / t_step;
    end
end
    

        
        