/*
 * cgeColorMappingFilter.h
 *
 *  Created on: 2016-8-5
 *      Author: Wang Yang
 * Description: Color mapping filter
 */

#ifndef _CGE_COLOR_MAPPING_FILTER_H_
#define _CGE_COLOR_MAPPING_FILTER_H_

#include "cgeGLFunctions.h"
#include <vector>
#include "cgeVec.h"

namespace CGE
{
class CGEColorMappingFilter : public CGEImageFilterInterface
{
public:
    CGEColorMappingFilter();
    ~CGEColorMappingFilter();

    enum MappingMode
    {
        MAPPINGMODE_DEFAULT = 0,
        MAPPINGMODE_BUFFERED_AREA = 0,
        MAPPINGMODE_SINGLE = 1,
    };

    struct MappingArea
    {
        Vec4f area;
        float weight;

        bool operator<(const MappingArea& m) const { return weight < m.weight; }
    };

    static CGEColorMappingFilter* createWithMode(MappingMode mode = MAPPINGMODE_DEFAULT);

    virtual void pushMappingArea(const MappingArea& area);
    virtual void endPushing();  // Called after pushMappingArea is done
    virtual void clearPushing() { m_mappingAreas.clear(); }


    // texWith, texHeight: texture size
    // texUnitWidth, texUnitHeight: resolution of each mapping unit after mapping
    virtual void setupMapping(GLuint mappingTex, int texWidth, int texHeight, int texResoWidth, int texResoHeight);

    inline GLuint getTexture() const { return m_mappingTexture; }
    inline const CGESizei& getTexSize() const { return m_texSize; }

protected:
    GLuint m_mappingTexture;
    CGESizei m_texSize;  // Texture size in pixels
    CGESizei m_texUnitResolution;
    std::vector<MappingArea> m_mappingAreas;
};

}  // namespace CGE

#endif  // !_CGE_COLOR_MAPPING_FILTER_H_
