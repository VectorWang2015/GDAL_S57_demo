#include <gdal/ogrsf_frmts.h>
#include <iostream>

int main()
{
    // 注册所有的驱动
    GDALAllRegister();

    // 打开S-57文件
    const char* pszFilename = "QuebecLand.shp"; // 此处为shp文件路径
    GDALDataset* poDS = (GDALDataset*)GDALOpenEx(pszFilename, GDAL_OF_VECTOR, NULL, NULL, NULL);
    if (poDS == nullptr)
    {
        std::cerr << "无法打开文件: " << pszFilename << std::endl;
        return 1;
    }
    std::cout << "打开" << pszFilename << std::endl;

    // shp应该只有一个图层
    OGRLayer* poLayer = poDS->GetLayer(0);
    if (poLayer == nullptr)
    {
        std::cerr << "无法找到图层" << std::endl;
        GDALClose(poDS);
        return 1;
    }
    std::cout << "找到图层" << std::endl;

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
        else if (poGeometry != nullptr && wkbFlatten(poGeometry->getGeometryType()) == wkbMultiPolygon)  // 多重 Polygon
        {
            OGRMultiPolygon* poMultiPolygon = (OGRMultiPolygon*)poGeometry;
            std::cout << "发现 MultiPolygon：" << std::endl;

            int numPolygons = poMultiPolygon->getNumGeometries();
            for (int i = 0; i < numPolygons; i++)
            {
                OGRPolygon* poPolygon = (OGRPolygon*)poMultiPolygon->getGeometryRef(i);
                std::cout << "  Polygon " << i + 1 << "：" << std::endl;

                // 解析多边形
                OGRLinearRing* poExteriorRing = poPolygon->getExteriorRing();
                if (poExteriorRing != nullptr)
                {
                    std::cout << "  外环：" << std::endl;
                    for (int j = 0; j < poExteriorRing->getNumPoints(); j++)
                    {
                        OGRPoint oPoint;
                        poExteriorRing->getPoint(j, &oPoint);
                        std::cout << "    " << oPoint.getX() << ", " << oPoint.getY() << std::endl;
                    }
                }
            }
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
