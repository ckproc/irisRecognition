
ntr = 0;
nte = 0;

mkdir iris
mkdir iris/trainingImages
%mkdir iris/testImages

f_bb_tr = fopen('iris/boundingbox.txt', 'w');
%f_bb_te = fopen('iris/boundingbox_test.txt', 'w');
f_kp_tr = fopen('iris/keypoints.txt', 'w');

fld = 'train'
nms = dir( fullfile(fld, '*.cir') );

for k = 1:length(nms)
	nm = nms(k).name;
	sprintf(nm(1:end-4));
	im = imread( fullfile(fld, [nm(1:end-4) '.jpg']) );
	v = dlmread( fullfile(fld, nm) );
	
	kps = [];
	
	
	
	x = v(1);   y = v(2); r = v(3);
	% kps = [kps; [x,y]];
	for theta = 0:30:330
		x_ = x + r*cos( theta*pi/180 );
		y_ = y + r*sin( theta*pi/180 );
		
		kps = [kps; [x_, y_]];
	end
	
	x = v(4);   y = v(5); r = v(6);
	% kps = [kps; [x,y]];
	for theta = 0:45:315
		x_ = x + r*cos( theta*pi/180 );
		y_ = y + r*sin( theta*pi/180 );
		
		kps = [kps; [x_, y_]];
	end
	
	kps = int16(round(kps));
	
	ntr = ntr + 1;
	imwrite( im,  fullfile('iris', 'trainingImages', [num2str(ntr) '.jpg'] ) );
	
	for ip = 1:size(kps, 1)
		fprintf( f_kp_tr, '%d ', kps(ip, 1) );
	end
	for ip = 1:size(kps, 1)
		fprintf( f_kp_tr, '%d ', kps(ip, 2) );
	end
	fprintf( f_kp_tr, '\n' );
	
	fprintf( f_bb_tr, '%d %d %d %d\n', 0, 0, size(im,2)-1, size(im,1)-1 );
			
	
end




        

%fclose(f_bb_te);
fclose(f_bb_tr);
fclose(f_kp_tr);

