# GDAL_S57_demo
C++ GDAL读取S57图层示例  

## 编译运行

更改`src/main.cpp`中的pszFilename为实际海图文件位置(.000)  
`src/main_shp.cpp`中则为实际shp文件位置  

使用以下指令编译运行（假设使用g++，使用IDE、make、cmake请自行配置）：  
```bash
g++ src/main.cpp -o gdal_demo -lgdal
#g++ src/main_shp.cpp -o gdal_demo -lgdal
./gdal_demo
```

程序应在检查文件是否包含`LNDARE`图层后，遍历该图层所有多边形几何要素，并对多边形外轮廓包含的所有点依次进行输出  

若为shp文件，则输出第一个图层信息（shp一般仅有单个图层），此外shp文件的demo支持**Multipolygon**数据  

## 编译环境配置

### Ubuntu

apt-get安装GDAL即可：  

```bash
sudo apt-get update
sudo apt-get install gdal-bin libgdal-dev
```

### Windows

**思路1：**  

安装OSGeo4W，并在编译器内配置GDAL路径：  
https://trac.osgeo.org/osgeo4w/  

GDAL的动态库位置视安装设置而定，OSGeo4W默认安装在`C:\OSGeo4W`  

**思路2：**  

使用vspkg + Visual Studio  
vspkg是微软团队开发的跨平台包管理器，Visual Studio 2022 17.6版本后已内置vspkg。如果没有找到相关设置，参考：  
https://devblogs.microsoft.com/cppblog/vcpkg-is-now-included-with-visual-studio/  

需要注意的是，Visual Studio内置的vcpkg只能使用manifest模式，而无法使用经典模式。因为经典模式下安装新的包需要对vcpkg目录内容进行写入，而自带vcpkg位于Visual Studio的安装目录下，默认无写入权限，会导致ports安装失败。  

**Manifest模式：**  

暂未找到合适的教程  

**经典模式：**  

目前大量教程（包括MS官方）均适用vcpkg源码进行开发配置，源码地址如下:  
https://github.com/microsoft/vcpkg  

随后参考 https://gdal.org/en/stable/download.html#vcpkg，安装GDAL：  
```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh  # ./bootstrap-vcpkg.bat for Windows
./vcpkg integrate install
./vcpkg search gdal --featurepackages  # list optional features
./vcpkg install gdal  # install with default configuration
./vcpkg install gdal[poppler,netcdf]  # install with Poppler and netdf support
```

安装完成后，可以参考MS的教程对CMake项目进行配置：  
https://learn.microsoft.com/zh-cn/vcpkg/get_started/get-started-vs?pivots=shell-powershell  

## S-57海图说明

### 浮标

| 浮标图层 | 英文说明                                                     | 中文说明                                                     |
| -------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| BOYCAR   | A cardinal buoy is used in conjunction with the compass to indicate where the mariner may find the best navigable water. It is placed in one of the four quadrants (North, East, South and West), bounded by inter-cardinal bearings from the point marked. (UKHO NP 735, 5th Edition) | 基点浮标与罗盘配合使用，指示水手可以找到最佳航行水域的位置。它位于四个象限之一（北、东、南和西），以标记点之间的基点方位为界。 |
| BOYINB   | An installation buoy is a buoy used for loading tankers with gas or oil. (IHO Chart Specifications, S-4) | 安装浮标是用于油轮装载天然气或石油的浮标。                   |
| BOYISD   | A isolated danger buoy is a buoy moored on or above an isolated danger of limited extent, which has navigable water all around it. (UKHO NP735, 5th Edition) | 孤立危险浮标是系泊在有限范围的孤立危险之上或之上的浮标，（除浮标标记的位置外）其周围均为可通航水域。 |
| BOYLAT   | A lateral buoy is used to indicate the port or starboard hand side of the route to be followed. They are generally used for well defined channels and are used in conjunction with a conventional direction of buoyage. (UKHO NP 735, 5th Edition) | 侧浮标用于指示要遵循的航线的左舷或右舷。它们通常用于明确的航道，并与常规方向浮标一起使用。 |
| BOYSAW   | A safe water buoy is used to indicate that there is navigable water around the mark. (UKHO NP735, 5th Edition) | 安全水域浮标用于指示浮标周围有可通航水域。                   |
| BOYSPP   | A special purpose buoy is primarily used to indicate an area or feature, the nature of which is apparent from reference to a chart, Sailing Directions or Notices to Mariners. (UKHO NP 735, 5th Edition) | 特殊用途浮标主要用于指示某个区域或特征，其性质可通过海图、航行指南或航海通告得知。 |