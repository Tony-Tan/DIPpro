PROJECT(DIPtest)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
    endif(COMMAND cmake_policy)
FIND_PACKAGE(OpenCV REQUIRED)

ADD_EXECUTABLE(DIPtest Contrast.c Contrast.h FFT_1D.c FFT_2D.c Fourer.h Image_FFT.c Image_FFT.h main.c fr.h adaboost.c adaboost.h Integral_Image.h Integral_Image.c lowpassfilter.c lowpassfilter.h highpassfilter.h highpassfilter.c convolution.c convolution.h filter.c filter.h Homomorphicfilter.c Homomorphicfilter.h GrayMorphology.c
GrayMorphology.h morphology.c morphology.h smooth.h smooth.c)


TARGET_LINK_LIBRARIES(DIPtest ${OpenCV_LIBS})