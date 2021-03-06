%% Urffer_Project2
% Driver function for Symbolic Regression


%% Terminal and Function Set
% Default Function Set
fSet = {'sin','cos','plus','minus','div','times'};
fProbability = ones(numel(fSet),1)/numel(fSet);
fSetMap = containers.Map(fSet,fProbability);

% Default Terminal Set
tSet = {'x','1','pi'};
tProbability = ones(numel(tSet),1)/numel(tSet);
tSetMap = containers.Map(tSet,tProbability);

%% Running Symbolic Regression
%matlabpool open
load Data.mat
dataSet = [x,y];

initMethod = struct('name','rampedHalfHalf','popFraction',0.5,'pruneDepth',2);
GPInit = struct('functionSet',fSetMap,'terminalSet',tSetMap,...
    'population',200,'treeDepth',4,...
    'initPopMethod',initMethod);
GPOptions = struct('crossOverFraction',0.7,'crossOverDepth',2,...
    'mutationFraction',0.7,'mutationRate',0.1,...
    'reproduceFraction',0.95,'minPopSize',100);
SymbolicRegression(GPInit,GPOptions,dataSet)

%matlabpool close
