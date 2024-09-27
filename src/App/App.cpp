

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

#include "App.h"
#include "Main_App.h"


static Color s_background_color = Make_Color(30, 30, 70);
static Platform_Calltable s_platform = {};
static Canvas s_canvas = {};
static Action_Context s_action_context = {};
static Font s_font;
static GUI_Theme s_theme;
static GUI_Context s_gui;

static Tetris s_tetris = {};

namespace Inputs
{
    enum T
    {
        down,
        left,
        right,
        clockwise_rotate,
        counter_cw_rotate,
        drop,
        pause,
        restart,
        quit,
        back,
        COUNT
    };
}
static Action s_actions[u32(Inputs::T::COUNT)] = {};


void Init_App(Platform_Calltable platform_calltable)
{
    s_platform = platform_calltable;
    Pixel_Buffer pxb = s_platform.Get_Pixel_Buffer();
    
    Init_Canvas(&s_canvas, pxb.memory, {u32(pxb.width), u32(pxb.height)});
    
    s_actions[Inputs::down] = Make_Action(Key_Code::S, Button::NONE);
    s_actions[Inputs::drop] = Make_Action(Key_Code::X, Button::NONE);
    s_actions[Inputs::left] = Make_Action(Key_Code::A, Button::NONE);
    s_actions[Inputs::right] = Make_Action(Key_Code::D, Button::NONE);
    s_actions[Inputs::clockwise_rotate] = Make_Action(Key_Code::E, Button::NONE);
    s_actions[Inputs::counter_cw_rotate] = Make_Action(Key_Code::Q, Button::NONE);
    s_actions[Inputs::pause] = Make_Action(Key_Code::P, Button::NONE);
    s_actions[Inputs::restart] = Make_Action(Key_Code::R, Button::NONE);
    s_actions[Inputs::quit] = Make_Action(Key_Code::ENTER, Button::NONE);
    s_actions[Inputs::back] = Make_Action(Key_Code::ESC, Button::NONE);
    
    s_font.char_width = s_terminus_font_char_width;
    s_font.char_height = s_terminus_font_char_height;
    s_font.data_buffer = (u8*)s_terminus_font;
    s_font.data_buffer_sc = (u8*)s_terminus_font_special_characters;
    s_theme = GUI_Create_Default_Theme(GUI_Default_Theme_Names::cyper, s_font); 
    
    s_gui = GUI_Create_Context();
    GUI_Set_Default_Menu_Actions();
    s_gui.platform = &s_platform;
    
    // For ease of creation the tetroes are defined upside down in code, so flip here.
    // Yes I'm contributing to global warming by doing this.
    for(i32 i = 0; i < Array_Lenght(s_tetrominoes); ++i)
    {
        s_tetrominoes[i] = Flip_Block_Grid_Y(s_tetrominoes[i]);
    }
    
    s_game_state = Game_State::menu;
    
    u32 buffer_size;
    if(s_platform.Get_File_Size("TIAD_Score", &buffer_size))
    {
        if(buffer_size == sizeof(s_tetris.highscore))
            s_platform.Read_File("TIAD_Score", (u8*)&s_tetris.highscore, sizeof(s_tetris.highscore));
    }
}



void Spawn_New_Tetro()
{
    u32 range = Array_Lenght(s_tetrominoes);
    u32 idx = s_tetris.random_machine.random_u32(range);
    
    v2i p = v2i{Tetris::board_width / 2, Tetris::board_height};
    s_tetris.active_tetro = Create_Tetronome(Block(idx + 1), p);
}


static void Reset_Game()
{
    s_game_state = Game_State::running;
    s_tetris.board = {};
    s_tetris.score = 0;
    s_tetris.step_frequency = 0.8f;
    s_tetris.is_highscore = false;
    
    Spawn_New_Tetro();
}

static inline bool Move_Tetro_Down()
{
    bool result = true;
    
    i32_Rect bounds = Get_Bounds(s_tetris.active_tetro.grid);
    if(s_tetris.active_tetro.pos.y - 1 + bounds.min.y >= 0)
    {
        s_tetris.active_tetro.pos.y -= 1;
        if(Check_Tetro_Against_Board_Collision(s_tetris.active_tetro, &s_tetris.board))
        {
            s_tetris.active_tetro.pos.y += 1;
            s_tetris.score += Apply_Tetro_To_Board(s_tetris.active_tetro, &s_tetris.board);
            Spawn_New_Tetro();
            result = false;
        }
    }
    else
    {
        s_tetris.score += Apply_Tetro_To_Board(s_tetris.active_tetro, &s_tetris.board);
        Spawn_New_Tetro();
        result = false;
    }
    
    return result;
}


