/*
    Borealis, a Nintendo Switch UI Library
    Copyright (C) 2019  WerWolv
    Copyright (C) 2019  p-sam
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

#include <borealis/application.hpp>
#include <borealis/image.hpp>

namespace brls
{

static float measureWidth(YGNodeRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode, float originalWidth, ImageScalingType type)
{
    if (widthMode == YGMeasureModeUndefined)
        return originalWidth;
    else if (widthMode == YGMeasureModeAtMost)
        if (type == ImageScalingType::FIT)
            return width;
        else
            return std::min(width, originalWidth);
    else if (widthMode == YGMeasureModeExactly)
        return width;
    else
        throw std::logic_error("Unsupported Image width measure mode: " + std::to_string(widthMode));
}

static float measureHeight(YGNodeRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode, float originalHeight, ImageScalingType type)
{
    if (heightMode == YGMeasureModeUndefined)
        return originalHeight;
    else if (heightMode == YGMeasureModeAtMost)
        if (type == ImageScalingType::FIT)
            return height;
        else
            return std::min(height, originalHeight);
    else if (heightMode == YGMeasureModeExactly)
        return height;
    else
        throw std::logic_error("Unsupported Image height measure mode: " + std::to_string(heightMode));
}

static YGSize imageMeasureFunc(YGNodeRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
{
    Image* image                 = (Image*)node->getContext();
    int texture                  = image->getTexture();
    float originalWidth          = image->getOriginalImageWidth();
    float originalHeight         = image->getOriginalImageHeight();
    ImageScalingType scalingType = image->getScalingType();

    YGSize size = {
        width : width,
        height : height,
    };

    if (texture == 0)
        return size;

    // Stretched mode: we don't care about the size of the image
    if (scalingType == ImageScalingType::STRETCH)
    {
        return size;
    }
    // Fit scaling mode: scale the view according to image ratio
    else if (scalingType == ImageScalingType::FIT && ntz(height) > 0)
    {
        float viewAspectRatio  = ntz(width) / ntz(height);
        float imageAspectRatio = originalWidth / originalHeight;

        // Grow height as much as possible then deduce width
        if (viewAspectRatio < imageAspectRatio)
        {
            size.height = measureHeight(node, width, widthMode, height, heightMode, originalHeight, scalingType);
            size.width  = measureWidth(node, width, widthMode, height, heightMode, size.height * imageAspectRatio, scalingType);
        }
        // Grow width as much as possible then deduce height
        else
        {
            size.width  = measureWidth(node, width, widthMode, height, heightMode, originalWidth, scalingType);
            size.height = measureHeight(node, width, widthMode, height, heightMode, size.width * imageAspectRatio, scalingType);
        }
    }
    // Crop (and fallback) method: grow as much as possible in both directions
    else
    {
        size.width  = measureWidth(node, width, widthMode, height, heightMode, originalWidth, scalingType);
        size.height = measureHeight(node, width, widthMode, height, heightMode, originalHeight, scalingType);
    }

    return size;
}

Image::Image()
{
    YGNodeSetMeasureFunc(this->ygNode, imageMeasureFunc);

    BRLS_REGISTER_ENUM_XML_ATTRIBUTE(
        "scalingType", ImageScalingType, this->setScalingType,
        {
            { "fit", ImageScalingType::FIT },
            { "stretch", ImageScalingType::STRETCH },
            { "crop", ImageScalingType::CROP },
        });

    BRLS_REGISTER_ENUM_XML_ATTRIBUTE(
        "imageAlign", ImageAlignment, this->setImageAlign,
        {
            { "top", ImageAlignment::TOP },
            { "right", ImageAlignment::RIGHT },
            { "bottom", ImageAlignment::BOTTOM },
            { "left", ImageAlignment::LEFT },
            { "center", ImageAlignment::CENTER },
        });

    this->registerFilePathXMLAttribute("image", [this](std::string value) {
        this->setImageFromFile(value);
    });
}

void Image::draw(NVGcontext* vg, float x, float y, float width, float height, Style style, FrameContext* ctx)
{
    if (this->texture == 0)
        return;

    if (this->scalingType == ImageScalingType::CROP)
    {
        nvgSave(vg);
        nvgIntersectScissor(vg, x, y, width, height);
    }

    float coordX = x + this->imageX;
    float coordY = y + this->imageY;

    this->paint.xform[4] = coordX;
    this->paint.xform[5] = coordY;

    nvgBeginPath(vg);
    nvgRect(vg, coordX, coordY, this->imageWidth, this->imageHeight);
    nvgFillPaint(vg, a(this->paint));
    nvgFill(vg);

    if (this->scalingType == ImageScalingType::CROP)
        nvgRestore(vg);
}

void Image::onLayout()
{
    this->invalidateImageBounds();
}

void Image::setImageAlign(ImageAlignment align)
{
    this->align = align;
    this->invalidateImageBounds();
}

void Image::invalidateImageBounds()
{
    if (this->texture == 0)
        return;

    float width  = this->getWidth();
    float height = this->getHeight();

    float viewAspectRatio  = width / height;
    float imageAspectRatio = this->originalImageWidth / this->originalImageHeight;

    switch (this->scalingType)
    {
        case ImageScalingType::FIT:
        {
            // TODO: alignment
            if (viewAspectRatio >= imageAspectRatio)
            {
                this->imageHeight = this->getHeight();
                this->imageWidth  = this->imageHeight * imageAspectRatio;
                this->imageX      = (width - this->imageWidth) / 2.0F;
                this->imageY      = 0;
            }
            else
            {
                this->imageWidth  = this->getWidth();
                this->imageHeight = this->imageWidth * imageAspectRatio;
                this->imageY      = (height - this->imageHeight) / 2.0F;
                this->imageX      = 0;
            }
            break;
        }
        case ImageScalingType::STRETCH:
            this->imageX      = 0;
            this->imageY      = 0;
            this->imageWidth  = this->getWidth();
            this->imageHeight = this->getHeight();
            break;
        case ImageScalingType::CROP:
            // TODO: alignment
            if (viewAspectRatio < imageAspectRatio)
            {
                this->imageHeight = this->originalImageHeight;
                this->imageWidth  = this->imageHeight * imageAspectRatio;
                this->imageX      = (width - this->imageWidth) / 2.0F;
                this->imageY      = 0;
            }
            else
            {
                this->imageWidth  = this->originalImageWidth;
                this->imageHeight = this->imageWidth * imageAspectRatio;
                this->imageY      = (height - this->imageHeight) / 2.0F;
                this->imageX      = 0;
            }
            break;
        default:
            throw std::logic_error("Unimplemented Image scaling type");
    }

    // Create the paint - actual X and Y positions are updated every frame in draw() to apply translation (scrolling...)
    NVGcontext* vg = Application::getNVGContext();
    this->paint    = nvgImagePattern(vg, 0, 0, this->imageWidth, this->imageHeight, 0, this->texture, 1.0f);
}

void Image::setImageFromRes(std::string name)
{
    this->setImageFromFile(std::string(BOREALIS_RESOURCES) + name);
}

void Image::setImageFromFile(std::string path)
{
    NVGcontext* vg = Application::getNVGContext();

    // Free the old texture if necessary
    if (this->texture != 0)
        nvgDeleteImage(vg, this->texture);

    // Load the new texture
    this->texture = nvgCreateImage(vg, path.c_str(), 0);

    if (this->texture == 0)
        throw std::logic_error("Cannot load image from file \"" + path + "\"");

    int width, height;
    nvgImageSize(vg, this->texture, &width, &height);
    this->originalImageWidth  = (float)width;
    this->originalImageHeight = (float)height;

    this->invalidate();
}

void Image::setScalingType(ImageScalingType scalingType)
{
    this->scalingType = scalingType;

    this->invalidate();
}

ImageScalingType Image::getScalingType()
{
    return this->scalingType;
}

float Image::getOriginalImageWidth()
{
    return this->originalImageWidth;
}

int Image::getTexture()
{
    return this->texture;
}

float Image::getOriginalImageHeight()
{
    return this->originalImageHeight;
}

Image::~Image()
{
    NVGcontext* vg = Application::getNVGContext();

    if (this->texture != 0)
        nvgDeleteImage(vg, this->texture);
}

View* Image::create()
{
    return new Image();
}

} // namespace brls
