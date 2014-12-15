figure;
% First group of 3 data set with standard deviation 2
subplot(2,1,1);
% Concatenate the data sets from each group in a 10000 x 3 matrix
x = [energyHarvestedProbing*1000];
y = [energyHarvestedBeaconing*1000];

% Concatenate the each group in a  3 x 10000 x 3 matrix
h = cat(1, reshape(x,[1 size(x)]), reshape(y,[1 size(y)]));

aboxplot(h,'labels',[-70, -65, -60, -55, -50], 'colorgrad','orange_up'); % Advanced box plot

xlabel('Communication threshold [dBm]');
ylabel('Harvested energy [mW]');
title('Harvested energy [mW]');

h_legend = legend('probing', 'beaconing'); % Add a legend
set(h_legend,'FontSize',8);

subplot(2,1,2);
% First group of 3 data set with standard deviation 2

% Concatenate the data sets from each group in a 10000 x 3 matrix
x = [efficiencyProbing*100];
y = [efficiencyBeaconing*100];

% Concatenate the each group in a  3 x 10000 x 3 matrix
h = cat(1, reshape(x,[1 size(x)]), reshape(y,[1 size(y)]));

aboxplot(h,'labels',[-70, -65, -60, -55, -50], 'colorgrad','orange_up'); % Advanced box plot

xlabel('Communication threshold [dBm]');
ylabel('Efficiency [%]');
title('Efficiency [%]');

h_legend = legend('probing', 'beaconing'); % Add a legend
set(h_legend,'FontSize',8);


figure;

subplot(2,1,1);
% Concatenate the data sets from each group in a 10000 x 3 matrix
x = [energyHarvestedBack(:,2) energyHarvestedBack(:,4)]*1000;
y = [energyHarvestedBack(:,1) energyHarvestedBack(:,3)]*1000;

% Concatenate the each group in a  3 x 10000 x 3 matrix
h = cat(1, reshape(x,[1 size(x)]), reshape(y,[1 size(y)]));

aboxplot(h,'labels',{'normal', 'back'}, 'colorgrad','orange_up'); % Advanced box plot

xlabel('Communication threshold [dBm]');
ylabel('Harvested energy [mW]');
title('Harvested energy [mW]');

h_legend = legend('probing', 'beaconing'); % Add a legend
set(h_legend,'FontSize',8);

subplot(2,1,2);
% Concatenate the data sets from each group in a 10000 x 3 matrix
x = [efficiencyBack(:,2) efficiencyBack(:,4)]*100;
y = [efficiencyBack(:,1) efficiencyBack(:,3)]*100;

% Concatenate the each group in a  3 x 10000 x 3 matrix
h = cat(1, reshape(x,[1 size(x)]), reshape(y,[1 size(y)]));

aboxplot(h,'labels',{'normal', 'back'}, 'colorgrad','orange_up'); % Advanced box plot

xlabel('Communication threshold [dBm]');
ylabel('Efficiency [%]');
title('Efficiency [%]');

h_legend = legend('probing', 'beaconing'); % Add a legend
set(h_legend,'FontSize',8);
