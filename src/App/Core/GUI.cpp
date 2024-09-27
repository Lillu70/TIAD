

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

static inline GUI_Theme GUI_Create_Default_Theme(GUI_Default_Theme_Names name, Font font)
{
    GUI_Theme result = {};
    
    switch(name)
    {
        case GUI_Default_Theme_Names::vintage:
        {
            result.selected_color              = Make_Color(165, 80, 80);
            result.background_color            = Make_Color(80, 55, 50);
            result.down_color                  = Make_Color(82, 40, 40);
            result.outline_color               = BLACK;
            result.text_color                  = Make_Color(160, 110, 100);
            result.widget_text_color           = BLACK;
            result.title_color                 = Make_Color(214, 104, 104);
            result.write_cursor_color          = Make_Color(180, 130, 150);
            result.write_cursor_limit_color    = RED;
        }break;
        
        case GUI_Default_Theme_Names::document:
        {
            result.selected_color              = Make_Color(140, 200, 200);
            result.background_color            = WHITE;
            result.down_color                  = GRAY;
            result.outline_color               = BLACK;
            result.text_color                  = Make_Color(60, 90, 90);
            result.widget_text_color           = BLACK;
            result.title_color                 = BLACK;
            result.write_cursor_color          = GRAY;
            result.write_cursor_limit_color    = RED;
        }break;
        
        case GUI_Default_Theme_Names::cyper:
        {
            result.selected_color              = Make_Color(165, 245, 90);
            result.background_color            = Make_Color(50, 50, 50);
            result.down_color                  = Make_Color(82, 122, 45);
            result.outline_color               = Make_Color(125, 125, 15);
            result.text_color                  = Make_Color(210, 190, 100);
            result.widget_text_color           = Make_Color(242, 225, 21);
            result.title_color                 = Make_Color(244, 11, 227);
            result.write_cursor_color          = Make_Color(180, 0, 180);
            result.write_cursor_limit_color    = RED;
        }break;
        
        case GUI_Default_Theme_Names::oasis:
        {
            result.selected_color              = Make_Color(230, 115, 150);
            result.background_color            = Make_Color(253, 228, 196);
            result.down_color                  = Make_Color(82, 40, 40);
            result.outline_color               = Make_Color(0, 155, 151);
            result.text_color                  = Make_Color(0, 80, 80);
            result.widget_text_color           = Make_Color(131, 75, 24);
            result.title_color                 = Make_Color(205, 202, 167);
            result.write_cursor_color          = Make_Color(180, 130, 150);
            result.write_cursor_limit_color    = RED;
        }break;
    }
    
    result.font = font;
    
    result.outline_thickness = 2;
    result.padding = 10;
    
    return result;
}


static inline void GUI_Set_Default_Menu_Actions()
{
    GUI_Context::actions[GUI_Menu_Actions::mouse] = Make_Action(Key_Code::MOUSE_LEFT, Button::NONE);
    GUI_Context::actions[GUI_Menu_Actions::up]    = Make_Action(Key_Code::UP, Button::DPAD_UP);
    GUI_Context::actions[GUI_Menu_Actions::down]  = Make_Action(Key_Code::DOWN, Button::DPAD_DOWN);
    GUI_Context::actions[GUI_Menu_Actions::left]  = Make_Action(Key_Code::LEFT, Button::DPAD_LEFT);
    GUI_Context::actions[GUI_Menu_Actions::right] = Make_Action(Key_Code::RIGHT, Button::DPAD_RIGHT);
    GUI_Context::actions[GUI_Menu_Actions::enter] = Make_Action(Key_Code::ENTER, Button::BUT_A);
    GUI_Context::actions[GUI_Menu_Actions::back]  = Make_Action(Key_Code::ESC, Button::BUT_Y);
}


static bool GUI_Character_Check_Numbers_Only(char* c)
{
    bool result = (*c >= '0' && *c <= '9');
    return result;
}


static bool GUI_Character_Check_View_Only(char* c)
{
    return false;
}


static inline v2f GUI_Scale_Default(v2f scale_factor)
{
    v2f result = Hadamar_Product(GUI_DEFAULT_TEXT_SCALE, scale_factor);
    return result;
}


static inline v2f GUI_Scale_Default(f32 scale_factor)
{
    v2f result = GUI_DEFAULT_TEXT_SCALE * scale_factor;
    return result;
}


static inline v2f GUI_Character_Size(GUI_Context* context, v2f scale_factor = GUI_DEFAULT_TEXT_SCALE)
{
    f32 y = f32(context->theme->font.char_height);
    f32 x = f32(context->theme->font.char_width);
    
    v2f result = Hadamar_Product(v2f{x, y}, scale_factor);
    
    return result;
}


static inline f32 GUI_Character_Height(
    GUI_Context* context, 
    f32 scale_factor = GUI_DEFAULT_TEXT_SCALE.y)
{
    f32 y = f32(context->theme->font.char_height);
    f32 result = y * scale_factor;
    
    return result;
}


static inline f32 GUI_Character_Width(
    GUI_Context* context, 
    f32 scale_factor = GUI_DEFAULT_TEXT_SCALE.x)
{
    f32 x = f32(context->theme->font.char_width);
    f32 result = x * scale_factor;
    
    return result;
}


static inline f32 GUI_Padding(GUI_Context* context)
{
    f32 result = f32(context->theme->padding);
    return result;
}


static inline u32 GUI_Make_Ignore_Selection_Mask()
{
    u32 result = GUI_Context_Flags::soft_ignore_selection | GUI_Context_Flags::hard_ignore_selection;
    return result;
}


static inline bool GUI_Is_Context_Ready(GUI_Context* context)
{
    bool result = context->flags & GUI_Context_Flags::context_ready;
    return result;
}


static Rect GUI_Get_Bounds_In_Pixel_Space(GUI_Context* context)
{
    Rect result = context->bounds_rel_anchor_base;
    result.min -= context->anchor_base;
    result.max -= context->anchor_base;
    
    return result;
}


static inline f32 GUI_Get_Collumn_Start(GUI_Context* context, u32 axis)
{
    f32 last_half_dim = context->layout.last_element.dim.elements[axis] / 2;
    f32 pixel_space_pos = context->layout.last_element.pos.elements[axis];
    pixel_space_pos -= context->anchor_base.elements[axis];
    
    f32 result = pixel_space_pos - last_half_dim;
    
    return result;
}


static inline void GUI_End_Collumn(
    GUI_Context* context, 
    f32 collumn_min_width, f32 collumn_start, u32 axis)
{
    v2f pixel_space_bounds = GUI_Get_Bounds_In_Pixel_Space(context).max;
    Assert(axis < Array_Lenght(pixel_space_bounds.elements));
    
    f32 collumn_width = pixel_space_bounds.elements[axis] - collumn_start;
    
    f32 last_directional_dim = context->layout.last_element.dim.elements[axis];
    context->layout.last_element.pos.elements[axis] -= last_directional_dim;
    
    if(collumn_width > collumn_min_width)
    {    
        context->layout.last_element.pos.elements[axis] += collumn_width + context->theme->padding;
    }else
    {
        context->layout.last_element.pos.elements[axis] += collumn_min_width;
    }
}


static inline void GUI_Push_Layout(GUI_Context* context)
{
    bool can_push = context->layout_stack_count < Array_Lenght(context->layout_stack);
    Assert(can_push);
    
    if(can_push)
    {
        context->layout_stack[context->layout_stack_count] = context->layout;
        context->layout_stack_count += 1;
    }
}


static inline void GUI_Pop_Layout(GUI_Context* context)
{
    bool can_pop = context->layout_stack_count > 0;
    Assert(can_pop);
    
    if(can_pop)
    {
        context->layout_stack_count -= 1;
        context->layout = context->layout_stack[context->layout_stack_count];
    }
}


static inline  bool GUI_Accelerated_Tick(
    GUI_Input_Acceleration_Behavior* p,
    f64 time,
    f64* input_start_time,
    f64* next_input_time)
{
    Assert(p->input_delay_time > 0);
    Assert(p->input_speed_up_time >= 0);
    
    if(time < *next_input_time)
        return false;
    
    if(*input_start_time == 0)
    {
        *input_start_time = time;
        *next_input_time = time + p->input_delay_time;
        
        return true;
    }
    else 
    {
        f64 s = p->input_speed_up_time > 0? p->input_speed_up_time : 1.0;
        
        f64 hold_duration = *next_input_time - *input_start_time;
        f64 t = Min(1.0 + (hold_duration / s), Max(1.0, p->max_speed_up_factor));
        f64 t2 = *next_input_time + (p->input_delay_time / t);
        
        *next_input_time = t2;
    
        return true;
    }
}


static inline GUI_Highlight GUI_Highlight_Nothing()
{
    return {};
}


static inline GUI_Highlight GUI_Highlight_Everything()
{
    return {0, I32_MAX};
}


static inline GUI_Highlight GUI_Highlight_Next(GUI_Context* context, i32 num_elements)
{
    return { context->widget_count, num_elements };
}


static inline GUI_Highlight GUI_Highlight_Next(GUI_Context* context)
{
    return { context->widget_count, 1 };
}


static inline GUI_Highlight GUI_Highlight_Prev(GUI_Context* context)
{
    return { context->widget_count - 1, 1 };
}


static inline v2f GUI_Tight_Fit_Text(
    char* text, 
    Font* font, 
    v2f text_scale = GUI_DEFAULT_TEXT_SCALE)
{
    v2f result = v2f
    {
        text_scale.x * f32(font->char_width) * f32(Null_Terminated_Buffer_Lenght(text)), 
        text_scale.y * f32(font->char_height)
    };
    
    return result;
}


static v2f GUI_Tight_Fit_Multi_Line_Text(
    Font* font, 
    char** text_array, 
    u32 array_count, 
    v2f text_scale = GUI_DEFAULT_TEXT_SCALE)
{
    Assert(font);
    Assert(text_array);
    Assert(array_count);
    Assert(text_scale.x != 0 && text_scale.y != 0);
    
    u32 long_idx = 0;
    u32 long_len = 0;
    for(u32 i = 0; i < array_count; ++i)
    {
        u32 len = Null_Terminated_Buffer_Lenght(text_array[i]);
        if(len > long_len)
        {
            long_idx = i;
            long_len = len;
        }
    }
    
    v2f result = v2f{f32(long_len * font->char_width), f32(font->char_height)};
    result = Hadamar_Product(result, text_scale);
    
    return result;
}


static inline void GUI_Update_Bounds(GUI_Context* context, Rect rect)
{
    if(rect.min.x < context->bounds_rel_anchor_base.min.x)
        context->bounds_rel_anchor_base.min.x = rect.min.x;
    
    if(rect.min.y < context->bounds_rel_anchor_base.min.y)
        context->bounds_rel_anchor_base.min.y = rect.min.y;
        
    if(rect.max.x > context->bounds_rel_anchor_base.max.x)
        context->bounds_rel_anchor_base.max.x = rect.max.x;
    
    if(rect.max.y > context->bounds_rel_anchor_base.max.y)
        context->bounds_rel_anchor_base.max.y = rect.max.y;
}


static inline GUI_Placement GUI_Get_Placement(
    GUI_Context* context, 
    v2f* dim, 
    v2f* pos, 
    bool allow_rect_to_be_invalid = true)
{
    GUI_Layout* layout = &context->layout;
    
    GUI_Placement result;
    
    v2f last_element_dim = layout->last_element.dim;
    
    // Auto positioning.
    // NOTE: Dimension effects positioning, so it's handeled first.
    result.dim = (dim)? *dim : last_element_dim;
    
    f32 padding;
    if(context->flags & GUI_Context_Flags::one_time_skip_padding)
    {
        padding = 0;
        Inverse_Bit_Mask(&context->flags, GUI_Context_Flags::one_time_skip_padding);
    }
    else
    {
        padding = context->theme->padding;
    }
    
    
    if(!pos)
    {
        v2f half_last_dim = last_element_dim / 2;
        v2f half_out_dim = result.dim / 2;
        
        v2f offset;
        switch(layout->build_direction)
        {
            case GUI_Build_Direction::up_center:
            {
                offset = v2f{0, last_element_dim.y / 2 + result.dim.y / 2 + padding};
            }break;
            
            
            case GUI_Build_Direction::down_center:
            {
                offset = v2f{0, -last_element_dim.y / 2 - result.dim.y / 2 - padding};
            }break;
            
            
            case GUI_Build_Direction::left_center:
            {
                offset = v2f{-last_element_dim.x / 2 - result.dim.x / 2 - padding, 0};
            }break;
            
            
            case GUI_Build_Direction::right_center:
            {
                offset = v2f{last_element_dim.x / 2 + result.dim.x / 2 + padding, 0};
            }break;
            
            
            case GUI_Build_Direction::down_left:
            {
                offset = v2f{0, -half_last_dim.y - half_out_dim.y - padding};
            
                f32 left_shift = half_out_dim.x - half_last_dim.x;
                offset.x += left_shift;
            }break;
            
            
            case GUI_Build_Direction::down_right:
            {
                f32 right_shift = half_last_dim.x - half_out_dim.x;
                offset = v2f{right_shift, -half_last_dim.y - half_out_dim.y - padding};
            }break;
            
            
            case GUI_Build_Direction::up_left:
            {
                f32 left_shift = half_out_dim.x - half_last_dim.x;
                offset = v2f{left_shift, half_last_dim.y + half_out_dim.y + padding};
            }break;
            
            
            case GUI_Build_Direction::up_right:
            {
                f32 right_shift = half_last_dim.x - half_out_dim.x;
                offset = v2f{right_shift, half_last_dim.y + half_out_dim.y + padding};
            }break;
            
            
            case GUI_Build_Direction::left_top:
            {
                f32 up_shift = half_last_dim.y - half_out_dim.y;
                offset = v2f{-half_last_dim.x - half_out_dim.x - padding, up_shift};
            }break;
            
            
            case GUI_Build_Direction::left_bottom:
            {
                f32 up_shift = half_out_dim.y - half_last_dim.y;
                offset = v2f{-half_last_dim.x - half_out_dim.x - padding, up_shift};
            }break;
            
            
            case GUI_Build_Direction::right_top:
            {
                f32 up_shift = half_last_dim.y - half_out_dim.y;
                offset = v2f{half_last_dim.x + half_out_dim.x + padding, up_shift};
            }break;
            
            
            case GUI_Build_Direction::right_bottom:
            {
                f32 up_shift = half_out_dim.y - half_last_dim.y;
                offset = v2f{half_last_dim.x + half_out_dim.x + padding, up_shift};
            }break;
        }
        
        result.pos = layout->last_element.pos + offset;
        
        result.rect = Create_Rect_Center_HZ(result.pos, result.dim);
    }
    else
    {
        GUI_Anchor anchor = layout->anchor;
        
        v2f p = {};
        v2f canvas_size = context->canvas_space_dim.As<f32>();
        
        enum class Offset_Dir
        {
            none,
            left,
            right,
            up,
            down,
        };
        Offset_Dir collision_offset = Offset_Dir::none;
        
        if(pos == &GUI_AUTO_TOP_CENTER)
        {
            pos = &p;
            p = {canvas_size.x / 2, canvas_size.y - padding};
            collision_offset = Offset_Dir::up;
        }
        else if(pos == &GUI_AUTO_TOP_RIGHT)
        {
            pos = &p;
            p = v2f{canvas_size.x - padding, canvas_size.y - padding};
            collision_offset = Offset_Dir::right;
        }
        else if(pos == &GUI_AUTO_TOP_LEFT)
        {
            pos = &p;
            p = {padding, canvas_size.y - padding};
            collision_offset = Offset_Dir::left;
        }
        else if(pos == &GUI_AUTO_MIDDLE_RIGHT)
        {
            pos = &p;
            
            p = {canvas_size.x - padding, canvas_size.y / 2};
            collision_offset = Offset_Dir::right;
        }
        else if(pos == &GUI_AUTO_MIDDLE_LEFT)
        {
            pos = &p;
            p = {padding, canvas_size.y / 2};
            collision_offset = Offset_Dir::left;
        }
        else if(pos == &GUI_AUTO_BOTTOM_CENTER)
        {
            pos = &p;
            p = {canvas_size.x / 2, padding};
            collision_offset = Offset_Dir::down;
        }
        else if(pos == &GUI_AUTO_BOTTOM_RIGHT)
        {
            pos = &p;
            p = {canvas_size.x - padding, padding};
            collision_offset = Offset_Dir::right;
        }
        else if(pos == &GUI_AUTO_BOTTOM_LEFT)
        {
            pos = &p;
            p = {padding, padding};
            collision_offset = Offset_Dir::left;
        }
        else if(pos == &GUI_AUTO_MIDDLE)
        {
            pos = &p;
            p = canvas_size / 2;
        }
        
        result.pos = context->anchor_base + *pos;
        
        v2f half_dim = result.dim / 2;
        switch(anchor)
        {
            case GUI_Anchor::top:
            {
                result.pos.y -= half_dim.y;
            }break;
            
            case GUI_Anchor::bottom:
            {
                result.pos.y += half_dim.y;
            }break;
            
            case GUI_Anchor::left:
            {
                result.pos.x += half_dim.x;
            }break;
            
            case GUI_Anchor::right:
            {
                result.pos.x -= half_dim.x;
            }break;
            
            case GUI_Anchor::top_left:
            {
                result.pos.y -= half_dim.y;
                result.pos.x += half_dim.x;
            }break;
            
            case GUI_Anchor::top_right:
            {
                result.pos.y -= half_dim.y;
                result.pos.x -= half_dim.x;
            }break;
            
            case GUI_Anchor::bottom_left:
            {
                result.pos.y += half_dim.y;
                result.pos.x += half_dim.x;
            }break;
            
            case GUI_Anchor::bottom_right:
            {
                result.pos.y += half_dim.y;
                result.pos.x -= half_dim.x;
            }break;
        }
        
        result.rect = Create_Rect_Center_HZ(result.pos, result.dim);

        if(context->flags & GUI_Context_Flags::enable_auto_layout_bounds_collision_offseting &&
            collision_offset != Offset_Dir::none &&
            Is_Rect_Valid(context->bounds_rel_anchor_base))
        {
            Rect padded_rect = Expand_Rect(result.rect, GUI_Padding(context));
            if(Rects_Overlap(padded_rect, context->bounds_rel_anchor_base))
            {
                Rect* a = &padded_rect;
                Rect* b = &context->bounds_rel_anchor_base;
                
                switch(collision_offset)
                {
                    case Offset_Dir::up:
                    {
                        result.pos.y += b->max.y - a->min.y;
                    }break;
                    
                    case Offset_Dir::down:
                    {
                        result.pos.y -= a->max.y - b->min.y;
                    }break;
                    
                    case Offset_Dir::left:
                    {
                        result.pos.x -= a->max.x - b->min.x;
                    }break;
                    
                    case Offset_Dir::right:
                    {
                        result.pos.x += b->max.x - a->min.x;
                    }break;
                }
                
                result.rect = Create_Rect_Center_HZ(result.pos, result.dim);
            }
        }
    }
    
    if(allow_rect_to_be_invalid || Is_Rect_Valid(result.rect))
    {
        layout->last_element.pos = result.pos;
        layout->last_element.dim = result.dim;
        layout->last_element.rect = result.rect;
        
        GUI_Update_Bounds(context, result.rect);
    }
    
    result.pos += context->rendering_offset;
    result.rect.min += context->rendering_offset;
    result.rect.max += context->rendering_offset;
    
    return result;
}


