

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

static thread_local v2f GUI_AUTO_FIT                = v2f{0, 0};

static thread_local v2f GUI_AUTO_TOP_CENTER         = v2f{0, 0};
static thread_local v2f GUI_AUTO_TOP_RIGHT          = v2f{0, 0};
static thread_local v2f GUI_AUTO_TOP_LEFT           = v2f{0, 0};

static thread_local v2f GUI_AUTO_MIDDLE_RIGHT       = v2f{0, 0};
static thread_local v2f GUI_AUTO_MIDDLE_LEFT        = v2f{0, 0};

static thread_local v2f GUI_AUTO_BOTTOM_CENTER      = v2f{0, 0};
static thread_local v2f GUI_AUTO_BOTTOM_RIGHT       = v2f{0, 0};
static thread_local v2f GUI_AUTO_BOTTOM_LEFT        = v2f{0, 0};

static thread_local v2f GUI_AUTO_MIDDLE             = v2f{0, 0};

static thread_local v2f GUI_DEFAULT_TEXT_SCALE      = v2f{1.f, 1.f};
static thread_local v2f GUI_DEFAULT_TITLE_SCALER    = v2f{1.5f, 1.5f};

static thread_local u32 GUI_NO_CHARACTER_LIMIT      = 0;


struct GUI_Theme
{
    Color selected_color;
    Color background_color;
    Color down_color;
    Color outline_color;
    Color text_color;
    Color widget_text_color;
    Color title_color;
    Color write_cursor_color;
    Color write_cursor_limit_color;
    
    u32 outline_thickness;
    f32 padding;
    
    Font font;
};

 
struct GUI_Input_Acceleration_Behavior
{
    f64 input_speed_up_time = 0.2f;
    f64 input_delay_time = 0.1f;
    f64 max_speed_up_factor = 50.f;
};


struct GUI_Highlight
{
    i32 idx = 0;
    i32 count = 0;
};


namespace GUI_Menu_Actions
{
    enum : u32
    {
        back = 0,
        enter,
        up,
        down,
        left,
        right,
        mouse,
        COUNT
    };
}


namespace GUI_Link_Direction
{
    enum Type : i8
    {
        up  = 1,
        down = -1
    };    
}

enum class GUI_Theme_Coloring : u8
{
    do_not_apply = 0,
    apply
};


enum class GUI_Build_Direction : u8
{
    up_center = 0,
    up_left,
    up_right,
    down_center,
    down_left,
    down_right,
    left_center,
    left_top,
    left_bottom,
    right_center,
    right_top,
    right_bottom
};


enum class GUI_Anchor : u8
{
    center = 0,
    top,
    top_left,
    top_right,
    left,
    right,
    bottom,
    bottom_left,
    bottom_right
};


enum class GUI_Defered_Render_Type
{
    none = 0,
    dropdown_button
};


enum class GUI_Mode : u8
{
    normal = 0,
    layout_only,
};


enum class GUI_Cardinal_Direction : u8
{
    left_right = 0,
    up_down
};


struct GUI_Button_State
{
    bool is_pressed_down;
};


struct GUI_Slider_State
{
    f64 input_start_time;
    f64 next_input_time;
    v2f drag_offset;
    
    bool is_held_down;
};


struct GUI_Dropdown_Button_State
{
    char** element_names;
    GUI_Theme* theme;
    
    u32 selected_element_idx;
    u32 element_count;
    Rect open_rect;
    v2f text_scale;
    v2f pos;
    v2f dim;
    
    bool is_pressed_down;
    bool is_open;
};


// NOTE: Is actually used for ML as well atm.
struct GUI_SL_Input_Field_State
{
    u32 view_offset = 0;
    u32 write_cursor_position = 0;
    u32 text_select_start_point = 0;
    u32 mouse_press_down_point = 0;
    u32 click_p = 0;

    v2f mouse_scroll_bar_drag_offset = v2f{0, 0};
    
    f64 flicker_start_time = 0;
    f64 next_input_time = 0;
    f64 input_start_time = 0;
    f64 mouse_hold_time = 0;
    
    // CONSIDER: Flags?
    bool scroll_bar_drag_mode = 0;
    bool text_select_mode = 0;
    bool is_pressed_down = 0;
    bool is_active = 0;
    bool draw_cursor = 0;
    bool cursor_is_active = 0; // NOTE: ML only?

