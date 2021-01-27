#pragma once

#include "svgelementimpl.h"

namespace lunasvg {

class SVGStyleElement : public SVGElementHead
{
public:
    SVGStyleElement(SVGDocument* document);
    SVGStyleElement* clone(SVGDocument* document) const override;
    void setContent(const std::string& content);
private:
    std::string m_content;
};

} // namespace lunasvg
