
figure;
subplot(2,2,1)
boxplot([energyHarvestedBeaconing*1000], 'labels', thresholds);
ylim([200 2200])
title('Energy');
subplot(2,2,2)
boxplot([efficiencyBeaconing*100], 'labels', thresholds);
ylim([0 0.5])
title('Efficiency');


subplot(2,2,3)
boxplot([energyHarvestedProbing*1000], 'labels', thresholds);
ylim([200 2200])
title('Energy');
subplot(2,2,4)
boxplot([efficiencyProbing*100], 'labels', thresholds);
ylim([0 0.5])
title('Efficiency');

figure;
subplot(1,2,1)
boxplot([energyHarvestedBack*1000], 'labels', {'-70bn', '-70pn', '-70bb', '-70pb'});
ylim([200 2200])
title('Energy');
subplot(1,2,2)
boxplot([efficiencyBack*100], 'labels', {'-70bn', '-70pn', '-70bb', '-70pb'});
ylim([0 0.5])
title('Efficiency');


figure;
subplot(2,2,1)
boxplot(packetsSendBeaconing, 'labels', thresholds);
%ylim([14 26])
title('PacketsSend');
subplot(2,2,2)
boxplot(packetsReceivedBeaconing, 'labels', thresholds);
%ylim([0 6])
title('PacketsReceived');


subplot(2,2,3)
boxplot(packetsSendProbing, 'labels', thresholds);
%ylim([14 26])
title('PacketsSend');
subplot(2,2,4)
boxplot(packetsReceivedProbing, 'labels', thresholds);
%ylim([0 6])
title('PacketsReceived');


figure;
subplot(1,2,1)
boxplot(packetsSendBack, 'labels', {'-70bn', '-70pn', '-70bb', '-70pb'});
%ylim([200 2200])
title('PacketsSend');
subplot(1,2,2)
boxplot(packetsReceivedBack, 'labels', {'-70bn', '-70pn', '-70bb', '-70pb'});
%ylim([0 0.5])
title('PacketsReceived');
