from win32com.client import Dispatch    
import win32com.client    
import math
import numpy as np
import cv2
import imutils

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



if __name__ == "__main__":    

      xls = easyExcel('E:\Pro\PythonPro\HEX_LINK_ReadFromTxt\Raw_Data_Single.xlsx')     
  
      #-----------------------------------------------------------------------
      Categories = []
      Feature_Name = []
      Feature_Entropy_Gain = []
      row = 2
      col = 3
      while(xls.getCell('All_Categories',row,col) is not None):
        Categories.append(xls.getCell('All_Categories',row,col))
        row += 1

      feature_data = []
      
      First_Feature_Col = 8
      row = 2
      tmp_data_1 = []
      while(xls.getCell('All_Categories',row,col) is not None):
        tmp_data_1.append(int(xls.getCell('All_Categories',row,First_Feature_Col)))
        row += 1
    
      Second_Feature_Col = First_Feature_Col + 1
      row = 2
      tmp_data_2 = []
      print(xls.getCell('All_Categories',1,First_Feature_Col) + "-" + xls.getCell('All_Categories',1,Second_Feature_Col))
      Feature_Name.append( xls.getCell('All_Categories',1,First_Feature_Col) + "-" + xls.getCell('All_Categories',1,Second_Feature_Col))
      while(xls.getCell('All_Categories',row,Second_Feature_Col) is not None):
        tmp_data_2.append(int(xls.getCell('All_Categories',row,Second_Feature_Col)))
        row += 1     
  
      feature_data = tmp_data_1 + tmp_data_2
      Categories=np.array(Categories).reshape(len(tmp_data_1),1)
      feature_data=np.array(feature_data).reshape(2,len(tmp_data_1))
      feature_data = np.transpose(feature_data) 

      im = cv2.imread('./bg.png')

      for point in feature_data:
        cv2.circle(im, point, 1,(0, 0, 255),4)

      feature_data = np.array(feature_data).reshape(1,len(tmp_data_1),1,2)

      print(feature_data)
      print(np.array(feature_data).shape)
      
      for cnt in feature_data:
    # 外接矩形框，没有方向角
        x, y, w, h = cv2.boundingRect(cnt)
        print(x,y,w,h)
        cv2.rectangle(im, (x, y), (x + w, y + h), (0, 255, 0), 2)
        ellipse = cv2.fitEllipse(cnt)
        print(ellipse)
        cv2.ellipse(im, ellipse, (255, 255, 0), 2)
      
      cv2.imshow('a',im)
      xls.save()    
      xls.close()
