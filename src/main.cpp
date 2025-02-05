#include <gdal/ogrsf_frmts.h>
#include <iostream>

int main()
{
    // 注册所有的驱动
    GDALAllRegister();

    // 打开S-57文件
    const char* pszFilename = "CN201301.000"; // 此处为S-57文件路径
    GDALDataset* poDS = (GDALDataset*)GDALOpenEx(pszFilename, GDAL_OF_VECTOR, NULL, NULL, NULL);
    if (poDS == nullptr)
    {
        std::cerr << "无法打开文件: " << pszFilename << std::endl;
        return 1;
    }
    std::cout << "打开" << pszFilename << std::endl;

    // 获取LNDARE图层
    OGRLayer* poLayer = poDS->GetLayerByName("LNDARE");
    if (poLayer == nullptr)
    {
        std::cerr << "无法找到LNDARE图层" << std::endl;
        GDALClose(poDS);
        return 1;
    }
    std::cout << "文件包含LNDARE图层" << std::endl;

    // 遍历图层中的要素
    OGRFeature* poFeature;
    poLayer->ResetReading();
    while ((poFeature = poLayer->GetNextFeature()) != nullptr)
    {
        OGRGeometry* poGeometry = poFeature->GetGeometryRef();
        if (poGeometry != nullptr && wkbFlatten(poGeometry->getGeometryType()) == wkbPolygon)
        {
            OGRPolygon* poPolygon = (OGRPolygon*)poGeometry;
            OGRLinearRing* poExteriorRing = poPolygon->getExteriorRing();
            if (poExteriorRing != nullptr)
            {
                std::cout << "发现多边形, 外环坐标：" << std::endl;
                for (int i = 0; i < poExteriorRing->getNumPoints(); i++)
                {
                    OGRPoint oPoint;
                    poExteriorRing->getPoint(i, &oPoint);
                    std::cout << "(" << oPoint.getX() << ", " << oPoint.getY() << "), ";
                }
            }
            std::cout << std::endl;
        }
        else
        {
            if (poGeometry != nullptr)
            {
                const char* pszGeometryTypeName = OGRGeometryTypeToName(wkbFlatten(poGeometry->getGeometryType()));
                std::cout << "要素几何类型不是多边形，而是: " << pszGeometryTypeName << std::endl;
            }
        }
        // 解除内存占用
        OGRFeature::DestroyFeature(poFeature);
    }
    // 关闭数据集
    GDALClose(poDS);

    return 0;
}
