export module soul.xml.axis;

import std;

export namespace soul::xml {

enum class Axis
{
    child, descendant, parent, ancestor, followingSibling, precedingSibling, following, preceding, attribute, ns, self, descendantOrSelf, ancestorOrSelf
};

std::string AxisName(Axis axis);

} // namespace soul::xml