static inline v2f GUI_Calc_Centered_Text_Position(char* text, v2f scale, v2f position, Font* font)
{
    f32 magic_offset = 1.0f;
    
    u32 text_lenght = Null_Terminated_Buffer_Lenght(text);
    v2f result = position;
    result.x -= text_lenght * scale.x * font->char_width / 2;
    result.y -= (scale.y * magic_offset) * font->char_height / 2;
    
    return result;
}


static inline void GUI_Reset_Selection_State(GUI_Context* context)
{
    u32 flag_mask = GUI_Context_Flags::cursor_mask_enabled | GUI_Context_Flags::cursor_mask_validation;
    Inverse_Bit_Mask(&context->flags, flag_mask);
    
    context->defered_render = GUI_Defered_Render_Type::none;
    context->selection_state = {};
    context->selected_id = 0;
}


static inline GUI_Context GUI_Create_Context()
{
    static u32 id = 1;
    
    GUI_Context result = {};
    result._context_id = id++;
    
    return result;
}


static inline void GUI_Reset_Context(GUI_Context* context)
{
    // Make sure context is not reset between begin and end.
    Assert(!GUI_Is_Context_Ready(context));
    
    u32 retained_flags = context->flags & GUI_Context_Flags::enable_dynamic_sliders | 
        context->flags & GUI_Context_Flags::enable_auto_layout_bounds_collision_offseting;
    f32 dynamic_slider_girth = context->dynamic_slider_girth;
    u32 id = context->_context_id;
    *context = GUI_Context();
    
    context->flags |= retained_flags;
    context->_context_id = id;
    context->dynamic_slider_girth = dynamic_slider_girth;
}


static inline void GUI_Try_Reset_Context(GUI_Context* context)
{
    if(!GUI_Is_Context_Ready(context) && context->last_widget_count)
        GUI_Reset_Context(context);
}


static inline void GUI_Update_Actions()
{
    Assert(GUI_Context::platform);
    
    Update_Actions(
        GUI_Context::platform, 
        GUI_Context::actions, 
        Array_Lenght(GUI_Context::actions), 
        &GUI_Context::action_context);
}


static inline void GUI_Activate_Context(GUI_Context* context)
{
    Assert(context->_context_id);
    GUI_Context::active_context_id = context->_context_id;
}


static inline bool GUI_Is_Context_Active(GUI_Context* context)
{
    Assert(context->_context_id);
    bool result = GUI_Context::active_context_id == context->_context_id;
    
    return result;
}


static void GUI_Begin_Context_In_Layout_Only_Mode(
    GUI_Context* context,
    Canvas* canvas,
    GUI_Theme* theme, 
    GUI_Anchor anchor = GUI_Anchor::top_left,
    GUI_Build_Direction build_direction = GUI_Build_Direction::down_left)
{
    Assert(theme);
    Assert(context->_context_id);
    Assert(!GUI_Is_Context_Ready(context));
    
    context->mode = GUI_Mode::layout_only;
    
    context->theme = theme;
    context->canvas = canvas;
    
    context->layout = {};
    context->layout.build_direction = build_direction;
    context->layout.anchor = anchor;
    
    context->flags |= GUI_Context_Flags::context_ready;
    
    context->canvas_space_dim = context->canvas->dim;
    context->canvas_rect = Create_Rect_Min_Dim(v2f{0}, v2u::Cast<f32>(context->canvas->dim));
    
    context->bounds_rel_anchor_base = { {F32_MAX, F32_MAX}, {-F32_MAX, -F32_MAX} };
}


static void GUI_Begin_Context(
    GUI_Context* context,
    Canvas* canvas,
    Action_Context* external_action_context,
    GUI_Theme* theme,
    v2i canvas_pos = v2i{0, 0},
    GUI_Anchor anchor = GUI_Anchor::top_left,
    GUI_Build_Direction build_direction = GUI_Build_Direction::down_left,
    GUI_Link_Direction::Type ld = GUI_Link_Direction::up)
{
    Assert(theme);
    Assert(canvas);
    Assert(context->platform);
    Assert(context->_context_id);
    Assert(!GUI_Is_Context_Ready(context));
    
    context->mode = GUI_Mode::normal;
    
    context->canvas = canvas;
    context->theme = theme;
    context->external_action_context = external_action_context;
    
    context->layout = {};
    context->layout.build_direction = build_direction;
    context->layout.anchor = anchor;
    context->widget_count = 0;
    context->rendering_offset = {};
    
    context->canvas_pos = canvas_pos;
    context->layout.last_element.pos = context->anchor_base;
    context->layout_stack_count = 0;
    context->selected_element_dim = {};
    context->selected_element_pos = {};
    context->bounds_rel_anchor_base = { {F32_MAX, F32_MAX}, {F32_MIN, F32_MIN} };
    
    context->canvas_space_dim = context->canvas->dim;
    context->canvas_rect = Create_Rect_Min_Dim(v2f{0}, v2u::Cast<f32>(context->canvas->dim));
    
    context->flags |= GUI_Context_Flags::context_ready;
    
    u32 app_flags = context->platform->Get_Flags();
    
    if(Is_Flag_Set(app_flags, (u32)App_Flags::window_has_resized) 
        && context->platform->Get_Frame_Count())
    {
        context->flags |= GUI_Context_Flags::dont_auto_activate;
        context->flags |= GUI_Context_Flags::soft_ignore_selection;
    }
    
    if(Bit_Not_Set(context->flags, GUI_Context_Flags::soft_ignore_selection))
        Inverse_Bit_Mask(&context->flags, GUI_Context_Flags::dont_auto_activate);
    
    if(Bit_Not_Set(context->flags, GUI_Context_Flags::hard_ignore_selection))
    {
        if(GUI_Context::active_context_id == context->_context_id)
        {
            if(Bit_Not_Set(context->flags, GUI_Context_Flags::dont_auto_activate))
                Inverse_Bit_Mask(&context->flags, GUI_Context_Flags::soft_ignore_selection);
        }
        else
        {
            context->flags |= GUI_Context_Flags::soft_ignore_selection;
        }        
    }
    
    Inverse_Bit_Mask(&context->flags, GUI_Context_Flags::cursor_mask_validation);
    
    if(Is_Flag_Set(app_flags, (u32)App_Flags::is_focused))
    {
        u32 action_mask = GUI_Make_Ignore_Selection_Mask() | 
            GUI_Context_Flags::one_time_disable_kc_navigation;
        
        if(Bit_Not_Set(context->flags, action_mask))
        {
            if(context->actions[GUI_Menu_Actions::up].Is_Pressed())
            {
                context->selected_index += -1 * ld;
                GUI_Reset_Selection_State(context);
            }
            
            if(context->actions[GUI_Menu_Actions::down].Is_Pressed())
            {
                context->selected_index += 1 * ld;
                GUI_Reset_Selection_State(context);
            }
        }
        
        context->cursor_position = context->platform->Get_Cursor_Position() - context->canvas_pos;
        context->cursor_fpos = v2i::Cast<f32>(context->cursor_position);
    }
    
    u32 flags_to_kill = GUI_Context_Flags::one_time_taking_control_of_external_input | 
        GUI_Context_Flags::one_time_disable_kc_navigation;
    
    Inverse_Bit_Mask(&context->flags, flags_to_kill);
}


