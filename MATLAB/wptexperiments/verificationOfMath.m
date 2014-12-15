clear all;
close all;

%folderName = 'meas092314_165224';
%folderName = 'meas092414_133526';
folderName = 'meas092414_152243';
measurements = importMeasurements(folderName);
generatePlots(measurements);


[numberOfExperiments, numberOfMeasurements] = size(measurements);

measurementSources = ['rx1'; 'tx1'; 'tx2'; 'tx3'; 'tx4'];
measurementSourcesCell = cellstr(measurementSources);


for i = 1:numberOfExperiments
    for j = 1:length(measurementSourcesCell)
        filenameMeasurements = [folderName '/' measurementSourcesCell{j} '/' 'EVENT' int2str(i) '.txt'];
        if (exist(filenameMeasurements, 'file') == 2)
            events{i,j} = importProbingEvent(filenameMeasurements);
        end
    end
end

numberOfTriesToChargeInRoundGlobal = [];
timeToChargeInRoundGlobal = []


for i = 1:numberOfExperiments
    tsToStartChargingInRound = [];
    numberOfTriesToChargeInRound = [];
    isRxInNetwork = false;
    indexEnterNetwork = 1;
    indexLeaveNetwork = 1;
    roundNumber = 0;
    unsuccesfulTriesCount = 0;

    
    currentRxEvents = events{i,1};
    for j = 1:length(currentRxEvents)
        if (strcmp('buttonPress', currentRxEvents{j,2}))
            isRxInNetwork = true;
            currentTs = str2num(currentRxEvents{j,1});
            tsRxEnterNetwork(indexEnterNetwork) = currentTs;
            roundNumber = indexEnterNetwork;
            indexEnterNetwork = indexEnterNetwork + 1;
        elseif (strcmp('buttonEnd', currentRxEvents{j,2}))
            isRxInNetwork = false;
            currentTs = str2num(currentRxEvents{j,1});
            tsRxLeaveNetwork(indexLeaveNetwork) = currentTs;
            indexLeaveNetwork = indexLeaveNetwork + 1;
            
            unsuccesfulTriesCount = 0;
        end
        
        if (isRxInNetwork)
            if (strcmp('event', currentRxEvents{j,2}))
                if (currentRxEvents{j,3} == 4 && currentRxEvents{j,4} == 1)
                    unsuccesfulTriesCount = unsuccesfulTriesCount + 1;
                elseif (currentRxEvents{j,3} == 2 && currentRxEvents{j,4} == 1)
                    tsToStartChargingInRound(roundNumber) = str2num(currentRxEvents{j,1});
                    numberOfTriesToChargeInRound(roundNumber) = unsuccesfulTriesCount + 1;
                end
            end
        end
    end
    
    % remove last neter that is just last measurements
    tsRxEnterNetwork = tsRxEnterNetwork(1:end-1);
    
    timeToChargeInRound = tsToStartChargingInRound - tsRxEnterNetwork;
    
    timeToChargeInRoundGlobal = [timeToChargeInRoundGlobal timeToChargeInRound];
    numberOfTriesToChargeInRoundGlobal = [numberOfTriesToChargeInRoundGlobal numberOfTriesToChargeInRound];
end

figure;
hist(timeToChargeInRoundGlobal(timeToChargeInRoundGlobal>0)/1000);

figure;
hist(numberOfTriesToChargeInRoundGlobal(numberOfTriesToChargeInRoundGlobal>0));
