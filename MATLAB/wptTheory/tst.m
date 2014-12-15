clear all;
close all;
N = 4;
K = 4;

delayPing = 4;
timePacket = 0.01;
timeProcessing = 0;
timeFirstCheck = 2;

timeNoSuccess = delayPing + 4*timePacket + 4*timeProcessing + timeFirstCheck;
timeSuccess = delayPing + 3*timePacket + 3*timeProcessing;

% for n = 1:N
%     for k = 1:K
%         if (k <= n)
%             timeToStart(n,k) = timeToStartCharging(n, k, timeNoSuccess, timeSuccess);
%         else
%             timeToStart(n,k) = NaN;
%         end
%         
%     end
% end

for k = 1:K
    timeToStart(k) = timeToStartCharging(N, k, timeNoSuccess, timeSuccess);
end

for k = 1:K
    for i = 1:(N-k+1)
        prob(k, i) = probChargedInRound(N, k, i);
    end
end

figure;
hold on;
plot(1:K, timeToStart, ':*', 'LineSmoothing','on');
title('Expected time to start charging (s) for N = 4');
xlabel('K - number of Tx in charging range');
ylabel('Expected time to start charging (s)');

markers = {'+','o','*','.','x','s','d','^','v','>','<','p','h'};
cc=hsv(K);
figure;
hold on;
for k = 1:K
    plot(prob(k, :), ':', 'Marker', markers{mod(k,numel(markers))+1}, 'color',cc(k,:), 'LineSmoothing','on');
    legendInfo{k} = ['K = ' num2str(k)];
end
legend(legendInfo);
title('Probability of charging in round i');
xlabel('i - round number');
ylabel('Probability of charging');