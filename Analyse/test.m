clc;clear all;
ThisChk = textread('check.txt');
groundTruth = textread('groundTruth.txt');
LastChk = textread('LastChk.txt');
LastRes = textread('LastRes.txt');
% saveChkMat = textread('saveChkMat.txt');
ThisRes = textread('ThisRes.txt');

ThisChk = ThisChk.';

for i=1:1:203
    if(ThisChk(i,2) > 2 && ThisChk(i,3) > 2 && ... 
       ThisChk(i,4) > 2 && ThisChk(i,5) > 2 && ...
       ThisChk(i,6) < 3)
       ThisChk(i,:)
       ThisRes(i,:)
    end
end
        