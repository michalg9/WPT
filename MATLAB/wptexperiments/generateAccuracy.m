function [accuracyRxCharging, accuracyTxCharging, accuracyCharging, accuracyNotCharging] = generateAccuracy(tsRxIn, tsRxOut, tsRxAbove, tsRxBelow, shouldBeCharged, tsTxOnArray, tsTxOffArray)
%UNTITLED8 Summary of this function goes here
%   Detailed explanation goes here

chargingVector = shouldBeCharged;
%timesOn = tsRxOut - tsRxIn(1:end-1);
%timeCouldBeCharged = chargingVector*timesOn';
%timeCouldNotBeCharged = ~chargingVector*timesOn';
timeCouldBeCharged = 0;
timeCouldNotBeCharged = 0;
timeCharged = zeros(1,10);
testedPositions = [1 1 1 1 1 1 1 1 1 1];
    
for i = 1:10
    if (testedPositions(i))
    startIndex = tsRxIn(i);
    stopIndex = tsRxOut(i);
    
    if (chargingVector(i) == 1)
        timeCouldBeCharged = timeCouldBeCharged + (stopIndex - startIndex);
    else
        timeCouldNotBeCharged = timeCouldNotBeCharged + (stopIndex - startIndex);
    end
    
    if (length(tsRxAbove) > 0)
        rxBelowFlag = true;
        while(rxBelowFlag)
            if (tsRxAbove(1) < stopIndex && tsRxBelow(1) < stopIndex)
                timeCharged(i) = timeCharged(i) + tsRxBelow(1) - tsRxAbove(1);

                tsRxAbove = tsRxAbove(2:end);
                tsRxBelow = tsRxBelow(2:end);
            elseif (tsRxAbove(1) < stopIndex && tsRxBelow(1) > stopIndex)
                timeCharged(i) = timeCharged(i) + stopIndex - tsRxAbove(1);

                tsRxAbove = tsRxAbove(2:end);
                tsRxBelow = tsRxBelow(2:end);
                rxBelowFlag = false;
            elseif (tsRxAbove(1) > stopIndex)
                rxBelowFlag = false;
            end
        end
    end
    
    for j = 1:4
        tsTxOn = tsTxOnArray{j};
        tsTxOff = tsTxOffArray{j};
        currentTimeTxWasOn = 0;
        for k = 1:length(tsTxOn)
            if (tsTxOn(k) >= startIndex && tsTxOn(k) <= stopIndex)
                if (tsTxOff(k) <= stopIndex)
                    currentTimeTxWasOn = currentTimeTxWasOn + tsTxOff(k) - tsTxOn(k);
                elseif (tsTxOff(k) > stopIndex)
                    currentTimeTxWasOn = currentTimeTxWasOn + stopIndex - tsTxOn(k);
                end
            end
        end
        
         timeTxWasOn(j, i) = currentTimeTxWasOn;
    end
    end
end

timesOn = tsRxOut - tsRxIn(1:end-1);

timeRxShouldBeCharged = timeCouldBeCharged;
timeRxWasCharged = 0;
timeTxWasOnSum = 0;
for i = find(chargingVector == 1)
    if (testedPositions(i))
    timeRxWasCharged = timeRxWasCharged + timeCharged(i);
    timeTxWasOnSum = timeTxWasOnSum + sum(timeTxWasOn(:,i));
    end
end

accuracyRxCharging = timeRxWasCharged/timeRxShouldBeCharged;
accuracyTxCharging = timeRxWasCharged/timeTxWasOnSum;
accuracyCharging = accuracyRxCharging*accuracyTxCharging;

timeRxShouldNotBeCharged = timeCouldNotBeCharged;
timeTxWasOnSum = 0;
for i = find(chargingVector == 0)
    if (testedPositions(i))
    timeTxWasOnSum = timeTxWasOnSum + sum(timeTxWasOn(:,i));
    end
end
accuracyNotCharging = 1 - timeTxWasOnSum/(4*timeRxShouldNotBeCharged);
% accuracy = accuracyTx*accuracyRx;


end

