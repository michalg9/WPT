numberOfExperiments = 50;

countOfSingleExperiment = 500;
N = 4;
K_max = 4;

% theory values
for k = 1:K_max
    for i = 1:(N-k+1)
        probTheory(k, i) = probChargedInRound(N, k, i);
    end
end

% simulation values
for i = 1:numberOfExperiments
    for k = 1:K_max
        probSimulation{k,i} = simulateChargingInRound(N, k, countOfSingleExperiment);
    end
end


% plotTheory
markers = {'+','o','*','.','x','s','d','^','v','>','<','p','h'};
cc=hsv(K_max);
figure;
hold on;
for k = 1:K_max
    plot(probTheory(k, :), ':', 'Marker', markers{mod(k,numel(markers))+1}, 'MarkerSize', 10, 'color',cc(k,:));
    xlim([0 5])
    ylim([0 1])
    legendInfo{k} = ['K = ' num2str(k)];
end
legend(legendInfo);
set(gca,'XTick',[1, 2, 3, 4])

% plotSimulation
for k = 1:K_max
    for i = 1:length(probSimulation)
        plot(probSimulation{k, i}, 'LineStyle', 'none', 'Marker', markers{mod(k,numel(markers))+1}, 'MarkerSize', 5, 'color',cc(k,:));
    end
end

title('Probability of charging in round i');
xlabel('i - round number');
ylabel('Probability of charging');
