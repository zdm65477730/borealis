/*
    Borealis, a Nintendo Switch UI Library
    Copyright (C) 2020  natinusala

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "captioned_image.hpp"

CaptionedImage::CaptionedImage()
{
    this->inflateFromXMLRes("xml/views/captioned_image.xml");

    this->image = (brls::Image*)this->getView("image");
    this->label = (brls::Label*)this->getView("label");

    this->label->hide([]() {});

    // Forward Image and Label XML attributes
    this->forwardXMLAttribute("scalingType", this->image);
    this->forwardXMLAttribute("image", this->image);
    this->forwardXMLAttribute("focusUp", this->image);
    this->forwardXMLAttribute("focusRight", this->image);
    this->forwardXMLAttribute("focusDown", this->image);
    this->forwardXMLAttribute("focusLeft", this->image);
    this->forwardXMLAttribute("imageWidth", this->image, "width");
    this->forwardXMLAttribute("imageHeight", this->image, "height");

    this->forwardXMLAttribute("caption", this->label, "text");
}

void CaptionedImage::onChildFocusGained(brls::View* directChild, brls::View* focusedView)
{
    Box::onChildFocusGained(directChild, focusedView);

    this->label->show([]() {});
}

void CaptionedImage::onChildFocusLost(brls::View* directChild, brls::View* focusedView)
{
    Box::onChildFocusLost(directChild, focusedView);

    this->label->hide([]() {});
}

brls::View* CaptionedImage::create()
{
    return new CaptionedImage();
}
