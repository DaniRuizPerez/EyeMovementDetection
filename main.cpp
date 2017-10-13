    /* 
 * File:   sightDetection.cpp
 * Author: dani
 *
 * Created on 11 de noviembre de 2014, 13:45
 */


#include <iostream>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string>     // std::string, std::to_string
#include <sstream>

#include<opencv2/highgui/highgui.hpp>
#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>     //for using the function sleep
#include "opencv2/opencv.hpp" 

using namespace std;
using namespace cv;

// Function Headers
cv::Rect detectAndDisplay(Mat frame);
cv::Rect detectAndDisplayEye(Mat frame);
Mat sobelF(Mat frame, int height, int width);

// Global variables
//esto es un archivo propio de opencv
string face_cascade_name = "/home/daniel/Descargas/OpenCV/opencv-2.4.9/data/haarcascades/haarcascade_frontalface_alt.xml";
string eye_cascade_name = "/home/daniel/Descargas/OpenCV/opencv-2.4.9/data/haarcascades/haarcascade_mcs_lefteye.xml";
CascadeClassifier face_cascade;
CascadeClassifier eye_cascade;
string path = "/home/daniel/NetBeansProjects/SightDetection/00025.avi";
bool condition = false;
int vecesParpadeado = 0;
    // HoughCircles

