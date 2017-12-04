#ifndef FILEINFO_HPP
#define FILEINFO_HPP

#include <string>
#include <vector>

#include <qfileinfo.h>
#include <QFile>
#include <QString>
#include <QDir>

#include "../Exception/customexception.hpp"

namespace SSDK
{
    namespace Archive
    {
    /**
        *  @brief  FileInfo类为工具类
        *   参考：
        * 　　参照QFileInfo，QDir，QFile封装为FileInfo
        *
        *          FileInfo基本操作:
        *                1.获取文件当前文件或目录信息
        *                           (1).获取当前路径类型，可以获得是文件类型,文件夹类型,链接文件,不合法的路径
        *                           (2).判断当前路径所代表的文件或目录是否正确，意味着路径是否正确
        *                           (3).获取当前路径
        *                           (4).获取父目录路径
        *                           (5).获取当前路径下带扩展名的文件名称
        *                           (6).获取当前路径下不带扩展名的文件名称
        *                           (7).获得绝对路径静态方法
        *                           (8).获取相对路径静态方法
        *                           (9).判断当前文件是否被其它对象进行I/O操作
        *                2.获取子文件信息
        *                           (1).获取文件夹内的所有子文件和文件夹信息(不包括子文件夹内的文件信息)
        *                           (2).获取当前文件夹内的所有文件及文件夹信息，可以迭代获取子文件夹内的文件信息
        *                3.文件或目录操作方法
        *                           (1).删除文件和文件夹，至于哪一种，依据是当前路径是文件还是目录
        *                           (2).拷贝操作，将当前的文件或者文件夹拷贝到指定目录下
        *
        *
        *  @author   vincent
        *  @version 1.00 2017-05-09  vincent
        *                   note: done
        */
        class FileInfo
        {
        public:
            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //enum

            /**
             * @brief The FileInfoType enum
             *       文件信息类型，有文件，文件夹，链接文件，非法路径
             */
            enum FileInfoType
            {
                FILE,
                DIRECTORY,
                SYMLINK,
                ILLEGAL
            };

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //constructor & deconstructor

            /**
             * @brief FileInfo
             *      构造函数
             * @param srcPath
             *       传入一个地址字符串
             */
            FileInfo(const std::string& srcPath) ;
            FileInfo(const QString& srcPath) ;

            /**
             * @brief FileInfo
             *       拷贝构造函数，拷贝FileInfo类型对象
             * @param fileInfo
             *       一个FileInfo类型对象
             */
            FileInfo(const FileInfo& fileInfo);

            /**
             * @brief FileInfo
             *       右值拷贝构造函数
             * @param fileInfo
             */
            FileInfo(FileInfo &&fileInfo);

            /**
             * @brief FileInfo
             *      析构函数
             */
            virtual~FileInfo();

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //operator override functions

            /**
             * @brief operator =
             *       重载"="
             * @param fileInfo
             *       一个FileInfo类型对象
             * @return
             *       返回FileInfo类型
             */
            FileInfo& operator =(const FileInfo& fileInfo);

            /**
             * @brief operator =
             *       右值重载"="
             * @param fileInfo
             *       一个FileInfo类型对象
             * @return
             *       返回FileInfo类型
             */
            FileInfo& operator =(FileInfo&& fileInfo);

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //get file information functions

            /**
             * @brief fileInfoType
             *       判断是文件,目录,链接还是一个不合法路径, 具体见FileInfoType的定义
             *
             * @return
             *       返回类型枚举
             */
            FileInfoType fileInfoType() const;
            static FileInfoType fileInfoType(const std::string& srcPath) ;
            static FileInfoType fileInfoType(const QString& srcPath) ;

            /**
             * @brief exists
             *       判断当前路径目录或文件是否存在
             * @return
             *       返回true为存在，反之无
             */
            bool exists() const;
            static bool exists(const std::string& srcPath) ;
            static bool exists(const QString& srcPath) ;

            /**
             * @brief isUsed
             *       判断当前文件是否正在进行I/O操作
             * @return
             *       返回false为没有被占用，返回true为正在进行I/O操作
             */
            bool isUsed() const;
            static bool isUsed(const std::string& srcPath) ;
            static bool isUsed(const QString& srcPath) ;

            /**
              *注意:
              * 1.因为该类返回的所有路径都是通过内部调用QFileInfo获取的, 其返回的都是QString类型, 如果这里的函数返回
              *    string的话, 一旦外部需要QString类型的, 又要转一道影响效率, 所以该类返回所有路径全是QString类型, 如果
              *    外部需要string的话, 自己调用toStdString()函数
              *
              * 2.除非是像getAbsolutePath/getRelativePath这类明确要求绝对路径还是相对路径的函, 否则的话全部根据构造的
              *    时候传入的路径类型确定, 传入的路径是相对路径返回就是相对路径, 传入的是绝对路径就返回绝对路径
              */

