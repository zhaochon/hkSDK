#ifndef DEFINEFEATURES_H
#define DEFINEFEATURES_H

#include "quickcheckdefine.h"

#define DEBUG_LINE
//qDebug()<<"<<<<<"<<__FUNCTION__<<__LINE__


#define FUNCTION_BEGIN
//qDebug()<<"Function begin"<<__FUNCTION__<<__LINE__
#define FUNCTION_END
//qDebug()<<"Function end"<<__FUNCTION__<<__LINE__

#define CATCH_BLOCK     catch (QException e){ qDebug()<<"QException:"<<e.what();}catch(std::exception e){    qDebug()<<"std::exception:"<<e.what();}


#define FEATURE_FRONTLED // front led


#define PRODUCT_ZHANGJIAKOU  // zhangjiakou product

#ifdef PRODUCT_ZHANGJIAKOU

//#define FEATURE_CARD_LESS    // carless field in the web request

#define FEATURE_WARNING_LIGHT   // double light led on the front

#endif

#define FEATURE_FZ6000N

//#define FEATURE_TWOSCREEN   // the device has two screen

#define FEATURE_FOURLINELED   // four line led under the plate camera

#define FEATURE_LEDPLAY   // play sound with the led broad

#define FEATURE_SEETAFACE

#endif // DEFINEFEATURES_H
