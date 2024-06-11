// 4.23.0 0xa4669202
// Generated by imageconverter. Please, do not edit!

#include <images/BitmapDatabase.hpp>
#include <touchgfx/Bitmap.hpp>

extern const unsigned char image_a2_re[]; // BITMAP_A2_RE_ID = 0, Size: 180x143 pixels
extern const unsigned char image_a3_re[]; // BITMAP_A3_RE_ID = 1, Size: 339x180 pixels
extern const unsigned char image_alternate_theme_images_widgets_swipecontainer_medium_off_normal[]; // BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_SWIPECONTAINER_MEDIUM_OFF_NORMAL_ID = 2, Size: 20x20 pixels
extern const unsigned char image_alternate_theme_images_widgets_swipecontainer_medium_on_active[]; // BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_SWIPECONTAINER_MEDIUM_ON_ACTIVE_ID = 3, Size: 20x20 pixels
extern const unsigned char image_icon_theme_images_action_done_50_50_e8f6fb_svg[]; // BITMAP_ICON_THEME_IMAGES_ACTION_DONE_50_50_E8F6FB_SVG_ID = 4, Size: 50x50 pixels
extern const unsigned char image_icon_theme_images_content_add_circle_50_50_e8f6fb_svg[]; // BITMAP_ICON_THEME_IMAGES_CONTENT_ADD_CIRCLE_50_50_E8F6FB_SVG_ID = 5, Size: 50x50 pixels
extern const unsigned char image_icon_theme_images_content_remove_circle_50_50_e8f6fb_svg[]; // BITMAP_ICON_THEME_IMAGES_CONTENT_REMOVE_CIRCLE_50_50_E8F6FB_SVG_ID = 6, Size: 50x50 pixels

const touchgfx::Bitmap::BitmapData bitmap_database[] = {
    { image_a2_re, 0, 180, 143, 4, 2, 173, ((uint8_t)touchgfx::Bitmap::ARGB8888) >> 3, 120, ((uint8_t)touchgfx::Bitmap::ARGB8888) & 0x7 },
    { image_a3_re, 0, 339, 180, 19, 5, 302, ((uint8_t)touchgfx::Bitmap::ARGB8888) >> 3, 170, ((uint8_t)touchgfx::Bitmap::ARGB8888) & 0x7 },
    { image_alternate_theme_images_widgets_swipecontainer_medium_off_normal, 0, 20, 20, 17, 6, 2, ((uint8_t)touchgfx::Bitmap::ARGB8888) >> 3, 8, ((uint8_t)touchgfx::Bitmap::ARGB8888) & 0x7 },
    { image_alternate_theme_images_widgets_swipecontainer_medium_on_active, 0, 20, 20, 8, 7, 4, ((uint8_t)touchgfx::Bitmap::ARGB8888) >> 3, 6, ((uint8_t)touchgfx::Bitmap::ARGB8888) & 0x7 },
    { image_icon_theme_images_action_done_50_50_e8f6fb_svg, 0, 50, 50, 16, 33, 4, ((uint8_t)touchgfx::Bitmap::ARGB8888) >> 3, 3, ((uint8_t)touchgfx::Bitmap::ARGB8888) & 0x7 },
    { image_icon_theme_images_content_add_circle_50_50_e8f6fb_svg, 0, 50, 50, 34, 12, 6, ((uint8_t)touchgfx::Bitmap::ARGB8888) >> 3, 24, ((uint8_t)touchgfx::Bitmap::ARGB8888) & 0x7 },
    { image_icon_theme_images_content_remove_circle_50_50_e8f6fb_svg, 0, 50, 50, 11, 9, 26, ((uint8_t)touchgfx::Bitmap::ARGB8888) >> 3, 13, ((uint8_t)touchgfx::Bitmap::ARGB8888) & 0x7 }
};

namespace BitmapDatabase
{
const touchgfx::Bitmap::BitmapData* getInstance()
{
    return bitmap_database;
}

uint16_t getInstanceSize()
{
    return (uint16_t)(sizeof(bitmap_database) / sizeof(touchgfx::Bitmap::BitmapData));
}
} // namespace BitmapDatabase
