% Parameters common among target files
TARGET_DIST = .12;
angles = [0 82.5:2.5:97.5 180 262.5:2.5:277.5]'; %[80:2:100]
N_tgts = length(angles);
Ntrials = 96;
Nsubj = 16;

tgt_files = cell(1,Nsubj);

for k = 4:Nsubj
    % 2x Familiarization / Baseline, 4x Mirror,2x NVF, 2x washout
    
    tgt_files = cell(1,9);
    for j = 1:length(tgt_files)
        angle_index_j = [];
        for q = 1:(Ntrials/N_tgts)
            angle_index_j = [angle_index_j;randperm(N_tgts)'];
        end
        tgt_files{j} = zeros(Ntrials,8);
        tgt_files{j}(:,1) = 1:Ntrials;
        tgt_files{j}(:,2:3) = [TARGET_DIST*cosd(angles(angle_index_j)) TARGET_DIST*sind(angles(angle_index_j))];
        %probe vs non-probe blocks        
        if ismember(j,[2 8])
            tgt_files{j}(:,8) = 1;
        elseif ismember(j,[9])
            tgt_files{j}(1:16,8) = 1;
            tgt_files{j}(17:end,7) = 1;
            tgt_files{j}(17:end,8) = 2;
        else
            tgt_files{j}(:,7) = 1;
            tgt_files{j}(:,8) = 2;
        end
        %mirroring
        if ismember(j,4:7)
            tgt_files{j}(:,5) = 1;
        end
    end
    %tgt_files{k} = tgt_files;
    for j = 1:length(tgt_files)
        dirname = ['tgt_files_dense_2side_with_x\Subject_',int2str(k)];
        if ~isdir(dirname), mkdir(dirname); end
        dlmwrite([dirname,'\B',int2str(j),'.tgt'],tgt_files{j},' ');
    end
end