static void Render()
{
    Clear_Canvas(&s_canvas, s_background_color);

    {                
        v2u score_area_offset = v2u{0, u32((Tetris::board_height + 2) * s_block_side_lenght)};
        
        Canvas score_area = 
            Create_Sub_Canvas(&s_canvas, v2u{s_canvas.dim.x, 32 * 6}, score_area_offset);
        
        Clear_Sub_Canvas(&score_area, BLACK);
        
        // Draw next block.
        u32 noise_position = s_tetris.random_machine.noise_position;
        u32 range = Array_Lenght(s_tetrominoes);
        u32 idx = s_tetris.random_machine.random_u32(range);
        s_tetris.random_machine.noise_position = noise_position;
        
        v2f score_area_origin = v2f{f32(s_canvas.dim.x) - s_block_side_lenght * 5, 0};
        
        i32_Rect bounds = Get_Bounds(s_tetrominoes[idx]);
        
        f32 width = f32((bounds.max.x + 1) - bounds.min.x) * s_block_side_lenght;
        f32 total_width = s_block_side_lenght * 4;
        f32 x_offset = total_width / 2 - width / 2;
        
        
        f32 height = f32((bounds.max.y + 1) - bounds.min.y) * s_block_side_lenght;
        f32 total_height = s_block_side_lenght * 4;
        f32 y_offset = total_height / 2 - height / 2;
        
        
        
        Block block = Block(idx + 1);
        for(i32 y = 0; y < 4; ++y)
        {
            for(i32 x = 0; x < 4; ++x)
            {
                if(u8(s_tetrominoes[idx][y * 4 + x]))
                {
                    Draw_Block(&score_area, v2i{x, y}, score_area_origin + v2f{x_offset, y_offset}, block);
                }
            }
        }
        
        {
            for(i32 y = -1; y < 5; ++y)
            {
                Draw_Block(&score_area, v2i{-1, y}, score_area_origin, Block::_);
                Draw_Block(&score_area, v2i{4, y}, score_area_origin, Block::_);
            }
            
            for(i32 x = 0; x < 5; ++x)
            {
                Draw_Block(&score_area, v2i{x, 4}, score_area_origin, Block::_);
            }
        }
        
        
        v2f p1 = score_area_origin;
        {
            p1.y += s_block_side_lenght * 5;
            Draw_Text(&score_area, (u8*)"Next Tetromeno:", p1, WHITE, &s_font, v2f{1.f, 1.f});
        }
        
        {
            f32 char_height = f32(s_font.char_height);
            
            v2f scale_title = v2f{1.f, 1.f};
            v2f scale_score = v2f{3.f, 3.f};
            
            v2f p = {s_block_side_lenght, p1.y};
            Draw_Text(&score_area, (u8*)"Score:", p, WHITE, &s_font, scale_title);
            
            p.y -= char_height * scale_score.y;
            u8 num[12];
            char* t = U32_To_Char_Buffer(num, s_tetris.score);
            Draw_Text(&score_area, (u8*)t, p, WHITE, &s_font, scale_score);
            
            p.y -= char_height * scale_score.y;
            Draw_Text(&score_area, (u8*)"Highscore:", p, WHITE, &s_font, scale_title);
            
            p.y -= char_height * scale_score.y;
            t = U32_To_Char_Buffer(num, s_tetris.highscore);
            Draw_Text(&score_area, (u8*)t, p, WHITE, &s_font, scale_score);
        }
    }
    
    
    v2f board_origin = v2f{} + s_block_side_lenght;
    
    // Draw outline
    {
        for(i32 y = -1; y < Tetris::board_height + 1; ++y)
        {
            Draw_Block(&s_canvas, v2i{-1, y}, board_origin, Block::_);
            Draw_Block(&s_canvas, v2i{Tetris::board_width, y}, board_origin, Block::_);
        }
        
        for(i32 x = 0; x < Tetris::board_width; ++x)
        {
            Draw_Block(&s_canvas, v2i{x, -1}, board_origin, Block::_);
            Draw_Block(&s_canvas, v2i{x, Tetris::board_height}, board_origin, Block::_);
        }
    }
    
    // Draw board
    {
        for(i32 y = 0; y < Tetris::board_height; ++y)
        {
            for(i32 x = 0; x < Tetris::board_width; ++x)
            {
                Block b = s_tetris.board[y * Tetris::board_width + x];
                if(u32(b))
                {
                    Draw_Block(&s_canvas, v2i{x, y}, board_origin, b);
                }
            }
        }
    }
    
    // Draw active tetro
    {
        for(i32 y = 0; y < 4; ++y)
        {
            for(i32 x = 0; x < 4; ++x)
            {
                i32 idx = y * 4 + x;
                if(s_tetris.active_tetro.grid[idx] != Block::none)
                {
                    v2i p = s_tetris.active_tetro.pos + v2i{x, y};
                    
                    if(p.y < Tetris::board_height && p.x < Tetris::board_width && p.y >= 0 && p.x >= 0)
                        Draw_Block(&s_canvas, p, board_origin, s_tetris.active_tetro.grid[idx]);
                }
            }
        }
    }
}