int main(void)
{
    
    VideoCapture capture(path); //open the video
    //VideoCapture capture(0); // open the webcam
    if(!capture.isOpened()){ // check if we succeeded
        printf("--(!)Error loading CAMERA\n");
        return -1;
    }
    
    //Cargamos el cascade
    if (!face_cascade.load(face_cascade_name)){
        printf("--(!)Error loading\n");
        return (-1);
    };
  
    Mat frame;
    namedWindow("original",WINDOW_NORMAL);
    namedWindow("detected",WINDOW_NORMAL);
    namedWindow("black&White",WINDOW_NORMAL);
    namedWindow("umbraliced",WINDOW_NORMAL);

    cv::Rect rectF;
    cv::Rect rectE;
    cv::Rect rectEaux;
    capture >> frame;
    rectF = detectAndDisplay(frame);
    
    rectF.x = rectF.x -50;
    rectF.y = rectF.y -50;
    rectF.height = rectF.height+50;
    rectF.width = rectF.width +50;
    int firstPartEye;
    int secondPartEye;
    int whitesFirstPartEye, whitesSecondPartEye;
    Mat face;
    Mat eye;
    Mat sobeli;
    Mat sobelT;
    string direction;
     //Cargamos el cascade
    if (!eye_cascade.load(eye_cascade_name)){
        printf("--(!)Error loading\n");
        return (-1);
    };
    int i,j;
            
    face = frame(Range(rectF.y,rectF.height+rectF.y),Range(rectF.x, rectF.width+rectF.x));
    rectEaux = detectAndDisplayEye(face);
     rectEaux.x = rectEaux.x + rectEaux.width/4 + 4;
    rectEaux.y = rectEaux.y + rectEaux.height /2 +1  ;
    rectEaux.height = rectEaux.height/5 ;
    rectEaux.width = rectEaux.width *2/5 ;
    //me quedo con la region del ojo, las dimensiones seran siempre las mismas que estas
    int width = rectEaux.width;
    int height = rectEaux.height -1;
    cv:Scalar tempVal;
    float myMAtMean;
    float lastMean = 0;
            
    int a = 0;
    for (;;){
        //nos saltamos unos cuantos frames para que no se pete
        a+=1;
        if (a % 10){
            capture >> frame;
            if (!frame.empty()){
                face = frame(Range(rectF.y,rectF.height+rectF.y),Range(rectF.x, rectF.width+rectF.x));
                rectE = detectAndDisplayEye(face);
                //si el ojo detectado esta a menos de 20 pixeles del anterior, lo considero como valido
                //si no me quedo con el anterior
                if (abs(rectE.x - rectEaux.x) < 25  && abs(rectE.y - rectEaux.y) < 25){
                    rectE.x = rectE.x + rectE.width/4 + 4;
                    rectE.y = rectE.y + rectE.height /2 +1 ;
                    rectE.height = height ;
                    rectE.width = width;                         
                    rectEaux = rectE;                    
                }
                else{
                    rectE = rectEaux;
                }
          
            //me quedo con la region de la cara que es el ojo
            eye = face(Range(rectE.y,rectE.height+rectE.y),Range(rectE.x, rectE.width+rectE.x));
            //lo paso a escala de grises
            cvtColor(eye, sobeli, COLOR_BGR2GRAY);
            //pinto un rectangulo sobre la cara
            rectangle(face, rectE, CV_RGB(0, 255,0), 1);
            rectangle(frame, rectF, CV_RGB(0, 255,0), 1);
            //aplico un umbral al ojo
            
            myMAtMean = mean( sobeli ).val[0];
            if (lastMean == 0)
                lastMean = myMAtMean;

            threshold(sobeli,sobelT, myMAtMean, 255, THRESH_BINARY);              

            firstPartEye  = ceil(sobelT.cols*1/4);
            secondPartEye  = ceil(sobelT.cols*3/4);
            whitesFirstPartEye = 0;
            whitesSecondPartEye = 0;

            for (i=0;i<sobelT.rows;i++)
                for (j=0;j<firstPartEye;j++)
                   if (sobelT.at<uchar>(i,j)  == 255)
                       whitesFirstPartEye++;
            
            for (i=0;i<sobelT.rows;i++)
                for (j=secondPartEye;j<sobelT.cols;j++)
                   if (sobelT.at<uchar>(i,j)  == 255)
                       whitesSecondPartEye++;


           //si la diferencia entre los dos, es menos a la mitad de la suma de las dimensiones,
                //no considero cambio
            condition = (abs(whitesFirstPartEye - whitesSecondPartEye) < (whitesFirstPartEye + whitesSecondPartEye)/2 );

            if (condition)
                direction = "al frente";
            else{
                if (whitesFirstPartEye > whitesSecondPartEye)
                    direction = "---->";
                else 
                    direction = "<----";
            }
            
            
            //30 para cam , 10 para video 
           if ((abs(lastMean - myMAtMean) > (0.15*lastMean))and (vecesParpadeado < 10)){
                    direction = "PARPADEO";
                    vecesParpadeado ++;
            }
            else{
                lastMean = myMAtMean;
                vecesParpadeado =0;
            }  
            
            
            putText(face, direction, Point(75, 75), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);

            imshow("black&White", sobeli);
            imshow("original",frame );
            imshow("detected",face );
            imshow("umbraliced", sobelT);
    
            }
            else{
                printf(" --(!) No captured frame -- Break!");
                break;
            }
        }
        //si pulsamos ESC, salimos
        int c = waitKey(10);
        if (27 == char(c)){
            break;
        }
          if ('p' == c)
            sleep(10);
        
    }
    return 0;
}
    
    
    
    
    
    
// Function detectAndDisplay
    Rect detectAndDisplayEye(Mat frame){
    std::vector<Rect> faces;
    Mat frame_gray;
    Mat crop;
    Mat res;
    Mat gray;
    string text;
    stringstream sstm;

    // pasamos a escala de grises
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    // ecualizamos el histograma
    equalizeHist(frame_gray, frame_gray);

    // Detect faces
    eye_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

    // Set Region of Interest
    cv::Rect roi_b;
    cv::Rect roi_c;

    size_t ic = 0; // ic is index of current element
    int ac = 0; // ac is area of current element

    size_t ib = 0; // ib is index of biggest element
    int ab = 0; // ab is area of biggest element

    // Iteramos sobre todas las caras detectadas para quedarnos con la de tamaño mas grande
    //hay que pensar que esto lo hace por cada frame
    for (ic = 0; ic < faces.size(); ic++) {

        //guardamos en estas variables las caracteristicas de la cara detectada
        roi_c.x = faces[ic].x;
        roi_c.y = faces[ic].y;
        roi_c.width = (faces[ic].width);
        roi_c.height = (faces[ic].height);

        //calculamos el area del elemento actual
        ac = roi_c.width * roi_c.height; 
        
        //recalculamos las caracteristicas del elemento mas grande, en la primera iteracion es la actual
        roi_b.x = faces[ib].x;
        roi_b.y = faces[ib].y;
        roi_b.width = (faces[ib].width);
        roi_b.height = (faces[ib].height);

        // calculamos el area del elemento mas grande
        ab = roi_b.width * roi_b.height; 
        
        //si la region actual es mayor que el antiguo maximo, swap
        if (ac > ab){
            ib = ic;
            roi_b.x = faces[ib].x;
            roi_b.y = faces[ib].y;
            roi_b.width = (faces[ib].width);
            roi_b.height = (faces[ib].height);
        }

    }
    return roi_b;
}
    
      