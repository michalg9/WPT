formatOut = 'mmddyy_HHMMSS';
folderName = ['meas' datestr(clock, formatOut)];
mkdir(folderName);

measurementSources = ['rx1'; 'tx1'; 'tx2'; 'tx3'; 'tx4'];
measurementSourcesCell = cellstr(measurementSources);

for i = 1:length(measurementSourcesCell)
    disp(['Insert ' measurementSourcesCell{i} ' card']);
    for j=10:-1:1
        fprintf('%i ', j);
        pause(1);
    end
    fprintf('\n');
    disp(['Copying from ' measurementSourcesCell{i} ' card']);
    copyfile('i:/', [folderName '/' measurementSourcesCell{i}]);
end
