/*
    Borealis, a Nintendo Switch UI Library
    Copyright (C) 2019  natinusala
    Copyright (C) 2019  p-sam

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

#include <borealis/theme.hpp>

namespace brls
{

static ThemeValues lightThemeValues = {
    // Generic values
    { "brls/background", nvgRGB(235, 235, 235) },
    { "brls/text", nvgRGB(51, 51, 51) },
    { "brls/backdrop", nvgRGBA(0, 0, 0, 178) },

    // Highlight
    { "brls/highlight/background", nvgRGB(252, 255, 248) },
    { "brls/highlight/color1", nvgRGB(13, 182, 213) },
    { "brls/highlight/color2", nvgRGB(80, 239, 217) },

    // AppletFrame
    { "brls/applet_frame/separator", nvgRGB(45, 45, 45) },

    // Sidebar
    { "brls/sidebar/background", nvgRGB(240, 240, 240) },
    { "brls/sidebar/active_item", nvgRGB(49, 79, 235) },
    { "brls/sidebar/separator", nvgRGB(208, 208, 208) },

    // Header
    { "brls/header/border", nvgRGB(207, 207, 207) },
    { "brls/header/rectangle", nvgRGB(127, 127, 127) },
    { "brls/header/subtitle", nvgRGB(140, 140, 140) },
};

static ThemeValues darkThemeValues = {
    // Generic values
    { "brls/background", nvgRGB(45, 45, 45) },
    { "brls/text", nvgRGB(255, 255, 255) },
    { "brls/backdrop", nvgRGBA(0, 0, 0, 178) },

    // Highlight
    { "brls/highlight/background", nvgRGB(31, 34, 39) },
    { "brls/highlight/color1", nvgRGB(25, 138, 198) },
    { "brls/highlight/color2", nvgRGB(137, 241, 242) },

    // AppletFrame
    { "brls/applet_frame/separator", nvgRGB(255, 255, 255) },

    // Sidebar
    { "brls/sidebar/background", nvgRGB(50, 50, 50) },
    { "brls/sidebar/active_item", nvgRGB(0, 255, 204) },
    { "brls/sidebar/separator", nvgRGB(81, 81, 81) },

    // Header
    { "brls/header/border", nvgRGB(78, 78, 78) },
    { "brls/header/rectangle", nvgRGB(160, 160, 160) },
    { "brls/header/subtitle", nvgRGB(163, 163, 163) },
};

static Theme lightTheme(&lightThemeValues);
static Theme darkTheme(&lightThemeValues);

ThemeValues::ThemeValues(std::initializer_list<std::pair<std::string, NVGcolor>> list)
{
    for (std::pair<std::string, NVGcolor> color : list)
        this->values.insert(color);
}

void ThemeValues::addColor(std::string name, NVGcolor color)
{
    this->values.insert(std::make_pair(name, color));
}

NVGcolor ThemeValues::getColor(std::string name)
{
    if (this->values.count(name) == 0)
        throw std::logic_error("Unknown theme value \"" + name + "\"");

    return this->values[name];
}

Theme::Theme(ThemeValues* values)
    : values(values)
{
}

NVGcolor Theme::getColor(std::string name)
{
    return this->values->getColor(name);
}

void Theme::addColor(std::string name, NVGcolor color)
{
    return this->values->addColor(name, color);
}

NVGcolor Theme::operator[](std::string name)
{
    return this->getColor(name);
}

Theme getLightTheme()
{
    return lightTheme;
}

Theme getDarkTheme()
{
    return darkTheme;
}

/*
HorizonLightTheme::HorizonLightTheme()
{
    this->textColor        = ;
    this->descriptionColor = nvgRGB(140, 140, 140);

    this->notificationTextColor = nvgRGB(255, 255, 255);
    this->backdropColor         = nvgRGBA(0, 0, 0, 178);

    this->sidebarColor          = ;
    this->activeTabColor        = nvgRGB(49, 79, 235);
    this->sidebarSeparatorColor = nvgRGB(208, 208, 208);



    this->listItemSeparatorColor  = nvgRGB(207, 207, 207);
    this->listItemValueColor      = nvgRGB(43, 81, 226);
    this->listItemFaintValueColor = nvgRGB(181, 184, 191);

    this->tableEvenBackgroundColor = nvgRGB(240, 240, 240);
    this->tableBodyTextColor       = nvgRGB(131, 131, 131);

    this->dropdownBackgroundColor = nvgRGBA(0, 0, 0, 178);

    this->nextStageBulletColor = nvgRGB(165, 165, 165);

    this->spinnerBarColor = nvgRGBA(131, 131, 131, 102);

    this->headerRectangleColor = ;

    this->buttonPrimaryEnabledBackgroundColor  = nvgRGB(50, 79, 241);
    this->buttonPrimaryDisabledBackgroundColor = nvgRGB(201, 201, 209);
    this->buttonPrimaryEnabledTextColor        = nvgRGB(255, 255, 255);
    this->buttonPrimaryDisabledTextColor       = nvgRGB(220, 220, 228);
    this->buttonBorderedBorderColor            = nvgRGB(45, 45, 45);
    this->buttonBorderedTextColor              = nvgRGB(45, 45, 45);
    this->buttonRegularBackgroundColor         = nvgRGB(255, 255, 255);
    this->buttonRegularTextColor               = nvgRGB(46, 46, 46);
    this->buttonRegularBorderColor             = nvgRGB(223, 223, 223);

    this->dialogColor                = nvgRGB(240, 240, 240);
    this->dialogBackdrop             = nvgRGBA(0, 0, 0, 100);
    this->dialogButtonColor          = nvgRGB(46, 78, 255);
    this->dialogButtonSeparatorColor = nvgRGB(210, 210, 210);
}

HorizonDarkTheme::HorizonDarkTheme()
{
    this->textColor        = nvgRGB(255, 255, 255);
    this->descriptionColor = nvgRGB(163, 163, 163);

    this->notificationTextColor = nvgRGB(255, 255, 255);
    this->backdropColor         = nvgRGBA(0, 0, 0, 178);

    this->sidebarColor          = nvgRGB(50, 50, 50);
    this->activeTabColor        = nvgRGB(0, 255, 204);
    this->sidebarSeparatorColor = nvgRGB(81, 81, 81);



    this->listItemSeparatorColor  = nvgRGB(78, 78, 78);
    this->listItemValueColor      = nvgRGB(88, 195, 169);
    this->listItemFaintValueColor = nvgRGB(93, 103, 105);

    this->tableEvenBackgroundColor = nvgRGB(57, 58, 60);
    this->tableBodyTextColor       = nvgRGB(155, 157, 156);

    this->dropdownBackgroundColor = nvgRGBA(0, 0, 0, 178); // TODO: 178 may be too much for dark theme

    this->nextStageBulletColor = nvgRGB(165, 165, 165);

    this->spinnerBarColor = nvgRGBA(131, 131, 131, 102); // TODO: get this right

    this->headerRectangleColor = ;

    this->buttonPrimaryEnabledBackgroundColor  = nvgRGB(1, 255, 201);
    this->buttonPrimaryDisabledBackgroundColor = nvgRGB(83, 87, 86);
    this->buttonPrimaryEnabledTextColor        = nvgRGB(52, 41, 55);
    this->buttonPrimaryDisabledTextColor       = nvgRGB(71, 75, 74);
    this->buttonBorderedBorderColor            = nvgRGB(255, 255, 255);
    this->buttonBorderedTextColor              = nvgRGB(255, 255, 255);
    this->buttonRegularBackgroundColor         = nvgRGB(80, 80, 80);
    this->buttonRegularTextColor               = nvgRGB(255, 255, 255);
    this->buttonRegularBorderColor             = nvgRGB(33, 33, 33);

    this->dialogColor                = nvgRGB(70, 70, 70);
    this->dialogBackdrop             = nvgRGBA(0, 0, 0, 100);
    this->dialogButtonColor          = nvgRGB(3, 251, 199);
    this->dialogButtonSeparatorColor = nvgRGB(103, 103, 103);
}
*/
} // namespace brls
