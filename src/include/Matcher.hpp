#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>



class Matcher{
     public:
      enum Method {
        CV_TM_SQDIFF        =0,
        CV_TM_SQDIFF_NORMED =1,
        CV_TM_CCORR         =2,
        CV_TM_CCORR_NORMED  =3,
        CV_TM_CCOEFF        =4,
        CV_TM_CCOEFF_NORMED =5

};
// https://github.com/opencv/opencv/blob/097d81363befbce1bf922379a4ea886e5f6dbd2b/modules/core/include/opencv2/core/base.hpp#L159
    enum NormTypes {                /**
                \f[
                norm =  \forkthree
                {\|\texttt{src1}\|_{L_{\infty}} =  \max _I | \texttt{src1} (I)|}{if  \(\texttt{normType} = \texttt{NORM_INF}\) }
                {\|\texttt{src1}-\texttt{src2}\|_{L_{\infty}} =  \max _I | \texttt{src1} (I) -  \texttt{src2} (I)|}{if  \(\texttt{normType} = \texttt{NORM_INF}\) }
                {\frac{\|\texttt{src1}-\texttt{src2}\|_{L_{\infty}}    }{\|\texttt{src2}\|_{L_{\infty}} }}{if  \(\texttt{normType} = \texttt{NORM_RELATIVE | NORM_INF}\) }
                \f]
                */
                NORM_INF       = 1,
                /**
                \f[
                norm =  \forkthree
                {\| \texttt{src1} \| _{L_1} =  \sum _I | \texttt{src1} (I)|}{if  \(\texttt{normType} = \texttt{NORM_L1}\)}
                { \| \texttt{src1} - \texttt{src2} \| _{L_1} =  \sum _I | \texttt{src1} (I) -  \texttt{src2} (I)|}{if  \(\texttt{normType} = \texttt{NORM_L1}\) }
                { \frac{\|\texttt{src1}-\texttt{src2}\|_{L_1} }{\|\texttt{src2}\|_{L_1}} }{if  \(\texttt{normType} = \texttt{NORM_RELATIVE | NORM_L1}\) }
                \f]*/
                 NORM_L1        = 2,
                 /**
                 \f[
                 norm =  \forkthree
                 { \| \texttt{src1} \| _{L_2} =  \sqrt{\sum_I \texttt{src1}(I)^2} }{if  \(\texttt{normType} = \texttt{NORM_L2}\) }
                 { \| \texttt{src1} - \texttt{src2} \| _{L_2} =  \sqrt{\sum_I (\texttt{src1}(I) - \texttt{src2}(I))^2} }{if  \(\texttt{normType} = \texttt{NORM_L2}\) }
                 { \frac{\|\texttt{src1}-\texttt{src2}\|_{L_2} }{\|\texttt{src2}\|_{L_2}} }{if  \(\texttt{normType} = \texttt{NORM_RELATIVE | NORM_L2}\) }
                 \f]
                 */
                 NORM_L2        = 4,
                 /**
                 \f[
                 norm =  \forkthree
                 { \| \texttt{src1} \| _{L_2} ^{2} = \sum_I \texttt{src1}(I)^2} {if  \(\texttt{normType} = \texttt{NORM_L2SQR}\)}
                 { \| \texttt{src1} - \texttt{src2} \| _{L_2} ^{2} =  \sum_I (\texttt{src1}(I) - \texttt{src2}(I))^2 }{if  \(\texttt{normType} = \texttt{NORM_L2SQR}\) }
                 { \left(\frac{\|\texttt{src1}-\texttt{src2}\|_{L_2} }{\|\texttt{src2}\|_{L_2}}\right)^2 }{if  \(\texttt{normType} = \texttt{NORM_RELATIVE | NORM_L2SQR}\) }
                 \f]
                 */
                 NORM_L2SQR     = 5,
                 /**
                 In the case of one input array, calculates the Hamming distance of the array from zero,
                 In the case of two input arrays, calculates the Hamming distance between the arrays.
                 */
                 NORM_HAMMING   = 6,
                 /**
                 Similar to NORM_HAMMING, but in the calculation, each two bits of the input sequence will
                 be added and treated as a single bit to be used in the same calculation as NORM_HAMMING.
                 */
                 NORM_HAMMING2  = 7,
                 NORM_TYPE_MASK = 7, //!< bit-mask which can be used to separate norm type from norm flags
                 NORM_RELATIVE  = 8, //!< flag
                 NORM_MINMAX    = 32 //!< flag
        };

  
   
   
    private:
    /* https://github.com/opencv/opencv/blob/c2b6c6743175241d895fe4e67028fd960a405b00/modules/core/include/opencv2/core/mat.hpp#L791 */
        cv::Mat  _template;
        cv::Mat  image;
        cv::Mat  result;
        Method method;
        int iMethod;

    public:
        Matcher();

        // Matcher& SetTemplate(cv::Mat * other);
        // Matcher& SetImage(cv::Mat * other);
        Matcher& SetTemplate(std::string other);
        Matcher& SetImage(std::string other);
        Matcher& SetTemplate(char* other);
        Matcher& SetImage(char* other);
        Matcher& SetMethod(Matcher::Method method);

        void Process();

        ~Matcher();

    

};
