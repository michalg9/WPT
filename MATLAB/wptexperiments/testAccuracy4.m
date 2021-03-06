
event = events{1};
measurement = measurements{1};

[numberOfExperiments, numberOfMeasurements] = size(measurements{1});

for i = 1:1
    rxEventLog = event{i,1};
    tsRxIn = [];
    tsRxOut = [];
    tsRxAbove = [];
    tsRxBelow = [];

    rxMeasurement = measurement{i,1};
    
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
        txEventLog = event{i,k};
        tsTxOn = [];
        tsTxOff = [];
        
        txMeasurement = measurement{i,k};
        
        for j = 1:length(txEventLog)
            if(strcmp(txEventLog{j,2}, 'action') && txEventLog{j,3} == 2)
                tsTxOn = [tsTxOn, txEventLog{j,1}];
            end
            
            if(strcmp(txEventLog{j,2}, 'action') && txEventLog{j,3} == 1 && txEventLog{j,4} == 2)
                tsTxOff = [tsTxOff, txEventLog{j,1}];
            end
        end
        
        tsTxOnArray{k-1} = tsTxOn;
        tsTxOffArray{k-1} = tsTxOff;
    end
end      


figure;
subplot(5,1,1)
plot(rxMeasurement.data(:,1)/1000, rxMeasurement.data(:,2));
hold on
plot(tsRxIn/1000, zeros(1, length(tsRxIn)), '*g')
plot(tsRxOut/1000, zeros(1, length(tsRxOut)), '*r')
plot(tsRxAbove/1000, zeros(1, length(tsRxAbove)), '*k')
plot(tsRxBelow/1000, zeros(1, length(tsRxBelow)), '*y')
xlim([0 700])

txMeasurement = measurement{1,2};
tsTxOn = tsTxOnArray{1};
tsTxOff = tsTxOffArray{1};

subplot(5,1,2)
plot(txMeasurement.data(:,1)/1000, txMeasurement.data(:,2));
hold on
plot(tsTxOn/1000, zeros(1, length(tsTxOn)), '*g')
plot(tsTxOff/1000, zeros(1, length(tsTxOff)), '*r')
xlim([0 700])
ylim([0 1.2])

txMeasurement = measurement{1,3};
tsTxOn = tsTxOnArray{2};
tsTxOff = tsTxOffArray{2};

subplot(5,1,3)
plot(txMeasurement.data(:,1)/1000, txMeasurement.data(:,2));
hold on
plot(tsTxOn/1000, zeros(1, length(tsTxOn)), '*g')
plot(tsTxOff/1000, zeros(1, length(tsTxOff)), '*r')
xlim([0 700])
ylim([0 1.2])

txMeasurement = measurement{1,4};
tsTxOn = tsTxOnArray{3};
tsTxOff = tsTxOffArray{3};

subplot(5,1,4)
plot(txMeasurement.data(:,1)/1000, txMeasurement.data(:,2));
hold on
plot(tsTxOn/1000, zeros(1, length(tsTxOn)), '*g')
plot(tsTxOff/1000, zeros(1, length(tsTxOff)), '*r')
xlim([0 700])
ylim([0 1.2])

txMeasurement = measurement{1,5};
tsTxOn = tsTxOnArray{4};
tsTxOff = tsTxOffArray{4};

subplot(5,1,5)
plot(txMeasurement.data(:,1)/1000, txMeasurement.data(:,2));
hold on
plot(tsTxOn/1000, zeros(1, length(tsTxOn)), '*g')
plot(tsTxOff/1000, zeros(1, length(tsTxOff)), '*r')
xlim([0 700])
ylim([0 1.2])

chargingVector = shouldBeCharged;
%timesOn = tsRxOut - tsRxIn(1:end-1);
%timeCouldBeCharged = chargingVector*timesOn';
%timeCouldNotBeCharged = ~chargingVector*timesOn';
timeCouldBeCharged = 0;
timeCouldNotBeCharged = 0;
timeCharged = zeros(1,10);

for i = 1:10
    startIndex = tsRxIn(i);
    stopIndex = tsRxOut(i);
    
    if (chargingVector(i) == 1)
        timeCouldBeCharged = timeCouldBeCharged + (stopIndex - startIndex);
    else
        timeCouldNotBeCharged = timeCouldNotBeCharged + (stopIndex - startIndex);
    end
    
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



shouldBeCharged = chargingVector;

timesOn = tsRxOut - tsRxIn(1:end-1);

timeRxShouldBeCharged = timeCouldBeCharged;
timeRxWasCharged = 0;
timeTxWasOnSum = 0;
for i = find(shouldBeCharged == 1)
    timeRxWasCharged = timeRxWasCharged + timeCharged(i);
    timeTxWasOnSum = timeTxWasOnSum + sum(timeTxWasOn(:,i));
end

accuracy1 = timeRxWasCharged/timeRxShouldBeCharged;
accuracy2 = accuracy1*timeRxWasCharged/timeTxWasOnSum;

timeRxShouldNotBeCharged = timeCouldNotBeCharged;
timeTxWasOnSum = 0;
for i = find(shouldBeCharged == 0)
    timeTxWasOnSum = timeTxWasOnSum + sum(timeTxWasOn(:,i));
end
accuracy3 = 1 - timeTxWasOnSum/(4*timeRxShouldNotBeCharged);
% accuracy = accuracyTx*accuracyRx;