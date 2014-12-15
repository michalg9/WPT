function [ events ] = importEvents(folderName)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

measurementSourcesCell = {'rx1'; 'tx1'; 'tx2'; 'tx3'; 'tx4'};

%max number of measurements we are importing
MAX_MEASUREMENTS = 5;

for i = 1:MAX_MEASUREMENTS
    for j = 1:length(measurementSourcesCell)
        filenameMeasurements = [folderName '/' measurementSourcesCell{j} '/' 'EVENT' int2str(i) '.txt'];
        if (exist(filenameMeasurements, 'file') == 2)
            events{i,j} = importSingleEvent(filenameMeasurements);
        end
    end
end



end

