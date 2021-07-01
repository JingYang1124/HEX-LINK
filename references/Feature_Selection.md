# 基于信息增益的特征选取  

----------
> 如果数学公式无法正常显示，请下载至本地使用Typora打开或者在Chrome上安装Mathjax插件  
## 引言

**信息熵**，是一个数学上颇为抽象的概念，在这里不妨把信息熵理解成某种特定信息的出现概率。信息量度量的是一个具体事件发生了所带来的信息，而熵则是在结果出来之前对可能产生的信息量的期望。  
##### 信息熵公式：
$$H(X)=-\sum^{\infty}_{i=1}{p(x_i)\log_2(p(x_i))}$$

## 计算举例

现假设有测量了两种动作运动过程中的X轴加速度平均值  
![](https://jingyang1124.oss-cn-shanghai.aliyuncs.com/img/hex_link/Features_Selection/A_B01.png)  
动作A的X轴加速度平均值范围为A1至A3  
动作B的X轴加速度平均值范围为A2至A4

**首先计算样本的的信息熵**  
$$ H(X)=(-\frac{20}{40}\times\log_2(\frac{20}{40}))+(-\frac{20}{40}\times\log_2(\frac{20}{40}))=1$$  

以A2与A3为分界点将A1与A4上的数据分为3段  
分别计算每一段下的X的熵:  
$$ H(X|A\in[A_1,A_2))=-1\times\log_2(1)+0=0$$  
$$ H(X|A\in[A_2,A_3))=(-\frac{10}{20}\times\log_2(\frac{10}{20}))+(-\frac{10}{20}\times\log_2(\frac{10}{20}))=1$$  
$$ H(X|A\in[A_3,A_4])=-1\times\log_2(1)+0=0$$  
**条件熵**是一个X熵对A的期望
$$ H(X|A)=\frac{10}{40}\times H(X|A\in[A_1,A_2))+\frac{20}{40}\times H(X|A\in[A_2,A_3))+\frac{10}{40}\times H(X|A\in[A_3,A_4])=0.5$$  
**信息增益：**
$$g(X|A)=H(X)-H(X|A)=0.5$$

现假设如果我们也测量了两种运动过程中的Y轴加速度平均值  
其分布如下：  
![](https://jingyang1124.oss-cn-shanghai.aliyuncs.com/img/hex_link/Features_Selection/A_B02.png)  
**信息熵**  
$$ H(X)=(-\frac{20}{40}\times\log_2(\frac{20}{40}))+(-\frac{20}{40}\times\log_2(\frac{20}{40}))=1$$  

同样以A2与A3为分界点将A1与A4上的数据分为3段  
分别计算每一段下的X的熵:  
$$ H(X|A\in[A_1,A_2))=-1\times\log_2(1)+0=0$$  
$$ H(X|A\in[A_2,A_3))=(-\frac{6}{13}\times\log_2(\frac{6}{13}))+(-\frac{7}{13}\times\log_2(\frac{7}{13}))=1$$  
$$ H(X|A\in[A_3,A_4])=-1\times\log_2(1)+0=0$$  
**条件熵**是一个X熵对A的期望
$$ H(X|A)=\frac{14}{40}\times H(X|A\in[A_1,A_2))+\frac{6+7}{40}\times H(X|A\in[A_2,A_3))+\frac{13}{40}\times H(X|A\in[A_3,A_4])=0.324$$  
**信息增益：**
$$g(X|A)=H(X)-H(X|A)=0.676$$  
**<u>第二个特征（Y轴加速度）的信息增益比第一个特征（X轴加速度）的信息增益大，更适合做为区分A、B两种动作的特征。</u>**  

## 多个特征的组合  

> 很多时候，单独对每个特征进行信息增益计算后，选择信息增益最大的几个特征来进行识别也不一定能够获得更好的效果。这种时候，可以在计算信息增益时直接计算多个特征组合下的信息增益。

对于如下数据：  
![](https://jingyang1124.oss-cn-shanghai.aliyuncs.com/img/hex_link/Features_Selection/Type3.PNG)  

>AcX:x轴加速度  AcY:y轴加速度  GyY：Y轴角速度  GyZ：Z轴角速度  

如果以单单一个特征计算其信息增益，结果为：  
$$g(X|AcY)>g(X|AcX)>g(X|GyZ)>g(X|GyY)$$  
似乎选择两个加速度值来判断三个类型最为合适。
那么我们将这两种特征画在一个XY平面上，同时作为比较也将两个角速度所组合而得到的平面点进行绘制：  
![](https://jingyang1124.oss-cn-shanghai.aliyuncs.com/img/hex_link/Features_Selection/Type3_xy.PNG)  
可以明显地看出，两个角速度所组成的特征能够将样本非常明显地划分为三类。  
实际上，如果我们想利用多个特征进行分类，在进行特征选择时，可以将多个特征在"空间”进行绘制，然后将“空间”划分为多个区域，之后根据每个区域中各类型出现的数量计算样本在这一组特征下的信息增益。  
例如我们分析AcX、AcY与GyY、GyZ两种特征时，可以将它们绘制在“2维平面上"，之后平面的横纵坐标都分成3份，把平面分成了9个区域， 
![](https://jingyang1124.oss-cn-shanghai.aliyuncs.com/img/hex_link/Features_Selection/Type3_xy_seperate.PNG)
之后计算每个区域上样本的信息熵。

>若是有3个特征，就是在“3维空间”划分多个区域计算每个区域内样本的信息熵。  

下面是可用于计算多个特征组合下的信息增益的Python代码：  

```python
def Calculate_Multi_D_Entropy_Gain(Categories,Feature_Data,Seperate_Num):
    """
    Categories：
    [[1]
     [1]
     ...
     [2]
     ..
     [n]]
    Feature_Data:
    [[f1_1  f1_2]
     [f2_1  f2_2]
     [f3_1  f3_2]
     ...
     [fn_1  fn_2]] (two features)
    Seperate_Num:
    Separate feature data as (Seperate_Num)^(Feature_Num) parts
    """
    Feature_Shape = np.array(Feature_Data).shape
    """
    Feature_Shape[0]:how many data of one feature
    Feature_Shape[1]:how many features
    """
    Feature_Num = Feature_Shape[1]
    Segment_Length = 1/Seperate_Num
    Sample_Num = len(Categories)
    Categories_Num = max(Categories) 

    Total_Entropy = 0
     
    i = 1
    while(i <= Categories_Num):
        Total_Entropy += -1 * list(Categories).count(i)/Sample_Num * \
        math.log(list(Categories).count(i)/Sample_Num,2)
        i+=1
    print(Total_Entropy)
    """
    Condition_Entropy_Matrix:
    The numbers of different types in each Segment of feature zoom
    """
    Condition_Entropy_Matrix = np.zeros((int(Categories_Num), int(pow(Seperate_Num,Feature_Num))))
    for row in range(0,Sample_Num):
        Condition_Entropy_index = 0
        for col in range(0, Feature_Num):
            Condition_Entropy_index += pow(Seperate_Num,col) * \
            int(Feature_Data[row][col]/Segment_Length)
        Condition_Entropy_Matrix[int(Categories[row]) - 1][Condition_Entropy_index] += 1    
    print(Condition_Entropy_Matrix)    
    """
    e.g.: two-features Condition_Entropy_Matrix(Seperate_Num is 3):
       [[ 0 10 10  0  0  0  0  0  0 ]
        [ 0  0  0 20  0  0  0  0  0 ]
        [ 0  0  0  0  0  0 20  0  0 ]]
	No.   1  2  3  4  5  6  7  8  9  Segment
    """
    Condition_Entropy = 0
    for col in range(0, int(pow(Seperate_Num,Feature_Num))):
        Sample_Num_This_Seg = 0
        for row in range(0, int(Categories_Num)):
            Sample_Num_This_Seg += Condition_Entropy_Matrix[row][col]
        if Sample_Num_This_Seg != 0:
            for row in range(0, int(Categories_Num)):
                if Condition_Entropy_Matrix[row][col] != 0:
                    Condition_Entropy += -1 * (Sample_Num_This_Seg/Sample_Num) * \
                    Condition_Entropy_Matrix[row][col]/Sample_Num_This_Seg * \
                    math.log(Condition_Entropy_Matrix[row][col]/Sample_Num_This_Seg,2)
            
    print(Total_Entropy - Condition_Entropy)
    return Total_Entropy - Condition_Entropy
```

