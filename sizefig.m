function sizefig(layout, layout2)

hf = gcf;
if nargin<1
    load('c:\Users\Tim\Documents\MATLAB\my functions\figsizes.mat','sz');
    set(hf,'position', sz);
elseif nargin == 1
    if strcmp(layout,'wee')
        set(hf,'position', [70 190 478 348]);
    elseif strcmp(layout,'banner')
        set(hf,'position', [70 190 650 232]);
    elseif strcmp(layout,'1x1')
        set(hf,'position', [70 190 650 500]);
    elseif strcmp(layout,'2x1')
        set(hf,'position', [70 190 650 1000]);
    elseif strcmp(layout,'1x2')
        set(hf,'position', [70 190 1300 500]);
    elseif strcmp(layout,'2x2')
        set(hf,'position', [70 190 1300 1000]);
    elseif strcmp(layout,'tall')
        set(hf,'position', [70 190 650 1050]);
    elseif strcmp(layout,'wide')
        set(hf,'position', [70 190 1500 500]);
    elseif strcmp(layout,'big')
        set(hf,'position', [70 190 1500 1050]);
    elseif strcmp(layout,'portrait')
        set(hf,'position', [70 190 600 800]);
    elseif strcmp(layout,'landscape')
        set(hf,'position', [70 190 1050 813]);
    elseif strcmp(layout,'port')
        set(hf,'position', [70 190 600 800]);
    elseif strcmp(layout,'land')
        set(hf,'position', [70 190 1050 813]);
    end
else
    set(hf,'position', [80 100 layout layout2]);
end
figure(hf);

end
