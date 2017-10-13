Eye Movement Detection
============

This was the final project for the Computer Vision course in my senior year of my undergrad in computer science at UDC (Spain).


## Input
It takes the input directly from the webcam or from a video file.


## Algorithm explained

First approaches used Sobel and Hough, but they turned out to be very slow. So finally, it uses a cascade detector to detected the face and just focus on that portion of the image and from then on it disregards the rest of the image.
Then it uses a cascade detector to detect the eye, which is very quick because of the size of the new input. Then it further reduces the size of the window of the eye based on empiric optimization, to get the image of the eye as small as possible. Then we apply a dynamic threshold to convert it to black and white and divide the eye horizontally in 3 parts. After that and based on the ratio of the number of pixels lightened up in the three regions we can know where the person is looking and even if he/she is blinking (by sudden changes in the intensity).

## Further explanation
You can take a look at the project report (in Spanish) [here](https://github.com/DaniRuizPerez/EyeMovementDetection/blob/master/Report.pdf).
The code is in the main.cpp file.

## Demonstration
A proof of the algorithm working can be viewed [here](https://github.com/DaniRuizPerez/EyeMovementDetection/demonstration/blob/master/DEMONSTRATION.ogv).

A snapshot of the video:
<img src="https://github.com/DaniRuizPerez/EyeMovementDetection/blob/master/demonstration/left.PNG" width="200">
<img src="https://github.com/DaniRuizPerez/EyeMovementDetection/blob/master/demonstration/right.PNG" width="200">
<img src="https://github.com/DaniRuizPerez/EyeMovementDetection/blob/master/demonstration/front.PNG" width="200">
<img src="https://github.com/DaniRuizPerez/EyeMovementDetection/blob/master/demonstration/blink.PNG" width="200">

## Tools

The project was developed with the following tools

- C++
- Netbeans


## Contact

Contact [Daniel Ruiz Perez](mailto:druiz072@fiu.edu) for requests, bug reports and good jokes.


## License

The software in this repository is available under the GNU General Public License, version 3. See the [LICENSE](https://github.com/DaniRuizPerez/EyeMovementDetection/blob/master/LICENSE) file for more information.
