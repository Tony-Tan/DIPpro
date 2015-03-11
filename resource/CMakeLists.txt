PROJECT(DIPdev)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
    endif(COMMAND cmake_policy)
FIND_PACKAGE(OpenCV REQUIRED)

ADD_EXECUTABLE(DIPdev Contrast.c Contrast.h FFT_1D.c FFT_2D.c Fourer.h Image_FFT.c Image_FFT.h main.c adaboost.c adaboost.h Integral_Image.h Integral_Image.c lowpassfilter.c lowpassfilter.h highpassfilter.h highpassfilter.c convolution.c convolution.h filter.c filter.h Homomorphicfilter.c Homomorphicfilter.h GrayMorphology.c
GrayMorphology.h morphology.c morphology.h smooth.h smooth.c sharpen.h sharpen.c Histogram.c Histogram.h Segment.c Segment.h mathematic.c mathematic.h Hough.c Hough.h Threshold.c Threshold.h watershed.h watershed.c regionSegment.c regionSegment.h color.h color.c)


TARGET_LINK_LIBRARIES(DIPdev ${OpenCV_LIBS})