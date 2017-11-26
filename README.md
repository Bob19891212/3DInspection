#3DInspection
基于电子制造业高精度的3D检测软件



##### 1.启动软件，加载AppSetting.ini和CaptureSetting.ini，考虑其中异常如何处理

####  2.在2个配置都加载成功的基础上，扫描JobFolderPath目录下的程式
	如果JobFolderPath没有文件的话，自动生成程序（包括内存中的数据结构）：
    生成50个Target，20个chip，30个ic，命名以"chip_xxx/ic_xxx"
    生成其它要求必要的信息
    如果JobFolderPath有文件的话，把文件全部在屏幕上列举出来，然后让用户选择

####  3.加载用户选择的程式   （考虑版本兼容的问题）

####  4.读取成功后把所有的InspectionData信息在屏幕上打印出来

####  5.读取成功后把所有的InspectionData信息同时以xml格式输出

######备注:输出到JobFolderPath，同名后缀名不同,详细流程可参考"AppLauncherFlowChart.xml"文件

