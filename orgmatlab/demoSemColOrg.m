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


function demo()
clear all; clc;
%% first set the parameters

    % start with default parameters
    params = defaultParameters();    
    
    % Nordland mkz dataset
    ds.name = 'mkz';
    ds.imagePath = '../datasets/SemColOrgLag/mkz625_370';    
    ds.prefix='images-';
    ds.extension='.bmp';
    ds.suffix='';
    ds.imageSkip = -1;     % use every n-nth image
    ds.imageIndices = 625:ds.imageSkip:370;    
    ds.savePath = 'results';
    ds.saveFile = sprintf('%s-%d-%d-%d', ds.name, ds.imageIndices(1), ds.imageSkip, ds.imageIndices(end));
    
    ds.preprocessing.save = 1;
    ds.preprocessing.load = 1;
    %ds.crop=[1 1 60 32];  % x0 y0 x1 y1  cropping will be done AFTER resizing!
    ds.crop=[];
    
    mkz=ds;


    % Nordland seg dataset
    ds.name = 'seg';
    ds.imagePath = '../datasets/SemColOrgLag/seg';  
    ds.imageSkip = 1;
    ds.imageIndices = 1:ds.imageSkip:203;    
    ds.saveFile = sprintf('%s-%d-%d-%d', ds.name, ds.imageIndices(1), ds.imageSkip, ds.imageIndices(end));
    % ds.crop=[5 1 64 32];
    ds.crop=[];
    
    seg=ds;        

    params.dataset(1) = mkz;
    params.dataset(2) = seg;

    % load old results or re-calculate?
    params.differenceMatrix.load = 0;
    params.contrastEnhanced.load = 0;
    params.matching.load = 0;
    
    % where to save / load the results
    params.savePath='results';
              
    
%% now process the dataset
   
    results = openSeqSLAM(params);          
    
%% show some results
    
    close all;
    % set(0, 'DefaultFigureColor', 'white');
    
    % Now results.matches(:,1) are the matched seg images for every 
    % frame from the mkz dataset.
    % results.matches(:,2) are the matching score 0<= score <=1
    % The LARGER the score, the WEAKER the match.
    
    m = results.matches(:,1);
    %thresh=0.9;  % you can calculate a precision-recall plot by varying this threshold
    %m(results.matches(:,2)>thresh) = NaN;  % remove the weakest matches
    plot(m,'.');      % ideally, this would only be the diagonal
    title('Matchings');
    for i=1:1:size(m,1)
        mm(i,:) = [i,m(i)];
    end
    fid=fopen('SemColOrgmkz625_370.txt','wt');
    [row,col]=size(mm);
    for i=1:1:row
        for j=1:1:col
            if(j==col)
                fprintf(fid,'%g\n',mm(i,j));
            else
                fprintf(fid,'%g\t',mm(i,j));
            end
        end
    end
    fclose(fid);
end