static inline void GUI_End_Context(GUI_Context* context)
{
    Assert(context);
    Assert(context->layout_stack_count == 0);
    
    if(context->mode == GUI_Mode::normal)
    {
        // DOC: Sometimes you have to rendering some things at the end in order to insure that,
        // draw order is correct.
        // TODO: That said, this is a temprory placeholder, that carrise manny issues.
        // For one the char*[] that is passed into the Do function, has still be valid,
        // when ending the context here.
        switch(context->defered_render)
        {
            case GUI_Defered_Render_Type::dropdown_button:
            {
                GUI_Dropdown_Button_State* state = &context->selection_state.dropdown_button;
                GUI_Theme* theme = state->theme;
                
                Font* font = &theme->font;

                Draw_Filled_Rect_With_Outline(
                    context->canvas, 
                    state->open_rect, 
                    theme->background_color,
                    theme->outline_thickness, 
                    theme->selected_color);
                
                v2f pos = state->pos;
                
                for(u32 i = 0; i < state->element_count; ++i)
                {
                    char* text = state->element_names[i];
                    Color text_color = theme->outline_color;
                    if(state->selected_element_idx == i)
                    {
                        Color bg_color = theme->outline_color;
                        text_color = theme->background_color;
                        
                        if(state->is_pressed_down)
                        {
                            text_color = theme->down_color;
                            bg_color = theme->background_color;
                        }
                        
                        Rect bg_rect = Create_Rect_Center(pos, state->dim);
                        
                        Draw_Filled_Rect_With_Outline(
                            context->canvas, 
                            bg_rect, bg_color, 
                            theme->outline_thickness, 
                            theme->selected_color);
                    }
                    
                    v2f text_p = GUI_Calc_Centered_Text_Position(text, state->text_scale, pos, font);
                    Draw_Text(context->canvas, (u8*)text, text_p, text_color, font, state->text_scale);
                    
                    pos.y -= state->dim.y;
                }
            }break;
        }
        
        context->defered_render = GUI_Defered_Render_Type::none;
        
        if(context->external_action_context && (context->flags & 
            GUI_Context_Flags::one_time_taking_control_of_external_input))
        {
            context->external_action_context->disable_for_one_frame = true;
        }
        
        if(Bit_Not_Set(context->flags, GUI_Context_Flags::cursor_mask_validation))
        {
            Inverse_Bit_Mask(&context->flags, GUI_Context_Flags::cursor_mask_enabled);
            context->cursor_mask_area = {};
        }
        
        u32 ignore_selection_mask = GUI_Make_Ignore_Selection_Mask();
        
        bool allow_snap_to_selected = true;
        
        // Selection wrapping.
        if(Bit_Not_Set(context->flags, ignore_selection_mask | GUI_Context_Flags::disable_wrapping))
        {
            if(context->widget_count > 0 &&
                context->selected_index > context->widget_count - 1 &&
                context->widget_count < context->last_widget_count)
            {
                context->selected_index = context->widget_count - 1;
                GUI_Reset_Selection_State(context);
                allow_snap_to_selected = false;
            }
            
            if(context->selected_index < 0)
            {
                context->selected_index = context->widget_count - 1;
                GUI_Reset_Selection_State(context);
                allow_snap_to_selected = false;
            }
            
            if(context->selected_index >= context->widget_count)
            {
                context->selected_index = 0;
                GUI_Reset_Selection_State(context);
                allow_snap_to_selected = false;
            }
        }
        
        context->last_widget_count = context->widget_count;
        Inverse_Bit_Mask(&context->flags, GUI_Context_Flags::disable_wrapping);
        
        // GUI scrolling
        if(context->flags & GUI_Context_Flags::enable_dynamic_sliders)
        {
            f32 padding = context->theme->padding;
            
            v2f canvas_dim = v2u::Cast<f32>(context->canvas_space_dim);
            
            f32 canvas_height = canvas_dim.y;
            f32 canvas_width = canvas_dim.x;
            f32 canvas_bottom = 0;
            
            Rect bounds = GUI_Get_Bounds_In_Pixel_Space(context);
            
            // Recovery points if sliderds schange effective canvas size.
            Rect bounds_recovery = bounds;
            
            f32 y_factor = 0;
            {
                if(bounds.min.y >= 0)
                    bounds.min.y = 0;
                else
                    bounds.min.y -= padding;
                
                if(bounds.max.y < canvas_height)
                    bounds.max.y = canvas_height;
                
                else
                {
                    bounds.max.y += padding;
                    y_factor = bounds.max.y - canvas_height;
                }
            }
            
            f32 x_factor = 0;
            {
                if(bounds.max.x < canvas_width)
                    bounds.max.x = canvas_width;
                
                else
                    bounds.max.x += padding;
                
                if(bounds.min.x >= 0)
                    bounds.min.x = 0;
                
                else
                {
                    bounds.min.x -= padding;
                    x_factor = 0 - bounds.min.x;
                }
            }
            
            v2f selected_element_half_dim = context->selected_element_dim / 2;
            
            // NOTE: Done up here as the window sliders can cause selected_element records to be modified.
            f32 selected_element_max_y = context->selected_element_pos.y + selected_element_half_dim.y;
            f32 selected_element_min_y = context->selected_element_pos.y - selected_element_half_dim.y;    
            f32 selected_element_max_x = context->selected_element_pos.x + selected_element_half_dim.x;
            f32 selected_element_min_x = context->selected_element_pos.x - selected_element_half_dim.x;    
            
            bool shift_down = context->platform->Get_Keyboard_Key_Down(Key_Code::LSHIFT) || 
                context->platform->Get_Keyboard_Key_Down(Key_Code::LSHIFT);
            
            // Dynamic sliders!
            {        
                f32 slider_girth = context->dynamic_slider_girth;
                f32 gui_height = bounds.max.y - bounds.min.y;
                
                bool enable_vertical_slider = gui_height > canvas_height && 
                    canvas_dim.x > slider_girth &&
                    canvas_dim.y > context->theme->outline_thickness * 2.f;
                
                if(enable_vertical_slider)
                {
                    canvas_width -= slider_girth;
                    
                    // Recover max.x and recalc.
                    bounds.max.x = bounds_recovery.max.x;
                    if(bounds.max.x < canvas_width)
                        bounds.max.x = canvas_width;
                    else
                        bounds.max.x += padding;
                }
                
                f32 gui_width = bounds.max.x - bounds.min.x;
                bool enable_horizontal_slider = gui_width > canvas_width && 
                    canvas_dim.y > slider_girth &&
                    canvas_dim.x > context->theme->outline_thickness * 2.f;
                
                if(enable_horizontal_slider)
                {
                    f32 canvas_bottom_and_height = canvas_height;
                    canvas_bottom = slider_girth;
                    canvas_height -= slider_girth;
                    
                    // Recover min/max.y and recalc.
                    bounds.max.y = bounds_recovery.max.y;
                    bounds.min.y = bounds_recovery.min.y;
                    {
                        if(bounds.min.y >= canvas_bottom)
                        {
                            bounds.min.y = canvas_bottom;
                        }
                        else
                        {   
                            bounds.min.y -= padding;
                        }
                        
                        if(bounds.max.y < canvas_bottom_and_height)
                        {
                            bounds.max.y = canvas_bottom_and_height;
                        }
                        else
                        {
                            y_factor = bounds.max.y - canvas_bottom_and_height;
                        }
                    }
                    
                    gui_height = bounds.max.y - bounds.min.y;
                    // TODO: On a related note, seems like bounds is effectivel unused after this point,
                    // but still updated, so look into deleteting some code related to it.
                    
                    if(!enable_vertical_slider)
                    {
                        enable_vertical_slider = gui_height > canvas_height && 
                            canvas_dim.x > slider_girth &&
                            canvas_dim.y > context->theme->outline_thickness * 2.f;
                        
                        if(enable_vertical_slider)
                        {
                            canvas_width -= slider_girth;
                            
                            // Recover max.x and recalc.
                            bounds.max.x = bounds_recovery.max.x;
                            if(bounds.max.x < canvas_width)
                                bounds.max.x = canvas_width;
                            else
                                bounds.max.x += padding;
                        }
                    }
                }
                
                if(enable_vertical_slider && enable_horizontal_slider)
                {
                    v2f min = v2f{context->canvas->dim.x - slider_girth, 0};
                    v2f dim = v2f{} + slider_girth;
                    
                    Color color = context->theme->outline_color;
                    Draw_Filled_Rect(context->canvas, Create_Rect_Min_Dim(min, dim), color);
                }
                
                f32 mouse_scroll;
                u32 mouse_scroll_mask = 
                    GUI_Context_Flags::disable_mouse_scroll | 
                    GUI_Context_Flags::one_time_disable_kc_navigation |
                    ignore_selection_mask;

                if(Bit_Not_Set(context->flags, mouse_scroll_mask) &&
                    Is_Point_Inside_Rect(context->cursor_fpos, context->canvas_rect) &&
                    Is_Flag_Set(context->platform->Get_Flags(), (u32)App_Flags::is_focused))
                {
                    mouse_scroll = context->platform->Get_Scroll_Wheel_Delta();
                }
                else
                    mouse_scroll = 0;
                
                
                v2f anchor_base = context->anchor_base;
                context->anchor_base = {};
                
                if(enable_vertical_slider)
                {
                    context->layout.anchor = GUI_Anchor::top_right;
                    
                    f32 slider_max = (gui_height / canvas_height) - 1;
                    v2f slider_dim = {slider_girth, canvas_height};
                    f32 slider_value = (anchor_base.y + y_factor) / canvas_height;

                    {
                        u32 auto_layout_bounds_collision_offseting_flag = 
                            context->flags & GUI_Context_Flags::enable_auto_layout_bounds_collision_offseting;
                        
                        Inverse_Bit_Mask(&context->flags, GUI_Context_Flags::enable_auto_layout_bounds_collision_offseting);
                        
                        context->flags |= GUI_Context_Flags::one_time_skip_padding;
                        GUI_Do_Handle_Slider(
                            context, 
                            &GUI_AUTO_TOP_RIGHT, 
                            &slider_dim, 
                            &slider_value,
                            slider_max, 
                            0,
                            GUI_Cardinal_Direction::up_down);
                        
                        context->flags |= auto_layout_bounds_collision_offseting_flag;
                    }
                    
                    f32 local_scroll_v = (!shift_down)? mouse_scroll : 0;
                    f32 scroll_delta = local_scroll_v / canvas_height * context->mouse_scroll_speed;
                    slider_value -= scroll_delta;
                    
                    if(context->flags & GUI_Context_Flags::maxout_vertical_slider)
                        slider_value = slider_max;
                    
                    slider_value = Clamp_Zero_To_Max(slider_value, slider_max);
        
                    anchor_base.y = (slider_value * canvas_height) - y_factor;
                    
                    // Selects the slider when using mouse scroll.
                    if(local_scroll_v != 0)
                        context->selected_index = context->widget_count - 1;
                    
                    
                    if(Bit_Not_Set(context->flags, ignore_selection_mask) && 
                        context->selected_index == context->widget_count - 1)
                    {
                        allow_snap_to_selected = false;
                        context->flags |= GUI_Context_Flags::one_time_disable_kc_navigation;
                        context->flags |= GUI_Context_Flags::disable_wrapping;
                        
                        v2f pos = context->layout.last_element.pos;
                        v2f dim = context->layout.last_element.dim;
                        Rect window_slider_rect;
                        
                        // NOTE: This is an unsafe union access!
                        if(context->selection_state.slider.is_held_down)
                        {
                            window_slider_rect = Create_Rect_Min_Dim(v2f{0}, canvas_dim);
                        }
                        else
                        {
                            window_slider_rect = Create_Rect_Center(pos, dim);
                        }
                        
                        if(Is_Point_Inside_Rect(context->cursor_fpos, window_slider_rect))
                        {
                            context->flags |= GUI_Context_Flags::cursor_mask_validation;
                            context->flags |= GUI_Context_Flags::cursor_mask_enabled;
                            
                            context->cursor_mask_area = window_slider_rect;
                        }
                    }
                }
                else
                {
                    anchor_base.y = 0;
                }
                
                if(enable_horizontal_slider)
                {
                    context->layout.anchor = GUI_Anchor::bottom_left;
                    
                    f32 slider_max = (gui_width / canvas_width) - 1;
                    v2f slider_dim = {canvas_width, slider_girth};
                    f32 slider_value = (-(anchor_base.x - x_factor)) / canvas_width;
                    
                    {
                        u32 auto_layout_bounds_collision_offseting_flag = 
                            context->flags & GUI_Context_Flags::enable_auto_layout_bounds_collision_offseting;
                        
                        Inverse_Bit_Mask(&context->flags, GUI_Context_Flags::enable_auto_layout_bounds_collision_offseting);
                        
                        context->flags |= GUI_Context_Flags::one_time_skip_padding;
                        GUI_Do_Handle_Slider(
                            context, 
                            &GUI_AUTO_BOTTOM_LEFT, 
                            &slider_dim, 
                            &slider_value,
                            slider_max, 
                            0,
                            GUI_Cardinal_Direction::left_right);
                            
                        context->flags |= auto_layout_bounds_collision_offseting_flag;
                    }
                    
                    f32 local_scroll_v = (shift_down)? mouse_scroll : 0;
                    f32 scroll_delta = local_scroll_v / canvas_width * context->mouse_scroll_speed;
                    slider_value -= scroll_delta;
                    
                    if(context->flags & GUI_Context_Flags::maxout_horizontal_slider)
                        slider_value = slider_max;
                    
                    slider_value = Clamp_Zero_To_Max(slider_value, slider_max);
                    
                    anchor_base.x = -(slider_value * canvas_width) + x_factor;
                    
                    // Selects the slider when using mouse scroll.
                    if(local_scroll_v != 0)
                        context->selected_index = context->widget_count - 1;
                    
                    if(Bit_Not_Set(context->flags, ignore_selection_mask) && 
                        context->selected_index == context->widget_count - 1)
                    {
                        allow_snap_to_selected = false;
                        context->flags |= GUI_Context_Flags::one_time_disable_kc_navigation;
                        context->flags |= GUI_Context_Flags::disable_wrapping;
                        
                        v2f pos = context->layout.last_element.pos;
                        v2f dim = context->layout.last_element.dim;
                        Rect window_slider_rect;
                        
                        // NOTE: This is an unsafe union access!
                        if(context->selection_state.slider.is_held_down)
                            window_slider_rect = Create_Rect_Min_Dim(v2f{0}, canvas_dim);
                        
                        else
                        {
                            window_slider_rect = Create_Rect_Center(pos, dim);
                        }
                        
                        if(Is_Point_Inside_Rect(context->cursor_fpos, window_slider_rect))
                        {
                            context->flags |= GUI_Context_Flags::cursor_mask_validation;
                            context->flags |= GUI_Context_Flags::cursor_mask_enabled;
                            
                            context->cursor_mask_area = window_slider_rect;
                        }
                    }
                }
                else
                {
                    anchor_base.x = 0;
                }
                
                context->anchor_base = anchor_base;
            }
            
            if(Bit_Not_Set(context->flags, ignore_selection_mask) 
                && allow_snap_to_selected && context->selected_id)
            {
                f32 true_canvas_height = canvas_dim.y;
            
                f32 padding_2 = padding * 2;
                
                f32 padded_e_height = selected_element_max_y - selected_element_min_y + padding_2;
                if(padded_e_height < canvas_height)
                {
                    // Selection below the canvas.
                    if(selected_element_min_y < canvas_bottom)
                    {
                        f32 dif = canvas_bottom - selected_element_min_y + padding;
                        context->anchor_base.y += dif;
                    }
                    
                    // Selection above the canvas.
                    else if(selected_element_max_y > true_canvas_height)
                    {
                        f32 dif = true_canvas_height - selected_element_max_y - padding;
                        context->anchor_base.y += dif;
                    }
                    
                }
                
                f32 padded_e_width = selected_element_max_x - selected_element_min_x + padding_2;
                if(padded_e_width < canvas_width)
                {
                    // Selection left the canvas.
                    if(selected_element_min_x < 0)
                    {
                        f32 dif = 0 - selected_element_min_x + padding;
                        context->anchor_base.x += dif;
                    }
                    
                    // Selection right the canvas.
                    else if(selected_element_max_x > canvas_width)
                    {
                        f32 dif = canvas_width - selected_element_max_x - padding;
                        context->anchor_base.x += dif;
                    }
                }
            }
        }
        
        // Reset state.
        {
            context->last_cursor_position = context->cursor_position;
            
            u32 reset_mask = 
                GUI_Context_Flags::context_ready | 
                GUI_Context_Flags::maxout_horizontal_slider | 
                GUI_Context_Flags::maxout_vertical_slider;
            
            Inverse_Bit_Mask(&context->flags, reset_mask);
            
            if(context->_master_id)
            {
                context->active_context_id = context->_master_id;
                context->_master_id = 0;
            }
        }
    }
    else if(context->mode == GUI_Mode::layout_only)
    {
        Inverse_Bit_Mask(&context->flags, GUI_Context_Flags::context_ready);
    }
    else
    {
        Terminate;
    }
}


// Generates a non zero id based on the position, dimensions and some mangle factor provided
// by the calling code. Usually that would be the line number from the __LINE__ macro.
// There is really no gurantee that there wont be ID collissions. This really is just hoping
// the program is correct, where randomly it might not be.
// TODO: upgrade to use a 64 bite noise function to at least reduce the change of collissions.
static u32 GUI_Generate_ID(Rect rect, u32 mangle_factor)
{
    v2i np = v2f::Cast<i32>(Round(rect.min + rect.max));
    u32 id;
    do
    {
        id = 1 + mangle_factor + Noise_Squirrel3_2D(np, mangle_factor++);
    }while(!id);
    
    return id;
}


static inline bool GUI_Is_Element_Selected(GUI_Context* context, bool cursor_on_selection, u32 id)
{
    // TODO: Again re-think this.. just kind of awfull.
    bool result = false;
    
    // This is the selected element.
    if(Bit_Not_Set(context->flags, GUI_Make_Ignore_Selection_Mask()) &&
        context->selected_index == context->widget_count)
    {
        // TODO: CONSIDER, is ID even something we want to have?
        // Buuut, it seems to be a different widget?
        bool not_ignore_id = Bit_Not_Set(context->flags, GUI_Context_Flags::one_time_ignore_id);
        if(id != context->selected_id && not_ignore_id)
        {
            GUI_Reset_Selection_State(context);
            context->selected_id = id;
        }
        
        context->selected_element_pos = context->layout.last_element.pos;
        context->selected_element_dim = context->layout.last_element.dim;
        
        result = true;
    }
    else if(cursor_on_selection && 
        (context->cursor_position != context->last_cursor_position || 
        context->actions[GUI_Menu_Actions::mouse].Is_Down()) &&
        Is_Point_Inside_Rect(context->cursor_fpos, context->canvas_rect) &&
        !((context->flags & GUI_Context_Flags::cursor_mask_enabled) && 
        Is_Point_Inside_Rect(context->cursor_fpos, context->cursor_mask_area)) &&
        Bit_Not_Set(context->flags, GUI_Context_Flags::hard_ignore_selection)
    )
    {
        GUI_Reset_Selection_State(context);
        context->selected_index = context->widget_count;
        context->selected_id = id;
        
        context->selected_element_pos = context->layout.last_element.pos;
        context->selected_element_dim = context->layout.last_element.dim;
        result = true;
        
        GUI_Context::active_context_id = context->_context_id;
        Inverse_Bit_Mask(&context->flags, GUI_Context_Flags::soft_ignore_selection);
    }        
    
    Inverse_Bit_Mask(&context->flags, GUI_Context_Flags::one_time_ignore_id);
    context->widget_count += 1; // Suprising side effect! But better to do it here than forget to do it ouside.
    return result;
}


static inline bool GUI_Is_Static_Element_Highlighted(GUI_Context* context, GUI_Highlight highlight)
{
    bool result = (Bit_Not_Set(context->flags, GUI_Make_Ignore_Selection_Mask()) && 
        highlight.count && context->selected_index >= highlight.idx && 
        context->selected_index < highlight.idx + highlight.count) ||
        highlight.count == I32_MAX && highlight.idx == 0;
        
    return result;
}



static inline bool GUI_On_Release_Action(
    GUI_Context* context, bool cursor_on_selection, bool* is_pressed_down)
{
    bool pressed = context->actions[GUI_Menu_Actions::enter].Is_Pressed() ||
        (cursor_on_selection && context->actions[GUI_Menu_Actions::mouse].Is_Pressed());
    
    bool released = context->actions[GUI_Menu_Actions::enter].Is_Released() ||
        (cursor_on_selection && context->actions[GUI_Menu_Actions::mouse].Is_Released());
    
    if(pressed)
    {
        *is_pressed_down = true;
    }
    else if(*is_pressed_down && released)
    {
        *is_pressed_down = false;
        return true;
    }
    else if(context->actions[GUI_Menu_Actions::mouse].Is_Up() && 
        context->actions[GUI_Menu_Actions::enter].Is_Up())
    {
        *is_pressed_down = false;
    }
    
    return false;
}


static bool GUI_Input_Field_Begin_Or_End_Text_Select_Mode(GUI_SL_Input_Field_State* state, bool shift_down, u32 wcp)
{
    bool pre_text_select = state->text_select_mode;
    u32 pre_start_point = state->text_select_start_point;
    if(shift_down)
    {
        if(!state->text_select_mode)
        {
            state->text_select_mode = true;
            state->text_select_start_point = wcp;
        }
    }
    else
        state->text_select_mode = false;
    
    // NOTE: this is for not doing kc movement when "unselecting".
    return pre_text_select && !state->text_select_mode && pre_start_point != state->write_cursor_position;
}


static bool GUI_Was_Last_Selected(GUI_Context* context)
{
    u32 mask = GUI_Make_Ignore_Selection_Mask();
    
    bool result = 
        context->selected_index == context->widget_count - 1 && Bit_Not_Set(context->flags, mask);
    
    return result;
}


