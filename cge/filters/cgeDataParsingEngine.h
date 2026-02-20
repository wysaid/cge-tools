/*
 * cgeMultipleEffects.h
 *
 *  Created on: 2013-12-13
 *      Author: Wang Yang
 */

#ifndef _CGEDATAPARSINGENGINE_H_
#define _CGEDATAPARSINGENGINE_H_

#include "cgeMultipleEffects.h"
#include "cgeBorderEffects.h"

namespace CGE
{
class CGEDataParsingEngine
{
public:
    static CGEImageFilterInterface* adjustParser(const char* pstr, CGEMutipleEffectFilter* fatherFilter = nullptr);
    static CGEImageFilterInterface* curveParser(const char* pstr, CGEMutipleEffectFilter* fatherFilter = nullptr);
    static CGEImageFilterInterface* lomoWithCurveParser(const char* pstr,
                                                        CGEMutipleEffectFilter* fatherFilter = nullptr);
    static CGEImageFilterInterface* lomoParser(const char* pstr, CGEMutipleEffectFilter* fatherFilter = nullptr);
    static CGEImageFilterInterface* blendParser(const char* pstr, CGEMutipleEffectFilter* fatherFilter = nullptr);
    static CGEImageFilterInterface* vignetteBlendParser(const char* pstr,
                                                        CGEMutipleEffectFilter* fatherFilter = nullptr);
    static CGEImageFilterInterface* colorScaleParser(const char* pstr, CGEMutipleEffectFilter* fatherFilter = nullptr);
    static CGEImageFilterInterface* specialParser(const char* pstr, CGEMutipleEffectFilter* fatherFilter = nullptr);
    static CGEImageFilterInterface* pixblendParser(const char* pstr, CGEMutipleEffectFilter* fatherFilter = nullptr);
    static CGEImageFilterInterface* krblendParser(const char* pstr, CGEMutipleEffectFilter* fatherFilter = nullptr);
    static CGEImageFilterInterface* vignetteParser(const char* pstr, CGEMutipleEffectFilter* fatherFilter = nullptr);
    static CGEImageFilterInterface* selfblendParser(const char* pstr, CGEMutipleEffectFilter* fatherFilter = nullptr);
    static CGEImageFilterInterface* colorMulParser(const char* pstr, CGEMutipleEffectFilter* fatherFilter = nullptr);
    static CGEImageFilterInterface* selectiveColorParser(const char* pstr,
                                                         CGEMutipleEffectFilter* fatherFilter = nullptr);
    static CGEImageFilterInterface* blendTileParser(const char* pstr, CGEMutipleEffectFilter* fatherFilter = nullptr);
    static CGEImageFilterInterface* advancedStyleParser(const char* pstr,
                                                        CGEMutipleEffectFilter* fatherFilter = nullptr);
    static CGEImageFilterInterface* beautifyParser(const char* pstr, CGEMutipleEffectFilter* fatherFilter = nullptr);
    static CGEImageFilterInterface* blurParser(const char* pstr, CGEMutipleEffectFilter* fatherFilter = nullptr);
    static CGEImageFilterInterface* dynamicParser(const char* pstr, CGEMutipleEffectFilter* fatherFilter = nullptr);

    static CGEBorderFilterInterface* borderSParser(const char* pstr, CGEBorderFilter* fatherFilter);
    static CGEBorderFilterInterface* borderMParser(const char* pstr, CGEBorderFilter* fatherFilter);
    static CGEBorderFilterInterface* borderCardParser(const char* pstr, CGEBorderFilter* fatherFilter);
    static CGEBorderFilterInterface* borderCoverSKRParser(const char* pstr, CGEBorderFilter* fatherFilter);
    static CGEBorderFilterInterface* borderCoverCornerParser(const char* pstr, CGEBorderFilter* fatherFilter);
    static CGEBorderFilterInterface* borderCoverUDTParser(const char* pstr, CGEBorderFilter* fatherFilter);
    static CGEBorderFilterInterface* borderCoverSWithTexParser(const char* pstr, CGEBorderFilter* fatherFilter,
                                                               bool keepRatio = false);
    static CGEBorderFilterInterface* borderCoverMWithTexParser(const char* pstr, CGEBorderFilter* fatherFilter);
    static CGEBorderFilterInterface* borderCoverRTAndLBParser(const char* pstr, CGEBorderFilter* fatherFilter);
    static CGEBorderFilterInterface* borderCoverBottomKRParser(const char* pstr, CGEBorderFilter* fatherFilter);
    static CGEBorderFilterInterface* borderCoverUpDownParser(const char* pstr, CGEBorderFilter* fatherFilter);
    static CGEBorderFilterInterface* borderCoverLeftRightParser(const char* pstr, CGEBorderFilter* fatherFilter);
};

}  // namespace CGE
#endif /* _CGEDATAPARSINGENGINE_H_ */
