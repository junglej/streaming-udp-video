#include "video/video_frame.h"

#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <iostream>


#include "opencv4/opencv2/core/core.hpp"
#include "opencv4/opencv2/highgui/highgui.hpp"
#include "opencv4/opencv2/imgproc.hpp"

namespace udp_streaming_video {
namespace {

  // The name of the window in which all frames will be displayed. If a new frame
  // is displayed, it will replace the previous frame displayed in that window.
  static const std::string kWindowName = "Streaming Video";

  // Delays thread execution for this amount of time after a frame is displayed.
  // This prevents the window from being refreshed too often, which can cause
  // display issues.
  constexpr int kDisplayDelayTimeMS = 15;

  int frameNumber = 0;

  // JPEG compression values.
  static const std::string kJPEGExtension = ".jpg";
  constexpr int kJPEGQuality = 50;

  }  // namespace

  VideoFrame::VideoFrame(const std::vector<unsigned char> frame_bytes) {
    frame_image_ = cv::imdecode(frame_bytes, cv::IMREAD_COLOR);
  }

  void VideoFrame::Display() const {
    // Do nothing for empty images.
    if (frame_image_.empty()) {
      return;
    }

    // std::time_t currentTime = std::time(nullptr);
    std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());//获取当前时间点
    std::time_t timestamp =  tp.time_since_epoch().count();
    std::string frameText = "Frame: " + std::to_string(frameNumber)+ "Time: " + std::to_string(timestamp);
    // cv::putText(frame_image_, frameText, cv::Point(10, 30),cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0, 0, 0), 1);
    std::cout << "Frame number is " << frameNumber << std::endl;
    std::string savingName = "/home/ad/Documents/ServerVideoFrameLog/Output_Server_" + std::to_string(frameNumber)+ "_" + std::to_string(timestamp) + ".jpg";
    std::cout << savingName << std::endl;
    frameNumber++;
    cv::imwrite(savingName, frame_image_);  // Save the frame with text
    
    cv::imshow(kWindowName, frame_image_);
    cv::waitKey(kDisplayDelayTimeMS);
  }


  std::vector<unsigned char> VideoFrame::GetJPEG() const {
    const std::vector<int> compression_params = {
        cv::IMWRITE_JPEG_QUALITY,
        kJPEGQuality
    };
    std::vector<unsigned char> data_buffer;
    cv::imencode(kJPEGExtension, frame_image_, data_buffer, compression_params);
    return data_buffer;
  }

}  // namespace udp_streaming_video
