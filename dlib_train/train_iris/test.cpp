#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/svm_threaded.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/data_io.h>
#include <iostream>
#include <fstream>
#include <typeinfo>
using namespace dlib;
using namespace std;

// ----------------------------------------------------------------------------------------

int main(int argc, char** argv)
{  
    try
    {
        // This example takes in a shape model file and then a list of images to
        // process.  We will take these filenames in as command line arguments.
        // Dlib comes with example images in the examples/faces folder so give
        // those as arguments to this program.
        if (argc == 1)
        {
            cout << "Call this program like this:" << endl;
            cout << "./face_landmark_detection_ex shape_predictor_68_face_landmarks.dat faces/*.jpg" << endl;
            cout << "\nYou can get the shape_predictor_68_face_landmarks.dat file from:\n";
            cout << "http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
            return 0;
        }

        // We need a face detector.  We will use this to get bounding boxes for
        // each face in an image.
        //frontal_face_detector detector = get_frontal_face_detector();
		typedef scan_fhog_pyramid<pyramid_down<6> > image_scanner_type;
		object_detector<image_scanner_type> detector;
        deserialize("../train_eye/object_detector.svm") >> detector;
        // And we also need a shape_predictor.  This is the tool that will predict face
        // landmark positions given an image and face bounding box.  Here we are just
        // loading the model from the shape_predictor_68_face_landmarks.dat file you gave
        // as a command line argument.
        shape_predictor sp;
        deserialize(argv[1]) >> sp;
        
		//dlib::array<array2d<unsigned char> > images_train;
        //std::vector<std::vector<rectangle> > face_boxes_train;
		//load_image_dataset(images_train, face_boxes_train, "/home/ckp/irisRecognition/iris-data/traindata/output.xml");

        image_window win, win_faces;
        // Loop over all the images provided on the command line.
        for (int i = 2; i < argc; ++i)
		//for(int i=0;i<images_train.size();i++)
        {
            cout << "processing image " << argv[i] << endl;
            array2d<rgb_pixel> img;
            load_image(img, argv[i]);
            // Make the image larger so we can detect small faces.
            //pyramid_up(img);

            // Now tell the face detector to give us a list of bounding boxes
            // around all the faces in the image.
            std::vector<rectangle> dets = detector(img);//images_train[i]);
            cout << "Number of faces detected: " << dets.size() << endl;
            if(dets.size()==0){
		      dlib::rectangle det=dlib::rectangle(10,420,757,860);
		      dets.push_back(det);
			}
            // Now we will go ask the shape_predictor to tell us the pose of
            // each face we detected.
            std::vector<full_object_detection> shapes;
            for (unsigned long j = 0; j < dets.size(); ++j)
			//for (unsigned long j = 0; j < face_boxes_train[i].size(); ++j)
            {
                //full_object_detection shape = sp(images_train[i], face_boxes_train[i][j]);
				full_object_detection shape = sp(img, dets[j]);
                cout << "number of parts: "<< shape.num_parts() << endl;
				for(int p=0;p<shape.num_parts();p++){
                //cout << p<<":"<< shape.part(p) << endl;
                //cout << "pixel position of second part: " << shape.part(1) << endl;
				}
				//cout <<"x:"<<shape.part(0)(0)<<endl;
				//cout <<"y:"<<shape.part(0)(1)<<endl;
				//cout<<shape.part(0)(0)+1<<endl;
				
                // You get the idea, you can get all the face part locations if
                // you want them.  Here we just store them in shapes so we can
                // put them on the screen.
                shapes.push_back(shape);
            }

            // Now let's view our face poses on the screen.
            win.clear_overlay();
            win.set_image(img);
			win.add_overlay(dets, rgb_pixel(255,0,0));
            win.add_overlay(render_face_detections(shapes));

            // We can also extract copies of each face that are cropped, rotated upright,
            // and scaled to a standard size as shown here:
            //dlib::array<array2d<rgb_pixel> > face_chips;
            //extract_image_chips(img, get_face_chip_details(shapes), face_chips);
            //win_faces.set_image(tile_images(face_chips));

            cout << "Hit enter to process the next image..." << endl;
            cin.get();
        }
    }
    catch (exception& e)
    {
        cout << "\nexception thrown!" << endl;
        cout << e.what() << endl;
    }
}
