Y     = rand(1000,1);
X     = Y-rand(1000,1);
ACTid = randi(6,1000,1);

xylabel = repmat('xy',1000,1);
boxplot([X; Y], {repmat(ACTid,2,1), xylabel(:)} ,'factorgap',10)

% Retrieve handles to text labels
h = allchild(findall(gca,'type','hggroup'));

% Delete x, y labels
throw = findobj(h,'string','x','-or','string','y');
h     = setdiff(h,throw);
delete(throw);

% Center labels
mylbl  = {'this','is','a','pain','in...','guess!'};
hlbl   = findall(h,'type','text');
pos    = cell2mat(get(hlbl,'pos'));

% New centered position for first intra-group label
newPos = num2cell([mean(reshape(pos(:,1),2,[]))' pos(1:2:end,2:end)],2);
set(hlbl(1:2:end),{'pos'},newPos,{'string'},mylbl')

% delete second intra-group label
delete(hlbl(2:2:end))