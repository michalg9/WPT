
shouldBeCharged = generateExperimentBenchmark();

measurementsBack{1} = measurementsProbing{1};
measurementsBack{2} = measurementsProbingBack;
measurementsBack{3} = measurementsBeaconing{1};
measurementsBack{4} = measurementsBeaconingBack;

eventsBack{1} = eventsProbing{1};
eventsBack{2} = eventsProbingBack;
eventsBack{3} = eventsBeaconing{1};
eventsBack{4} = eventsBeaconingBack;

for i=1:2
    [accuracyRxChargingBeaconing{i}, accuracyTxCharging{i}, accuracyCharging{i}, accuracyNotChargingBeaconing{i}] = generateAccuracyAll(eventsBack{i}, measurementsBack{i}, shouldBeCharged, false);
end

for i=3:4
    [accuracyRxChargingBeaconing{i}, accuracyTxCharging{i}, accuracyCharging{i}, accuracyNotChargingBeaconing{i}] = generateAccuracyAll(eventsBack{i}, measurementsBack{i}, shouldBeCharged, true);
end

figure;
hold on;
for i=1:4
    plot(i, accuracyRxChargingBeaconing{i}, 'r*');
end

for i=1:4
    plot(i, accuracyTxCharging{i}, 'b*');
end