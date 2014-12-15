close all
clear all

thresholds = [-70 -65 -60 -55 -50];

measurements = cell(length(thresholds), 1, 1);

measurementSources = {'rx1'; 'tx1'; 'tx2'; 'tx3'; 'tx4'};

for i=1:length(thresholds)
    currentThresholdFolder = abs(thresholds(i));
    for j=1:length(measurementSources)
        folderNamePrefix = ['meas101414_194142fixedNames/' measurementSources{j}];
        fileNameSpecific = [folderNamePrefix '/' 'MEAS*.TXT'];
        list = dir(fileNameSpecific);
        
        currentIndex = 1;
        for idx = 1:length(list)   % # we go through every file name

            name = list(idx).name;   
            number = sscanf(name,'MEAS%f.TXT',1);   % # we extract the number
            if (number > currentIndex)
                nameBeforeChange = [folderNamePrefix '/' name];
                nameAfterChange = [folderNamePrefix '/' 'MEAS' num2str(currentIndex) '.TXT' ];
                movefile(nameBeforeChange, nameAfterChange);
                
                nameBeforeChange = [folderNamePrefix '/' 'EVENT' num2str(number) '.TXT'];
                nameAfterChange = [folderNamePrefix '/' 'EVENT' num2str(currentIndex) '.TXT' ];
                movefile(nameBeforeChange, nameAfterChange);
                
                currentIndex = currentIndex + 1;
            elseif (number == currentIndex)
                currentIndex = currentIndex + 1;
            end
                    

        end
    end
    
end