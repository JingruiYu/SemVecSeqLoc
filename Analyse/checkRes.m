clc;clear all;
Org = textread('OrgResult.txt');
Org99 = textread('OrgResult99.txt');
SemOrg = textread('SemOrgResult.txt');
SemOrg99 = textread('SemOrgResult99.txt');
SemColOrg = textread('SemColOrgResult.txt');
SemColOrg99 = textread('SemColOrgResult99.txt');
SemColOrgLag = textread('SemColOrgLagRes.txt');
SemColOrgLag99 = textread('SemColOrgLagRes99.txt');
ManVec = textread('ManVecResult.txt');

ResMat = [Org(:,1),Org99(:,2),SemOrg99(:,2),SemColOrg99(:,2),SemColOrgLag99(:,2),ManVec(:,2)];

for i = 1:1:size(ResMat,1)
    for j = 1:1:size(ResMat,2)
        if isnan(ResMat(i,j))
            ResMat(i,j) = 0;
        end
    end
end

ResChecMat = Org(:,1);
for i = 1:1:size(ResMat,1)
    if (ResMat(i,2) == ResMat(i,3) && ResMat(i,3) == ResMat(i,4) )
        ResChecMat(i,2) = ResMat(i,2);
    else
        ResChecMat(i,2) = ResMat(i,2);
        disp('result is same');
        ResMat(i,2)
        ResMat(i,3)
        ResMat(i,4)
    end
    ResChecMat(i,3) = ResMat(i,5);
    ResChecMat(i,4) = ResMat(i,6);
end

dlmwrite('X.txt', ResChecMat, 'precision', '%1f', 'delimiter', '\t');