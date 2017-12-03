#ifndef XML_HPP
#define XML_HPP

#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <memory>

#include <boost/archive/tmpdir.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include "Exception/customexception.hpp"

namespace SSDK
{
    namespace Archive
    {
        class Xml
        {
        public:
            Xml();

            /**
             * @brief readFromFilePath
             *          读取一个xml文件，然后把内容更新到T类型的 destObject对象
             * @param destObject
             *          输出参数，目标对象，读取xml后得到的数据更新到该对象中
             * @tparam distObject
             *
             * @param filePath
             */
            template<typename T>
            void readFromFilePath(const std::string& filePath, const T &destObject) const;

            template<typename T>
            void write(const T &object, const std::string& filePath);
        };

        template<typename T>
        void Xml::readFromFilePath(const std::string& filePath, const T &destObject) const
        {
            //rime 170419 这里后续要增加更多的判断，如:
            //filePath是否为空，上级目录是否存在, 路径是否存在等
            // To Do ...

            std::ifstream ifs(filePath);
            assert(ifs.good());
            boost::archive::xml_iarchive input(ifs);

            // restore the schedule from the archive
            input >> BOOST_SERIALIZATION_NVP(destObject);

            THROW_EXCEPTION_WITH_OBJ("asas");
        }

        template<typename T>
        void Xml::write(const T &object, const std::string& filePath)
        {
            //rime 170419 这里后续要增加更多的判断，如:
            //filePath是否为空，上级目录是否存在, 路径是否存在等
            // To Do ...

            std::ofstream ofs(filePath);
            assert(ofs.good());
            boost::archive::xml_oarchive output(ofs);
            output<<BOOST_SERIALIZATION_NVP(object);
        }
    }//End of namespace Archive
}//End of namespace SSDK

#endif // XML_HPP
