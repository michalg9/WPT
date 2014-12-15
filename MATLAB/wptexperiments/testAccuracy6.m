shouldBeCharged = generateExperimentBenchmark();
%shouldBeCharged = [1 0 0 0 0 0 0 0 0 0];

thresholds = [-70 -65 -60 -55 -50];

figure(1);
hold on;

for i=1:length(thresholds)
    [accuracyRxChargingBeaconing{i}, accuracyTxCharging{i}, accuracyCharging{i}, accuracyNotChargingBeaconing{i}] = generateAccuracyAll(eventsBeaconing{i}, measurementsBeaconing{i}, shouldBeCharged, true);
end
% 
% for i=1:length(thresholds)
%     plot(thresholds(i), accuracyRxCharging{i}, 'b*');
% end

for i=1:length(thresholds)
    [accuracyRxChargingProbing{i}, accuracyTxCharging{i}, accuracyCharging{i}, accuracyNotChargingProbing{i}] = generateAccuracyAll(eventsProbing{i}, measurementsProbing{i}, shouldBeCharged, false);
end

% for i=1:length(thresholds)
%     plot(thresholds(i), accuracyRxCharging{i}, 'r*');
% end

colors = [[0 0 0.6]; [0.81 0.88 1]];
fontSize = 12;
fontSizeAxes = 10;
fontWeight = 'normal';

%figurePosition = [x y width height];
figurePosition = [300 300 600 300];
hFig = figure(1);
set(hFig, 'Position', figurePosition)
set(gcf,'Renderer','painters');

x = cell2mat(accuracyRxChargingProbing');
x = x';
y = cell2mat(accuracyRxChargingBeaconing');
y = y'

spacer = '       ';
thresholds  = repmat({[spacer '-70dBm'], [spacer '-65dBm'], [spacer '-60dBm'], [spacer '-55dBm'], [spacer '-50dBm']},1,2);
probingBeaconing = [repmat({'P'},1,5),repmat({'B'},1,5)];
h = boxplot([x, y],{thresholds,probingBeaconing}, 'factorgap',10, 'labelverbosity','minor', 'factorseparator',1);
set(findobj(get(h(1), 'parent'), 'type', 'text'), 'FontSize', fontSize, 'FontWeight', fontWeight);

h = findobj(gca,'Tag','Box');
for j=1:length(h)
   patch(get(h(j),'XData'),get(h(j),'YData'),colors(1+mod(j, size(colors,1)),:),'FaceAlpha',1);
end

%fix cause there is a bug with OpenGL renderer that doesnt draw borders
%http://stackoverflow.com/questions/9775020/black-lines-missing-in-the-box-holding-the-axes-of-a-matlab-plot
h = findobj(gca,'Tag','Median')
%set(h, 'LineWidth',2, 'Color', 'r');
for j=1:length(h)
   line(get(h(j),'XData'),get(h(j),'YData'), 'LineWidth',2, 'Color', [0.86471    0.090196           0]);
end

%xlabel('Communication threshold [dBm]');
h = ylabel('Charging accuracy');
set(h,'FontSize',fontSize);
set(h,'fontweight', fontWeight);

set(findobj('type','axes'),'fontsize',fontSizeAxes)

txt=findobj(gca,'Type','text');
set(txt,'VerticalAlignment','Middle');

set(hFig, 'paperpositionmode', 'auto');
%saveas(hFig, 'D:\mgolinski\My Documents\wptnPaper\fig\resultsAccuracyRx.eps', 'epsc');




hFig = figure(2);
set(hFig, 'Position', figurePosition)
set(gcf,'Renderer','painters');

x = cell2mat(accuracyNotChargingProbing');
x = x';
y = cell2mat(accuracyNotChargingBeaconing');
y = y'

spacer = '       ';
thresholds  = repmat({[spacer '-70dBm'], [spacer '-65dBm'], [spacer '-60dBm'], [spacer '-55dBm'], [spacer '-50dBm']},1,2);
probingBeaconing = [repmat({'P'},1,5),repmat({'B'},1,5)];
h = boxplot([x, y],{thresholds,probingBeaconing}, 'factorgap',10, 'labelverbosity','minor', 'factorseparator',1);
set(findobj(get(h(1), 'parent'), 'type', 'text'), 'FontSize', fontSize, 'FontWeight', fontWeight);

h = findobj(gca,'Tag','Box');
for j=1:length(h)
   patch(get(h(j),'XData'),get(h(j),'YData'),colors(1+mod(j, size(colors,1)),:),'FaceAlpha',1);
end

%fix cause there is a bug with OpenGL renderer that doesnt draw borders
%http://stackoverflow.com/questions/9775020/black-lines-missing-in-the-box-holding-the-axes-of-a-matlab-plot
h = findobj(gca,'Tag','Median')
%set(h, 'LineWidth',2, 'Color', 'r');
for j=1:length(h)
   line(get(h(j),'XData'),get(h(j),'YData'), 'LineWidth',2, 'Color', [0.86471    0.090196           0]);
end

%xlabel('Communication threshold [dBm]');
h = ylabel('Charging accuracy');
set(h,'FontSize',fontSize);
set(h,'fontweight', fontWeight);

set(findobj('type','axes'),'fontsize',fontSizeAxes)

txt=findobj(gca,'Type','text');
set(txt,'VerticalAlignment','Middle');

set(hFig, 'paperpositionmode', 'auto');
%saveas(hFig, 'D:\mgolinski\My Documents\wptnPaper\fig\resultsAccuracyNotCharging.eps', 'epsc');