static bool GUI_Input_Field_Insert_Characters(
    Platform_Calltable* platform, 
    GUI_SL_Input_Field_State* state, 
    String* str,
    u32 character_limit,
    u32 wcp,
    bool allow_new_line_insertion,
    bool (*character_check)(char*))
{
    bool(*Is_Allowed_Character)(char, bool) = [](char c, bool allow_new_line)
    {
        bool result = 
            (c >= 32 && c <= 127) || 
            c == -28 ||
            c == -60 ||
            c == -10 ||
            c == -42 ||
            (allow_new_line && c == '\t') ||
            (allow_new_line && c == '\n');
        
        return result;
    };
    
    void(*Conditional_Erase_Selection)(GUI_SL_Input_Field_State*, String*) =
        [](GUI_SL_Input_Field_State* state, String* str)
    {
        if(state->text_select_mode)
        {
            u32 p0 = state->write_cursor_position;
            u32 p1 = state->text_select_start_point;
            
            if(p0 != p1)
            {
                if(p0 > p1)
                    Swap(&p0, &p1);
                
                str->erase(p0, p1);
                
                state->write_cursor_position = p0;
            }
            
            state->text_select_mode = false;
        }
    };
    
    
    Char_Array typing_info = platform->Get_Typing_Information();
    
    bool result = false;
    
    
    for(u32 i = 0; i < typing_info.count; ++i)
    {
        char c = typing_info.buffer[i];
        if(c == '\r')
            c = '\n';
        
        switch(c)
        {
            case '\x1': // CTRL A (Select all)
            {
                state->write_cursor_position = str->lenght;
                state->text_select_start_point = 0;
                state->text_select_mode = true;
                
            }break;
            
            
            case '\x3': // CTRL C (Copy selection to clipboard)
            {
                if(state->text_select_mode)
                {
                    u32 p0 = state->write_cursor_position;
                    u32 p1 = state->text_select_start_point;
                    
                    if(p0 != p1)
                    {
                        if(p0 > p1)
                            Swap(&p0, &p1);
                        
                        char* buffer_start = str->buffer + p0;
                        u32 buffer_lenght = p1 - p0;
                        
                        platform->Set_Clipboard_Data_As_Text(buffer_start, buffer_lenght);
                    }
                }
                
            }break;
            
            
            case '\x18': // CTRL X (Copy selection to clipboard, then delete selection)
            {
                if(state->text_select_mode)
                {
                    u32 p0 = state->write_cursor_position;
                    u32 p1 = state->text_select_start_point;
                    
                    if(p0 != p1)
                    {
                        if(p0 > p1)
                            Swap(&p0, &p1);
                        
                        char* buffer_start = str->buffer + p0;
                        u32 buffer_lenght = p1 - p0;
                        
                        platform->Set_Clipboard_Data_As_Text(buffer_start, buffer_lenght);
                        
                        str->erase(p0, p1);
                        state->write_cursor_position = p0;
                        state->text_select_mode = false;
                    }
                }
                
            }break;
            
            
            case '\x16': // CTRL V (Paste from clipboard)
            {
                Conditional_Erase_Selection(state, str);
                
                char* clip_buffer = platform->Get_Clipboard_Data_As_Text();
                if(clip_buffer)
                {
                    for(char cb = *clip_buffer; cb; ++clip_buffer, cb = *clip_buffer)
                    {
                        #if 0 // this doens't do anthing? Just delete?
                        if(cb == '\r')
                        {
                            //cb = '\n';
                            int a = 0;
                        }
                        #endif
                        
                        if(Is_Allowed_Character(cb, allow_new_line_insertion) && 
                            (character_limit == 0 || str->lenght < character_limit) &&
                            (!character_check || (character_check && character_check(&cb))))
                        {
                            if(cb == '\t')
                            {
                                cb = ' ';
                                u32 insert_count = 4;
                                for(u32 y = 0; y < insert_count; ++y)
                                {
                                    str->insert_at(state->write_cursor_position, cb);
                                    state->write_cursor_position += 1;
                                }
                            }
                            else
                            {
                                str->insert_at(state->write_cursor_position, cb);
                                state->write_cursor_position += 1;
                            }
                            
                        }
                    }
                }
                
            }break;
            
            
            case '\n': // ENTER hopefully, depends on the OS?
            {
                result = true;
                if(allow_new_line_insertion)
                {
                    goto DEFAULT_CASE;
                }
            }break;
            
            
            case '\b': // BACKSPACE
            {
                if(state->text_select_mode && state->write_cursor_position != state->text_select_start_point) 
                {
                    Conditional_Erase_Selection(state, str);
                    continue;
                }
                state->text_select_mode = false;
                
                if(str->lenght > 0)
                {
                    state->write_cursor_position -= 1;
                    if(state->write_cursor_position > wcp)
                        state->write_cursor_position = 0;
                    
                    str->remove_at(state->write_cursor_position);
                }
                
            }break;
            
            
            default:
            {
                DEFAULT_CASE:
                
                if(Is_Allowed_Character(c, allow_new_line_insertion) && 
                    (character_limit == 0 || str->lenght < character_limit) &&
                    (!character_check || (character_check && character_check(&c))))
                {
                    Conditional_Erase_Selection(state, str);
                    
                    if(c == '\t')
                    {
                        c = ' ';
                        u32 insert_count = 4;
                        for(u32 y = 0; y < insert_count; ++y)
                        {
                            str->insert_at(state->write_cursor_position, c);
                            state->write_cursor_position += 1;
                        }
                    }
                    else
                    {
                        str->insert_at(state->write_cursor_position, c);
                        state->write_cursor_position += 1;
                    }
                    
                }
            }
        }
    }
    
    return result;
}


static void GUI_Do_Spacing(GUI_Context* context, v2f* dim)
{
    Assert(GUI_Is_Context_Ready(context));
    
    Rect bounds = context->bounds_rel_anchor_base;
    
    context->flags |= GUI_Context_Flags::one_time_skip_padding;
    if(dim)
    {
        v2f d = *dim;
        if(d.x == 0)
            d.x = context->layout.last_element.dim.x;
        
        if(d.y == 0)
            d.y = context->layout.last_element.dim.y;
        
        GUI_Get_Placement(context, &d, 0);
    }
    else // AUTO case
    {        
        GUI_Get_Placement(context, dim, 0);
    }
    
    context->bounds_rel_anchor_base = bounds;
}


static inline void GUI_Do_Spacing(GUI_Context* context, v2f dim)
{
    GUI_Do_Spacing(context, &dim);
}


static void GUI_Do_Text(
    GUI_Context* context, 
    v2f* pos, 
    char* text,
    GUI_Highlight highlight = GUI_Highlight_Nothing(),
    v2f text_scale = GUI_DEFAULT_TEXT_SCALE,
    Color* color_override = 0)
{
    Assert(GUI_Is_Context_Ready(context));
    
    // --------------------------------------------------------------------------
    
    GUI_Theme* theme = context->theme;
    
    v2f dim = GUI_Tight_Fit_Text(text, &theme->font, text_scale);
    
    GUI_Placement p = GUI_Get_Placement(context, &dim, pos);
    if(!Is_Rect_Valid(p.rect) || context->mode == GUI_Mode::layout_only)
        return;
    
    // --------------------------------------------------------------------------
    
    if(Rects_Overlap(p.rect, context->canvas_rect))
    {
        Color color;
        if(color_override)
        {
            color = *color_override;
        }
        else
        {
            color = GUI_Is_Static_Element_Highlighted(context, highlight)?
                theme->selected_color : theme->text_color;
        }
        
        v2f text_p = GUI_Calc_Centered_Text_Position(text, text_scale, p.pos, &theme->font);
        Draw_Text(context->canvas, (u8*)text, text_p, color, &theme->font, text_scale);
    }
}


static inline void GUI_Do_Text(
    GUI_Context* context, 
    v2f* pos, 
    char* text, 
    Color* color_override,
    v2f text_scale = GUI_DEFAULT_TEXT_SCALE)
{
    GUI_Do_Text(context, pos, text, {}, text_scale, color_override);
}


static inline void GUI_Do_Title_Text(
    GUI_Context* context, 
    v2f* pos, 
    char* text, 
    v2f text_scale = Hadamar_Product(GUI_DEFAULT_TEXT_SCALE, GUI_DEFAULT_TITLE_SCALER))
{
    GUI_Do_Text(context, pos, text, {}, text_scale, &context->theme->title_color);
}


static void GUI_Do_Panel(
    GUI_Context* context, 
    v2f* pos, 
    v2f* dim, 
    Color* bg_color_ptr = 0,
    GUI_Highlight highlight = GUI_Highlight_Nothing())
{
    // --------------------------------------------------------------------------
    GUI_Placement p = GUI_Get_Placement(context, dim, pos);
    if(!Is_Rect_Valid(p.rect) || context->mode == GUI_Mode::layout_only)
        return;
    // --------------------------------------------------------------------------
    
    // Draw
    if(Rects_Overlap(p.rect, context->canvas_rect))
    {
        GUI_Theme* theme = context->theme;
        
        Color bg_color = bg_color_ptr? *bg_color_ptr : theme->background_color;
        Color outline_color = GUI_Is_Static_Element_Highlighted(context, highlight)?
            theme->selected_color : theme->outline_color;
        
        Draw_Filled_Rect_With_Outline(
            context->canvas, 
            p.rect, 
            bg_color,
            theme->outline_thickness, 
            outline_color);
    }
}


static void GUI_Do_Panel(GUI_Context* context, 
    Rect rect, 
    Color* bg_color_ptr = 0,
    GUI_Highlight highlight = GUI_Highlight_Nothing())
{
    v2f dim = Get_Rect_Dimensions(rect);
    v2f pos = rect.min + dim * 0.5f;
    
    GUI_Anchor a = context->layout.anchor;
    context->layout.anchor = GUI_Anchor::center;
    
    //NOTE: Calling the regular override that calls GUI_Get_Placement seems silly since,
    // we already have a rect,
    // BUT it does additional book keeping (for auto layout stuff) so by-passing it,
    // would be more trouble than it's worth.
    GUI_Do_Panel(context, &pos, &dim, bg_color_ptr, highlight);
    
    context->layout.anchor = a;
}


static void GUI_Do_Image_Panel(GUI_Context* context, v2f* pos, v2f* dim, Image* img)
{
    // --------------------------------------------------------------------------
    GUI_Placement p = GUI_Get_Placement(context, dim, pos);
    if(!Is_Rect_Valid(p.rect) || context->mode == GUI_Mode::layout_only)
        return;
    // --------------------------------------------------------------------------
    
    // Draw
    if(Rects_Overlap(p.rect, context->canvas_rect))
    {
        Draw_Filled_Rect_With_Outline(
            context->canvas, 
            p.rect, 
            context->theme->background_color,
            context->theme->outline_thickness, 
            context->theme->outline_color);
        
        if(img->buffer)
        {
            Rect img_rect = Shink_Rect(p.rect, f32(context->theme->outline_thickness));
            Draw_Image(context->canvas, img, img_rect);
        }
    }
}


static bool GUI_Do_Button(
    GUI_Context* context, 
    v2f* pos, 
    v2f* dim, 
    char* text, 
    v2f text_scale = GUI_DEFAULT_TEXT_SCALE,
    Color* background_color_override = 0)
{
    Assert(GUI_Is_Context_Ready(context));
    
    // --------------------------------------------------------------------------
    
    GUI_Theme* theme = context->theme;
    
    if(dim == &GUI_AUTO_FIT)
    {
        *dim = GUI_Tight_Fit_Text(text, &theme->font, text_scale) + theme->padding;
    }
    
    GUI_Placement p = GUI_Get_Placement(context, dim, pos);
    if(!Is_Rect_Valid(p.rect) || context->mode == GUI_Mode::layout_only)
        return false;
    
    u32 id = GUI_Generate_ID(p.rect, __LINE__);
    
    bool cursor_on_selection = Is_Point_Inside_Rect(context->cursor_fpos, p.rect);
    bool is_selected = GUI_Is_Element_Selected(context, cursor_on_selection, id);
    
    // --------------------------------------------------------------------------
    
    bool result = false;
    
    Color outline_color = outline_color = theme->outline_color;
    
    if(is_selected)
    {
        GUI_Button_State* state = &context->selection_state.button;
        
        // Handle input.
        if(GUI_On_Release_Action(context, cursor_on_selection, &state->is_pressed_down))
        {
            result = true;
        }
        
        if(state->is_pressed_down)
            outline_color = theme->down_color;
        else
            outline_color = theme->selected_color;
    }
    
    // Draw
    if(Rects_Overlap(p.rect, context->canvas_rect))
    {
        Draw_Filled_Rect_With_Outline(
            context->canvas, 
            p.rect, 
            (background_color_override)? *background_color_override : theme->background_color,
            theme->outline_thickness, 
            outline_color);
        
        if(text)
        {
            Color text_color = is_selected? outline_color : theme->widget_text_color;
            
            v2f text_p = GUI_Calc_Centered_Text_Position(text, text_scale, p.pos, &theme->font);
            Draw_Text(context->canvas, (u8*)text, text_p, text_color, &theme->font, text_scale);
        }
    }
    
    return result;
}


static bool GUI_Do_Image_Button(
    GUI_Context* context, 
    v2f* pos, 
    v2f* dim, 
    Image* img,
    v2f img_scale = v2f{1.f, 1.f},
    GUI_Theme_Coloring apply_theme_color_to_image = GUI_Theme_Coloring::apply)
{
    Assert(GUI_Is_Context_Ready(context));
    
    // --------------------------------------------------------------------------
    
    GUI_Theme* theme = context->theme;
    if(dim == &GUI_AUTO_FIT)
    {
        Assert(img);
        
        *dim = v2f{
            f32(img->dim.x + context->theme->outline_thickness), 
            f32(img->dim.y + context->theme->outline_thickness),    
            };
    }
    
    GUI_Placement p = GUI_Get_Placement(context, dim, pos);
    if(!Is_Rect_Valid(p.rect) || context->mode == GUI_Mode::layout_only)
        return false;
    
    u32 id = GUI_Generate_ID(p.rect, __LINE__);
    
    bool cursor_on_selection = Is_Point_Inside_Rect(context->cursor_fpos, p.rect);
    bool is_selected = GUI_Is_Element_Selected(context, cursor_on_selection, id);
    
    // --------------------------------------------------------------------------
    
    bool result = false;
    
    Color outline_color = outline_color = theme->outline_color;
    
    if(is_selected)
    {
        GUI_Button_State* state = &context->selection_state.button;
        
        // Handle input.
        if(GUI_On_Release_Action(context, cursor_on_selection, &state->is_pressed_down))
        {
            result = true;
        }
        
        if(state->is_pressed_down)
            outline_color = theme->down_color;
        else
            outline_color = theme->selected_color;
    }
    
    // Draw
    if(Rects_Overlap(p.rect, context->canvas_rect))
    {
        Draw_Filled_Rect_With_Outline(
            context->canvas, 
            p.rect, 
            theme->background_color,
            theme->outline_thickness, 
            outline_color);
        
        if(img)
        {
            Rect img_rect = Shink_Rect(p.rect, f32(context->theme->outline_thickness));
            v2f img_rect_dim = Get_Rect_Dimensions(img_rect);
            img_rect_dim = Hadamar_Product(img_rect_dim, img_scale);
            img_rect = Create_Rect_Center(p.pos, img_rect_dim);
            
            if(apply_theme_color_to_image == GUI_Theme_Coloring::apply)
            {    
                Draw_Image(
                    context->canvas, 
                    img, 
                    img_rect,
                    Unpack_Color(outline_color) / 255.f);                
            }
            else
            {
                Draw_Image(
                    context->canvas, 
                    img, 
                    img_rect);
            }
        }
    }
    
    return result;
}


static bool GUI_Do_Fill_Slider(
    GUI_Context* context, 
    v2f* pos, 
    v2f* dim, 
    f32* value,
    f32 max = 1.f, 
    f32 min = 0.f, 
    f32 step = 0.01f,
    GUI_Input_Acceleration_Behavior inp_accel = GUI_Input_Acceleration_Behavior())
{
    Assert(GUI_Is_Context_Ready(context));
    
    Assert(step > 0);
    Assert(max > min);
    Assert(value);
    
    // --------------------------------------------------------------------------
    
    GUI_Placement p = GUI_Get_Placement(context, dim, pos);
    if(!Is_Rect_Valid(p.rect) || context->mode == GUI_Mode::layout_only)
        return false;
    
    u32 id = GUI_Generate_ID(p.rect, __LINE__);
    
    bool cursor_on_selection = Is_Point_Inside_Rect(context->cursor_fpos, p.rect);
    bool is_selected = GUI_Is_Element_Selected(context, cursor_on_selection, id);
    
    GUI_Theme* theme = context->theme;
    
    // --------------------------------------------------------------------------
    
    f32 pre_val = *value;
    
    *value = Min(*value, max);
    *value = Max(*value, min);
    
    Color bar_color; 
    
    if(is_selected)
    {
        GUI_Slider_State* state = &context->selection_state.slider;
        bar_color = theme->selected_color;
        
        f64 time = context->platform->Get_Time_Stamp();
        
        // Handle mouse input -----------------
        if(context->actions[GUI_Menu_Actions::mouse].Is_Pressed() && cursor_on_selection)
            state->is_held_down = true;
        
        if(context->actions[GUI_Menu_Actions::mouse].Is_Up())
            state->is_held_down = false;
        
        if(state->is_held_down)
        {
            context->cursor_mask_area = context->canvas_rect;
            context->flags |= GUI_Context_Flags::cursor_mask_enabled;
            context->flags |= GUI_Context_Flags::cursor_mask_validation;
            
            f32 internal_width = p.dim.x - (f32(theme->outline_thickness) * 2);
            f32 rel_cursor_x = Min(internal_width, Max(0.f, context->cursor_fpos.x - p.rect.min.x));
            
            f32 fill_percent = rel_cursor_x / internal_width;
            f32 fill = (max - min) * fill_percent;
            i32 steps = Round_To_Signed_Int32(fill / step);
            
            *value = min + steps * step;
        }
        
        // Handle input Keyboard/Controller.
        Action* left = context->actions + GUI_Menu_Actions::left;
        Action* right = context->actions + GUI_Menu_Actions::right; 
        
        if(left->Is_Pressed() || right->Is_Pressed())
        {
            state->input_start_time = 0;
            state->next_input_time = 0;
        }    
    
        while(left->Is_Down() &&
            GUI_Accelerated_Tick(&inp_accel, time, &state->input_start_time, &state->next_input_time))
        {
            *value = Max(*value - step, min);
        }
        
        while(right->Is_Down() &&
            GUI_Accelerated_Tick(&inp_accel, time, &state->input_start_time, &state->next_input_time))
        {        
            *value = Min(*value + step, max);
        }
    }
    else
    {
        bar_color = theme->outline_color;
    }
    
    // Draw
    if(Rects_Overlap(p.rect, context->canvas_rect))
    {
        f32 d = max - min;
        Assert(d > 0);
        
        f32 fill = 0;
        if(d != 0)
            fill = (*value - min) / d;
        
        Draw_Percentile_Bar(
            context->canvas,
            p.rect,
            theme->background_color,
            theme->outline_thickness,
            bar_color,
            bar_color,
            fill);        
    }
    
    return pre_val != *value;
}


