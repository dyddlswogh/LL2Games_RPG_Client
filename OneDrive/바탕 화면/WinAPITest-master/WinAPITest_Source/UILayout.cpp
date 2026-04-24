#define NOMINMAX
#include "UILayout.h"
#include <algorithm>


UIRect UILayout::CalcRect(
    int windowWidth,
    int windowHeight,
    int uiWidth,
    int uiHeight,
    UIAnchor anchor,
    int offsetX,
    int offsetY)
{
    UIRect rc{};
    rc.width = uiWidth;
    rc.height = uiHeight;

    switch (anchor)
    {
    case UIAnchor::TopLeft:
        rc.x = offsetX;
        rc.y = offsetY;
        break;

    case UIAnchor::TopRight:
        rc.x = windowWidth - uiWidth - offsetX;
        rc.y = offsetY;
        break;

    case UIAnchor::BottomLeft:
        rc.x = offsetX;
        rc.y = windowHeight - uiHeight - offsetY;
        break;

    case UIAnchor::BottomRight:
        rc.x = windowWidth - uiWidth - offsetX;
        rc.y = windowHeight - uiHeight - offsetY;
        break;

    case UIAnchor::Center:
        rc.x = (windowWidth - uiWidth) / 2 + offsetX;
        rc.y = (windowHeight - uiHeight) / 2 + offsetY;
        break;
    }

    rc.x = std::max(0, rc.x);
    rc.y = std::max(0, rc.y);

    return rc;
}