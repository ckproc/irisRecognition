#include <iostream>
#include "irisRecognition.h"
#include "time.h"

using namespace std;
using namespace cv;

int main(int argc, char **argv){
	
	if(argc < 3){
        fprintf(stderr, "usage: [iris] [database-path] [query-path]\n");
        return 0;
    }
	
	IrisRecognition IR;
	cout << "start build feature pool..." << endl;
	IR.buildFeaturePool(argv[1]);
	cout << "build feature pool end." << endl;
	
	clock_t starttime = clock();
	RetrievalResult result;
	int num = 0, correct = 0,fail=0;
	vector<String> filenames = list_all_image_in_file(argv[2]);
	for (size_t i = 0; i < filenames.size(); ++i){
		String filename = filenames[i];
		result = IR.retrievalImage(filename.c_str());
		if(result.imagePath==""){
			cout<<"No eye found"<<endl;
			fail=fail+1;
			continue;
		}
		if (isMatch(filename, result.imagePath))
			correct ++;
		num ++;
		cout << i << ":\t" << filename << "\t-> "  << "  \t" << result.imagePath << "\t"<< result.score << endl;
		cout<<endl;
	}
	clock_t endtime = clock();
	cout << "correct / num : " << correct << " / " << num << endl;
	cout << fail<<" images not found eye"<<endl;
	cout << "Retrieval time: " << (double)(endtime-starttime) / CLOCKS_PER_SEC << endl;
	
	return 0;
	
}

