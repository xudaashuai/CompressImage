
# 灰度图像压缩

##代码清单

 - File.h
    二进制文件基类
    
    函数：
     - seek : 设置文件指针
     - flush : 刷新文件
     - finish : 刷新并关闭文件
 - OutputFile.h
    二进制文件输出类
    
    函数：
     - bitWrite : 向文件中写入data的低bitLength位
 - InputFile.h
    二进制文件输出类
    
    函数：
     - bitWrite : 从文件读出bitLength位的数并以unsigned int返回
 - CompressImage.h
    图像压缩操作类
    
    函数：
     - readfile 读取filename文件
     - compress 将内存中文件压缩并写入到filename文件中
     - unCompress 读取压缩文件filename并解压到内存中
     - writefile 将内存中数据写入到图像文件filename中
     
     
实习报告见 report.docx
