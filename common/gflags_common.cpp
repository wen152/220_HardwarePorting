#include <gflags/gflags.h>
//track参数
DEFINE_string(track_data,"","input data.csv");
DEFINE_string(track_dataref,"","input data_ref.csv");
DEFINE_string(track_targettocamera,"","input TargetToCamera01s.csv");
//isar参数
DEFINE_string(isar_output_dir,"","output result iimage");
DEFINE_string(isar_data,"","input data.csv");
DEFINE_string(isar_posxy,"","input posxy.csv");
//mask参数
DEFINE_string(mask_meanfile, "", "mean file used to subtract from the input image.");
DEFINE_string(mask_meanvalue, "",
    "If specified, can be one value or can be same as image channels"
    " - would subtract from the corresponding channel). Separated by ','."
    "Either meanfile or meanvalue should be provided, not both.");
DEFINE_int32(mask_threads, 1, "threads should "
                         "be lower than or equal to 32 ");
DEFINE_int32(mask_channel_dup, 1, "Enable const memory auto channel duplication. "
                         "Could improve performance when multithreading."
                         "Works only with apiversion 3");
DEFINE_int32(mask_simple_compile, 1, "Use simple compile interface.");
DEFINE_string(mask_images, "", "input file list");
DEFINE_string(mask_dataset_path, "", "absolute dataset path");
DEFINE_string(mask_labels, "", "label to name");
DEFINE_int32(mask_fix8, 0, "fp16(0) or fix8(1) mode. Default is fp16");
DEFINE_int32(mask_int8, -1, "invalid(-1), fp16(0) or int8(1) mode. Default is invalid(-1)."
    "If specified, use int8 value, else, use fix8 value");
DEFINE_int32(mask_yuv, 0, "bgr(0) or yuv(1) mode. Default is bgr");
DEFINE_int32(mask_rgb, 1, "bgr(0) or rgb(1) mode. Default is rgb");
DEFINE_int32(mask_interpolation, 3, "opencv resize interpolation method"
                          "INTER_NEAREST(0), \
                           INTER_LINEAR(1), \
                           INTER_CUBIC(2), \
                           INTER_AREA(3), \
                           INTER_LANCZOS4(4), \
                           INTER_LINEAR_EXACT(5), \
                           INTER_NEAREST_EXACT(6), \
                           INTER_MAX(7), \
                           WARP_FILL_OUTLIERS(8), \
                           WARP_INVERSE_MAP(16). Default is INTER_AREA");
DEFINE_double(mask_scale, 1, "scale for input data, mobilenet...");
DEFINE_string(mask_logdir, "", "path to dump log file, to terminal stderr by default");
DEFINE_int32(mask_fifosize, 2, "set FIFO size of mlu input and output buffer, default is 2");
DEFINE_string(mask_mludevice, "0",
    "set using mlu device number, set multidevice seperated by ','"
    "eg 0,1 when you use device number 0 and 1, default: 0");
DEFINE_string(mask_resize, "",
    "set preprocess resizes, set height,width seperated by ',' eg 256,256 "
    "when size is an int, the smaller edge is resize, "
    "the larger edge is (size * larger_edge / smaller_edge)");
DEFINE_int32(mask_apiversion, 2, "specify the version of CNRT to run.");
DEFINE_string(mask_functype, "1H16",
              "Specify the core to run on the arm device. "
              "Set the options to 1H16 or 1H8, the default is 1H16.");
DEFINE_int32(mask_async_copy, 0,
             "Enable async copy, 0-disable 1-enable. Default is 0-disable.");
DEFINE_int32(mask_input_format, 0, "input image channel order, default is 0(rgba)");
DEFINE_int32(mask_dim_mutable, 0,
             "nchw is not mutable(0) or nchw is mutable(1). Default is not mutable(0)");
DEFINE_int32(mask_dim_mutable_max_batch, 4,
             "when n is mutable, dim_mutable_max_batch is the maxsize of n ");