void Update_Game()
{
    f64 frame_time = s_platform.Get_Frame_Time();
    if(frame_time <= 1.f / 30.f)
    {
        s_tetris.accumilated_time += frame_time;
    }
    
    // Logic
    {   
        if(s_actions[Inputs::drop].Is_Pressed())
        {
            while(Move_Tetro_Down());
            s_tetris.accumilated_time = 0;
        }
        
        if(s_actions[Inputs::down].Is_Pressed() || s_tetris.accumilated_time >= s_tetris.step_frequency)
        {
            Move_Tetro_Down();
            s_tetris.accumilated_time = 0;
            s_tetris.step_frequency = Max(s_tetris.step_frequency - 0.01 * Cube(s_tetris.step_frequency), 0.05);
        }
        
        if(s_actions[Inputs::left].Is_Pressed())
        {
            i32_Rect bounds = Get_Bounds(s_tetris.active_tetro.grid);
            if(s_tetris.active_tetro.pos.x - 1 + bounds.min.x >= 0)
            {
                s_tetris.active_tetro.pos.x -= 1;
                if(Check_Tetro_Against_Board_Collision(s_tetris.active_tetro, &s_tetris.board))
                {
                    s_tetris.active_tetro.pos.x += 1;
                }                    
            }
        }
        
        if(s_actions[Inputs::right].Is_Pressed())
        {
            i32_Rect bounds = Get_Bounds(s_tetris.active_tetro.grid);
            if(s_tetris.active_tetro.pos.x + 1 + bounds.max.x < Tetris::board_width)
            {
                s_tetris.active_tetro.pos.x += 1;
                if(Check_Tetro_Against_Board_Collision(s_tetris.active_tetro, &s_tetris.board))
                {
                    s_tetris.active_tetro.pos.x -= 1;
                } 
            }
        }
        
        if(s_actions[Inputs::clockwise_rotate].Is_Pressed())
        {
            Tetronome tetro = s_tetris.active_tetro;
            tetro.grid = Clockwise_Rotate(s_tetris.active_tetro.grid, s_tetris.active_tetro.origin);
            Snap_Tetro_To_Board(&tetro);
            if(!Check_Tetro_Against_Board_Collision(tetro, &s_tetris.board))
            {
                s_tetris.active_tetro = tetro;
            }
        }
        
        if(s_actions[Inputs::counter_cw_rotate].Is_Pressed())
        {
            Tetronome tetro = s_tetris.active_tetro;
            tetro.grid = Counter_CW_Rotate(s_tetris.active_tetro.grid, s_tetris.active_tetro.origin);
            Snap_Tetro_To_Board(&tetro);
            if(!Check_Tetro_Against_Board_Collision(tetro, &s_tetris.board))
            {
                s_tetris.active_tetro = tetro;
            }
        }
    }
}


