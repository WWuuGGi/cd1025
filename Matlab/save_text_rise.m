function [] = save_text(RA_motor, t_vec, i_trj)
%%

file_id=fopen('data_rise.txt','a+');
inum=size(RA_motor,1);
jnum=size(RA_motor,2);
% fprintf(file_id,'uint8_t step_num0%u=%u;	\r\n',i_trj,jnum);
fprintf(file_id,'float winch_rise_angle[4][%u]=\r\n',i_trj,jnum);
fprintf(file_id,'{');
for i0=1:inum
    for j0=1:jnum
        if j0==1
            fprintf(file_id,'{%.4f,',RA_motor(i0,j0));
        elseif j0>1 && j0 < jnum
            fprintf(file_id,'%.4f,',RA_motor(i0,j0));
        else
            if i0==inum
                fprintf(file_id,'%.4f}\r\n};\r\n',RA_motor(i0,j0));
            else
                fprintf(file_id,'%.4f},\r\n',RA_motor(i0,j0));
            end
        end
    end
end
fclose(file_id);