static bool GUI_Do_Handle_Slider(
    GUI_Context* context, 
    v2f* pos, 
    v2f* dim, 
    f32* value,
    f32 max,
    f32 min,
    GUI_Cardinal_Direction cardinal_dir,
    f32 step_count,
    GUI_Input_Acceleration_Behavior inp_accel)
{
    Assert(GUI_Is_Context_Ready(context));
    
    static constexpr f32 min_handle_width = 3.f;
    
    f32 pre_val = *value;
    
    if(*value < min)
        *value = min;
    
    if(*value > max)
        *value = max;
    
    // --------------------------------------------------------------------------
    
    GUI_Placement p = GUI_Get_Placement(context, dim, pos);
    if(!Is_Rect_Valid(p.rect) || context->mode == GUI_Mode::layout_only)
        return false;
    
    u32 id = GUI_Generate_ID(p.rect, __LINE__);
    
    bool cursor_on_selection = Is_Point_Inside_Rect(context->cursor_fpos, p.rect);
    bool is_selected = GUI_Is_Element_Selected(context, cursor_on_selection, id);
    
    GUI_Theme* theme = context->theme;
    
    // --------------------------------------------------------------------------
    
    Color outline_color = theme->outline_color;
    Color handle_color = theme->selected_color;
    
    v2f internal_dim = p.dim - (f32(theme->outline_thickness) * 2);
    
    f32 range = max - min;
    f32 possibility_space = range + 1;
    v2f handle_pos = p.pos;
    
    f32 bar_internal_lenght = internal_dim.elements[u8(cardinal_dir)];
    
    if(step_count == 0)
        step_count = bar_internal_lenght;
    
    
    f32 handle_lenght = Max(bar_internal_lenght / possibility_space, min_handle_width);
    f32 half_handle_lenght = handle_lenght / 2;
    f32 slidable_lenght = bar_internal_lenght - handle_lenght;
    if(slidable_lenght <= 0)
        return false;
    
    f32 divizor = possibility_space - 1;
    
    if(divizor == 0) // CONSIDER: Epsilon?
        divizor = 1;
    
    f32 directional_offset = 
        (-bar_internal_lenght / 2 + half_handle_lenght) + ((*value - min) * (slidable_lenght / divizor));
    
    v2f handle_dim;
    
    switch(cardinal_dir)
    {
        case GUI_Cardinal_Direction::left_right:
        {
            handle_dim = v2f{handle_lenght, internal_dim.y};
            handle_pos += v2f{directional_offset, 0};
        }break;
        
        
        case GUI_Cardinal_Direction::up_down:
        {
            handle_dim = v2f{internal_dim.x, handle_lenght};
            handle_pos += v2f{0, -directional_offset};
        }break;
    }
    
    Rect handle_rect = Create_Rect_Center(handle_pos, handle_dim);
    
    bool cursor_on_handle = Is_Point_Inside_Rect(context->cursor_fpos, handle_rect);
    if(is_selected)
    {
        if(cursor_on_handle)
        {
            handle_color = theme->title_color;
        }
        
        GUI_Slider_State* state = &context->selection_state.slider;
        
        outline_color = theme->selected_color;
        f32 step = possibility_space / step_count;
        
        // Handle mouse input -----------------
        if(context->actions[GUI_Menu_Actions::mouse].Is_Pressed() && cursor_on_selection)
        {
            if(cursor_on_handle)
                state->drag_offset = context->cursor_fpos - handle_pos;
            else
                state->drag_offset = v2f{0};
            
            state->is_held_down = true;
        }
        
        
        if(context->actions[GUI_Menu_Actions::mouse].Is_Up())
            state->is_held_down = false;
        
        
        if(state->is_held_down)
        {
            f32 cursor_pos_on_sliding_axis = context->cursor_fpos.elements[u8(cardinal_dir)];
            cursor_pos_on_sliding_axis -= state->drag_offset.elements[u8(cardinal_dir)];
            
            f32 y_flip = (1 - (f32(cardinal_dir) * 2));
            
            f32 rel_cursor = 
                cursor_pos_on_sliding_axis - (p.pos.elements[u8(cardinal_dir)] - (bar_internal_lenght / 2));
            
            rel_cursor -= half_handle_lenght * y_flip;
            
            rel_cursor = (f32(cardinal_dir) * bar_internal_lenght) + (rel_cursor * y_flip);
            
            f32 percent = rel_cursor / slidable_lenght;
            
            f32 v = (percent * range);
            f32 steps = Round(v / step);
            v = step * steps;
            
            v += min;
            v = Min(v, max);
            v = Max(v, min);
            
            *value = v;
        }
        
        f64 time = context->platform->Get_Time_Stamp();
        
        // Handle input Keyboard/Controller.
        Action* left = context->actions + GUI_Menu_Actions::left;
        Action* right = context->actions + GUI_Menu_Actions::right; 
        
        if(left->Is_Pressed() || right->Is_Pressed())
        {
            state->input_start_time = 0;
            state->next_input_time = 0;
        }    
        
        if(left->Is_Down())
        {
            handle_color = theme->title_color;
            while(GUI_Accelerated_Tick(&inp_accel, time, &state->input_start_time, &state->next_input_time))
            {
                f32 new_value = *value - step;
                *value = Min(max, Max(min, new_value));
            }
        }

        
        if(right->Is_Down())
        {
            handle_color = theme->title_color;
            while(GUI_Accelerated_Tick(&inp_accel, time, &state->input_start_time, &state->next_input_time))
            {
                f32 new_value = *value + step;
                *value = Min(max, Max(min, new_value));
            }
        }
    }
    
    // Draw
    if(Rects_Overlap(p.rect, context->canvas_rect))
    {
        Draw_Filled_Rect_With_Outline(
            context->canvas, 
            p.rect, 
            theme->background_color,
            theme->outline_thickness, 
            outline_color);
        
        Draw_Filled_Rect(context->canvas, handle_rect, handle_color);
    }
    
    return pre_val != *value;
}    


static bool GUI_Do_Checkbox(GUI_Context* context, v2f* pos, v2f* dim, bool* value)
{
    Assert(GUI_Is_Context_Ready(context));
    
    // --------------------------------------------------------------------------
    
    GUI_Placement p = GUI_Get_Placement(context, dim, pos);
    if(!Is_Rect_Valid(p.rect) || context->mode == GUI_Mode::layout_only)
        return false;
    
    u32 id = GUI_Generate_ID(p.rect, __LINE__);
    
    bool cursor_on_selection = Is_Point_Inside_Rect(context->cursor_fpos, p.rect);
    bool is_selected = GUI_Is_Element_Selected(context, cursor_on_selection, id);
    
    GUI_Theme* theme = context->theme;
    
    // --------------------------------------------------------------------------
    
    bool pre_val = *value;
    
    
    Color outline_color = theme->outline_color;
    if(is_selected)
    {
        GUI_Button_State* state = &context->selection_state.button;
        
        if(GUI_On_Release_Action(context, cursor_on_selection, &state->is_pressed_down))
        {
            *value = !(*value);
        }
        
        if(state->is_pressed_down)
            outline_color = theme->down_color;
        else
            outline_color = theme->selected_color;
    }
    
    // Draw
    {        
        Draw_Filled_Rect_With_Outline(
            context->canvas, 
            p.rect, 
            theme->background_color,
            theme->outline_thickness, 
            outline_color);

        if(*value)
        {
            // half here on purpose
            Rect rect = Create_Rect_Center(p.pos, p.dim * 0.5f);
            Draw_Filled_Rect(context->canvas, rect, outline_color);
        }
    }
    
    return pre_val != *value;
}


static u32 GUI_Do_Dropdown_Button(
    GUI_Context* context, 
    v2f* pos,
    v2f* dim,
    char* text,
    u32 element_count,
    char** element_names,
    v2f text_scale = GUI_DEFAULT_TEXT_SCALE)
{
    Assert(GUI_Is_Context_Ready(context));
    
    Assert(element_count > 0);
    
    // --------------------------------------------------------------------------
    
    GUI_Theme* theme = context->theme;
    
    if(dim == &GUI_AUTO_FIT)
    {
        u32 button_text_lenght = Null_Terminated_Buffer_Lenght(text);
        u32 longest_sub_lenght = Null_Terminated_Buffer_Lenght(element_names[0]);
        u32 longest_sub_idx = 0;
        
        for(u32 i = 1; i < element_count; ++i)
        {
            u32 text_lenght = Null_Terminated_Buffer_Lenght(element_names[i]);
            if(text_lenght > longest_sub_lenght)
            {
                longest_sub_lenght = text_lenght;
                longest_sub_idx = i;
            }
        }
        
        char* longest_text = (button_text_lenght >= longest_sub_lenght)? 
            text : element_names[longest_sub_idx];
        
        *dim = GUI_Tight_Fit_Text(longest_text, &theme->font, text_scale) + theme->padding;
    }
    
    
    GUI_Placement p = GUI_Get_Placement(context, dim, pos);
    if(!Is_Rect_Valid(p.rect) || context->mode == GUI_Mode::layout_only)
        return false;
    
    
    Rect id_rect = {p.rect.min - context->anchor_base, p.rect.max - context->anchor_base};
    
    u32 id = GUI_Generate_ID(id_rect, __LINE__);
    
    bool cursor_on_selection = Is_Point_Inside_Rect(context->cursor_fpos, p.rect);
    bool is_selected = GUI_Is_Element_Selected(context, cursor_on_selection, id);
    
    // --------------------------------------------------------------------------
    
    Color outline_color = outline_color = theme->outline_color;
    
    u32 result = 0;
    
    f32 half_dim_y = p.dim.y / 2;
    v2f open_center = v2f{p.pos.x, p.pos.y - (half_dim_y * f32(element_count) - half_dim_y)};
    v2f open_dim = v2f{p.dim.x, p.dim.y * f32(element_count)};
    
    Rect open_rect = Create_Rect_Center(open_center, open_dim);
    
    
    if(is_selected)
    {
        GUI_Dropdown_Button_State* state = &context->selection_state.dropdown_button;
        
        bool cursor_is_in_open_rect = Is_Point_Inside_Rect(context->cursor_fpos, open_rect);
        
        if(state->is_open)
        {
            context->flags |= GUI_Context_Flags::one_time_taking_control_of_external_input;
            
            if(context->actions[GUI_Menu_Actions::back].Is_Pressed() || 
                (context->actions[GUI_Menu_Actions::mouse].Is_Pressed() && !cursor_is_in_open_rect))
            {
                state->is_open = false;
                state->is_pressed_down = false;
            }
            else
            {
                GUI_Update_Bounds(context, open_rect);
                context->selected_element_pos = open_center;
                context->selected_element_dim = open_dim;
                
                context->flags |= GUI_Context_Flags::one_time_disable_kc_navigation;
                
                // KC controls
                if(context->actions[GUI_Menu_Actions::up].Is_Pressed())
                {
                    if(state->selected_element_idx == 0)
                        state->selected_element_idx = element_count - 1;
                    else
                        state->selected_element_idx -= 1;
                    
                    state->is_pressed_down = false;
                }
                
                
                if(context->actions[GUI_Menu_Actions::down].Is_Pressed())
                {
                    if(state->selected_element_idx == element_count - 1)
                        state->selected_element_idx = 0;
                    else
                        state->selected_element_idx += 1;
                    
                    state->is_pressed_down = false;
                }
                
                bool cursor_on_sub_selection = false;
                
                // Mouse controls
                if(cursor_is_in_open_rect)
                {
                    u32 pre_selection = state->selected_element_idx;
                    f32 level = p.pos.y - p.dim.y / 2;
                    for(u32 i = 0; i < element_count; ++i)
                    {
                        if(f32(context->cursor_fpos.y) > level)
                        {
                            if(context->cursor_position != context->last_cursor_position || 
                                context->actions[GUI_Menu_Actions::mouse].Is_Pressed())
                            {
                                state->selected_element_idx = i;
                            }
                            
                            cursor_on_sub_selection = true;
                            break;
                        }
                        level -= p.dim.y;
                    }
                    
                    if(pre_selection != state->selected_element_idx)
                        state->is_pressed_down = false;
                }
                
                
                if(GUI_On_Release_Action(context, cursor_on_sub_selection, &state->is_pressed_down))
                {
                    state->is_open = false;
                    result = state->selected_element_idx + 1;
                }
            }
        }
        
        else
        {
            if(state->is_pressed_down)
                outline_color = theme->down_color;
            else
                outline_color = theme->selected_color;
            
            if(GUI_On_Release_Action(context, cursor_on_selection, &state->is_pressed_down))
            {
                state->is_open = true;
                if(context->actions[GUI_Menu_Actions::mouse].Is_Released())
                    state->selected_element_idx = 0;
            }
        }        
        
        // Re-check if the box is open or not. May have changed in the above code block.
        if(state->is_open)
        {
            context->defered_render = GUI_Defered_Render_Type::dropdown_button;
            
            context->cursor_mask_area = open_rect;
            context->flags |= GUI_Context_Flags::cursor_mask_enabled;
            context->flags |= GUI_Context_Flags::cursor_mask_validation;
            
            state->selected_element_idx = Min(state->selected_element_idx, element_count - 1);
            state->element_count = element_count;
            state->element_names = element_names;
            state->open_rect = open_rect;
            state->pos = p.pos;
            state->dim = p.dim;
            state->theme = theme;
            state->text_scale = text_scale;
            
            return 0; // Skip the rest of the function. Drawring is handled in the End_Context function.
        }
    }
    
    // Draw closed mode.
    if(Rects_Overlap(p.rect, context->canvas_rect))
    {
        Draw_Filled_Rect_With_Outline(
            context->canvas, 
            p.rect, 
            theme->background_color,
            theme->outline_thickness, 
            outline_color);
        
        if(text)
        {
            Color text_color = is_selected? outline_color : theme->widget_text_color;
            
            v2f text_p = GUI_Calc_Centered_Text_Position(text, text_scale, p.pos, &theme->font);
            Draw_Text(context->canvas, (u8*)text, text_p, text_color, &theme->font, text_scale);
        }
    }
    
    return result;
}


static inline u32 GUI_Do_Dropdown_Button(GUI_Context* context, 
    v2f* pos,
    v2f* dim,
    char* text,
    CSTR_List elements,
    v2f text_scale = GUI_DEFAULT_TEXT_SCALE)
{
    u32 result = GUI_Do_Dropdown_Button(
        context,
        pos,
        dim,
        text,
        elements.count,
        elements.list,
        text_scale);
    
    return result;
}


static inline u32 GUI_Do_Dropdown_Button(GUI_Context* context, 
    v2f* pos,
    v2f* dim,
    u32 selected_element,
    CSTR_List elements,
    v2f text_scale = GUI_DEFAULT_TEXT_SCALE)
{
    Assert(selected_element < elements.count);
    
    u32 result = GUI_Do_Dropdown_Button(
        context,
        pos,
        dim,
        elements.list[selected_element],
        elements.count,
        elements.list,
        text_scale);
        
    return result;
}


