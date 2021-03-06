### Real-time Directional Segmentation from Kinect RGB-D Stream

[![Real-time Directional Segmentation using DDP-vMF-means](./doc/ddpVideoPreview.png)](http://www.youtube.com/watch?v=wLP18q80oAE)

This package implements real-time temporally consistent directional
segmentation from Kinect RGB-D streams. It relies on the
[dpMMlowVar](https://github.com/jstraub/dpMMlowVar)
library for the actual implementation of the DDP-vMF-means algorithm.
The approach and the algorithm is described in more detail on our
[project page](http://www.jstraub.de/small-variance-nonparametric-clustering-on-the-hypersphere/) and [CVPR 2015 paper](http://www.jstraub.de/download/straub2015ddpvmf.pdf). The algorithms are described in more detail in the [supplement](http://www.jstraub.de/download/straub2015ddpvmf_supp.pdf).
See below for install instructions.

If you use DP-vMF-means or DDP-vMF-means please cite:
```
Julian Straub, Trevor Campbell, Jonathan P. How, John W. Fisher III. 
"Small-Variance Nonparametric Clustering on the Hypersphere", In CVPR,
2015.
```

### Dependencies
This code is dependent on Eigen3, Boost, CUDA, OpenCV and PCL.
It has been tested on Ubuntu 14.04 with 
- Eigen3 (3.0.5) 
- Boost (1.54)
- CUDA (6.5)
- OpenCV (2.4)
- PCL (1.7)

### Install

This package uses [the pods build
system](http://sourceforge.net/p/pods/home/Home/). Used widely at CSAIL
MIT the build system makes it easy to break up software projects into
small packages that can be checked out and compiled automatically (see
below).

- *Linux:* 

    Install Eigen3, Boost, OpenCV, and PCL

    ```
    sudo apt-get install libeigen3-dev libboost-dev libopencv-dev libpcl-1.7-all-dev
    ```

    Install the appropriate CUDA version matching with your nvidia
    drivers. On our machines we use `nvidia-340-dev` with
    `libcuda1-340 cuda-6-5 cuda-toolkit-6-5`

    Clone this repository and compile the code:

    ```
    git clone git@github.com:jstraub/rtDDPvMF; cd rtDDPvMF;
    make checkout; make configure; make -j6; make install;
    ```
    
    Note that this will checkout several other necessary repositories.
    To update all repositories run
    
    ```
    make update; make configure; make -j6; make install;
    ```

### Getting Started

Plug in your Kinect and run the following from the rtDDPvMF folder:
```
./build/bin/realtimeDDPvMF_openni --lambdaDeg 100 
```

### Usage 

```
./build/bin/realtimeDDPvMF_openni -h
Allowed options:
  -h [ --help ]               produce help message
  -K [ --K ] arg              K for spkm clustering
  -l [ --lambdaDeg ] arg      lambda in degree for DP-vMF-means and 
                              DDP-vMF-means
  -b [ --beta ] arg           beta parameter of the DDP-vMF-means
  -s [ --nFramesSurvive ] arg number of frames a cluster survives without 
                              observation
  -o [ --out ] arg            path to output file
  -d [ --display ]            display results
  -B [ --B ] arg              filter windows size B for guided filter
  --eps arg                   epsilon parameter for guided filter
  -f [ --f_d ] arg            focal length of depth camera
``` 
