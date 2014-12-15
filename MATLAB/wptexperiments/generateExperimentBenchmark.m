function [ shouldBeCharged ] = generateExperimentBenchmark()
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here

events = importEvents('meas110714_171415');;
measurements = importMeasurements('meas110714_171415');

[numberOfExperiments, numberOfMeasurements] = size(measurements);


for i = 1:numberOfExperiments
    rxEventLog = events{i,1};
    tsRxIn = [];
    tsRxOut = [];
    
    for j = 1:length(rxEventLog)
        if(strcmp(rxEventLog{j,2}, 'buttonPress'))
            tsRxIn = [tsRxIn, rxEventLog{j,1}];
        end
        
        if(strcmp(rxEventLog{j,2}, 'buttonEnd'))
            tsRxOut = [tsRxOut, rxEventLog{j,1}];
        end
    end
    
    rxMeasurement = measurements{i,1};
    rxMeasurementIndex = 1;
    for j = 1:10
        startIndex = 0;
        endIndex = 0;
        while (rxMeasurement.data(rxMeasurementIndex,1) <= tsRxOut(j))
            if (rxMeasurement.data(rxMeasurementIndex,1) < tsRxIn(j))
                startIndex = rxMeasurementIndex;
            end
            rxMeasurementIndex = rxMeasurementIndex + 1;
        end
        startIndex = startIndex+2;
        endIndex = rxMeasurementIndex;
        samples = rxMeasurement.data(startIndex:endIndex,2);
        sampleArray{i,j} = samples;
    end
end


[numberOfTx, numberOfMeasuringPoints] = size(sampleArray);
meanValues = zeros(4,10);
maxValues = zeros(4,10);

for i = 1:numberOfTx
    for j = 1:numberOfMeasuringPoints
        samples = sampleArray{i,j};
        meanValues(i,j) = mean(samples);
        maxValues(i,j) = max(samples);
    end
end


shouldBeCharged = NaN*zeros(1,10);
voltageThreshold = 500;

for i = 1:10
    evaluatedValues = meanValues;
    if (max(evaluatedValues(:,i)) >= voltageThreshold)
        shouldBeCharged(i) = 1;
    else
        shouldBeCharged(i) = 0;
    end
end

end
