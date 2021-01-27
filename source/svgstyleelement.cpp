#include "svgstyleelement.h"

#include <map>

#include "cssproperty.h"

namespace lunasvg {
    
static std::map<std::string, DOMSVGStyle> classStyleMap = {};

SVGStyleElement::SVGStyleElement(SVGDocument* document)
    : SVGElementHead(DOMElementIdStyle, document)
{}

SVGStyleElement* SVGStyleElement::clone(SVGDocument* document) const
{
    SVGStyleElement* e = new SVGStyleElement(document);
    baseClone(*e);
    return e;
}

void SVGStyleElement::setContent(const std::string& content) {
	m_content = content;
	//TODO
}

} // namespace lunasvg
