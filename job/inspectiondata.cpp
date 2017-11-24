#include "inspectiondata.hpp"

using namespace std;
using namespace Job;

InspectionData::InspectionData()
{
    this->m_version = "";
    this->m_lastEditingTime = "";
}

InspectionData::~InspectionData()
{

}

void InspectionData::writeToXml(QString path)
{
    try
    {

        QDomDocument jobInfo;
        QDomElement inspectionData = jobInfo.createElement("Job");

        inspectionData.setAttribute("版本号",QString::fromStdString(this->getVersion()));
        inspectionData.setAttribute("上次编辑时间",QString::fromStdString(this->getLastEditingTime()));

        jobInfo.appendChild(inspectionData);

        QString filePath( path );
        QFile file(filePath);

        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
               THROW_EXCEPTION("打开文件失败!!");
        }

        QTextStream out(&file);

        jobInfo.save(out,4);

        file.close();
        this->getBoard()->writeToXml(path);


    }
    catch(const exception &ex)
    {
        THROW_EXCEPTION(ex.what());
    }
}