static bool GUI_Do_SL_Input_Field(
    GUI_Context* context, 
    v2f* pos, 
    f32* width,
    String* str,
    u32 character_limit = 256,
    v2f text_scale = GUI_DEFAULT_TEXT_SCALE,
    bool (*character_check)(char*) = 0,
    GUI_Input_Acceleration_Behavior keyboard_acceleration = {0.2, 0.3, 30})
{
    Assert(GUI_Is_Context_Ready(context));
    
    Assert(str);
    
    static constexpr f32 min_handle_width = 3.f;
    
    GUI_Theme* theme = context->theme;
    Font* font = &theme->font;
    
    f32 outline_thickness = f32(theme->outline_thickness);
    f32 char_width = f32(font->char_width) * text_scale.x;
    f32 char_height = f32(font->char_height) * text_scale.y;
    
    f32 bar_height = Ceil(char_height * 0.33f);
    
    v2f dim;
    {
        f32 _width = width? *width : context->layout.last_element.dim.x;
        f32 _height = char_height + bar_height + outline_thickness * 2;
        if((u32)_height % 2)
            _height += 1;
        
        dim = {_width, _height};
    }
    
    // --------------------------------------------------------------------------
    
    GUI_Placement p = GUI_Get_Placement(context, &dim, pos);
    if(!Is_Rect_Valid(p.rect) || context->mode == GUI_Mode::layout_only)
        return false;
    
    u32 id = GUI_Generate_ID(p.rect, __LINE__);
    
    bool cursor_on_selection = Is_Point_Inside_Rect(context->cursor_fpos, p.rect);
    bool is_selected = GUI_Is_Element_Selected(context, cursor_on_selection, id);
    
    
    // --------------------------------------------------------------------------
    
    bool result = false;
    
    Color outline_color = theme->outline_color;
    u32 view_offset = 0;
    
    v2f text_p = p.pos + v2f{1, -1} * outline_thickness + v2f{2,-2} +
        Hadamar_Product(v2f{-1, 1}, (p.dim / 2));
    text_p.y -= char_height;
    
    u32 view_limit; // Amount of charactes that can fit inside the text area.
    // Calculate view limit.
    {
        // w: dif between text_p and p.pos twice.
        f32 w = (text_p.x - p.rect.min.x) * 2;
        view_limit = (u32)Floor((p.dim.x - w) / char_width);
    }
    
    // "Bar" is the slider area at the bottom.
    v2f bar_dim;
    v2f bar_center;
    Rect bar_rect;
    
    {
        f32 bar_offset_y = p.dim.y / 2 - bar_height / 2 - outline_thickness;
        bar_dim = { p.dim.x - outline_thickness * 2, bar_height };
        bar_center = p.pos - v2f{0, bar_offset_y};
        bar_rect = Create_Rect_Center(bar_center, bar_dim);
    }
    
    f32 f_lenght = f32(str->lenght) + 1.f;
    f32 handle_width = Max(bar_dim.x * (view_limit / f_lenght), min_handle_width);
    
    bool draw_cursor = false;
    u32 write_cursor_position = 0;
    
    bool text_select_mode = false;
    u32 text_select_start_point = 0;
    
    if(is_selected)
    {
        GUI_SL_Input_Field_State* state = &context->selection_state.sl_input_field;
        
        draw_cursor = state->draw_cursor && state->is_active;
        
        // Handle input
        bool mouse_pressed_down = context->actions[GUI_Menu_Actions::mouse].Is_Pressed();
        bool mouse_is_down = context->actions[GUI_Menu_Actions::mouse].Is_Down();
        
        if(state->is_active)
        {
            context->flags |= GUI_Context_Flags::one_time_taking_control_of_external_input;
            
            if(context->actions[GUI_Menu_Actions::back].Is_Pressed() || 
                (!cursor_on_selection && mouse_pressed_down))
            {
                draw_cursor = false;
                state->is_active = false;
                state->view_offset = 0;
                
                Inverse_Bit_Mask(&context->flags, GUI_Context_Flags::cursor_mask_validation);
            }
            else
            {
                context->flags |= GUI_Context_Flags::cursor_mask_validation;
                context->flags |= GUI_Context_Flags::cursor_mask_enabled;    
                
                context->cursor_mask_area = context->canvas_rect;
                
                u32 wcp = state->write_cursor_position;
                
                if(GUI_Input_Field_Insert_Characters(
                    context->platform, 
                    state, 
                    str, 
                    character_limit, 
                    wcp,
                    false,
                    character_check))
                {
                    GUI_Reset_Selection_State(context);
                    result = true;
                }
                
                // NOTE: string lenght is a dependancy for f_lenght, thefore it's updated here,
                // now that the final lenght for the frame is known.
                f_lenght = f32(str->lenght) + 1.f;
                handle_width = Max(bar_dim.x * view_limit / f_lenght, min_handle_width);
                
                f64 time = context->platform->Get_Time_Stamp();
                state->draw_cursor = !(time > state->flicker_start_time) || (u64)time % 2;
                
                if(context->actions[GUI_Menu_Actions::left].Is_Pressed() ||
                    context->actions[GUI_Menu_Actions::right].Is_Pressed())
                {
                    state->input_start_time = 0;
                    state->next_input_time = 0;
                }
                
                bool shift_down = context->platform->Get_Keyboard_Key_Down(Key_Code::LSHIFT) || 
                    context->platform->Get_Keyboard_Key_Down(Key_Code::LSHIFT);
                
                // Arrowkey/controller input 
                while(context->actions[GUI_Menu_Actions::left].Is_Down() && GUI_Accelerated_Tick(
                    &keyboard_acceleration, time, &state->input_start_time, &state->next_input_time))
                {
                    if(GUI_Input_Field_Begin_Or_End_Text_Select_Mode(state, shift_down, state->write_cursor_position))
                        break;
                    
                    state->write_cursor_position -= 1;
                    
                    if(state->write_cursor_position > wcp)
                    {
                        if(context->actions[GUI_Menu_Actions::left].Is_Pressed())
                            state->write_cursor_position = str->lenght;
                        
                        else
                            state->write_cursor_position = wcp;
                    }
                }
                
                
                while(context->actions[GUI_Menu_Actions::right].Is_Down() && GUI_Accelerated_Tick(
                    &keyboard_acceleration, time, &state->input_start_time, &state->next_input_time))
                {
                    if(GUI_Input_Field_Begin_Or_End_Text_Select_Mode(state, shift_down, state->write_cursor_position))
                        break;
                    
                    state->write_cursor_position += 1;
                    
                    if(state->write_cursor_position > str->lenght)
                    {
                        if(context->actions[GUI_Menu_Actions::right].Is_Pressed())
                            state->write_cursor_position = 0;
                        else
                            state->write_cursor_position = wcp;
                    }
                }
                
                // mouse behavior with cursor and handle bar.
                if(str->lenght > 0)
                {
                    bool mouse_y_on_text_area = (context->cursor_fpos.y >= text_p.y &&
                        context->cursor_fpos.y < text_p.y + char_height);
                    
                    if(mouse_pressed_down && mouse_y_on_text_area)
                    {
                        GUI_Input_Field_Begin_Or_End_Text_Select_Mode(state, shift_down, state->write_cursor_position);
                        
                        f32 rel_cursor_x = context->cursor_fpos.x - text_p.x;
                        if(rel_cursor_x < 0)
                            rel_cursor_x = 0;
                        
                        // TODO: This is check of < 0 on unsigned?
                        u32 click_p = Max(u32(0), Min((u32)Round(rel_cursor_x / char_width), view_limit));
                        
                        state->write_cursor_position = Min(state->view_offset + click_p, str->lenght);
                        
                        state->click_p = click_p;
                        state->mouse_press_down_point = state->write_cursor_position;
                        state->mouse_hold_time = time + state->mouse_hold_delay;
                    }
                    
                    
                    if(mouse_is_down)
                    {
                        if(state->mouse_hold_time != 0)
                        {
                            // mouse drag selection!
                            
                            f32 rel_cursor_x = context->cursor_fpos.x - text_p.x;
                            if(rel_cursor_x < 0)
                                rel_cursor_x = 0;
                            
                            u32 click_p = Max(u32(0), Min((u32)Round(rel_cursor_x / char_width), view_limit));
                            
                            if(time >= state->mouse_hold_time || click_p != state->click_p)
                            {
                                state->click_p = click_p;
                                state->mouse_hold_time = time + state->mouse_hold_delay;
                                state->write_cursor_position = Min(state->view_offset + click_p, str->lenght);
                                state->text_select_start_point = state->mouse_press_down_point;
                                state->text_select_mode = true;
                            }
                        }
                    }
                    else
                    {
                        state->mouse_hold_time = 0;
                    }
                    
                    
                    if(str->lenght >= view_limit)
                    {
                        v2f shrink_bar_dim = bar_dim - v2f{handle_width / 2, 0};
                        Rect shrink_bar = Create_Rect_Center(bar_center, shrink_bar_dim);
                        
                        if(mouse_pressed_down && Is_Point_Inside_Rect(context->cursor_fpos, bar_rect))
                        {
                            state->scroll_bar_drag_mode = true;
                            state->text_select_mode = false;
                        }
                        
                        if(state->scroll_bar_drag_mode)
                        {
                            if(mouse_is_down)
                            {
                                state->flicker_start_time = time + state->flicker_delay;
                                
                                f32 rel_shrink_bar_x = context->cursor_fpos.x - shrink_bar.min.x;
                                
                                f32 percent = Clamp_Zero_To_One(rel_shrink_bar_x / shrink_bar_dim.x);
                                
                                // +1 here to allow scrolling 1 "slot" into the white space at the end.
                                // (Lookahead)
                                state->view_offset = 
                                    (u32)Round(f32(str->lenght - view_limit + 1) * percent);
                                
                                
                                if(state->write_cursor_position < state->view_offset + 1)
                                {
                                    state->write_cursor_position = state->view_offset + 1;
                                }
                                
                                else if(state->write_cursor_position > state->view_offset + view_limit - 1)
                                {
                                    state->write_cursor_position = state->view_offset + view_limit - 1;
                                }
                            }
                            else
                                state->scroll_bar_drag_mode = false;
                        }
                    }
                }
                
                text_select_mode = state->text_select_mode;
                text_select_start_point = state->text_select_start_point;
                
                if(wcp != state->write_cursor_position)
                    state->flicker_start_time = time + state->flicker_delay;
                
                // View offset snapping! v2.0
                {
                    if(str->lenght < view_limit || state->write_cursor_position == 0)
                    {
                        state->view_offset = 0;
                    }
                    
                    else if(state->write_cursor_position == str->lenght)
                    {
                        state->view_offset = str->lenght - view_limit + 1;
                    }
                    
                    else if(state->view_offset + view_limit > str->lenght + 1)
                    {
                        state->view_offset = str->lenght + 1 - view_limit;
                    }
                    
                    else if(state->write_cursor_position > state->view_offset + view_limit - 1)
                    {
                        state->view_offset = state->write_cursor_position - view_limit + 1;
                    }
                    
                    else if(state->write_cursor_position < state->view_offset + 1)
                    {
                        state->view_offset = state->write_cursor_position - 1;
                    }
                }
            }
        }
        else
        {
            if(GUI_On_Release_Action(context, cursor_on_selection, &state->is_pressed_down))
            {
                // NOTE: Marikas idea!
                if(str->lenght > 0)
                {
                    state->text_select_mode = true;
                    state->text_select_start_point = 0;
                }
                
                state->is_active = true;
                state->write_cursor_position = str->lenght;
                state->flicker_start_time = context->platform->Get_Time_Stamp() + state->flicker_delay;
            }
        }
        
        view_offset = state->view_offset;
        
        write_cursor_position = state->write_cursor_position;
        
        if(state->is_pressed_down)
            outline_color = theme->down_color;
        else
            outline_color = theme->selected_color;
    }

    
    // Draw -------------------------------------------------------------------------------------------
    if(Rects_Overlap(p.rect, context->canvas_rect))
    {        
        Draw_Filled_Rect_With_Outline(
            context->canvas, 
            p.rect, 
            theme->background_color,
            theme->outline_thickness, 
            outline_color);
        
        // If the text can fit in the box, move it down so it's centered.
        if(str->lenght < view_limit)
            text_p.y = p.pos.y - char_height / 2;
        
        if(text_select_mode)
        {
            i32 p0 = (i32)write_cursor_position;
            i32 p1 = (i32)text_select_start_point;
            
            if(p1 != p0)
            {
                // p0 is forced to be less than p1.
                if(p0 > p1)
                    Swap(&p0, &p1);
                
                p0 = Max(i32(0), i32(p0 - view_offset));
                p1 = Min(i32(view_limit), i32(p1 - view_offset));
                p1 -= p0;
                
                if(p1 != 0)
                {
                    v2f selection_pos = {text_p.x + p0 * char_width, text_p.y};
                    v2f selection_dim = {p1 * char_width, char_height};
                    
                    Rect select_rect = Create_Rect_Min_Dim(selection_pos, selection_dim);
                    
                    Draw_Filled_Rect(context->canvas, select_rect, theme->write_cursor_color);
                }
            }
        }
        
        if(str->lenght > 0)
        {    
            u32 view_lenght = Min(str->lenght - view_offset, view_limit);
            
            Draw_Text(
                context->canvas, 
                Create_String_View(str, view_offset, view_lenght),
                text_p, 
                theme->widget_text_color, 
                &theme->font, 
                text_scale);
        }
        
        if(draw_cursor)
        {
            v2f cursor_p = text_p;
            
            cursor_p.x += f32(write_cursor_position - view_offset) * char_width;
            
            Color cursor_color = (str->lenght < character_limit || character_limit == 0)? 
                theme->write_cursor_color : theme->write_cursor_limit_color;
            
            Draw_Vertical_Line(
                context->canvas, 
                cursor_p, 
                f32(font->char_height) * text_scale.y, 
                cursor_color);
        }
        
        // bar area.
        if(str->lenght >= view_limit)
        {
            Draw_Filled_Rect(context->canvas, bar_rect, theme->down_color);
            
            v2f handle_center = bar_center + v2f{-bar_dim.x / 2 + handle_width / 2, 0};
            
            f32 xoff = (f32)view_offset / f_lenght * bar_dim.x;
            handle_center.x += xoff;
            Rect handle_rect = Create_Rect_Center(handle_center, v2f{handle_width, bar_dim.y});
            
            Draw_Filled_Rect(context->canvas, handle_rect, theme->outline_color);
        }    
        
    }
    
    return result;
}


// TODO: 
// Lookahead
// Sensible minimums.
// BUG FIX: Using the mouse selecting a last line that has only one character selectes the wrong one. (minor). 
// BUG FIX: When selecting text and the initial mouse click is bellow text selection; text select start point isn't properly set.

