#include <iostream>
#include "Track.hpp"
#include "ISARimaging_main.hpp"
#include "fusion_measure.hpp"
#include "command_option.hpp"
#include "mask_rcnn_main.hpp"
#include "fcn_offline_multicore.hpp"

int Pattern_1 = 0;//模式1
int Pattern_2 = 0;//模式2
int Pattern_3 = 1;//模式3
int Pattern_4 = 0;//模式4

int main(int argc, char* argv[])
{
    /*
    CSV csv;
    csv.readCSV("gassap1.csv");
    std::cout << "Rows: " << csv.getRows() << std::endl;
    std::cout << "Cols: " << csv.getCols() << std::endl;
    const std::vector<std::vector<double>>& data = csv.getData();
    for (const auto& row : data) {
        for (double cell : row) {
            std::cout << cell << "\t";
        }
        std::cout << std::endl;
    }
    */
    gflags::ParseCommandLineFlags(&argc, &argv, true);//解析传入的参数
    cout<<"*********************************************************************"<<endl;
    cout<<"进入远距点目标融合定位阶段"<<endl;    
    Track();

    cout<<"*********************************************************************"<<endl;
    cout<<"进入雷达成像阶段"<<endl;    
    ISARimaging();
    
    cout<<"*********************************************************************"<<endl;
    cout<<"进入雷达图像分割阶段"<<endl;
    mask_rcnn_main(); 

    cout<<"*********************************************************************"<<endl;
    cout<<"进入融合图像分割阶段"<<endl;
    fcn_main(); 
    
    cout<<"*********************************************************************"<<endl;
    cout<<"进入融合测量阶段"<<endl;    
    Fusion_Measure();
    return 0;
}
