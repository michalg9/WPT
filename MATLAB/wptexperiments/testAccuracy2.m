
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
    
    txEventLog = event{i,2};
    tsTxOn = [];
    tsTxOff = [];
    
    txMeasurement = measurement{i,2};
    
    for j = 1:length(txEventLog)      
        if(strcmp(txEventLog{j,2}, 'action') && txEventLog{j,3} == 2)
            tsTxOn = [tsTxOn, txEventLog{j,1}];
        end
        
        if(strcmp(txEventLog{j,2}, 'action') && txEventLog{j,3} == 1 && txEventLog{j,4} == 2)
            tsTxOff = [tsTxOff, txEventLog{j,1}];
        end
    end
end      


figure;
subplot(2,1,1)
plot(rxMeasurement.data(:,1)/1000, rxMeasurement.data(:,2));
hold on
plot(tsRxIn/1000, zeros(1, length(tsRxIn)), '*g')
plot(tsRxOut/1000, zeros(1, length(tsRxOut)), '*r')
plot(tsRxAbove/1000, zeros(1, length(tsRxAbove)), '*k')
plot(tsRxBelow/1000, zeros(1, length(tsRxBelow)), '*y')
xlim([0 700])
subplot(2,1,2)
plot(txMeasurement.data(:,1)/1000, txMeasurement.data(:,2));
hold on
plot(tsTxOn/1000, zeros(1, length(tsTxOn)), '*g')
plot(tsTxOff/1000, zeros(1, length(tsTxOff)), '*r')
xlim([0 700])
ylim([0 1.2])

timeRequestedCharging = sum(tsRxOut - tsRxIn(1:end-1));
timeCharged = 0;
timeTxOn = 0;
for i = 1:10
    startIndex = tsRxIn(i);
    stopIndex = tsRxOut(i);

    rxBelowBelowFlag = true;
    while(rxBelowBelowFlag)
        if (tsRxAbove(1) < stopIndex && tsRxBelow(1)< stopIndex)
            timeCharged = timeCharged + (tsRxBelow(1) - tsRxAbove(1));

            for k = 2:5
                
                txMeasurement = measurement{1,k};
                txOnFlag = false;
                txOnStartTime = 0;
                txOnStopTime = 0;
                for j = 1:length( txMeasurement.data(:,1))
                   if (~txOnFlag && txMeasurement.data(j,1) >= tsRxAbove(1) && txMeasurement.data(j,1) < tsRxBelow(1) && txMeasurement.data(j,2) == 1)
                      txOnFlag = true;
                      txOnStartTime = txMeasurement.data(j,1);
                   end

                   if (txOnFlag && txMeasurement.data(j,1) >= tsRxAbove(1) && txMeasurement.data(j,1) < tsRxBelow(1) && txMeasurement.data(j,2) == 0)
                       txOnFlag = false;
                       txOnStopTime = txMeasurement.data(j,1);

                       timeTxOn = timeTxOn + txOnStopTime - txOnStartTime;
                   end

                   if (txOnFlag && txMeasurement.data(j,1) >= tsRxBelow(1) && txMeasurement.data(j,2) == 1)
                      txOnFlag = false;
                      txOnStopTime = txMeasurement.data(j,1);

                      timeTxOn = timeTxOn + txOnStopTime - txOnStartTime;
                   end
                end
            end
            
            tsRxAbove = tsRxAbove(2:end);
            tsRxBelow = tsRxBelow(2:end);
        elseif (tsRxAbove(1) < stopIndex && tsRxBelow(1) > stopIndex)
            timeCharged = timeCharged + (stopIndex - tsRxAbove(1));
                      
            for k = 2:5
                
                txMeasurement = measurement{1,k};
                txOnFlag = false;
                txOnStartTime = 0;
                txOnStopTime = 0;
                for j = 1:length( txMeasurement.data(:,1))
                   if (~txOnFlag && txMeasurement.data(j,1) >= tsRxAbove(1) && txMeasurement.data(j,1) < tsRxBelow(1) && txMeasurement.data(j,2) == 1)
                      txOnFlag = true;
                      txOnStartTime = txMeasurement.data(j,1);
                   end

                   if (txOnFlag && txMeasurement.data(j,1) >= tsRxAbove(1) && txMeasurement.data(j,1) < tsRxBelow(1) && txMeasurement.data(j,2) == 0)
                       txOnFlag = false;
                       txOnStopTime = txMeasurement.data(j,1);

                       timeTxOn = timeTxOn + txOnStopTime - txOnStartTime;
                   end

                   if (txOnFlag && txMeasurement.data(j,1) >= tsRxBelow(1) && txMeasurement.data(j,2) == 1)
                      txOnFlag = false;
                      txOnStopTime = txMeasurement.data(j,1);

                      timeTxOn = timeTxOn + txOnStopTime - txOnStartTime;
                   end
                end
            end
            
            tsRxAbove = tsRxAbove(2:end);
            tsRxBelow = tsRxBelow(2:end);
            rxBelowBelowFlag = false;
        elseif (tsRxAbove(1) > stopIndex)
            rxBelowBelowFlag = false;
        end
    end
end


accuracyRx = timeCharged/timeRequestedCharging;
accuracyTx = timeCharged/timeTxOn;

accuracy = accuracyTx*accuracyRx;