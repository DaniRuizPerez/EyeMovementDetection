Eye Movement Detection
============

This was the final project for the Computer Vision course in my senior year of my undergrad in computer science at UDC (Spain).


## Input
It takes the input directly from the webcam or from a video file.


## Algorithm explained

First approaches used Sobel and Hough, but they turned out to be very slow. So finally, it uses a cascade detector to dected the face and just focus on that portion of the image and from then on it disregards the rest of the image.
Then it uses a cascade detector to detect the eye, which is very quick because of the size of the new input. Then it further reduces the size of the window of the eye based on empiric optimization, to get the an image of the eye as small as possible. Then we apply a dynamic threshold to convert it to black and white and divide the eye horizontally in 3 parts. After that and based on the ratio of the number of pixels lightened up in the three regions we can know where the person is looking and even if he/she is blinking (by sudden changes in the intensity).

## Further explanation
You can take a look at the project report (in Spanish) [here](https://github.com/DaniRuizPerez/EyeMovementDetection/Report.pdf).
The code is in the main.cpp file.

## Demonstration
A proof of the algorithm working can be viewed [here](https://github.com/DaniRuizPerez/EyeMovementDetection/DEMONSTRATION.ogv).

## Tools

The project was developed with the following tools

- C++
- Netbeans


## Contact

Contact [Daniel Ruiz Perez](mailto:druiz072@fiu.edu) for requests, bug reports and Rick and Morty jokes.

### Maintainers

- [Daniel Ruiz Perez](mailto:druiz072@fiu.edu).


## License

The software in this repository is available under the [GNU General Public License, version 3](https://github.com/DaniRuizPerez/EyeMovementDetection/LICENSE).  See the [LICENSE](https://github.com/DaniRuizPerez/EyeMovementDetection/LICENSE) file for more information.
