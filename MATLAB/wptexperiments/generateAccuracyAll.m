function [accuracyRxCharging, accuracyTxCharging, accuracyCharging, accuracyNotCharging] = generateAccuracyAll(events, measurements, shouldBeCharged, isBeaconing)
%UNTITLED9 Summary of this function goes here
%   Detailed explanation goes here

if (isBeaconing)
    actionOnCode = 0;
    actionOffCode = 1;
    stateOn = 1;
else
    actionOnCode = 2;
    actionOffCode = 1;
    stateOn = 2;
end

accuracyRxCharging = NaN * zeros(1,5);
accuracyTxCharging = NaN * zeros(1,5);
accuracyCharging = NaN * zeros(1,5);
accuracyNotCharging = NaN * zeros(1,5);

for i = 1:5
    rxEventLog = events{i,1};
    tsRxIn = [];
    tsRxOut = [];
    tsRxAbove = [];
    tsRxBelow = [];

    rxMeasurement = measurements{i,1};
    
    isCharged = false;
    threshold = 500;
    for j = 1:length(rxMeasurement.data(:,1))
        if (~isCharged && rxMeasurement.data(j,2) >= threshold)
           isCharged = true;
           tsRxAbove = [tsRxAbove, rxMeasurement.data(j,1)];
        end
        
        if (isCharged && rxMeasurement.data(j,2) < threshold)
           isCharged = false;
           tsRxBelow = [tsRxBelow, rxMeasurement.data(j,1)];
        end
    end
    
    for j = 1:length(rxEventLog)
        if(strcmp(rxEventLog{j,2}, 'buttonPress'))
            tsRxIn = [tsRxIn, rxEventLog{j,1}];
        end
                
        if(strcmp(rxEventLog{j,2}, 'buttonEnd'))
            tsRxOut = [tsRxOut, rxEventLog{j,1}];
        end
    end
    
    for k=2:5
        txEventLog = events{i,k};
        tsTxOn = [];
        tsTxOff = [];
        
        txMeasurement = measurements{i,k};
        
        for j = 1:length(txEventLog)
            if(strcmp(txEventLog{j,2}, 'action') && txEventLog{j,3} == actionOnCode)
                tsTxOn = [tsTxOn, txEventLog{j,1}];
            end
            
            if(strcmp(txEventLog{j,2}, 'action') && txEventLog{j,3} == actionOffCode && txEventLog{j,4} == stateOn)
                tsTxOff = [tsTxOff, txEventLog{j,1}];
            end
        end
        
        tsTxOnArray{k-1} = tsTxOn;
        tsTxOffArray{k-1} = tsTxOff;
    end
    
    
    
    
    %generateGraphsForAccuracyTesting(measurements, i, tsRxIn, tsRxOut, tsRxAbove, tsRxBelow, tsTxOnArray, tsTxOffArray);
    
    [accuracyRxCharging(i), accuracyTxCharging(i), accuracyCharging(i), accuracyNotCharging(i)] = generateAccuracy(tsRxIn, tsRxOut, tsRxAbove, tsRxBelow, shouldBeCharged, tsTxOnArray, tsTxOffArray);
    
end      





end