static void GUI_Do_ML_Input_Field(
    GUI_Context* context, 
    v2f* pos, 
    v2f* dim,
    String* str,
    u32 character_limit = 256,
    v2f text_scale = GUI_DEFAULT_TEXT_SCALE,
    bool (*character_check)(char*) = 0,
    GUI_Input_Acceleration_Behavior keyboard_acceleration = {0.2, 0.3, 30})
{
    Assert(GUI_Is_Context_Ready(context));
    
    constexpr f32 scroll_bar_width = 20.f;
    constexpr f32 min_handle_height = 3.f;
    
    // --------------------------------------------------------------------------
    
    GUI_Theme* theme = context->theme;
    
    GUI_Placement p = GUI_Get_Placement(context, dim, pos);
    if(!Is_Rect_Valid(p.rect) || context->mode == GUI_Mode::layout_only)
        return;
    
    u32 id = GUI_Generate_ID(p.rect, __LINE__);
    
    bool cursor_on_selection = Is_Point_Inside_Rect(context->cursor_fpos, p.rect);
    bool is_selected = GUI_Is_Element_Selected(context, cursor_on_selection, id);
    
    // --------------------------------------------------------------------------
    
    Font* font = &theme->font;
    
    f32 outline_thickness = f32(theme->outline_thickness);
    f32 char_width = f32(font->char_width) * text_scale.x;
    f32 char_height = f32(font->char_height) * text_scale.y;
    
    v2f text_p = p.pos + v2f{1, -1} * outline_thickness + v2f{2,-2} +
        Hadamar_Product(v2f{-1, 1}, (p.dim / 2));
    
    text_p.y -= char_height;
    
    Color box_color = theme->background_color;
    Color outline_color = theme->outline_color;
    
    u32 view_limit_x;
    // Calculate view limit.
    {
        // w: dif between text_p and p.pos twice.
        f32 w = (text_p.x - p.rect.min.x) * 2;
        view_limit_x = (u32)Floor((p.dim.x - w) / char_width);
    }
    
    u32 view_limit_y;
    {
        // w: dif between text_p and p.pos twice.
        f32 h = (p.rect.max.y - (text_p.y + char_height)) * 2;
        f32 text_area_height = p.dim.y - h;
        
        // Just exit of the text box is too small to draw any text in.
        if(text_area_height <= char_height)
            return;
        
        view_limit_y = (u32)Floor(text_area_height / char_height);
    }

    bool enable_scroll_bar = false;
    bool draw_cursor = false;
    bool text_select_mode = false;
    u32 text_select_start_point = 0;
    
    u32 write_cursor_position = 0;
    u32 line_count = view_limit_y;
    u32 cursor_line = 0;
    u32 view_offset = 0;
    char* visible_text_start = 0;
    
    f32 handle_height = 0;
    v2f handle_center = v2f{0, 0};
    v2f scroll_bar_center = v2f{p.rect.max.x - scroll_bar_width / 2, p.pos.y};
    v2f scroll_bar_dim = v2f{scroll_bar_width, p.dim.y};
    v2f scroll_bar_internal_dim = scroll_bar_dim - outline_thickness * 2;
    Rect scroll_bar_rect = Create_Rect_Center(scroll_bar_center, scroll_bar_dim);
    
    // CONSIDER: Better way to capture state. Perhaps a struct?
    if(is_selected)
    {
        GUI_SL_Input_Field_State* state = &context->selection_state.sl_input_field;
        
        if(state->is_active)
        {
            context->flags |= GUI_Context_Flags::one_time_taking_control_of_external_input;
            
            // Handle input
            bool mouse_pressed_down = context->actions[GUI_Menu_Actions::mouse].Is_Pressed();
            bool mouse_is_down = context->actions[GUI_Menu_Actions::mouse].Is_Down();
            
            if(context->actions[GUI_Menu_Actions::back].Is_Pressed() || 
                (!cursor_on_selection && mouse_pressed_down))
            {
                draw_cursor = false;
                state->is_active = false;
                state->view_offset = 0;
                
                u32 mask = GUI_Context_Flags::cursor_mask_validation | 
                    GUI_Context_Flags::one_time_disable_kc_navigation;
                
                Inverse_Bit_Mask(&context->flags, mask);
            }
            else
            {
                context->flags |= GUI_Context_Flags::cursor_mask_validation;
                context->flags |= GUI_Context_Flags::cursor_mask_enabled;
                context->flags |= GUI_Context_Flags::one_time_disable_kc_navigation;
                
                context->cursor_mask_area = context->canvas_rect;
                
                bool up_is_down = context->actions[GUI_Menu_Actions::up].Is_Down();
                bool down_is_down = context->actions[GUI_Menu_Actions::down].Is_Down();
                bool right_is_down = context->actions[GUI_Menu_Actions::right].Is_Down();
                bool left_is_down = context->actions[GUI_Menu_Actions::left].Is_Down();
                bool navigation 
                    = mouse_is_down || up_is_down || down_is_down || left_is_down || right_is_down;
                
                u32 wcp = state->write_cursor_position;
                
                i32 desired_row_jumps = 0;
                bool click_in_text_region = false;
                v2u mouse_cell;
                
                u32 line_start_buffer[16] = {};
                i32 line_start_buffer_len = Array_Lenght(line_start_buffer);
                
                f64 time = context->platform->Get_Time_Stamp();
                state->draw_cursor = !(time > state->flicker_start_time) || (u64)time % 2;
                
                bool shift_down = context->platform->Get_Keyboard_Key_Down(Key_Code::LSHIFT) || 
                    context->platform->Get_Keyboard_Key_Down(Key_Code::LSHIFT);
                
                if(navigation)
                {    
                    if(!state->scroll_bar_drag_mode)
                    {        
                        if(context->actions[GUI_Menu_Actions::left].Is_Pressed() ||
                            context->actions[GUI_Menu_Actions::right].Is_Pressed() || 
                            context->actions[GUI_Menu_Actions::up].Is_Pressed() ||
                            context->actions[GUI_Menu_Actions::down].Is_Pressed())
                        {
                            state->input_start_time = 0;
                            state->next_input_time = 0;
                        }
                        
                        f64* inp_start_t = &state->input_start_time;
                        f64* next_inp_t = &state->next_input_time;
                        
                        GUI_Input_Acceleration_Behavior* kca = &keyboard_acceleration;
                        
                        // Arrowkey/controller input 
                        while(left_is_down && GUI_Accelerated_Tick(kca, time, inp_start_t, next_inp_t))
                        {
                            if(GUI_Input_Field_Begin_Or_End_Text_Select_Mode(state, shift_down, wcp))
                                break;
                            
                            state->write_cursor_position -= 1;
                            
                            if(state->write_cursor_position > wcp)
                            {
                                if(context->actions[GUI_Menu_Actions::left].Is_Pressed())
                                    state->write_cursor_position = str->lenght;    
                                
                                else
                                    state->write_cursor_position = wcp;
                            }
                        }
                    
                        while(right_is_down && GUI_Accelerated_Tick(kca, time, inp_start_t, next_inp_t))
                        {
                            if(GUI_Input_Field_Begin_Or_End_Text_Select_Mode(state, shift_down, wcp))
                                break;
                            
                            state->write_cursor_position += 1;
                            
                            if(state->write_cursor_position > str->lenght)
                            {
                                if(context->actions[GUI_Menu_Actions::right].Is_Pressed())
                                    state->write_cursor_position = 0;
                                else
                                    state->write_cursor_position = wcp;
                            }
                        }
                        
                        while(up_is_down && GUI_Accelerated_Tick(kca, time, inp_start_t, next_inp_t))
                        {
                            if(GUI_Input_Field_Begin_Or_End_Text_Select_Mode(state, shift_down, wcp))
                                break;
                            
                            desired_row_jumps -= 1;
                        }
                        
                        
                        while(down_is_down && GUI_Accelerated_Tick(kca, time, inp_start_t, next_inp_t))
                        {
                            if(GUI_Input_Field_Begin_Or_End_Text_Select_Mode(state, shift_down, wcp))
                                break;
                            
                            desired_row_jumps += 1;
                        }
                        
                        
                        if(mouse_is_down && cursor_on_selection)
                        {
                            desired_row_jumps = 0;
                            
                            v2f mouse_rel_pos = text_p + v2f{0, char_height};
                            v2f rel_mouse_pos = context->cursor_fpos - mouse_rel_pos;
                            rel_mouse_pos.y *= -1;
                            v2f cell_dim = v2f{char_width, char_height};
                            
                            // This depends on underflow.
                            v2f fcell = Hadamar_Division(rel_mouse_pos, cell_dim);
                            fcell.x = Round(fcell.x);
                            
                            mouse_cell = fcell.As<u32>();
                            if(mouse_cell.y <= view_limit_y)
                            {
                                mouse_cell.y += state->view_offset;
                                click_in_text_region = true;
                            }
                        }
                        
                        // NOTE: Because of how jumping up rows, has to work, it requires use of memory.
                        // So the max up jumps "min number" has to be limited to size of the array.
                        if(desired_row_jumps < -line_start_buffer_len)
                            desired_row_jumps = -line_start_buffer_len;
                    }
                }
                else if(state->cursor_is_active && character_check != GUI_Character_Check_View_Only)
                {
                    GUI_Input_Field_Insert_Characters(
                        context->platform, 
                        state, 
                        str, 
                        character_limit, 
                        state->write_cursor_position,
                        true,
                        character_check);
                }
                
                // If the line count exceeds the view limit, then a scroll bar is enabled and
                // because that effects the view_limit_x counting has to be restarted.
                if(str->lenght > view_limit_x * view_limit_y)
                {
                    enable_scroll_bar = true;
                    
                    // Calculate view limit.
                    {
                        // w: dif between text_p and p.pos twice.
                        f32 w = (text_p.x - p.rect.min.x) * 2 + scroll_bar_width;
                        view_limit_x = (u32)Floor((p.dim.x - w) / char_width);
                    }
                }
                
                // Count the number of virtual and real new lines.
                
                b32 cursor_on_virtual_last_line = false;
                u32 new_cursor_write_pos;
                bool cursor_moved_with_mouse;
                bool recalc;
                do
                {
                    if(str->lenght == 0)
                    {
                        cursor_line = 1;
                        line_count = 1;
                        new_cursor_write_pos = 0;
                        state->view_offset = 0;
                        break;
                    }
                    
                    // These values can't be used directly,
                    // because this loop might have to restart, when the scroll bar is enabled,
                    // Therefore we take copies and write out the results after the loop.
                    new_cursor_write_pos = state->write_cursor_position;
                    i32 desired_row_jumps_cpy = desired_row_jumps;
                    
                    recalc = false;
                    cursor_on_virtual_last_line = false;
                    cursor_moved_with_mouse = false;
                    cursor_line = 0;
                    line_count = 0;
                    visible_text_start = 0;
                    
                    u32 line_start = 0;
                    bool cursor_state_is_unknown = state->cursor_is_active;
                    char* str_begin = str->buffer;
                    char* str_end = str->buffer + str->lenght - 1;
                    
                    for(char* c = str_begin; c <= str_end; ++c)
                    {
                        b32 is_last_char = c == str_end;
                        b32 is_new_line = *c == '\n';
                        u32 exclusive_lenght = u32(c - str_begin) - line_start;
                        b32 is_virtual_line = exclusive_lenght >= view_limit_x;
                        
                        if(is_new_line || is_virtual_line || c == str_end)
                        {
                            u32 effct_len = exclusive_lenght + (is_new_line | is_last_char);
                            Assert(effct_len);
                            
                            if(line_count == state->view_offset)
                                visible_text_start = str_begin + line_start;
                            
                            // On mouse click move the cursor here.
                            if(click_in_text_region)
                            {
                                u32 cell_x = Min(mouse_cell.x, view_limit_x);
                                
                                if(mouse_cell.y == line_count)
                                {
                                    u32 line_lenght = exclusive_lenght;
                                    if(line_lenght > 0 && !is_new_line)
                                        line_lenght += is_last_char;
                                    
                                    u32 offset = Min(cell_x, line_lenght - is_virtual_line);
                                    new_cursor_write_pos = line_start + offset;
                                    cursor_line = line_count + 1;
                                    cursor_state_is_unknown = false;
                                    state->cursor_is_active = true;
                                    cursor_moved_with_mouse = true;
                                }
                                else if(is_last_char && is_new_line && mouse_cell.y == line_count + 1)
                                {
                                    new_cursor_write_pos = line_start + effct_len;
                                    cursor_line = line_count + 1;
                                    cursor_state_is_unknown = false;
                                    state->cursor_is_active = true;
                                    cursor_on_virtual_last_line = true;
                                    cursor_moved_with_mouse = true;
                                }
                            }
                            
                            // --- above this line_count depends on not being incerement yet. ---
                            
                            line_count += 1;
                            
                            
                            // If this is a virual line, go back one character as,
                            // we're not actually counting this one.
                            // Dont do on last char to avoid infinite loop.
                            if((c != str_end || is_virtual_line) && !is_new_line)
                            {
                                c -= 1;
                                is_last_char = false;
                                effct_len = exclusive_lenght;
                            }
                            
                            
                            // If cursor on this line.
                            if(cursor_state_is_unknown)
                            {
                                if(new_cursor_write_pos >= line_start &&
                                    new_cursor_write_pos < line_start + effct_len + is_last_char)
                                {
                                    cursor_state_is_unknown = false;
                                    u32 local_line_count = line_count;
                                    b32 local_is_last_char = is_last_char;
                                    b32 local_is_new_line = is_new_line;
                                    i32 row_jumps = 0;
                                    
                                    if(desired_row_jumps_cpy > 0)
                                    {
                                        if(local_is_last_char && local_is_new_line)
                                        {
                                            new_cursor_write_pos = str->lenght;
                                            row_jumps = 0;
                                        }
                                        else
                                        {
                                            u32 local_line_start = line_start;
                                            u32 next_line_start = line_start + effct_len;
                                            u32 num_rows_counted = 0;
                                            for(char* _c2 = c + 1; _c2 <= str_end; ++_c2)
                                            {
                                                local_is_last_char = _c2 == str_end;
                                                local_is_new_line = *_c2 == '\n';
                                                u32 local_exclusive_lenght = u32(_c2 - str_begin) - next_line_start;
                                                if(local_is_new_line || local_exclusive_lenght >= view_limit_x || _c2 == str_end)
                                                {
                                                    u32 local_effct_len = 
                                                        local_exclusive_lenght + (local_is_new_line | local_is_last_char);
                                                    
                                                    if(_c2 != str_end && !local_is_new_line)
                                                        _c2 -= 1;
                                                    
                                                    num_rows_counted += 1;
                                                    
                                                    u32 cursor_x_pos = new_cursor_write_pos - local_line_start;
                                                    local_line_start = next_line_start;
                                                    
                                                    u32 line_len = local_is_last_char && !local_is_new_line?
                                                        local_effct_len : local_exclusive_lenght;
                                                    
                                                    u32 collum_offset = Min(line_len, cursor_x_pos);
                                                    new_cursor_write_pos = next_line_start + collum_offset;
                                                    if(num_rows_counted == desired_row_jumps_cpy || _c2 == str_end)
                                                    {
                                                        row_jumps = num_rows_counted;
                                                        break;
                                                    }
                                                    
                                                    next_line_start += local_effct_len;
                                                }
                                            }
                                        }
                                    }
                                    else if(desired_row_jumps_cpy < 0)
                                    {
                                        // Special case where cursor is at the end of the string and the last character is a new line.
                                        if(new_cursor_write_pos == str->lenght && local_is_new_line)
                                        {
                                            new_cursor_write_pos -= effct_len;
                                            desired_row_jumps_cpy += 1;
                                            row_jumps = 0;
                                        }
                                        
                                        if(desired_row_jumps_cpy < 0)
                                        {
                                            u32 abs_desired_row_jumbs = Min(line_count - 1, u32(Abs(desired_row_jumps_cpy)));
                                            Assert(abs_desired_row_jumbs <= u32(line_start_buffer_len));
                                            u32 local_line_start = line_start;
                                            
                                            // If line_count is 1 (min possible at this point), this will underflow.
                                            // abs_desired_row_jumbs, should then be 0 though so the loop will just not run.
                                            u32 line_count_s2 = line_count - 2;
                                            
                                            for(u32 i = 0; i < abs_desired_row_jumbs; ++i)
                                            {
                                                u32 idx = (line_count_s2 - i) % line_start_buffer_len; 
                                                u32 ls = line_start_buffer[idx];
                                                u32 local_excl_line_len = 0;
                                                
                                                // -> now I need to know the line lenght!
                                                for(char* _c2 = str_begin + ls; _c2 <= str_end; ++_c2)
                                                {
                                                    local_is_new_line = *_c2 == '\n';
                                                    local_is_last_char = _c2 == str_end;
                                                
                                                    local_excl_line_len = u32(_c2 - str_begin) - ls;
                                                    if(local_excl_line_len >= view_limit_x)
                                                    {
                                                        // Gone over on the next line so step back.
                                                        local_excl_line_len -= 1;
                                                        break;
                                                    }
                                                    
                                                    if(local_is_new_line || local_is_last_char)
                                                        break;
                                                }
                                                
                                                u32 cursor_x_pos = new_cursor_write_pos - local_line_start;
                                                u32 collum_offset = Min(local_excl_line_len, cursor_x_pos);
                                                new_cursor_write_pos = ls + collum_offset;
                                                
                                                local_line_start = ls;
                                                row_jumps -= 1;
                                            }
                                        }
                                    }
                                    
                                    cursor_on_virtual_last_line = b32(new_cursor_write_pos == str->lenght && local_is_new_line);
                                    cursor_line = local_line_count + row_jumps;
                                }
                                
                                // Write into line start buffer!
                                line_start_buffer[(line_count - 1) % line_start_buffer_len] = line_start;
                            }
                            
                            
                            if(c == str_end)
                            {
                                if(is_new_line)
                                    line_count += 1;
                            }
                            
                            line_start += effct_len;
                            
                            if(!enable_scroll_bar && line_count > view_limit_y)
                            {
                                enable_scroll_bar = true;
                                
                                // Calculate view limit.
                                {
                                    // w: dif between text_p and p.pos twice.
                                    f32 w = (text_p.x - p.rect.min.x) * 2 + scroll_bar_width;
                                    view_limit_x = (u32)Floor((p.dim.x - w) / char_width);
                                }
                                
                                recalc = true;
                                break;
                            }
                        }
                    }
                    
                }while(recalc);
                
                // Write cursor position set in stone!
                state->write_cursor_position = new_cursor_write_pos;
                if(wcp != state->write_cursor_position)
                    state->flicker_start_time = time + state->flicker_delay;
                
                // Mouse highlight
                {
                
                    if(mouse_pressed_down && cursor_moved_with_mouse)
                    {
                        if(shift_down && state->text_select_mode)
                        {
                            u32 s = state->text_select_start_point;
                            
                            if((s > wcp && state->write_cursor_position > s) || 
                                (s < wcp && state->write_cursor_position < s))
                            {
                                state->text_select_start_point = wcp;
                            }
                        }
                        else
                        {
                            state->text_select_start_point = state->write_cursor_position;
                        }
                        
                        state->text_select_mode = true;
                    }
                }
                
                Assert(line_count > 0);
                Assert(!state->cursor_is_active || cursor_line > 0);
                Assert(cursor_line <= line_count);
                
                // View offset snapping.
                {
                    u32 adj_cl = cursor_line + cursor_on_virtual_last_line;
                    
                    if(line_count == 0 || !enable_scroll_bar)
                    {
                        state->view_offset = 0;
                        visible_text_start = 0;
                    }
                    else if(line_count > view_limit_y && state->view_offset + view_limit_y > line_count)
                    {
                        state->view_offset = line_count - view_limit_y;
                        visible_text_start = 0;
                        if(state->view_offset > line_count)
                        {
                            state->view_offset = 0;
                        }
                    }
                    
                    if(state->cursor_is_active)
                    {
                        if(adj_cl <= state->view_offset)
                        {
                            visible_text_start = 0;
                            state->view_offset = adj_cl - 1;
                        }
                        
                        if(adj_cl > state->view_offset + view_limit_y)
                        {
                            visible_text_start = 0;
                            state->view_offset = adj_cl - view_limit_y;
                        }
                    }
                }
                
                // --- Scroll bar control ----
                if(enable_scroll_bar)
                {
                    // Mouse wheel.
                    {
                        i32 wheel_delta = (i32)context->platform->Get_Scroll_Wheel_Delta();
                        
                        if(wheel_delta > 0 && state->view_offset > 0)
                        {
                            state->view_offset -= 1;
                            
                            state->cursor_is_active = false;
                            visible_text_start = 0;
                        }
                        else if(wheel_delta < 0 && state->view_offset < line_count - view_limit_y)
                        {
                            state->view_offset += 1;
                            
                            state->cursor_is_active = false;
                            visible_text_start = 0;
                        }
                    }
                    
                    // Calculate handle position/dimensions.
                    
                    f32 r = f32(view_limit_y) / f32(line_count);
                    f32 h = scroll_bar_internal_dim.y * r;
                    handle_height = Max(h, min_handle_height);
                    
                    handle_center = 
                        scroll_bar_center + v2f{0, scroll_bar_internal_dim.y / 2 - handle_height / 2};
                    
                    v2f hc_rec = handle_center;
                    
                    f32 option_space = scroll_bar_internal_dim.y - handle_height;
                    f32 option_count = f32(line_count - view_limit_y);
                    
                    f32 yoff = (option_space / option_count) * state->view_offset;
                    handle_center.y -= yoff;
                    
                    // --
                    Rect scroll_bar_internal_rect = Create_Rect_Center(scroll_bar_center, scroll_bar_internal_dim);
                    
                    if(mouse_pressed_down)
                    {
                        state->scroll_bar_drag_mode =
                            Is_Point_Inside_Rect(context->cursor_fpos, scroll_bar_internal_rect);
                        
                        if(state->scroll_bar_drag_mode)
                        {
                            Rect handle_rect = 
                                Create_Rect_Center(handle_center, v2f{scroll_bar_internal_dim.x, handle_height});
                            
                            if(Is_Point_Inside_Rect(context->cursor_fpos, handle_rect))
                                state->mouse_scroll_bar_drag_offset = handle_center - context->cursor_fpos;
                            else
                                state->mouse_scroll_bar_drag_offset = v2f{0, 0};
                        }
                    }
                    
                    if(state->scroll_bar_drag_mode)
                    {    
                        state->cursor_is_active = false;
                        cursor_line = view_offset;
                        
                        
                        f32 effective_bar_height = scroll_bar_internal_dim.y - handle_height;
                        
                        f32 rel_to_pos = scroll_bar_internal_rect.min.y + handle_height / 2;
                        
                        f32 rel_mouse_pos_y = 
                            (context->cursor_fpos.y + state->mouse_scroll_bar_drag_offset.y) - rel_to_pos;
                        
                        f32 r2 = Clamp_Zero_To_One(1.0f - (rel_mouse_pos_y / effective_bar_height));
                        
                        u32 option_space2 = line_count - view_limit_y;
                        
                        view_offset = state->view_offset;
                        
                        state->view_offset = (u32)Round(r2 * f32(option_space2));
                        if(view_offset != state->view_offset)
                        {
                            // Recalculate where the handle center is.
                            handle_center = hc_rec;
                            yoff = (option_space / option_count) * state->view_offset;
                            handle_center.y -= yoff;
                            visible_text_start = 0;
                        }
                    }
                }
                
                if(!enable_scroll_bar || !mouse_is_down)
                    state->scroll_bar_drag_mode = false;
                
                // --
                
                view_offset = state->view_offset;
            }
            
            draw_cursor = state->cursor_is_active && state->draw_cursor;
            write_cursor_position = state->write_cursor_position;
            
            text_select_mode = state->text_select_mode;
            text_select_start_point = state->text_select_start_point;
        }
        else
        {
            if(GUI_On_Release_Action(context, cursor_on_selection, &state->is_pressed_down))
            {
                state->text_select_mode = false;
                state->cursor_is_active = true;
                state->is_active = true;
                state->write_cursor_position = str->lenght;
                state->flicker_start_time = context->platform->Get_Time_Stamp() + state->flicker_delay;
            }
        }
        
        if(state->is_pressed_down)
            outline_color = theme->down_color;
        else
            outline_color = theme->selected_color;
    }
    
    // Draw ---------------------------------------------------------------------
    if(Rects_Overlap(p.rect, context->canvas_rect))
    {    
        Draw_Filled_Rect_With_Outline(
            context->canvas, 
            p.rect, 
            box_color, 
            theme->outline_thickness, 
            outline_color);
        
        if(enable_scroll_bar)
        {
            Color scroll_bar_color = theme->down_color;
            
            Draw_Filled_Rect_With_Outline(
                context->canvas,
                scroll_bar_rect, 
                scroll_bar_color,
                theme->outline_thickness,
                outline_color);
            
            Rect handle_rect = 
                Create_Rect_Center(handle_center, v2f{scroll_bar_internal_dim.x, handle_height});
                
            Draw_Filled_Rect(context->canvas, handle_rect, theme->outline_color);
        }
        
        
        char* str_begin; 
        char* str_end = str->buffer + str->lenght - ((str->lenght > 0)? 1 : 0);
        u32 effctive_offset;
        
        Assert(!draw_cursor || cursor_line - view_offset <= view_limit_y);
        
        
        // If the draw starting point is known, skip up to it. Otherwise; start from the begining...
        u32 char_skip_count;
        if(visible_text_start)
        {
            str_begin = visible_text_start;
            char_skip_count = u32(visible_text_start - str->buffer);
            effctive_offset = 0;
            cursor_line -= view_offset;
        }
        else
        {
            char_skip_count = 0;
            str_begin = str->buffer;
            effctive_offset = view_offset;
        }
        
        u32 line_start = 0;
        u32 line_num = 0;
        u32 lines_drawn = 0;
        
        u32 hl_p0 = write_cursor_position;
        u32 hl_p1 = text_select_start_point;
        if(hl_p1 < hl_p0)
            Swap(&hl_p0, &hl_p1);
        
        u32 hl_dif = hl_p1 - hl_p0;
        
        for(char* c = str_begin; c <= str_end; ++c)
        {
            u32 begin_offset = u32(c - str_begin);
            u32 exclusive_lenght = begin_offset - line_start;
            
            b32 is_new_line = *c == '\n';
            b32 is_last_char = c == str_end;
            
            if(is_new_line || exclusive_lenght >= view_limit_x || is_last_char)
            {
                if((!is_last_char || exclusive_lenght >= view_limit_x) && !is_new_line)
                {
                    c -= 1;
                    is_last_char = false;
                }
                
                if(line_num >= effctive_offset)
                {
                    lines_drawn += 1;
                    if(lines_drawn > view_limit_y)
                        break;
                    
                    // Draw highlight
                    if(text_select_mode && hl_dif)
                    {
                        Color hl_color = theme->write_cursor_color;
                        
                        u32 effct_line_start = line_start + char_skip_count;
                        
                        u32 effct_len = exclusive_lenght + is_last_char;
                        u32 line_end = effct_line_start + effct_len;
                        
                        // Highlight starts on this line.
                        if(hl_p0 >= effct_line_start && hl_p0 < line_end)
                        {
                            u32 start_offset = hl_p0 - effct_line_start;
                            u32 max_len = effct_len - start_offset;
                            
                            u32 hl_len = Min(hl_dif, max_len);
                            if(hl_len > 0)
                            {
                                v2f hl_pos = v2f{text_p.x + f32(start_offset) * char_width, text_p.y};
                                v2f hl_dim = v2f{char_width * f32(hl_len), char_height};
                                Rect hl_rect = Create_Rect_Min_Dim(hl_pos, hl_dim);
                                
                                Draw_Filled_Rect(context->canvas, hl_rect, hl_color);
                            }
                        }
                        
                        // Highlight is surrounded by this line.
                        else if(hl_p0 <= effct_line_start && hl_p1 >= line_end)
                        {
                            u32 hl_len = effct_len;
                            
                            // EDGE_CASE: only char on the line is a new line.
                            if(hl_len == 0 && is_new_line)
                                hl_len = 1;
                            
                            // EDGE_CASE: last char is a new line.
                            else if(is_last_char && is_new_line && hl_len > 1)
                                hl_len -= 1;
                            
                            v2f hl_dim = v2f{char_width * f32(hl_len), char_height};
                            Rect hl_rect = Create_Rect_Min_Dim(text_p, hl_dim);
                            
                            Draw_Filled_Rect(context->canvas, hl_rect, hl_color);
                        }
                        
                        // Highlight start on previous line, but ends on this line.
                        else if(hl_p0 < effct_line_start && hl_p1 > effct_line_start && hl_p1 < line_end)
                        {
                            u32 hl_len = hl_p1 - effct_line_start;
                            v2f hl_dim = v2f{char_width * f32(hl_len), char_height};
                            
                            Rect hl_rect = Create_Rect_Min_Dim(text_p, hl_dim);
                            
                            Draw_Filled_Rect(context->canvas, hl_rect, hl_color);
                        }
                    }
                    
                    String_View view = {str_begin + line_start, exclusive_lenght + is_last_char};
                    Draw_Text(context->canvas, view, text_p, theme->widget_text_color, font, text_scale);
                    
                    if(draw_cursor && line_num + 1 == cursor_line)
                    {
                        draw_cursor = false;
                        v2f cursor_p = text_p;
                        if(is_new_line && is_last_char && write_cursor_position == str->lenght)
                        {
                            cursor_p.y -= char_height;
                        }
                        else
                        {
                            u32 begin_offset = u32(str_begin - str->buffer);
                            
                            Assert(line_start + begin_offset <= write_cursor_position);
                            cursor_p.x += f32(write_cursor_position - line_start - begin_offset) * char_width;
                        }
                        
                        Color cursor_color = (str->lenght < character_limit || character_limit == 0)? 
                            theme->write_cursor_color : theme->write_cursor_limit_color;
                        
                        Draw_Vertical_Line(
                            context->canvas, 
                            cursor_p, 
                            f32(font->char_height) * text_scale.y, 
                            cursor_color);
                    }
                    
                    text_p.y -= char_height;
                }
                
                line_start += exclusive_lenght;
                if(is_new_line)
                    line_start += 1;
                
                line_num += 1;
            }
        }
    }
}


