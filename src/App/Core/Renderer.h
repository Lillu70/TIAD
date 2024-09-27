

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

# pragma once

struct Font
{
    u32 char_width = 0;
    u32 char_height = 0;
    u8* data_buffer = 0;     // reqular ascii
    u8* data_buffer_sc = 0; // äöÄÖ
};


struct Image
{
    // TODO: Make this into Color*
    u8* buffer;
    v2i dim;
};


struct Canvas
{
    // TODO: Make this into Color*
    u32* buffer = 0;
    v2u dim = {};
    u32 row_stride = 0;
};


static inline v2f Get_Middle(Canvas* canvas);

static inline v3f Unpack_Color(Color color);

static inline v4f Unpack_Color_With_Alpha(Color color);

static inline Color Pack_Color(v3f color);

static inline Color Pack_Color(v4f color);

static inline bool Init_Canvas(Canvas* canvas, u32* buffer, v2u dim);

static inline Canvas Create_Sub_Canvas(Canvas* master_canvas, v2u dim);

static inline Canvas Create_Sub_Canvas(Canvas* master_canvas, v2u dim, u32 buffer_offset);

static inline Canvas Create_Sub_Canvas(Canvas* master_canvas, v2u dim, v2u buffer_offset);

// Does bounds checking.
static inline bool Get_Pixel_Idx(Canvas* canvas, v2i p, u32* out_idx);

static inline void Set_Pixel(Canvas* canvas, v2i p, Color color);

// Does not do bounds checking.
static inline u32 Get_Pixel_Idx_HZ(Canvas* canvas, v2i p);

static inline void Set_Pixel_HZ(Canvas* canvas, v2i p, Color color);

static inline void Set_Pixel_Idx_HZ(Canvas* canvas, u32 idx, Color color);

static inline void Blend_Pixel_With_Color(Canvas* canvas, v2i p, v3f color, f32 fraction);

static inline void Blend_Pixel_With_Color(Canvas* canvas, v2i p, v3f color, f32 fraction, u8 alpha);

static inline Color Get_Pixel_HZ(Canvas* canvas, v2i p);

static void Clear_Canvas(Canvas* canvas, Color color);

static void Clear_Sub_Canvas(Canvas* canvas, Color color);

static void Dim_Entire_Screen(Canvas* canvas, f32 s);

static void Draw_Vertical_Line(Canvas* canvas, v2f pos, f32 height, Color color);

static void Draw_Filled_Rect(Canvas* canvas, Rect rect, Color fill_color);

static void Draw_Filled_Rect_With_Outline(
    Canvas* canvas, 
    Rect rect,
    Color fill_color,
    u32 outline_thickness, 
    Color outline_color);


static void Draw_Percentile_Bar(
    Canvas* canvas, 
    Rect rect, 
    Color fill_color,
    u32 outline_thickness, 
    Color outline_color,
    Color bar_color,
    f32 fill_percent);


static void Draw_Text(
    Canvas* canvas,
    u8* text,
    v2f pos,
    Color color,
    Font* font,
    v2i scale);

    
static void Draw_Text(
    Canvas* canvas,
    String_View text,
    v2f pos,
    Color color,
    Font* font,
    v2f scale);    


static void Draw_Glyph(
    Canvas* canvas,
    v2f pos,
    v2f scale,
    Color color,
    char character,
    Font* font);


static void Draw_Image(Canvas* canvas, Image* img);

static void Draw_Image(Canvas* canvas, Image* img, Rect rect);

static void Draw_Image(Canvas* canvas, Image* img, Rect rect, v3f color_mult);

static void Draw_Image_Badly(Canvas* canvas, Image* img, Rect rect);

static void Convert_From_RGB_To_Color_And_Flip_Y(Image* img);

static void Convert_From_RGB_To_Color(Image* img);

static void Resize_Image(Image* dest, Image* src);