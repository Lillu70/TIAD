

#pragma once

#include <intrin.h>

#include "Utility\Primitives.h"
#include "Utility\Assert.h"
#include "Utility\Vector.h"
#include "Utility\Vector_Ex.h"
#include "Utility\Bits.h"
#include "Utility\Rect.h"
#include "Utility\Color.h"
#include "Utility\Maths.h"
#include "Utility\Utility.h"

#include "Platform\Input.h"
#include "Platform\os.h"

#include "Core\Action.h"
#include "Core\Renderer.h"
#include "Core\Renderer.cpp"

#include "Misc\Random_Machine.h"
#include "Misc\Terminus_Font.h"

#include "Tetris.h"
#include "Tetronome.cpp"

static Color s_background_color = Make_Color(30, 30, 70);
static Canvas s_canvas = {};
static Action_Context s_action_context = {};
static Font s_font;

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


void Init_Tetris()
{
    {
        v2s window_dim = Platform_Get_Window_Dimensions();
        u32* rc_buffer = (u32*)Platform_Resize_Software_Render_Target_Pixel_Buffer(window_dim);
        
        Init_Canvas(&s_canvas, rc_buffer, window_dim.As<u32>());
    }
    
    s_actions[Inputs::down]                 = Make_Action(Key_Code::S, Button::DPAD_UP);
    s_actions[Inputs::drop]                 = Make_Action(Key_Code::X, Button::BUT_A);
    s_actions[Inputs::left]                 = Make_Action(Key_Code::A, Button::DPAD_LEFT);
    s_actions[Inputs::right]                = Make_Action(Key_Code::D, Button::DPAD_RIGHT);
    s_actions[Inputs::clockwise_rotate]     = Make_Action(Key_Code::E, Button::R_SHLD);
    s_actions[Inputs::counter_cw_rotate]    = Make_Action(Key_Code::Q, Button::L_SHLD);
    s_actions[Inputs::pause]                = Make_Action(Key_Code::P, Button::START);
    s_actions[Inputs::restart]              = Make_Action(Key_Code::R, Button::BACK);
    s_actions[Inputs::quit]                 = Make_Action(Key_Code::ENTER, Button::START);
    s_actions[Inputs::back]                 = Make_Action(Key_Code::ESC, Button::BACK);
    
    s_font.char_width       = s_terminus_font_char_width;
    s_font.char_height      = s_terminus_font_char_height;
    s_font.data_buffer      = (u8*)s_terminus_font;
    s_font.data_buffer_sc   = (u8*)s_terminus_font_special_characters;
    
    
    // For ease of creation the tetroes are defined upside down in code, so flip here.
    // Yes I'm contributing to global warming by doing this.
    for(s32 i = 0; i < Array_Lenght(s_tetrominoes); ++i)
    {
        s_tetrominoes[i] = Flip_Block_Grid_Y(s_tetrominoes[i]);
    }
    
    s_game_state = Game_State::menu;
    
    u32 buffer_size;
    if(Platform_Get_File_Size("TIAD_Score", &buffer_size))
    {
        if(buffer_size == sizeof(s_tetris.highscore))
        {
            Platform_Read_File("TIAD_Score", (u8*)&s_tetris.highscore, sizeof(s_tetris.highscore));
        }
    }
}


void Spawn_New_Tetro()
{
    u32 range = Array_Lenght(s_tetrominoes);
    u32 idx = s_tetris.random_machine.random_u32(range);
    
    v2s p = v2s{Tetris::board_width / 2, Tetris::board_height};
    s_tetris.active_tetro = Create_Tetronome(Block(idx + 1), p);
}


static void Reset_Game()
{
    s_game_state = Game_State::running;
    s_tetris = {};
    s_tetris.step_frequency = 0.8f;
    s_tetris.random_machine.seed = (u32)Platform_Get_Time_Stamp();
    
    Spawn_New_Tetro();
}

