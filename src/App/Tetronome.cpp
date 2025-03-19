
#pragma once


static Tetronome Create_Tetronome(Block type, v2s position)
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
    
    v2s o = v2s{1,1};
    switch(type)
    {
        case Block::I:
        {
            o = v2s{0, 0};
        }break;
        
        case Block::O:
        {
            o = v2s{-1, -1};
        }break;
    }
    
    result.origin = o;
    return result;
}



static inline Block_Grid_4x4 Flip_Block_Grid_Y(Block_Grid_4x4 grid)
{
    Block_Grid_4x4 result = {};
    
    for(s32 i = 0; i < 2; ++i)
    {
        for(s32 x = 0; x < 4; ++x)
        {
            s32 a = 4 * (3 - i) + x;
            s32 b = 4 * i + x;
            
            result[a] = grid[b];
            result[b] = grid[a];
        }        
    }
    return result;
}


static Block_Grid_4x4 Counter_CW_Rotate(Block_Grid_4x4 grid, v2s origin)
{
    Block_Grid_4x4 result = {};
    
    if(origin.x >= 0 && origin.y >= 0)
    {
        for(s32 y = 0; y < 4; ++y)
        {
            for(s32 x = 0; x < 4; ++x)
            {
                Block b = grid[y * 4 + x];
                if(u32(b) > 0)
                {                
                    v2s p0 = v2s{x, y};
                    v2s p1 = CCW_Perp(p0 - origin) + origin;
                    
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


static inline Block_Grid_4x4 Clockwise_Rotate(Block_Grid_4x4 grid, v2s origin)
{
    Block_Grid_4x4 result = {};
    
    if(origin.x >= 0 && origin.y >= 0)
    {
        for(s32 y = 0; y < 4; ++y)
        {
            for(s32 x = 0; x < 4; ++x)
            {
                Block b = grid[y * 4 + x];
                if(u32(b) > 0)
                {                
                    v2s p0 = v2s{x, y};
                    v2s p1 = CW_Perp(p0 - origin) + origin;
                    
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


static s32_Rect Get_Bounds(Block_Grid_4x4 grid)
{
    s32_Rect result = {{100, 100}, {-100, -100}};
    
    for(s32 y = 0; y < 4; ++y)
    {
        for(s32 x = 0; x < 4; ++x)
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
    s32_Rect bounds = Get_Bounds(tetro->grid);
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
    for(s32 y = 0; y < 4; ++y)
    {
        for(s32 x = 0; x < 4; ++x)
        {
            Block b = tetro.grid[y * 4 + x];
            if(u8(b))
            {
                v2s offset_p = tetro.pos + v2s{x, y};
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
    for(s32 y = tetro.pos.y; y < tetro.pos.y + 4; ++y)
    {
        if(y < Tetris::board_height && y >= 0)
        {
            bool full_row = true;
            
            for(s32 x = 0; x < Tetris::board_width; ++x)
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
                for(s32 x = 0; x < Tetris::board_width; ++x)
                {
                    (*board)[y * Tetris::board_width + x] = Block::none;
                }
                
                // Move down all rows.
                for(s32 j = y + 1; j < Tetris::board_height - 1; ++j)
                {
                    for(s32 x = 0; x < Tetris::board_width; ++x)
                    {
                        Block b = (*board)[j * Tetris::board_width + x];
                        
                        (*board)[(j - 1) * Tetris::board_width + x] = b;
                    }
                }
                
                // Clear new row.
                for(s32 x = 0; x < Tetris::board_width; ++x)
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
    for(s32 y = 0; y < 4; ++y)
    {
        for(s32 x = 0; x < 4; ++x)
        {
            Block b0 = tetro.grid[y * 4 + x];
            Block b1 = Block(0); 
            
            v2s offset_p = tetro.pos + v2s{x, y};
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

