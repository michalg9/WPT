function [ measurements ] = importMeasurements(folderName)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

delimiterIn  = ',';
headerlinesIn = 1;

measurementSourcesCell = {'rx1'; 'tx1'; 'tx2'; 'tx3'; 'tx4'};

%max number of measurements we are importing
MAX_MEASUREMENTS = 5;

for i = 1:MAX_MEASUREMENTS
    for j = 1:length(measurementSourcesCell)
        filenameMeasurements = [folderName '/' measurementSourcesCell{j} '/' 'MEAS' int2str(i) '.txt'];
        if (exist(filenameMeasurements, 'file') == 2)
            measurements{i,j} = importdata(filenameMeasurements, delimiterIn, headerlinesIn);
        end
    end
end

end

