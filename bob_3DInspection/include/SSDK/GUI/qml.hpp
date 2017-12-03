#ifndef QMLHELPER_H
#define QMLHELPER_H

#include <string>

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QMetaEnum>

#include "TypeTraits/typetraits.hpp"

    namespace SSDK
    {
        namespace GUI
        {
            /**
            *  @brief
            *       find root qml-object pointer
            *
            *  @param objName: name of object as "rectMain"
            *  @param pEngine: pointer of app QQmlApplicationEngine, usually defined in main.cpp
            *  @param pDestObj: output parm, root qml-object pointer
            *
            */
            #define FIND_QML_ROOT_OBJ_PTR(objName,pEngine,pDestObj)\
            {\
                     if(nullptr!=pEngine)\
                     {\
                         auto objs = pEngine->rootObjects();\
                        \
                           for(int i = 0;i<objs.size();++i)\
                           {\
                                auto tempObjName = objs.at(i)->objectName();\
                                if(tempObjName==objName)\
                                {\
                                    pDestObj = objs.at(i);\
                                    break;\
                                }\
                           }\
                       }\
            }


            /**
            *  @brief
            *       find child qml-object pointer based on name and parent qml-object pointer
            *
            *  @param childObjName: name of child qml-object
            *  @param pParentObj: pointer of parent qml-object pointer
            *  @param pChildObj: output parm,child qml-object pointer
            */
            #define FIND_QML_CHILD_OBJ_PTR(childObjName,pParentObj,pChildObj)\
            {\
                    if(nullptr!=pParentObj)\
                    {\
                        pChildObj = pParentObj->findChild<QObject*>(childObjName);\
                    }\
            }

        }//End of namespace Cire
    }//End of namespace SDK

//#include "Core/qmlhelper.hpp.inl"

#endif // QMLHELPER_H
