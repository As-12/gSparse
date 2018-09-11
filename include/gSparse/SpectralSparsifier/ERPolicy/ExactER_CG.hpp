 // Decode this stuff
 
 elseif strcmp(type_,'org')
        [m,~] = size(elist);
        B = sparse([1:m 1:m],[elist(:,1) elist(:,2)],[ones(m,1) -1*ones(m,1)],m,length(L));
            if length(L) > 600 % bigger graphs
                if nargin == 7
                    pfun = pfun_;
                else
                    pfun = cmg_sdd(L);
                end
                optimset('display','off');
                for j=1:m
                    [Z flag]= pcg(L,B(j,:)',1e-10,numIterations,pfun);
                    if flag > 0
                        error(['PCG FLAG: ' num2str(flag)])
                    end
                    eff_res(j) = B(j,:)*Z;
                end
            else % smaller graphs
                opts.type = 'nofill'; opts.michol = 'on';
                for j=1:m
                    [Z,flag] = pcg(L,B(j,:)',1e-10,numIterations);
                    eff_res(j) = B(j,:)*Z;
                end
            end
        eff_res = eff_res';