colors = [[0 0 0.6]; [0.81 0.88 1]];
fontSize = 12;
fontSizeAxes = 10;
fontWeight = 'normal';
energySymbol = '(\delta)';
efficiencySymbol = '(\xi)';

%figurePosition = [x y width height];
figurePosition = [300 300 600 300];
hFig = figure(1);
set(hFig, 'Position', figurePosition)
set(gcf,'Renderer','painters');

x = [energyHarvestedProbing];
y = [energyHarvestedBeaconing];

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
h = ylabel(['Harvested energy ' energySymbol ' [J]']);
set(h,'FontSize',fontSize);
set(h,'fontweight', fontWeight);

set(findobj('type','axes'),'fontsize',fontSizeAxes)

txt=findobj(gca,'Type','text');
set(txt,'VerticalAlignment','Middle');

set(hFig, 'paperpositionmode', 'auto');
%saveas(hFig, 'D:\mgolinski\My Documents\wptnPaper\fig\resultsHarvestedEnergy.eps', 'epsc');

hFig = figure(2);
set(hFig, 'Position', figurePosition)
x = [efficiencyProbing*100];
y = [efficiencyBeaconing*100];

spacer = '       ';
thresholds  = repmat({[spacer '-70dBm'], [spacer '-65dBm'], [spacer '-60dBm'], [spacer '-55dBm'], [spacer '-50dBm']},1,2);
probingBeaconing = [repmat({'P'},1,5),repmat({'B'},1,5)];
h = boxplot([x, y],{thresholds,probingBeaconing}, 'factorgap',10, 'labelverbosity','minor', 'factorseparator',1);
set(findobj(get(h(1), 'parent'), 'type', 'text'), 'FontSize', fontSize, 'FontWeight', fontWeight);

h = findobj(gca,'Tag','Box');
for j=1:length(h)
   patch(get(h(j),'XData'),get(h(j),'YData'),colors(1+mod(j, size(colors,1)),:),'FaceAlpha',1);
end

h = findobj(gca,'Tag','Median')
%set(h, 'LineWidth',2, 'Color', 'r');
for j=1:length(h)
   line(get(h(j),'XData'),get(h(j),'YData'), 'LineWidth',2, 'Color', [0.86471    0.090196           0]);
end

%xlabel('Communication threshold [dBm]');
h = ylabel(['Efficiency ' efficiencySymbol ' [%]']);
set(h,'FontSize',fontSize);
set(h,'fontweight', fontWeight);

set(findobj('type','axes'),'fontsize',fontSizeAxes)

txt=findobj(gca,'Type','text');
set(txt,'VerticalAlignment','Middle');

set(hFig, 'paperpositionmode', 'auto');
%saveas(hFig, 'D:\mgolinski\My Documents\wptnPaper\fig\resultsEfficiency.eps', 'epsc');

hFig = figure(3);
set(hFig, 'Position', figurePosition)


subplot(1,2,1)
x = [energyHarvestedBack(:,2) energyHarvestedBack(:,4)];
y = [energyHarvestedBack(:,1) energyHarvestedBack(:,3)];

spacer = '       ';
thresholds  = repmat({[spacer 'normal'], [spacer 'back']},1,2);
probingBeaconing = [repmat({'P'},1,2),repmat({'B'},1,2)];
h = boxplot([x, y],{thresholds,probingBeaconing}, 'factorgap',10, 'labelverbosity','minor', 'factorseparator',1);
set(findobj(get(h(1), 'parent'), 'type', 'text'), 'FontSize', fontSize, 'FontWeight', fontWeight);

h = findobj(gca,'Tag','Box');
for j=1:length(h)
   patch(get(h(j),'XData'),get(h(j),'YData'),colors(1+mod(j, size(colors,1)),:),'FaceAlpha',1);
end

h = findobj(gca,'Tag','Median')
%set(h, 'LineWidth',2, 'Color', 'r');
for j=1:length(h)
   line(get(h(j),'XData'),get(h(j),'YData'), 'LineWidth',2, 'Color', [0.86471    0.090196           0]);
end

%xlabel('Communication threshold [dBm]');
h = ylabel(['Harvested energy ' energySymbol ' [J]']);
set(h,'FontSize',fontSize);
set(h,'fontweight', fontWeight);

set(findobj('type','axes'),'fontsize',fontSizeAxes)

txt=findobj(gca,'Type','text');
set(txt,'VerticalAlignment','Middle');

subplot(1,2,2)
%set(hFig, 'Position', figurePosition)

x = [efficiencyBack(:,2) efficiencyBack(:,4)]*100;
y = [efficiencyBack(:,1) efficiencyBack(:,3)]*100;

spacer = '       ';
thresholds  = repmat({[spacer 'normal'], [spacer 'back']},1,2);
probingBeaconing = [repmat({'P'},1,2),repmat({'B'},1,2)];
h = boxplot([x, y],{thresholds,probingBeaconing}, 'factorgap',10, 'labelverbosity','minor', 'factorseparator',1);
set(findobj(get(h(1), 'parent'), 'type', 'text'), 'FontSize', fontSize, 'FontWeight', fontWeight);

h = findobj(gca,'Tag','Box');
for j=1:length(h)
   patch(get(h(j),'XData'),get(h(j),'YData'),colors(1+mod(j, size(colors,1)),:),'FaceAlpha',1);
end

h = findobj(gca,'Tag','Median')
%set(h, 'LineWidth',2, 'Color', 'r');
for j=1:length(h)
   line(get(h(j),'XData'),get(h(j),'YData'), 'LineWidth',2, 'Color', [0.86471    0.090196           0]);
end

%xlabel('Communication threshold [dBm]');
h = ylabel(['Efficiency ' efficiencySymbol ' [%]']);
set(h,'FontSize',fontSize);
set(h,'fontweight', fontWeight);

set(findobj('type','axes'),'fontsize',fontSizeAxes)

txt=findobj(gca,'Type','text');
set(txt,'VerticalAlignment','Middle');

set(hFig, 'paperpositionmode', 'auto');
%saveas(hFig, 'D:\mgolinski\My Documents\wptnPaper\fig\resultsBack.eps', 'epsc');


hFig = figure(4);
set(hFig, 'Position', figurePosition)
set(gcf,'Renderer','painters');

x = [energyConsumptionProbing];
y = [energyConsumptionBeaconing];

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
h = ylabel(['Energy consumption ' energySymbol ' [J]']);
set(h,'FontSize',fontSize);
set(h,'fontweight', fontWeight);

set(findobj('type','axes'),'fontsize',fontSizeAxes)

txt=findobj(gca,'Type','text');
set(txt,'VerticalAlignment','Middle');

set(hFig, 'paperpositionmode', 'auto');