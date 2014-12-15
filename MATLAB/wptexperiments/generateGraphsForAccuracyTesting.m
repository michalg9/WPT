function [] = generateGraphsForAccuracyTesting(measurement, i, tsRxIn, tsRxOut, tsRxAbove, tsRxBelow, tsTxOnArray, tsTxOffArray)
%UNTITLED6 Summary of this function goes here
%   Detailed explanation goes here

figure;
subplot(5,1,1)
rxMeasurement = measurement{i,1};
plot(rxMeasurement.data(:,1)/1000, rxMeasurement.data(:,2));
hold on
plot(tsRxIn/1000, zeros(1, length(tsRxIn)), '*g')
plot(tsRxOut/1000, zeros(1, length(tsRxOut)), '*r')
plot(tsRxAbove/1000, zeros(1, length(tsRxAbove)), '*k')
plot(tsRxBelow/1000, zeros(1, length(tsRxBelow)), '*y')
xlim([0 700])

txMeasurement = measurement{i,2};
tsTxOn = tsTxOnArray{1};
tsTxOff = tsTxOffArray{1};

subplot(5,1,2)
plot(txMeasurement.data(:,1)/1000, txMeasurement.data(:,2));
hold on
plot(tsTxOn/1000, zeros(1, length(tsTxOn)), '*g')
plot(tsTxOff/1000, zeros(1, length(tsTxOff)), '*r')
xlim([0 700])
ylim([0 1.2])

txMeasurement = measurement{i,3};
tsTxOn = tsTxOnArray{2};
tsTxOff = tsTxOffArray{2};

subplot(5,1,3)
plot(txMeasurement.data(:,1)/1000, txMeasurement.data(:,2));
hold on
plot(tsTxOn/1000, zeros(1, length(tsTxOn)), '*g')
plot(tsTxOff/1000, zeros(1, length(tsTxOff)), '*r')
xlim([0 700])
ylim([0 1.2])

txMeasurement = measurement{i,4};
tsTxOn = tsTxOnArray{3};
tsTxOff = tsTxOffArray{3};

subplot(5,1,4)
plot(txMeasurement.data(:,1)/1000, txMeasurement.data(:,2));
hold on
plot(tsTxOn/1000, zeros(1, length(tsTxOn)), '*g')
plot(tsTxOff/1000, zeros(1, length(tsTxOff)), '*r')
xlim([0 700])
ylim([0 1.2])

txMeasurement = measurement{i,5};
tsTxOn = tsTxOnArray{4};
tsTxOff = tsTxOffArray{4};

subplot(5,1,5)
plot(txMeasurement.data(:,1)/1000, txMeasurement.data(:,2));
hold on
plot(tsTxOn/1000, zeros(1, length(tsTxOn)), '*g')
plot(tsTxOff/1000, zeros(1, length(tsTxOff)), '*r')
xlim([0 700])
ylim([0 1.2])


end

