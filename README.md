# GDAL_S57_demo
C++ GDAL读取S57图层示例  

## 编译运行

更改`src/main.cpp`中的pszFilename为实际海图文件位置(.000)  

使用以下指令编译运行（假设使用g++，使用IDE、make、cmake请自行配置）：  
```bash
g++ src/main.cpp -o gdal_demo -lgdal
./gdal_demo
```

程序应在检查文件是否包含`LNDARE`图层后，遍历该图层所有多边形几何要素，并对多边形外轮廓包含的所有点依次进行输出  

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