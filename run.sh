#CURRENT_DIR=$PWD
TRACK_INPUTDIR=$PWD/track_input
ISAR_INPUTDIR=$PWD/ISARimaging_input
ISAR_OUTPUTDIR=$PWD/ISARimaging_output
MASK_OUTPUTDIR=$PWD/mask_rcnn_output
FCN_OUTPUTDIR_OPT=$PWD/fcn_output/opt
FCN_OUTPUTDIR_RADAR=$PWD/fcn_output/radar
MASK_RCNN_OFFLINE_MODEL_DIR="./mask_rcnn_input/models/offline_model_mlu270"
FCN_RCNN_OFFLINE_MODEL_DIR="./fcn_input/fcn_offlinemodel"
mask_offlinemodel=$MASK_RCNN_OFFLINE_MODEL_DIR/1.cambricon
fcn_offlinemodel_opt=$FCN_RCNN_OFFLINE_MODEL_DIR/fcn-opt-270.cambricon
fcn_offlinemodel_radar=$FCN_RCNN_OFFLINE_MODEL_DIR/fcn-radar-270.cambricon
COCO_PATH="./mask_rcnn_input/dataset"
FCN_DATASET_PATH="./fcn_input/dataset"
MASK_FILE_LIST="./mask_rcnn_input/file_list"
FCN_FILE_LIST_OPT="./fcn_input/file_list_opt"
FCN_FILE_LIST_RADAR="./fcn_input/file_list_radar"
MASK_LABEL_MAP="./mask_rcnn_input/label_map.txt"
#if [ ! -d ${OUTPUT_DIR} ]; then
#    echo "mkdir build folder"
#    mkdir ${OUTPUT_DIR}
#fi

./build/main -isar_output_dir ${ISAR_OUTPUTDIR} -isar_data ${ISAR_INPUTDIR}/data.csv -isar_posxy ${ISAR_INPUTDIR}/posxy.csv -track_data ${TRACK_INPUTDIR}/data1.csv -track_dataref ${TRACK_INPUTDIR}/data_ref.csv -track_targettocamera ${TRACK_INPUTDIR}/TargetToCamera01s.csv \
-mask_offlinemodel $mask_offlinemodel \
-mask_threads 1 \
-mask_images $MASK_FILE_LIST \
-mask_labels $MASK_LABEL_MAP \
-mask_dataset_path $COCO_PATH \
-mask_outputdir $MASK_OUTPUTDIR \
-mask_mludevice 0 \
-mask_confidence 0.0 \
-fcn_offlinemodel $fcn_offlinemodel_radar \
-fcn_rgb 1 \
-fcn_threads 1 \
-fcn_dataset_path $FCN_DATASET_PATH \
-fcn_images $FCN_FILE_LIST_RADAR \
-fcn_outputdir $FCN_OUTPUTDIR_RADAR \
