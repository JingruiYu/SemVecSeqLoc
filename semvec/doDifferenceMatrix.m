%
% Copyright 2013, Niko Sünderhauf
% niko@etit.tu-chemnitz.de
%
% This file is part of OpenSeqSLAM.
%
% OpenSeqSLAM is free software: you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation, either version 3 of the License, or
% (at your option) any later version.
% 
% OpenSeqSLAM is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
% 
% You should have received a copy of the GNU General Public License
% along with OpenSeqSLAM.  If not, see <http://www.gnu.org/licenses/>.
function results = doDifferenceMatrix(results, params)
        
    filename = sprintf('%s/difference-%s-%s%s.mat', params.savePath, params.dataset(1).saveFile, params.dataset(2).saveFile, params.saveSuffix);  

    if params.differenceMatrix.load && exist(filename, 'file')
        display(sprintf('Loading image difference matrix from file %s ...', filename));

        d = load(filename);
        results.D = d.D;                                    
    else
        if length(results.dataset)<2
            display('Error: Cannot calculate difference matrix with less than 2 datasets.');
            return;
        end

        display('Calculating image difference matrix ...');
  %      h_waitbar = waitbar(0,'Calculating image difference matrix');

        n = size(results.dataset(1).preprocessing,2);
        m = size(results.dataset(2).preprocessing,2);

        D = zeros(n,m, 'single');   

        parfor i = 1:n            
            % d = results.dataset(2).preprocessing - repmat(results.dataset(1).preprocessing(:,i),1,m);
            d = calDifference(results.dataset(2).preprocessing, results.dataset(1).preprocessing(:,i));
            D(i,:) = sum(abs(d))/n;            
            %waitbar(i/n);
        end
        results.D=D;
        
        % save it
        if params.differenceMatrix.save                   
            save(filename, 'D');
        end

     %   close(h_waitbar);
    end    
end

function d = calDifference(dataset2, dataset1i)
    [dn,dm] = size(dataset2);
    d = zeros(dn,dm,'single');
    vd1 = dataset1i;
    for i = 1:1:size(dataset2,2)
        vd2 = dataset2(:,i);

        subd = zeros(dn,1,'single');

        % arrow
        for j = 1:1:5
            subd(j,1) = abs(vd2(j,1) - vd1(j,1));
        end
        
        % dash
        subd(6,1) = abs(vd2(6,1) - vd1(6,1));

        % slt
        disAA = caldis(vd1(7:10,1),vd2(7:10,1));
        disAB = caldis(vd1(7:10,1),vd2(11:14,1));
        disBA = caldis(vd1(11:14,1),vd2(7:10,1));
        disBB = caldis(vd1(11:14,1),vd2(11:14,1));
        
        disAABB = findMIN(disAA,disAB,disBA,disBB);

        subd(7,1) = disAABB(1,1);
        disAABBD = num2str(disAABB(1,2));
        N = length(disAABBD);
        if N == 3
            subd(8,1) = str2num(disAABBD(N));
            subd(9,1) = str2num(disAABBD(N-1));
            subd(10,1) = str2num(disAABBD(N-2));
        elseif N == 2
            subd(8,1) = 0;
            subd(9,1) = str2num(disAABBD(N));
            subd(10,1) = str2num(disAABBD(N-1));
        elseif N == 1
            subd(8,1) = 0;
            subd(9,1) = 0;
            subd(10,1) = str2num(disAABBD(N));
        else
            display('error');
        end
        
        for j = 11:1:14
            subd(j,1) = 0;
        end

        d(:,i) = subd;
    end
end


function dis = caldis(vd1,vd2)
    
    vd1Full = Isdatafull(vd1);
    vd2Full = Isdatafull(vd2);

    dis = 155;
    regdis = 3;

    if vd1Full(1) ~= 0 || vd2Full(1) ~= 0
        regdis = abs(vd1(1,1) - vd2(1,1));
    end

    if vd1Full(2) >= 2 || vd2Full(2) >= 2
        dis = abs(vd1(4,1) - vd2(4,1));
        dis = dis + 10 * abs(vd1(3,1) - vd2(3,1));
        dis = dis + 100 * abs(vd1(2,1) - vd2(2,1));
    else
        dis = abs(vd1(4,1) - vd2(4,1));
        dis = dis + 10 * abs(vd1(3,1) - vd2(3,1));
        dis = dis + 100 * abs(vd1(2,1) - vd2(2,1));
        if dis > 2
            dis = dis +50;
        end
    end

    dis = [regdis,dis];
end

function Isfull = Isdatafull(vd)
    if vd(1,1) ~= 0
        Isfull = 1;
    else
        Isfull = 0;
    end
    
    subIsfull = 0;
    for i = 2:1:4
        if vd(i,1) ~= 0
            subIsfull = subIsfull + 1;
        end
    end
    Isfull= [Isfull, subIsfull];

end

function disM = findMIN(disA,disB,disC,disD)
    disM = disA;

    if disM(1,1) > disB(1,1)
        disM = disB;
    elseif disM(1,1) == disB(1,1) && disM(1,2) > disB(1,2)
        disM = disB;
    end

    if disM(1,1) > disC(1,1)
        disM = disC;
    elseif disM(1,1) == disC(1,1) && disM(1,2) > disC(1,2)
        disM = disC;
    end

    if disM(1,1) > disD(1,1)
        disM = disD;
    elseif disM(1,1) == disD(1,1) && disM(1,2) > disD(1,2)
        disM = disD;
    end
    
end