    static constexpr f64 mouse_hold_delay = 3.0;
    static constexpr f64 flicker_delay = 3.0;
};


struct GUI_Sub_Context_State
{
    bool is_pressed_down;
    bool is_active;
};


union GUI_Element_State
{
    GUI_Button_State button;
    GUI_Slider_State slider;
    GUI_Dropdown_Button_State dropdown_button;
    GUI_SL_Input_Field_State sl_input_field;
    GUI_Sub_Context_State sub_context;
};


// TODO: Rework the flags so it's clear wich ones are for internal state!
namespace GUI_Context_Flags
{
    enum : u32
    {
        soft_ignore_selection                           = 1 << 0, // Makes it so that nothing is selected, but mouse selection is still possible to exit this state.
        enable_dynamic_sliders                          = 1 << 1,
        cursor_mask_validation                          = 1 << 2,
        cursor_mask_enabled                             = 1 << 3,
        disable_wrapping                                = 1 << 4,
        one_time_disable_kc_navigation                  = 1 << 5,
        disable_mouse_scroll                            = 1 << 6,
        context_ready                                   = 1 << 7,
        maxout_vertical_slider                          = 1 << 8,
        maxout_horizontal_slider                        = 1 << 9,
        hard_ignore_selection                           = 1 << 10, // Makes it so that nothing is selected and to leave this state this flag has to manually set to 0.
        one_time_skip_padding                           = 1 << 11,
        dont_auto_activate                              = 1 << 12,
        one_time_ignore_id                              = 1 << 13, // CONSIDER: This is really hacky, but do we event need ids?
        one_time_taking_control_of_external_input       = 1 << 14,
        enable_auto_layout_bounds_collision_offseting   = 1 << 15,
    }; // max shift is 31
}


struct GUI_Placement
{
    v2f pos;
    v2f dim;
    
    Rect rect;
};


struct GUI_Layout
{
    GUI_Placement last_element;
    
    GUI_Build_Direction build_direction;
    GUI_Anchor anchor;
};


struct GUI_Context
{
    // Globals
    static inline u32 active_context_id = 0;
    static inline Action actions[GUI_Menu_Actions::COUNT];
    static inline Action_Context action_context;
    static inline Platform_Calltable* platform = 0;
    // -------
    
    Canvas* canvas = 0;
    Action_Context* external_action_context;
    Rect bounds_rel_anchor_base = {};
    GUI_Theme* theme = 0;
    
    u32 _master_id = 0;
    u32 _context_id = 0;
    u32 flags = 0;
    i32 selected_index = 0;
    i32 widget_count = 0; // NOTE: Amount of selectable elements.
    u32 selected_id = 0;
    f32 mouse_scroll_speed = 50.f;
    f32 dynamic_slider_girth = 15.f;
    i32 last_widget_count = 0;
    
    v2i last_cursor_position = {};
    v2i cursor_position = {};
    v2u canvas_space_dim = {};
    v2f rendering_offset = {};
    v2f cursor_fpos;
    v2i canvas_pos = {};
    v2f anchor_base = {};
    v2f selected_element_pos = {};
    v2f selected_element_dim = {};
    
    Rect cursor_mask_area = {};
    Rect canvas_rect = {};
    
    GUI_Layout layout = {};
    
    
    GUI_Element_State selection_state = {};
    GUI_Defered_Render_Type defered_render = GUI_Defered_Render_Type::none;

    u32 layout_stack_count = 0;
    GUI_Layout layout_stack[5];
    
    GUI_Mode mode = {};
};


static bool GUI_Do_Handle_Slider(
    GUI_Context* context, 
    v2f* pos, 
    v2f* dim, 
    f32* value,
    f32 max = 100,
    f32 min = 0,
    GUI_Cardinal_Direction cardinal_dir = GUI_Cardinal_Direction::left_right,
    f32 step_count = 0,
    GUI_Input_Acceleration_Behavior inp_accel = GUI_Input_Acceleration_Behavior());


enum class GUI_Default_Theme_Names : u8
{
    vintage,
    document,
    cyper,
    oasis,
};