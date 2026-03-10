function f1=fun_min_slove_acce(fx,~)
% f1=10000*(fx(4)^2+fx(5)^2)+100*((fx(1)-pose_ee(1))^2+(fx(2)-pose_ee(2))^2)+10*fx(6)^2+(fx(3)-pose_ee(3))^2;
f1=(norm(fx(2))-15)^2;
end