DEFINE_int32(mask_timestamp, 0, "print pipeline timestamp");
DEFINE_string(mask_offlinemodel, "", "prototxt file used to find net configuration");
//fcn参数
DEFINE_string(fcn_meanfile, "", "mean file used to subtract from the input image.");
DEFINE_string(fcn_meanvalue, "",
    "If specified, can be one value or can be same as image channels"
    " - would subtract from the corresponding channel). Separated by ','."
    "Either meanfile or meanvalue should be provided, not both.");
DEFINE_int32(fcn_threads, 1, "threads should "
                         "be lower than or equal to 32 ");
DEFINE_int32(fcn_channel_dup, 1, "Enable const memory auto channel duplication. "
                         "Could improve performance when multithreading."
                         "Works only with apiversion 3");
DEFINE_int32(fcn_simple_compile, 1, "Use simple compile interface.");
DEFINE_string(fcn_images, "", "input file list");
DEFINE_string(fcn_dataset_path, "", "absolute dataset path");
DEFINE_string(fcn_labels, "", "label to name");
DEFINE_int32(fcn_fix8, 0, "fp16(0) or fix8(1) mode. Default is fp16");
DEFINE_int32(fcn_int8, -1, "invalid(-1), fp16(0) or int8(1) mode. Default is invalid(-1)."
    "If specified, use int8 value, else, use fix8 value");
DEFINE_int32(fcn_ycbcr, 0, "bgr(0) or ycbcr(1) mode. Default is bgr");
DEFINE_int32(fcn_yuv, 0, "bgr(0) or yuv(1) mode. Default is bgr");
DEFINE_int32(fcn_rgb, 1, "bgr(0) or rgb(1) mode. Default is rgb");
DEFINE_int32(fcn_interpolation, 1, "opencv resize interpolation method"
                          "INTER_LINEAR(0) or INTER_AREA(1). Default is INTER_AREA");
DEFINE_double(fcn_scale, 1, "scale for input data, mobilenet...");
DEFINE_string(fcn_logdir, "", "path to dump log file, to terminal stderr by default");
DEFINE_int32(fcn_fifosize, 2, "set FIFO size of mlu input and output buffer, default is 2");
DEFINE_string(fcn_mludevice, "0",
    "set using mlu device number, set multidevice seperated by ','"
    "eg 0,1 when you use device number 0 and 1, default: 0");
DEFINE_string(fcn_resize, "",
    "set preprocess resizes, set height,width seperated by ',' eg 256,256 "
    "when size is an int, the smaller edge is resize, "
    "the larger edge is (size * larger_edge / smaller_edge)");
DEFINE_int32(fcn_apiversion, 2, "specify the version of CNRT to run.");
DEFINE_string(fcn_functype, "1H16",
              "Specify the core to run on the arm device. "
              "Set the options to 1H16 or 1H8, the default is 1H16.");
DEFINE_int32(fcn_async_copy, 0,
             "Enable async copy, 0-disable 1-enable. Default is 0-disable.");
DEFINE_int32(fcn_input_format, 0, "input image channel order, default is 0(rgba)");
DEFINE_int32(fcn_dim_mutable, 0,
             "nchw is not mutable(0) or nchw is mutable(1). Default is not mutable(0)");
DEFINE_int32(fcn_dim_mutable_max_batch, 4,
             "when n is mutable, dim_mutable_max_batch is the maxsize of n ");
DEFINE_int32(fcn_timestamp, 0, "print pipeline timestamp");
DEFINE_int32(fcn_preprocess_method, 0, "Use it to choose Image preprocess");
// parameters for performance mode
DEFINE_int32(fcn_perf_mode, 0, "close(0) or open(1) performance mode, Default is close(0)");
DEFINE_int32(fcn_perf_mode_img_num, 4800, "number of images for performance mode, Default is 4800");
