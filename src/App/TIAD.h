
// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

static f32 s_block_side_lenght = 32;


enum class Game_State
{
    running = 0,
    paused,
    game_over,
    quit,
    menu
};
static Game_State s_game_state = {};
static Game_State s_stored_game_state = {};

enum class Block : u8
{
    none = 0,
    I,
    J,
    L,
    O,
    S,
    T,
    Z,
    _,
    COUNT
};


struct Block_Grid_4x4
{
    Block blocks[4 * 4];
    
    inline Block& operator[] (i32 idx);
};


Block& Block_Grid_4x4::operator[](i32 idx)
{
    return blocks[idx];
}


struct Block_Grid_10x20
{
    Block blocks[10 * 20];
    
    inline Block& operator[] (i32 idx);
};


Block& Block_Grid_10x20::operator[](i32 idx)
{
    Assert(idx < (10 * 20));
    Assert(idx >= 0);
    return blocks[idx];
}


struct Tetronome
{
    v2i pos;
    v2i origin;
    Block_Grid_4x4 grid;
};


struct Tetris
{
    static inline i32 board_width = 10;
    static inline i32 board_height = 20;
    
    Block_Grid_10x20 board = {};
    Tetronome active_tetro;

    u32 score = 0;
    u32 highscore = 0;
    
    f64 accumilated_time;
    f64 step_frequency;
    
    bool is_highscore;
    

    
    Random_Machine random_machine = {};
};


Color s_block_color_table[u32(Block::COUNT)] = 
{
    Make_Color(0, 0, 0),      // black
    Make_Color(0, 240, 240),  // teal
    Make_Color(0, 0, 240),    // blue
    Make_Color(240, 161, 0),  // orange
    Make_Color(240, 240, 0),  // yellow
    Make_Color(0, 240, 0),    // green
    Make_Color(161, 0, 240),  // purple
    Make_Color(240, 0, 0),    // red
    Make_Color(120, 120, 120) // gray
};


