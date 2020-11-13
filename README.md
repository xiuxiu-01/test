1.项目需要用到 vs2017+qt5+qgis
2.vs2017 正常下载部署即可，需要下载的是sdk以及c++桌面开发 在工具->拓展与更新—>联机 搜qt visoul sdtio tolls组件
3.qt5需要下载msvc2017-64的版本，其他可以忽略.
4.在vs安装qt工具正确后的情况下重新启动vs会有一个qt vs tools组件 
点击进去选择最后面的qt options 选择add找到刚刚下载的msvc2017-64这个文件夹，
添加然后在下面的default qt/win version选择出来即可完成qt与vs的配置.
5.上述都正常后可以新建一个qt widgets application然后测试能不能运行，如果能运行就配置成功，
不能的话可以右键项目看看属性下的配置属性常规的sdk版本10.0以上以及平台工具集14.1v
再不行查看问题到网络上搜索解决方案
6.配置qgis以及他的依赖库OSGeo4W64 这里为了方便直接下载链接网络上有很多教程这里为了方便可以直接下载以及下载好的文件安装到c盘
链接
然后就是配置属性中的c/c++ 附加包含目录   C:\OSGeo4W64\include
C:\OSGeo4W64\apps\qgis-ltr\include
预处理器添加 _USE_MATH_DEFINES;
链接器->输入中附加依赖库
qgis_app.lib
qgis_core.lib
qgis_gui.lib
链接器常规中添加库目录
C:\OSGeo4W64\apps\qgis-ltr\lib
