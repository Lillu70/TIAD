
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
    
    inline Block& operator[] (s32 idx);
};


Block& Block_Grid_4x4::operator[](s32 idx)
{
    return blocks[idx];
}


struct Block_Grid_10x20
{
    Block blocks[10 * 20];
    
    inline Block& operator[] (s32 idx);
};


Block& Block_Grid_10x20::operator[](s32 idx)
{
    Assert(idx < (10 * 20));
    Assert(idx >= 0);
    return blocks[idx];
}


struct Tetronome
{
    v2s pos;
    v2s origin;
    Block_Grid_4x4 grid;
};


struct Tetris
{
    static inline s32 board_width = 10;
    static inline s32 board_height = 20;
    
    Block_Grid_10x20 board = {};
    Tetronome active_tetro;

    u32 score = 0;
    u32 highscore = 0;
    
    f64 accumilated_time;
    f64 step_frequency;
    
    bool is_highscore;
    
    Random_Machine random_machine = {};
};


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



struct s32_Rect
{
    v2s min;
    v2s max;
};