            /**
             * @brief path
             *       获取当前路径
             * @return
             *       返回值为QString类型，且不可修改
             */
            const QString path() const;

            /**
             * @brief getParentFileInfoPath
             *       获取父目录路径
             * @return
             *       返回值QString类型，且不可修改
             */
            const QString getParentPath()const;

            /**
             * @brief getNameWithExtension
             *       获取带扩展名的文件名称，如路径为/home/vincent/bin/a.txt,将返回a.txt
             * @return
             *       返回值为string类型，且不可修改
             */
            const QString getNameWithExtension()const;

            /**
             * @brief getNameWithoutExtension
             *       得到不带扩展名的文件名
             * @return
             *       返回string类型，且不可修改
             */
            const QString  getNameWithoutExtension()const;

            /**
             * @brief getAbsolutePath
             *       获取绝对路径的静态方法
             * @param srcPath
             *       待转换的源路径
             * @return
             */
            static  const QString getAbsolutePath(const std::string &srcPath);

            /**
             * @brief getAbsolutePath
             *        重载getAbsolutePath方法，实现对传入QString的支持
             * @param srcPath
             *        待转换的源路径
             * @return
             */
            static  const QString getAbsolutePath(const QString &srcPath);

            /**
             * @brief getRelativePath
             *       获取相对路径的静态方法
             * @param srcPath
             *       待转换的源路径
             * @return
             */
            static const QString getRelativePath(const std::string  &srcPath);

            /**
             * @brief getRelativePath
             *        重载getRelativePath,实现对传入QString的支持
             * @param srcPath
             *        待转换的源路径
             * @return
             */
            static const QString getRelativePath(const QString  &srcPath);

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //traverse children fileInfo functions

            /**
             * @brief getChildren
             *       获得当前目录下的所有子文件及子目录
             * @return
             *       返回一个FileInfo*类型的vector容器
             */
            const std::vector<FileInfo>& getChildren();

            /**
             * @brief getChildrenRecursively
             *       获取当前目录下的所有文件和文件夹，包括子目录下的文件
             *       迭代获取所有文件
             * @return
             *       返回一个FileInfo*类型的vector容器
             */
           const std::vector<FileInfo> getChildrenRecursively();

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

           //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
           // operation functions

           /**
            * @brief remove
            *       移除当前文件或者文件夹
            * @return
            *       返回true为移除成功，反之无
            */
           bool remove() const;

           /**
            * @brief copyTo
            *       将当前文件copy到指定路径
            * @param destDir
            *       目标文件路径，或者是目录
            * @param isCover
            *        true为覆盖目标文件，false是不执行覆盖操作
            * @return
            *        返回true为复制成功，返回false将复制失败
            */
           bool copyTo(std::string destDir,bool isCover);

           //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        private:

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //member variant

            /**
             * @brief m_childrenFileInfo
             *       所有的子对象
             *       1.如果当前路径目录的话，那么获取仅仅在该目录下的子目录及对应的文件
             *       2.如果当前路径是文件的话，返回为空
             *       3.带目录结构的vector
             */
            std::vector<FileInfo> m_childrenFileInfo;

            /**
              *FileInfo内部调用了一些Qt中关于QFileInfo的函数, 所有这里封装了QFileInfo的嵌入实例对象
              */
            QFileInfo m_qFileinfo;//涉及到路径的获取, 是否存在的判断, 扩展名等信息

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //traverse children fileinfo functions

            /**
             * @brief getFileList
             *       内部调用函数，获得当前目录下的文件列表
             * @param srcPath
             * 　   srcPath为QString类型，目录路径
             * @return
             * 　  返回值为一个FileInfo指针类型的vector容器的向量表
             */
            void setFileList(const QString& srcPath);

            /**
             * @brief getFileListRescursively
             *       内部调用函数，迭代获取目录及文件
             * @param srcPath
             * 　  srcPath为文件夹路径
             * @return
             */
            QFileInfoList getFileListRescursively(QString &srcPath);

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            //>>>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            // operation functions

            /**
             * @brief copySimpleFile
             *       内调用的拷贝单个文件方法
             * @param destPath
             *       要拷贝到目录下的目标路径
             * @param isCover
             *       选择是否被覆盖，true为选择覆盖，false为不覆盖
             * @return
             *       返回true复制文件成功，反之失败
             */
            bool copyFileTo(const QString& destPath, bool isCover) const;

            /**
             * @brief copyDir
             *       内部调用的拷贝目录方法
             * @param srcDirPath
             *       原目录路径
             * @param destDirPath
             *       目标目录的路径
             * @param isCover
             *       选择是否覆盖，true为选择覆盖，false为不覆盖
             * @return
             *       返回true为复制成功，false复制失败
             */
            bool copyDirTo(const QString &srcDirPath,const QString &destDirPath,bool isCover) const;

            //<<<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
           };//End of class FileInfo
    }//End of namespace Archive
}//End of namespace SSDK


#endif // FILEINFO_HPP
