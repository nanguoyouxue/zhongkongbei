import tensorflow as tf#导入tensorflow神经网络
import numpy as np#导入科学计算库函数
import matplotlib.pyplot as plt#导入可视化绘图库
seed=2#随机种子是2

STEPS=40000#对数据进行40000次训练
BATCH_SIZE=30#一次训练所用的样本数为30
LEARNING_RATE_BASE=0.001#学习率初始值
LEARNING_RATE_DECAY=0.999#学习率衰减率
REGULARIZER=0.01#正则化系数

x=tf.placeholder(tf.float32,shape=(None,2))#为输入点集占位
y_=tf.placeholder(tf.float32,shape=(None,1))#为输入的标准答案占位

rdm=np.random.RandomState(seed)#基于seed产生正态分布随机数，数据主要集中在0附近
X=rdm.randn(300,2)#X保存了300个数据点，作为输入集
Y_=[int(x0*x0+x1*x1<2) for (x0,x1) in X]#Y_保存这个点是否在圆内，若是，则为1
Y_c=[['red' if y else 'blue'] for y in Y_]#Y_c保存这个点的颜色，为可视化做准备

X=np.vstack(X).reshape(-1,2)#整理产生的数据集使之增加一个维度
Y_=np.vstack(Y_).reshape(-1,1)#同上

w1=tf.Variable(tf.random_normal([2,11]),dtype=tf.float32)#先随机出一个权重
tf.add_to_collection('losses',tf.contrib.layers.l2_regularizer(REGULARIZER)(w1))#将每一个权重的正则化损失放入总损失中
b1=tf.Variable(tf.constant(0.01,shape=[11]))#这个偏置项貌似是是固定的（真实处理过程可以将偏置项进行衰减之类的处理）
w2=tf.Variable(tf.random_normal([11,1]),dtype=tf.float32)#先随机出一个权重
tf.add_to_collection('losses',tf.contrib.layers.l2_regularizer(REGULARIZER)(w2))#将每一个权重的正则化损失放入总损失中
b2=tf.Variable(tf.constant(0.01,shape=[1]))#同上

y1=tf.nn.relu(tf.matmul(x,w1)+b1)#relu激活函数进行第1步（隐藏层）计算，注意激活函数不能用于输出层
y=tf.matmul(y1,w2)+b2#y保存神经网络生成的输出值

global_step=tf.Variable(0,trainable=False)#这是用来记录训练进行的次数，为调整学习率做准备

learning_rate=tf.train.exponential_decay(
        LEARNING_RATE_BASE,
        global_step,
        300/BATCH_SIZE,#每300/BATCH_SIZE次训练更新一次学习率
        LEARNING_RATE_DECAY,
        staircase=True)#learing_rate是实时的学习率（指数衰减学习率）

loss_mse=tf.reduce_mean(tf.square(y-y_))#定义损失函数，这里使用常用的均方误差
loss_total=loss_mse+tf.add_n(tf.get_collection('losses'))#加上正则化的修正得到最后的损失函数

train_step=tf.train.AdamOptimizer(learning_rate).minimize(loss_total)#定义反向传播函数

with tf.Session() as sess:
    init_op=tf.global_variables_initializer()
    sess.run(init_op)#初始化所有变量
    for i in range (STEPS):#循环训练40000次
        start=(i*BATCH_SIZE)%300
        end=start+BATCH_SIZE#一共300个训练点，每次训练30个，并且反复循环
        sess.run(train_step,feed_dict={x:X[start:end],y_:Y_[start:end]})#将训练集X和它们的标准答案Y_喂入神经网络
        if i%2000==0:
            loss_v=sess.run(loss_total,feed_dict={x:X,y_:Y_})#算出实时的损失函数值
            print("After %d steps,loss is: %f"%(i,loss_v))#每2000轮训练就输出一次结果
        
    xx,yy=np.mgrid[-3:3:.01,-3:3:.01]#生成x轴y轴区间都在-3到3之间，并且以0.01为间距的网格点
    grid=np.c_[xx.ravel(),yy.ravel()]#生成坐标集
    probs=sess.run(y,feed_dict={x:grid})#把grid喂入神经网络，并输出probs
    probs=probs.reshape(xx.shape)#整理形状
        
plt.scatter(X[:,0],X[:,1],c=np.squeeze(Y_c))#画出离散点
plt.contour(xx,yy,probs,levels=[.5])#画出两种离散点的训练边界（probs值为0.5）
plt.show()#显示图像

        






