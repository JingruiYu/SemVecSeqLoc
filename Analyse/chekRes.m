clc;clear all;
chkRes = textread('H:\openslam_openseqslam\results\res1-6\checkMat.txt');
Res = textread('H:\openslam_openseqslam\results\res1-6\ResMat.txt');

[row,col] = size(chkRes);
col = col - 1;
smu = zeros(1,col);
uni = zeros(1,col);
unk = zeros(1,col);

for i= 1:1:col
    for j = 1:1:row
        if chkRes(j,i) > 2
            smu(1,i) = smu(1,i) + 1;
        end

        if ( i == 1 && chkRes(j,i) > 2 && chkRes(j,2) < 3 && chkRes(j,3) < 3 ...
           && chkRes(j,4) < 3 && chkRes(j,5) < 3 && chkRes(j,6) < 3 && chkRes(j,7) < 3 ...
           && chkRes(j,8) < 3 && chkRes(j,9) < 3 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
           uni(1,i) = uni(1,i) + 1;
        end

        if ( i == 2 && chkRes(j,1) < 3 && chkRes(j,i) > 2 && chkRes(j,3) < 3 ...
            && chkRes(j,4) < 3 && chkRes(j,5) < 3 && chkRes(j,6) < 3 && chkRes(j,7) < 3 ...
            && chkRes(j,8) < 3 && chkRes(j,9) < 3 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
            uni(1,i) = uni(1,i) + 1;
        end

        if ( i == 3 && chkRes(j,1) < 3 && chkRes(j,2) < 3 && chkRes(j,3) > 2 ...
            && chkRes(j,4) < 3 && chkRes(j,5) < 3 && chkRes(j,6) < 3 && chkRes(j,7) < 3 ...
            && chkRes(j,8) < 3 && chkRes(j,9) < 3 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
            uni(1,i) = uni(1,i) + 1;
        end
 
        if ( i == 4 && chkRes(j,1) < 3 && chkRes(j,2) < 3 && chkRes(j,3) < 3 ...
            && chkRes(j,4) > 2 && chkRes(j,5) < 3 && chkRes(j,6) < 3 && chkRes(j,7) < 3 ...
            && chkRes(j,8) < 3 && chkRes(j,9) < 3 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
            uni(1,i) = uni(1,i) + 1;
        end

        if ( i == 5 && chkRes(j,1) < 3 && chkRes(j,2) < 3 && chkRes(j,3) < 3 ...
            && chkRes(j,4) < 3 && chkRes(j,5) > 2 && chkRes(j,6) < 3 && chkRes(j,7) < 3 ...
            && chkRes(j,8) < 3 && chkRes(j,9) < 3 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
            uni(1,i) = uni(1,i) + 1;
        end

        if ( i == 6 && chkRes(j,1) < 3 && chkRes(j,2) < 3 && chkRes(j,3) < 3 ...
            && chkRes(j,4) < 3 && chkRes(j,5) < 3 && chkRes(j,6) > 2 && chkRes(j,7) < 3 ...
            && chkRes(j,8) < 3 && chkRes(j,9) < 3 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
            uni(1,i) = uni(1,i) + 1;
        end

        if ( i == 7 && chkRes(j,1) < 3 && chkRes(j,2) < 3 && chkRes(j,3) < 3 ...
            && chkRes(j,4) < 3 && chkRes(j,5) < 3 && chkRes(j,6) < 3 && chkRes(j,7) > 2 ...
            && chkRes(j,8) < 3 && chkRes(j,9) < 3 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
            uni(1,i) = uni(1,i) + 1;
        end

        if ( i == 8 && chkRes(j,1) < 3 && chkRes(j,2) < 3 && chkRes(j,3) < 3 ...
            && chkRes(j,4) < 3 && chkRes(j,5) < 3 && chkRes(j,6) < 3 && chkRes(j,7) < 3 ...
            && chkRes(j,8) > 2 && chkRes(j,9) < 3 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
            uni(1,i) = uni(1,i) + 1;
        end

        if ( i == 9 && chkRes(j,1) < 3 && chkRes(j,2) < 3 && chkRes(j,3) < 3 ...
            && chkRes(j,4) < 3 && chkRes(j,5) < 3 && chkRes(j,6) < 3 && chkRes(j,7) < 3 ...
            && chkRes(j,8) < 3 && chkRes(j,9) > 2 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
            uni(1,i) = uni(1,i) + 1;
        end

        if ( i == 10 && chkRes(j,1) < 3 && chkRes(j,2) < 3 && chkRes(j,3) < 3 ...
            && chkRes(j,4) < 3 && chkRes(j,5) < 3 && chkRes(j,6) < 3 && chkRes(j,7) < 3 ...
            && chkRes(j,8) < 3 && chkRes(j,9) < 3 && chkRes(j,10) > 2 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
            uni(1,i) = uni(1,i) + 1;
        end

        if ( i == 11 && chkRes(j,1) < 3 && chkRes(j,2) < 3 && chkRes(j,3) < 3 ...
            && chkRes(j,4) < 3 && chkRes(j,5) < 3 && chkRes(j,6) < 3 && chkRes(j,7) < 3 ...
            && chkRes(j,8) < 3 && chkRes(j,9) < 3 && chkRes(j,10) < 3 && chkRes(j,11) > 2 && chkRes(j,12) < 3 )
            uni(1,i) = uni(1,i) + 1;
        end

        if ( i == 12 && chkRes(j,1) < 3 && chkRes(j,2) < 3 && chkRes(j,3) < 3 ...
            && chkRes(j,4) < 3 && chkRes(j,5) < 3 && chkRes(j,6) < 3 && chkRes(j,7) < 3 ...
            && chkRes(j,8) < 3 && chkRes(j,9) < 3 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) > 2 )
            uni(1,i) = uni(1,i) + 1;
        end

%%%%%%%%%%%%%%%%%%%%%%%%%%%

        
        if ( i == 1 && chkRes(j,i) > 2 && chkRes(j,9) < 3 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
            unk(1,i) = unk(1,i) + 1;
        end

        if ( i == 2 && chkRes(j,i) > 2 && chkRes(j,9) < 3 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
            unk(1,i) = unk(1,i) + 1;
        end

        if ( i == 3 && chkRes(j,3) > 2 && chkRes(j,9) < 3 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
            unk(1,i) = unk(1,i) + 1;
        end

        if ( i == 4 && chkRes(j,4) > 2 && chkRes(j,9) < 3 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
            unk(1,i) = unk(1,i) + 1;
        end

        if ( i == 5 && chkRes(j,5) > 2 && chkRes(j,9) < 3 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
            unk(1,i) = unk(1,i) + 1;
        end

        if ( i == 6 && chkRes(j,6) > 2 && chkRes(j,9) < 3 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
            unk(1,i) = unk(1,i) + 1;
        end

        if ( i == 7 && chkRes(j,7) > 2 && chkRes(j,9) < 3 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
            unk(1,i) = unk(1,i) + 1;
        end

        if ( i == 8 && chkRes(j,7) < 3 && chkRes(j,9) < 3 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
            unk(1,i) = unk(1,i) + 1;
        end

        if ( i == 9 && chkRes(j,1) < 3 && chkRes(j,2) < 3 && chkRes(j,3) < 3 ...
            && chkRes(j,4) < 3 && chkRes(j,5) < 3 && chkRes(j,6) < 3 && chkRes(j,7) < 3 ...
            && chkRes(j,8) < 3 && chkRes(j,9) > 2 && chkRes(j,10) < 3 && chkRes(j,11) < 3 && chkRes(j,12) < 3 )
            unk(1,i) = unk(1,i) + 1;
        end

        if ( i == 10 && chkRes(j,1) < 3 && chkRes(j,2) < 3 && chkRes(j,3) < 3 ...
            && chkRes(j,4) < 3 && chkRes(j,5) < 3 && chkRes(j,6) < 3 && chkRes(j,7) < 3 ...
            && chkRes(j,8) < 3 && chkRes(j,9) < 3 && chkRes(j,10) > 2)
            unk(1,i) = unk(1,i) + 1;
        end

        if ( i == 11 && chkRes(j,1) < 3 && chkRes(j,2) < 3 && chkRes(j,3) < 3 ...
            && chkRes(j,4) < 3 && chkRes(j,5) < 3 && chkRes(j,6) < 3 && chkRes(j,7) < 3 ...
            && chkRes(j,8) < 3 && chkRes(j,9) < 3 && chkRes(j,11) > 2 )
            unk(1,i) = unk(1,i) + 1;
        end

        if ( i == 12 && chkRes(j,1) < 3 && chkRes(j,2) < 3 && chkRes(j,3) < 3 ...
            && chkRes(j,4) < 3 && chkRes(j,5) < 3 && chkRes(j,6) < 3 && chkRes(j,7) < 3 ...
            && chkRes(j,8) < 3 && chkRes(j,9) < 3 && chkRes(j,12) > 2 )
            unk(1,i) = unk(1,i) + 1;
        end
    end
end