static inline void Draw_Block(Canvas* canvas, v2i p, v2f o, Block block)
{
    u8 b = u32(block);
    
    Color color = s_block_color_table[b];
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



#define o Block::none
static Block_Grid_4x4 s_tetrominoes[] = 
{
    #define X Block::I
    {
        X,o,o,o,
        X,o,o,o,
        X,o,o,o,
        X,o,o,o,
    },
    #undef X
    
    #define X Block::J
    {
        o,o,o,o,
        o,X,o,o,
        o,X,o,o,
        X,X,o,o,
    },
    #undef X
    
    #define X Block::L
    {
        o,o,o,o,
        X,o,o,o,
        X,o,o,o,
        X,X,o,o,
    },
    #undef X
    
    #define X Block::O
    {
        o,o,o,o,
        o,o,o,o,
        X,X,o,o,
        X,X,o,o,
    },
    #undef X
    
    #define X Block::S
    {
        o,o,o,o,
        o,o,o,o,
        o,X,X,o,
        X,X,o,o,
    },
    #undef X
    
    #define X Block::T
    {
        o,o,o,o,
        o,o,o,o,
        X,X,X,o,
        o,X,o,o,
    },
    #undef X
    
    #define X Block::Z
    {
        o,o,o,o,
        o,o,o,o,
        X,X,o,o,
        o,X,X,o,
    },
    #undef X
};

#undef o


static Block_Grid_4x4 Flip_Block_Grid_Y(Block_Grid_4x4 grid)
{
    Block_Grid_4x4 result = {};
    
    for(i32 i = 0; i < 2; ++i)
    {
        for(i32 x = 0; x < 4; ++x)
        {
            i32 a = 4 * (3 - i) + x;
            i32 b = 4 * i + x;
            
            result[a] = grid[b];
            result[b] = grid[a];
        }        
    }
    return result;
}


static inline v2i CW_Perp(v2i p)
{
    v2i result;
    result.x = p.y;
    result.y = p.x * -1;
    
    return result;
}


static Block_Grid_4x4 Clockwise_Rotate(Block_Grid_4x4 grid, v2i origin)
{
    Block_Grid_4x4 result = {};
    
    if(origin.x >= 0 && origin.y >= 0)
    {
        for(i32 y = 0; y < 4; ++y)
        {
            for(i32 x = 0; x < 4; ++x)
            {
                Block b = grid[y * 4 + x];
                if(u32(b) > 0)
                {                
                    v2i p0 = v2i{x, y};
                    v2i p1 = CW_Perp(p0 - origin) + origin;
                    
                    p1.x = Abs(p1.x);
                    p1.y = Abs(p1.y);
                    
                    Assert(p1.x >= 0 && p1.y >= 0 && p1.x < 4 && p1.y < 4);
                    
                    
                    result[p1.y * 4 + p1.x] = b;
                }
            }
        }        
    }
    else
    {
        result = grid;
    }
    
    return result;
}


static inline v2i CCW_Perp(v2i p)
{
    v2i result;
    result.x = p.y * -1;
    result.y = p.x;
    
    return result;
}


static Block_Grid_4x4 Counter_CW_Rotate(Block_Grid_4x4 grid, v2i origin)
{
    Block_Grid_4x4 result = {};
    
    if(origin.x >= 0 && origin.y >= 0)
    {
        for(i32 y = 0; y < 4; ++y)
        {
            for(i32 x = 0; x < 4; ++x)
            {
                Block b = grid[y * 4 + x];
                if(u32(b) > 0)
                {                
                    v2i p0 = v2i{x, y};
                    v2i p1 = CCW_Perp(p0 - origin) + origin;
                    
                    p1.x = Abs(p1.x);
                    p1.y = Abs(p1.y);
                    
                    Assert(p1.x >= 0 && p1.y >= 0 && p1.x < 4 && p1.y < 4);
                    
                    
                    result[p1.y * 4 + p1.x] = b;
                }
            }
        }        
    }
    else
    {
        result = grid;
    }
    
    return result;
}


static Tetronome Create_Tetronome(Block type, v2i position)
{
    Tetronome result = {};
    
    result.grid = {};
    u32 itype = u32(type);
    Assert(itype - 1 < Array_Lenght(s_tetrominoes));
    
    if(itype > 0)
    {    
        result.grid = s_tetrominoes[itype - 1];
    }
    
    result.pos = position;
    
    v2i o = v2i{1,1};
    switch(type)
    {
        case Block::I:
        {
            o = v2i{0, 0};
        }break;
        
        case Block::O:
        {
            o = v2i{-1, -1};
        }break;
    }
    
    result.origin = o;
    return result;
}


struct i32_Rect
{
    v2i min;
    v2i max;
};


static i32_Rect Get_Bounds(Block_Grid_4x4 grid)
{
    i32_Rect result = {{100, 100}, {-100, -100}};
    
    for(i32 y = 0; y < 4; ++y)
    {
        for(i32 x = 0; x < 4; ++x)
        {
            if(u8(grid[y * 4 + x]))
            {
                if(result.min.y > y)
                    result.min.y = y;

                if(result.min.x > x)
                    result.min.x = x;
                
                if(result.max.y < y)
                    result.max.y = y;

                if(result.max.x < x)
                    result.max.x = x;
            }
        }
    }
    
    return result;
}


static void Snap_Tetro_To_Board(Tetronome* tetro)
{
    i32_Rect bounds = Get_Bounds(tetro->grid);
    if(tetro->pos.x + bounds.min.x < 0)
    {
        tetro->pos.x +=  0 - (tetro->pos.x + bounds.min.x);
    }
    
    else if(tetro->pos.x + bounds.max.x > Tetris::board_width - 1)
    {
        tetro->pos.x += 
            (Tetris::board_width - 1) - (tetro->pos.x + bounds.max.x);
    }
}


static u32 Apply_Tetro_To_Board(Tetronome tetro, Block_Grid_10x20* board)
{
    for(i32 y = 0; y < 4; ++y)
    {
        for(i32 x = 0; x < 4; ++x)
        {
            Block b = tetro.grid[y * 4 + x];
            if(u8(b))
            {
                v2i offset_p = tetro.pos + v2i{x, y};
                if(offset_p.x >= 0 && 
                    offset_p.y >= 0 && 
                    offset_p.x < Tetris::board_width &&
                    offset_p.y < Tetris::board_height)
                {
                    *(board->blocks + (offset_p.y * Tetris::board_width + offset_p.x)) = b;
                }
                else
                {
                    s_game_state = Game_State::game_over;
                }
            }
        }
    }
    
    u32 rows_cleared = 0;
    u32 score = 0;
    // Check for blocks that need clearing.
    for(i32 y = tetro.pos.y; y < tetro.pos.y + 4; ++y)
    {
        if(y < Tetris::board_height && y >= 0)
        {
            bool full_row = true;
            
            for(i32 x = 0; x < Tetris::board_width; ++x)
            {
                if((*board)[y * Tetris::board_width + x] == Block::none)
                {
                    full_row = false;
                    break;
                }
            }
            
            if(full_row)
            {
                rows_cleared += 1;
                score += 100 * rows_cleared;
                
                // Clear the row.
                for(i32 x = 0; x < Tetris::board_width; ++x)
                {
                    (*board)[y * Tetris::board_width + x] = Block::none;
                }
                
                // Move down all rows.
                for(i32 j = y + 1; j < Tetris::board_height - 1; ++j)
                {
                    for(i32 x = 0; x < Tetris::board_width; ++x)
                    {
                        Block b = (*board)[j * Tetris::board_width + x];
                        
                        (*board)[(j - 1) * Tetris::board_width + x] = b;
                    }
                }
                
                // Clear new row.
                for(i32 x = 0; x < Tetris::board_width; ++x)
                {
                    (*board)[(Tetris::board_height - 1) * Tetris::board_width + x] = Block::none;
                }
                
                // Decrement y to rework this row.
                y -= 1;
            }
        }
    }
    
    return score;
}


static bool Check_Tetro_Against_Board_Collision(Tetronome tetro, Block_Grid_10x20* board)
{
    for(i32 y = 0; y < 4; ++y)
    {
        for(i32 x = 0; x < 4; ++x)
        {
            Block b0 = tetro.grid[y * 4 + x];
            Block b1 = Block(0); 
            
            v2i offset_p = tetro.pos + v2i{x, y};
            if(offset_p.x >= 0 && 
                offset_p.y >= 0 && 
                offset_p.x < Tetris::board_width &&
                offset_p.y < Tetris::board_height)
            {
                b1 = (*board)[offset_p.y * Tetris::board_width + offset_p.x]; 
            }
            
            if(u8(b0) && u8(b1))
            {
                return true;
            }
        }
    }
    
    return false;
}