import win32com.client    
import math
import numpy as np


class easyExcel:    
      """A utility to make it easier to get at Excel.    Remembering  
      to save the data is your problem, as is    error handling.  
      Operates on one workbook at a time."""    
      def __init__(self, filename=None):  #打开文件或者新建文件（如果不存在的话）  
          self.xlApp = win32com.client.Dispatch('Excel.Application')    
          if filename:    
              self.filename = filename    
              self.xlBook = self.xlApp.Workbooks.Open(filename)    
          else:    
              self.xlBook = self.xlApp.Workbooks.Add()    
              self.filename = ''  
        
      def save(self, newfilename=None):  #保存文件  
          if newfilename:    
              self.filename = newfilename    
              self.xlBook.SaveAs(newfilename)    
          else:    
              self.xlBook.Save()        
      def close(self):  #关闭文件  
          self.xlBook.Close(SaveChanges=0)    
          del self.xlApp    
      def getCell(self, sheet, row, col):  #获取单元格的数据  
          "Get value of one cell"    
          sht = self.xlBook.Worksheets(sheet)    
          return sht.Cells(row, col).Value    
      def setCell(self, sheet, row, col, value):  #设置单元格的数据  
          "set value of one cell"    
          sht = self.xlBook.Worksheets(sheet)    
          sht.Cells(row, col).Value = value  
          
      def deleteRow(self, sheet, row):  
          sht = self.xlBook.Worksheets(sheet)  
          sht.Rows(row).Delete()#删除行  
          sht.Columns(row).Delete()#删除列
      def getRange(self, sheet, row1, col1, row2, col2):  #获得一块区域的数据，返回为一个二维元组  
          "return a 2d array (i.e. tuple of tuples)"    
          sht = self.xlBook.Worksheets(sheet)  
          return sht.Range(sht.Cells(row1, col1), sht.Cells(row2, col2)).Value    
      def addPicture(self, sheet, pictureName, Left, Top, Width, Height):  #插入图片  
          "Insert a picture in sheet"    
          sht = self.xlBook.Worksheets(sheet)    
          sht.Shapes.AddPicture(pictureName, 1, 1, Left, Top, Width, Height)    
      
      def cpSheet(self, before):  #复制工作表  
          "copy sheet"    
          shts = self.xlBook.Worksheets    
          shts(1).Copy(None,shts(1))

      def inserRow(self,sheet,row):
          sht = self.xlBook.Worksheets(sheet)
          sht.Rows(row).Insert(1)

      def getCellColor(self, sheet, row, col):
          sht = self.xlBook.Worksheets(sheet)
          return(sht.Cells(row, col).Interior.ColorIndex)

def Rearrange_Data_Smaller_Firset(list1,list2):
    n = len(list2)
    for i in range(n-1):
        for j in range(i+1,n):
            if list2[i]>list2[j]:   #通过交换让最小的在最前面
                list2[i],list2[j]=list2[j],list2[i]
                list1[i],list1[j]=list1[j],list1[i]
    return list1,list2

def Rearrange_Data_Greater_Firset(list1,list2):
    n = len(list2)
    for i in range(n-1):
        for j in range(i+1,n):
            if list2[i]<list2[j]:   #通过交换让最大的在最前面
                list2[i],list2[j]=list2[j],list2[i]
                list1[i],list1[j]=list1[j],list1[i]
    return list1,list2

     

def Calculate_Multi_D_Entropy_Gain(Categories,Feature_Data,Seperate_Num):
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

    Condition_Entropy_Matrix = np.zeros((int(Categories_Num), int(pow(Seperate_Num,Feature_Num))))
    for row in range(0,Sample_Num):
        Condition_Entropy_index = 0
        for col in range(0, Feature_Num):
            Condition_Entropy_index += pow(Seperate_Num,col) * \
            int(Feature_Data[row][col]/Segment_Length)
        Condition_Entropy_Matrix[int(Categories[row]) - 1][Condition_Entropy_index] += 1    

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
    print("Info_Gain is:")
    print(Total_Entropy - Condition_Entropy)
    return Total_Entropy - Condition_Entropy
    
     
def Uniform_List(list_data):
    tmp_max = max(list_data) *1.02 #避免出现1与0这样的值
    tmp_min = min(list_data) *0.98
    output_list=[]
    for i in list_data:
      i -= tmp_min
      i/=(tmp_max - tmp_min)  #对数据逐个处理
      output_list.append(i)
    return output_list


if __name__ == "__main__":    

      xls = easyExcel('E:\Pro\PythonPro\HEX_LINK_ReadFromTxt\Raw_Data.xlsx')     
  
      #-----------------------------------------------------------------------
      Categories = []
      Feature_Name = []
      Feature_Entropy_Gain = []
      row = 2
      col = 3
      while(xls.getCell('All_Categories',row,col) is not None):
        Categories.append(xls.getCell('All_Categories',row,col))
        row += 1

      col=4

      feature_data = []

      First_Feature_Col = 4
      while(xls.getCell('All_Categories',1,First_Feature_Col) is not None):
          row = 2
          tmp_data_1 = []
          while(xls.getCell('All_Categories',row,col) is not None):
            tmp_data_1.append(xls.getCell('All_Categories',row,First_Feature_Col))
            row += 1
          
          Second_Feature_Col = First_Feature_Col + 1
          while(xls.getCell('All_Categories',1,Second_Feature_Col) is not None):
              row = 2
              tmp_data_2 = []
              
              print(xls.getCell('All_Categories',1,First_Feature_Col) + "-" + xls.getCell('All_Categories',1,Second_Feature_Col))
              Feature_Name.append( xls.getCell('All_Categories',1,First_Feature_Col) + "-" + xls.getCell('All_Categories',1,Second_Feature_Col))
              while(xls.getCell('All_Categories',row,Second_Feature_Col) is not None):
                tmp_data_2.append(xls.getCell('All_Categories',row,Second_Feature_Col))
                row += 1     
              
              feature_data = Uniform_List(tmp_data_1) + Uniform_List(tmp_data_2)         
              Categories=np.array(Categories).reshape(len(tmp_data_1),1)
              feature_data=np.array(feature_data).reshape(2,len(tmp_data_1))
              feature_data = np.transpose(feature_data) 
              Feature_Entropy_Gain.append(Calculate_Multi_D_Entropy_Gain(Categories,feature_data,3))
              
              Second_Feature_Col += 1
          First_Feature_Col += 1
      
      Feature_Name,Feature_Entropy_Gain = Rearrange_Data_Greater_Firset(Feature_Name,Feature_Entropy_Gain)
      """从大到小排列特征组合，越靠前的越适合作为特征"""
      print(Feature_Name) 

      
      xls.save()    
      xls.close()