static inline bool Move_Tetro_Down()
{
    bool result = true;
    
    s32_Rect bounds = Get_Bounds(s_tetris.active_tetro.grid);
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



static inline void Draw_Block(Canvas* canvas, v2s p, v2f o, Block block)
{
    constexpr Color block_color_table[u32(Block::COUNT)] = 
    {
        Make_Color(0,   0,   0),    // black
        Make_Color(0,   240, 240),  // teal
        Make_Color(0,   0,   240),  // blue
        Make_Color(240, 161, 0),    // orange
        Make_Color(240, 240, 0),    // yellow
        Make_Color(0,   240, 0),    // green
        Make_Color(161, 0,   240),  // purple
        Make_Color(240, 0,   0),    // red
        Make_Color(120, 120, 120)   // gray
    };
    
    
    u8 b = u32(block);
    
    Color color = block_color_table[b];
    Color outline = color;
    
    for(u8* c = (u8*)(&outline); c < (u8*)(&outline) + 3; ++c)
    {
        f32 f = *c * 1.1f;
        
        *c = u8(Min(f, 255.f));
    }
    
    v2f pos = p.As<f32>() * s_block_side_lenght + o;
    
    Rect rect = Create_Rect_Min_Dim(pos, v2f{s_block_side_lenght, s_block_side_lenght});
    Draw_Filled_Rect_With_Outline(canvas, rect, color, 3, outline);
}



static void Render_Game()
{
    Clear_Canvas(&s_canvas, s_background_color);

    {                
        v2u score_area_offset = v2u{0, u32((Tetris::board_height + 2) * s_block_side_lenght)};
        
        Canvas score_area = Create_Sub_Canvas(&s_canvas, v2u{s_canvas.dim.x, 32 * 6}, score_area_offset);
        
        Clear_Sub_Canvas(&score_area, BLACK);
        
        // Draw next block.
        u32 noise_position = s_tetris.random_machine.noise_position;
        u32 range = Array_Lenght(s_tetrominoes);
        u32 idx = s_tetris.random_machine.random_u32(range);
        s_tetris.random_machine.noise_position = noise_position;
        
        v2f score_area_origin = v2f{f32(s_canvas.dim.x) - s_block_side_lenght * 5, 0};
        
        s32_Rect bounds = Get_Bounds(s_tetrominoes[idx]);
        
        f32 width = f32((bounds.max.x + 1) - bounds.min.x) * s_block_side_lenght;
        f32 total_width = s_block_side_lenght * 4;
        f32 x_offset = total_width / 2 - width / 2;
        
        
        f32 height = f32((bounds.max.y + 1) - bounds.min.y) * s_block_side_lenght;
        f32 total_height = s_block_side_lenght * 4;
        f32 y_offset = total_height / 2 - height / 2;
        
        
        
        Block block = Block(idx + 1);
        for(s32 y = 0; y < 4; ++y)
        {
            for(s32 x = 0; x < 4; ++x)
            {
                if(u8(s_tetrominoes[idx][y * 4 + x]))
                {
                    Draw_Block(&score_area, v2s{x, y}, score_area_origin + v2f{x_offset, y_offset}, block);
                }
            }
        }
        
        {
            for(s32 y = -1; y < 5; ++y)
            {
                Draw_Block(&score_area, v2s{-1, y}, score_area_origin, Block::_);
                Draw_Block(&score_area, v2s{4, y}, score_area_origin, Block::_);
            }
            
            for(s32 x = 0; x < 5; ++x)
            {
                Draw_Block(&score_area, v2s{x, 4}, score_area_origin, Block::_);
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
        for(s32 y = -1; y < Tetris::board_height + 1; ++y)
        {
            Draw_Block(&s_canvas, v2s{-1, y}, board_origin, Block::_);
            Draw_Block(&s_canvas, v2s{Tetris::board_width, y}, board_origin, Block::_);
        }
        
        for(s32 x = 0; x < Tetris::board_width; ++x)
        {
            Draw_Block(&s_canvas, v2s{x, -1}, board_origin, Block::_);
            Draw_Block(&s_canvas, v2s{x, Tetris::board_height}, board_origin, Block::_);
        }
    }
    
    // Draw board
    {
        for(s32 y = 0; y < Tetris::board_height; ++y)
        {
            for(s32 x = 0; x < Tetris::board_width; ++x)
            {
                Block b = s_tetris.board[y * Tetris::board_width + x];
                if(u32(b))
                {
                    Draw_Block(&s_canvas, v2s{x, y}, board_origin, b);
                }
            }
        }
    }
    
    // Draw active tetro
    {
        for(s32 y = 0; y < 4; ++y)
        {
            for(s32 x = 0; x < 4; ++x)
            {
                s32 idx = y * 4 + x;
                if(s_tetris.active_tetro.grid[idx] != Block::none)
                {
                    v2s p = s_tetris.active_tetro.pos + v2s{x, y};
                    
                    if(p.y < Tetris::board_height && p.x < Tetris::board_width && p.y >= 0 && p.x >= 0)
                        Draw_Block(&s_canvas, p, board_origin, s_tetris.active_tetro.grid[idx]);
                }
            }
        }
    }
}



void Update_Game()
{
    f64 frame_time = Platform_Get_Frame_Time();
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
            s32_Rect bounds = Get_Bounds(s_tetris.active_tetro.grid);
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
            s32_Rect bounds = Get_Bounds(s_tetris.active_tetro.grid);
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


static _inline v2f Draw_Text_Centered(v2f pos, char* text, v2f scale = v2f{2.f, 2.f})
{
    constexpr Color text_color = WHITE;
    constexpr f32 padding = 10;
    v2f d = v2f
    {
        scale.x * Null_Terminated_Buffer_Lenght(text) * s_font.char_width * 0.5f, 
        scale.y * s_font.char_height * 0.5f
    };
    
    v2f p = pos - d;
    
    Draw_Text(&s_canvas, (u8*)text, p, text_color, &s_font, scale);
    
    return pos - v2f{0, d.y * 2 + padding};
}


bool Update_App()
{
    bool keep_running = true;
    
    u64 app_flags = Platform_Get_Flags();
    
    // User asked to end the process.
    if(app_flags & Platform_Flags::wants_to_exit)
    {
        if(s_game_state == Game_State::menu)
        {
            keep_running = false;
        }
        else
        {
            s_stored_game_state = s_game_state;
            s_game_state = Game_State::quit;
        }
    }
    
    Update_Actions(s_actions, Array_Lenght(s_actions), &s_action_context);
    
    if(app_flags & Platform_Flags::focused)
    {
        Game_State pre_state = s_game_state;
        
        switch(s_game_state)
        {
            case Game_State::running:
            {
                if(s_actions[Inputs::pause].Is_Pressed())
                    s_game_state = Game_State::paused;
                
                Update_Game();
                Render_Game();
            }break;
            
            case Game_State::paused:
            {
                if(s_actions[Inputs::pause].Is_Pressed())
                    s_game_state = Game_State::running;
                
                Render_Game();
                Dim_Entire_Screen(&s_canvas, 0.1f);
                
                Draw_Text_Centered(Get_Middle(&s_canvas), "Paused [P]", v2f{3.f, 3.f});
            }break;
            
            case Game_State::game_over:
            {
                Render_Game();
                Dim_Entire_Screen(&s_canvas, 0.1f);

                if(s_tetris.is_highscore)
                {
                    v2f p = Draw_Text_Centered(Get_Middle(&s_canvas), "New Highscore!", v2f{3.f, 3.f});
                    p = Draw_Text_Centered(p, "Press [R]");
                }
                else
                {
                    v2f p = Draw_Text_Centered(Get_Middle(&s_canvas), "Game Over [R]", v2f{3.f, 3.f});
                }
                
                if(s_actions[Inputs::restart].Is_Pressed())
                {
                    Reset_Game();
                }
                
            }break;
            
            case Game_State::menu:
            {
                // Draw outline
                {
                    v2f board_size = v2f{f32(Tetris::board_width), f32(Tetris::board_height)};
                    v2f dim = Hadamar_Product(v2f{} + s_block_side_lenght, board_size);
                    Rect rect = Create_Rect_Min_Dim(v2f{} + s_block_side_lenght, dim);
                    Draw_Filled_Rect(&s_canvas, rect, s_background_color);
                    
                    for(s32 y = 0; y < Tetris::board_height + 2; ++y)
                    {
                        Draw_Block(&s_canvas, v2s{0, y}, v2f{0, 0}, Block::_);
                        Draw_Block(&s_canvas, v2s{Tetris::board_width + 1, y}, v2f{0, 0}, Block::_);
                    }
                    
                    for(s32 x = 1; x < Tetris::board_width + 1; ++x)
                    {
                        Draw_Block(&s_canvas, v2s{x, 0}, v2f{0, 0}, Block::_);
                        Draw_Block(&s_canvas, v2s{x, Tetris::board_height + 1}, v2f{0, 0}, Block::_);
                    }
                }
                
                u8 num[12];
                char* highscore_string = U32_To_Char_Buffer(num, s_tetris.highscore);
                
                v2f p = Draw_Text_Centered(Get_Middle(&s_canvas), "Tetris in a day", v2f{3.f, 3.f});
                p = Draw_Text_Centered(p, "Highscore:");
                p = Draw_Text_Centered(p, highscore_string);
                p = Draw_Text_Centered(p, "Press [R] to play");
                
                if(s_actions[Inputs::restart].Is_Pressed())
                {
                    Reset_Game();
                }
                
            }break;
            
            case Game_State::quit:
            {
                Render_Game();
                Dim_Entire_Screen(&s_canvas, 0.1f);
                
                v2f p = Draw_Text_Centered(Get_Middle(&s_canvas), "Quit Game?", v2f{3.f, 3.f});
                p = Draw_Text_Centered(p, "Yes [Enter]");
                p = Draw_Text_Centered(p, "No [ESC]");
                
                if(s_actions[Inputs::quit].Is_Pressed())
                {
                    keep_running = false;
                }
                
                if(s_actions[Inputs::back].Is_Pressed())
                {
                    s_game_state = s_stored_game_state;
                }
            }break;
        }
        
        if(s_game_state == Game_State::game_over && pre_state != Game_State::game_over)
        {
            if(s_tetris.score > s_tetris.highscore)
            {
                s_tetris.is_highscore = true;
                s_tetris.highscore = s_tetris.score;
                Platform_Write_File("TIAD_Score", (char*)&s_tetris.highscore, sizeof(s_tetris.highscore));
            }
        }
    }
    
    return keep_running;
}


static void Tetris()
{
    s32 block = s32(s_block_side_lenght);
    Platform_Init("Tetris in a day", {}, {block * 12, block * 28});
    Init_Tetris();
    
    while(Update_App()) Platform_Update();
}