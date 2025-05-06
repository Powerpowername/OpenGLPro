# OpenGLPro
 learnOpenGL网站项目

 unite1  创建glfw窗口
 
 unite2  理解VAO，VBO，EBO
 
 unite3  使用uniform 从CPU传数据至GPU实现图像实时变色
 
 unite4  使用stb_image加载图像纹理，并使用纹理给图像上色
 
 unite5  封装着色器程序类Shader,学习纹理环绕方式、纹理过滤、多级渐近纹理
 
 unite6  建立摄像机类Camera

 unite7 无材质的普通光照

 unite8 使用Materia,Light结构体，光的参数由cpu端传入
 
 unite9  建立普通光照，一个固定位置的点光源，和一个通过材质纹理建立的物体
 
 unite10 平行光照射的情况
 
 unite11 点光源照射情况、光强衰减、边缘光柔和

 unite12 多光源照射情况

 unite13 深度测试

 unite14 模板测试

 unite15 颜色混合

 unite16 面剔除

 unite17 自定义帧缓冲

 unite18 天空盒子，将摄像头的摆动引起的镜面反射的变化引入

 unite19 uniform块布局

 unite20 几何着色器

 unite21 实例化学习，将位置数据通过定点属性传入到GPU，避免以前画一个传一个的操作（多次从CPU传到GPU）

 unite22 离屏多重采样抗锯齿

 unite23 Bling——Phong式光照模型gamma校正

 unite24 平行光阴影

 unite25 利用Assimp导入模型，使用GCC编译器，在cmake构建工程时要注意libassimp-5.dll的查找，只找libassimp.dll.a是不够的，libassimp.dll.a文件只是在链接阶段的时候告诉程序该链接什么函数，但是程序执行的时候如果没有libassimp-5.dll是不可以运行的，也就是说，链接的其实是动态库，最简单的解决办法是把libassimp-5.dll放在，执行文件的目录下，或者使用cmake的add_custom_command（学习）
 
 unite26 法线贴图
 
 unite27 陡峭视差贴图
 
 unite28 泛光

 unite29 延迟着色