void Update_App()
{
    u32 app_flags = s_platform.Get_Flags();
    
    // User asked to end the process.
    if(Is_Flag_Set(app_flags, u32(App_Flags::wants_to_exit)))
    {
        if(s_game_state == Game_State::menu)
        {
            s_platform.Set_Flag(App_Flags::is_running, false);
        }
        else            
        {
            s_platform.Set_Flag(App_Flags::wants_to_exit, false);
            s_stored_game_state = s_game_state;
            s_game_state = Game_State::quit;
        }
    }
    
    Update_Actions(&s_platform, s_actions, Array_Lenght(s_actions), &s_action_context);
    
    if(Is_Flag_Set(app_flags, u32(App_Flags::is_focused)))
    {
        Game_State pre_state = s_game_state;
        
        switch(s_game_state)
        {
            case Game_State::running:
            {
                if(s_actions[Inputs::pause].Is_Pressed())
                    s_game_state = Game_State::paused;
                
                Update_Game();
                Render();
            }break;
            
            case Game_State::paused:
            {
                if(s_actions[Inputs::pause].Is_Pressed())
                    s_game_state = Game_State::running;
                
                Render();
                Dim_Entire_Screen(&s_canvas, 0.1f);
                
                char* t = "Paused [P]";
                v2f p = Get_Middle(&s_canvas);
                u32 leng = Null_Terminated_Buffer_Lenght(t);
                p.x -= (f32(s_font.char_width) * f32(leng) * 3.f) / 2;
                
                Draw_Text(&s_canvas, (u8*)t, p, WHITE, &s_font, v2f{3.f, 3.f});
            }break;
            
            case Game_State::game_over:
            {
                Render();
                Dim_Entire_Screen(&s_canvas, 0.1f);
                
                GUI_Begin_Context(
                    &s_gui, 
                    &s_canvas, 
                    &s_action_context,
                    &s_theme,
                    v2i{0, 0},
                    GUI_Anchor::top,
                    GUI_Build_Direction::down_center);
                {
                    v2f scale = v2f{3.f, 3.f};
                    Color c = WHITE;
                    
                    if(s_tetris.is_highscore)
                    {
                        GUI_Do_Text(&s_gui, &GUI_AUTO_MIDDLE, "New Highscore!", &c, scale);
                        GUI_Do_Text(&s_gui, 0, "Press [R]", &c, scale);
                    }
                    else
                    {                    
                        GUI_Do_Text(&s_gui, &GUI_AUTO_MIDDLE, "Game Over [R]", &c, scale);
                    }
                }                
                GUI_End_Context(&s_gui);
                
                if(s_actions[Inputs::restart].Is_Pressed())
                    Reset_Game();
                
            }break;
            
            case Game_State::menu:
            {
                // Draw outline
                {
                    v2f board_size = v2f{f32(Tetris::board_width), f32(Tetris::board_height)};
                    v2f dim = Hadamar_Product(v2f{} + s_block_side_lenght, board_size);
                    Rect rect = Create_Rect_Min_Dim(v2f{} + s_block_side_lenght, dim);
                    Draw_Filled_Rect(&s_canvas, rect, s_background_color);
                    
                    for(i32 y = 0; y < Tetris::board_height + 2; ++y)
                    {
                        Draw_Block(&s_canvas, v2i{0, y}, v2f{0, 0}, Block::_);
                        Draw_Block(&s_canvas, v2i{Tetris::board_width + 1, y}, v2f{0, 0}, Block::_);
                    }
                    
                    for(i32 x = 1; x < Tetris::board_width + 1; ++x)
                    {
                        Draw_Block(&s_canvas, v2i{x, 0}, v2f{0, 0}, Block::_);
                        Draw_Block(&s_canvas, v2i{x, Tetris::board_height + 1}, v2f{0, 0}, Block::_);
                    }
                }
                
                GUI_Begin_Context(
                    &s_gui, 
                    &s_canvas, 
                    &s_action_context,
                    &s_theme,
                    v2i{0, 0},
                    GUI_Anchor::top,
                    GUI_Build_Direction::down_center);
                {
                    v2f scale = v2f{3.f, 3.f};
                    Color c = WHITE;
                    
                    GUI_Do_Text(&s_gui, &GUI_AUTO_TOP_CENTER, "TIAD", &c, scale);
                    
                    scale = v2f{2.f, 2.f};
                    GUI_Do_Text(&s_gui, 0, "HIGHSCORE:", &c, scale);
                    
                    u8 num[12];
                    char* t = U32_To_Char_Buffer(num, s_tetris.highscore);
                    GUI_Do_Text(&s_gui, 0, t, &c, scale);
                
                    GUI_Do_Text(&s_gui, 0, "Press [R] to play", &c, scale);
                    
                    if(s_actions[Inputs::restart].Is_Pressed())
                    {
                        Reset_Game();
                    }
                }                
                GUI_End_Context(&s_gui);
            }break;
            
            case Game_State::quit:
            {
                Render();
                Dim_Entire_Screen(&s_canvas, 0.1f);
                
                GUI_Begin_Context(
                    &s_gui, 
                    &s_canvas, 
                    &s_action_context,
                    &s_theme,
                    v2i{0, 0},
                    GUI_Anchor::top,
                    GUI_Build_Direction::down_center);
                {
                    v2f scale = v2f{3.f, 3.f};
                    Color c = WHITE;
                    
                    GUI_Do_Text(&s_gui, &GUI_AUTO_MIDDLE, "QUIT GAME?", &c, scale);
                    GUI_Do_Text(&s_gui, 0, "Yes [Enter]", &c, scale);
                    GUI_Do_Text(&s_gui, 0, "No [ESC]", &c, scale);
                    
                    if(s_actions[Inputs::quit].Is_Pressed())
                    {
                        s_platform.Set_Flag(App_Flags::is_running, false);
                    }
                    
                    if(s_actions[Inputs::back].Is_Pressed())
                    {
                        s_game_state = s_stored_game_state;
                    }
                }
                GUI_End_Context(&s_gui);
            }break;
        }
        
        if(s_game_state == Game_State::game_over && pre_state != Game_State::game_over)
        {
            if(s_tetris.score > s_tetris.highscore)
            {
                s_tetris.is_highscore = true;
                s_tetris.highscore = s_tetris.score;
                s_platform.Write_File("TIAD_Score", (u8*)&s_tetris.highscore, sizeof(s_tetris.highscore));
            }
        }
    } 
}