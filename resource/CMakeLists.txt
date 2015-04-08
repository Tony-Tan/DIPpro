PROJECT(DIPdev)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
    endif(COMMAND cmake_policy)
FIND_PACKAGE(OpenCV REQUIRED)

ADD_EXECUTABLE(DIPdev Adaboost.c
Adaboost.h
Color.c
Color.h
ColorProcess.c
ColorProcess.h
Contrast.c
Contrast.h
Convolution.c
Convolution.h
DataOperation.c
DataOperation.h
FFT_1D.c
FFT_2D.c
Filter.c
Filter.h
Fourer.h
GrayMorphology.c
GrayMorphology.h
Highpassfilter.c
Highpassfilter.h
Histogram.c
Histogram.h
Homomorphicfilter.c
Homomorphicfilter.h
Hough.c
Hough.h
Image_FFT.c
Image_FFT.h
License.txt
Lowpassfilter.c
Lowpassfilter.h
Mathematic.c
Mathematic.h
Morphology.c
Morphology.h
PCA.c
PCA.h
Pseudo_Color.c
Pseudo_Color.h
RegionSegment.c
RegionSegment.h
Resize.c
Resize.h
SIFT.c
SIFT.h
SURF.c
SURF.h
Segment.c
Segment.h
Sharpen.c
Sharpen.h
Smooth.c
Smooth.h
Threshold.c
Threshold.h
Watershed.c
Watershed.h
main.c)


TARGET_LINK_LIBRARIES(DIPdev ${OpenCV_LIBS})