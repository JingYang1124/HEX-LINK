# 根据特征完成分类

----------
> 如果数学公式无法正常显示，请下载至本地使用Typora打开或者在Chrome上安装Mathjax插件  
## 引言

通过信息增益我们可以选择出最能够帮我们实现分类的特征。而知道了要用哪些特征之后，我们可以通过特征将各类样本划分在不同的区域并完成分类。  
例如对于如下三类样本点，使用GyY以及GyZ能够十分明显地将其划分为三类：  
![](https://jingyang1124.oss-cn-shanghai.aliyuncs.com/img/hex_link/scatters_fit/scatters_fitType3.png)  


## 椭圆拟合后进行类型的判断

对于上图的数据，我们可以找出3个椭圆，分别包含每一组样本。
![](https://jingyang1124.oss-cn-shanghai.aliyuncs.com/img/hex_link/scatters_fit/scatters_fitType3_Ellipse.png)  

我们把GyY作为x轴，GyZ作为y轴，则对于椭圆方程：  
$$ f(x,y)=\frac{[(x-x_0)\times\cos(\theta)+(y-y_0)\times\sin(\theta)]^2}{a^2}+\frac{[-(x-x_0)\times\sin(\theta)+(y-y_0)\times\cos(\theta)]^2}{b^2}=1$$  

![](https://jingyang1124.oss-cn-shanghai.aliyuncs.com/img/hex_link/scatters_fit/scatters_fitEllipse.png)  

> 其中(x0, y0）代表椭圆中心点的位置a、b为长短轴的半径,theta代表了中心旋转的角度  

**则f(GyY,GyZ)<1为椭圆内的点，f(GyY,GyZ)>1为椭圆外的点。**


## 利用OpenCV求出椭圆方程

利用OpenCV的cv2.fitEllipse() 函数可以帮我们快速地计算一组数据的椭圆拟合参数：  


```python
feature_data = np.array(feature_data).reshape(1,len(sample_num),1,feature_num)

for cnt in feature_data:
  ellipse = cv2.fitEllipse(cnt)
  print("The ellipse's parameters:")
  print(ellipse)
```

Print result：  

	The ellipse parameter:
	((81.27285766601562, 141.21487426757812), (6.844329833984375, 12.807960510253906), 109.20125579833984)  

> cv2.fitEllipse()返回值：ellipse = [ (x0, y0) , (2a, 2b), theta]  
> 注意，feature_data的shape需要是(1, 样本数量, 1, 特征数量)
> 例如5个样本，两个特征如下：  
>
> ```python
> [[[[ 82 146]]  
>   [[ 85 145]]  
>   [[ 82 143]]  
>   [[ 78 143]]  
>   [[ 80 143]]]]
> ```

