# 软渲染器

## 说明

软渲染器加载 `obj` 格式模型文件，`tga` 格式纹理贴图。

在对渲染管线的模拟过程中，模型顶点数据经过模型（Model），视图（View），透视投影（Perspective Projection），视口（Viewport）变换及三角形光栅化的处理后，由片元着色器对片元进行着色处理。着色过程有纹理映射，使用 Phong 光照模型，最终生成一帧图像。

生成的结果图像保存在 `./output/framebuffer.tga` 文件中，目前渲染结果如下图所示。

<div align="center">
  <img src="./Images/01.png" width="300"/>
</div>

项目中使用的模型是 [tinyrender](https://github.com/ssloy/tinyrenderer) 里面的开源模型。

## 构建

```bash
git clone https://github.com/dxm3dp/SoftRender.git
cd SoftRender
mkdir build
cd build
cmake ..
cmake --build .
```

## 运行

```bash
cd SoftRender
```

渲染默认模型（Diablo）：

```bash
./build/SoftRender
```

## 参考资料

项目

- [tinyrenderer](https://github.com/ssloy/tinyrenderer)

课程

- [【GAMES101-现代计算机图形学入门】](https://www.bilibili.com/video/BV1X7411F744/?share_source=copy_web&vd_source=e1b8baee842192a0e6b2b7d9ef8e10ef)

书籍

- 《3D Graphics for Game Programming》
- 《Real-Time Rendering Fourth Edition》