static bool GUI_Do_Sub_Context(
    GUI_Context* master, 
    GUI_Context* sub_context,
    Canvas* sub_canvas, 
    v2f* pos,
    v2f* dim,
    Color* bg_color_ptr = 0,
    GUI_Link_Direction::Type ld = GUI_Link_Direction::up)
{
    // --------------------------------------------------------------------------
    GUI_Placement p = GUI_Get_Placement(master, dim, pos);
    if(!Is_Rect_Valid(p.rect) || master->mode == GUI_Mode::layout_only)
        return false;
    
    u32 id = GUI_Generate_ID(p.rect, __LINE__);
    
    bool cursor_on_selection = Is_Point_Inside_Rect(master->cursor_fpos, p.rect);
    bool is_selected = GUI_Is_Element_Selected(master, cursor_on_selection, id);
    bool is_sub_context_active = false;
    
    if(is_selected)
    {
        GUI_Sub_Context_State* state = &master->selection_state.sub_context;

        if(GUI_On_Release_Action(master, cursor_on_selection, &state->is_pressed_down))
        {
            state->is_active = true;
        }
        
        if(master->actions[GUI_Menu_Actions::back].Is_Pressed() &&
            Bit_Not_Set(sub_context->flags, GUI_Context_Flags::one_time_taking_control_of_external_input))
        {
            state->is_active = false;
        }
        
        is_sub_context_active = state->is_active;
    }
    
    if(is_sub_context_active)
    {
        master->flags |= GUI_Context_Flags::one_time_disable_kc_navigation;
        
        if(master->external_action_context)
            master->external_action_context->disable_for_one_frame = true;
    }
    
    // --------------------------------------------------------------------------
    
    
    bool result = false;
    if(Is_Rect_Valid(p.rect) && Rects_Overlap(p.rect, master->canvas_rect))
    {
        GUI_Theme* theme = master->theme;
        f32 outline_thickness = f32(theme->outline_thickness);
        
        // Drawing
        {
            
            Color bg_color = bg_color_ptr? *bg_color_ptr : theme->background_color;
            Color outline_color = theme->outline_color;
            
            if(is_selected)
            {
                outline_color = is_sub_context_active?  theme->selected_color : theme->down_color;
            }
            
            Draw_Filled_Rect_With_Outline(
                master->canvas, 
                p.rect, 
                bg_color,
                theme->outline_thickness, 
                outline_color);
        }
        
        v2f pf = p.rect.min - Trunc(p.rect.min);
        
        p.rect.min += outline_thickness;
        v2f cp = Floor(p.rect.min);
        cp.x = Max(cp.x, 0.f);
        cp.y = Max(cp.y, 0.f);
        
        v2f rendering_offset = {};
        rendering_offset.x = (p.rect.min.x < 0)? p.rect.min.x : 0.f;
        rendering_offset.y = (p.rect.min.y < 0)? p.rect.min.y : 0.f;
       
        v2i canvas_pos = cp.As<i32>();
        v2u buffer_offset = canvas_pos.As<u32>();
        
        if(buffer_offset.x < master->canvas->dim.x - 1 && buffer_offset.y < master->canvas->dim.y - 1)
        {
            v2f sub_dim_f32 = Ceil(p.dim + rendering_offset - outline_thickness * 2);
            if(sub_dim_f32.x > 0 && sub_dim_f32.y > 0)
            {
                v2u sub_dim = sub_dim_f32.As<u32>();
                
                v2u sub_canvas_dim_max = master->canvas->dim - v2u{1, 1} - buffer_offset;
                v2u sub_canvas_dim = {};
                sub_canvas_dim.x = Min(sub_dim.x, sub_canvas_dim_max.x);
                sub_canvas_dim.y = Min(sub_dim.y, sub_canvas_dim_max.y);
                
                Assert(sub_canvas_dim.x && sub_canvas_dim.y);
                
                *sub_canvas = Create_Sub_Canvas(
                    master->canvas,
                    sub_canvas_dim,
                    buffer_offset);
                
                {
                    if(master->flags & GUI_Context_Flags::hard_ignore_selection || !is_sub_context_active)
                        sub_context->flags |= GUI_Context_Flags::hard_ignore_selection;
                    else
                        Inverse_Bit_Mask(&sub_context->flags, GUI_Context_Flags::hard_ignore_selection);
                    
                    if(master->flags & GUI_Context_Flags::enable_dynamic_sliders)
                        sub_context->flags |= GUI_Context_Flags::enable_dynamic_sliders;   
                }
                
                if(is_sub_context_active)
                {
                    sub_context->_master_id = master->_context_id;
                    GUI_Context::active_context_id = sub_context->_context_id;
                }                
                
                GUI_Begin_Context(
                    sub_context, 
                    sub_canvas, 
                    master->external_action_context,
                    master->theme,
                    canvas_pos,
                    master->layout.anchor,
                    master->layout.build_direction,
                    ld);

                sub_context->canvas_space_dim = sub_dim - Ceil(rendering_offset).As<u32>();
                
                if(rendering_offset.x == 0)
                {
                    rendering_offset.x = pf.x;
                }
                
                if(rendering_offset.y == 0)
                {
                    rendering_offset.y = pf.y;
                }
                
                sub_context->rendering_offset = rendering_offset;
                
                result = true;                
            }
        }
    }
    
    return result;
}