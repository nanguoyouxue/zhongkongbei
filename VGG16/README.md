# 视觉识别采用Google推出的slim框架，经过微调完成。
by czl

2019/03/15
## python3.6.8+TensorFlow1.13.1实现
> * ### 准备工作
> * [点击下载本项目](https://minhaskamal.github.io/DownGit/#/home?url=https://github.com/nanguoyouxue/zhongkongbei/tree/master/VGG16)
> * 解压至C盘以外的任何磁盘的一级目录下
--------
> * ### 制作数据集
> * 将训练集的图片放到`pic/train`文件夹下,同时用每个物件的名字命名，每个训练集的图片数应保持一致
> * 将测试集的图片放到`pic/validation`文件夹下,操作同上
> * **图片应为`jpg`格式**
> * 运行python文件`tfrecord_main.py`
> * 你将在`pic`文件夹中发现多出了5个文件，包括一个叫做`label.txt`的txt文件
> * 数据集制作完毕
-------
> * ### 修改登记信息
> * 打开`slim/datasets/satellite.py`，修改：
```python
SPLITS_TO_SIZES = {'train':1200（训练集的图片总数）,'validation':200（测试集的图片总数）}
_NUM_CLASSES = 2（这个是指你一共要识别多少类物体）
```
--------
> * ### 准备训练文件夹
> * 将`pic`文件夹下面制作出的5个文件复制到`slim/satellite/data`
> * [点击下载预训练模型](http://download.tensorflow.org/models/inception_v3_2016_08_28.tar.gz)
> * 解压并将解压得到的ckpt文件放入`satellite/pretrained`
--------
> * ### 开始训练
> * 运行`train_image_classifier.py`
> * 训练一段时间，每隔5分钟会在`train_dir`中保存一个训练好的模型
> * 模型文件名称为model.ckpt-x，x为训练的步数
> * 如果下次进行训练时程序发现`train_dir`中已有模型，就会自动从这个模型开始训练,所以要按需删除
--------
> * ### 验证准确率
> * 训练完模型，自然要看看训练后的识别准确率，运行`eval_image_classifier.py`
> * 在输出中，你应该会看到一行
```C
eval/Accuracy[1(有可能是其他数)]
```
> * []中的数字就是准确率了，1代表100%
--------
> * ### 导出模型
> * 运行`exprot_inference_graph.py`，在`satellite`中生成`inception_v3_inf_graph.pb`文件
> * 修改`freeze_graph.py`参数：
```python
parser.add_argument(
      "--input_checkpoint",
      type=str,
      default="slim/satellite/train_dir/model.ckpt-76",
      help="TensorFlow variables file to load.")
```
> * 其中ckpt-x的x指你所要用的`train_dir`中的模型名称中的编号
> * 运行`freeze_graph.py`在`satellite`中生成`frozen_graph.pb`文件
--------
> * ### 开始识别
> * 修改`classify_image_inception_v3.py`参数：
```python
parser.add_argument(
      '--image_file',
      type=str,
      default='3.jpg',
      help='Absolute path to image file.'
  )
```
> * *default*就是指待识别文件的默认名称，将这个`3.jpg`改为你要识别的文件名称
> * 运行约10s后输出（下面为输出示例）：
```
hongniu(score = 0.76852)
xuehua(score = -0.12522)
```
## 注意：路径中不能有中文，否则可能会导致程序报错
## 注意：由于源代码年久失修，对高版本的TensorFlow和python可能不兼容
## 参考：《21个项目玩转深度学习》何之源
[GitHub主页传送门](https://github.com/hzy46/Deep-Learning-21-Examples/tree/master/chapter_3)
## 参考：Google TensorFlow开源项目
[GitHub主页传送门](https://github.com/tensorflow/models/tree/master/research/slim)
## 拓展阅读
> [基于深度学习的手部实时捕捉](https://victordibia.github.io/handtrack.js/#/)

> [体验Google Cloud云端图像识别](https://cloud.google.com/vision/)
