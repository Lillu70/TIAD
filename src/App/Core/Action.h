

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

enum class Input_Mode : u8
{
    keyboard,
    controller
};


struct Action_Context
{
    Input_Mode last_used_mode = Input_Mode::keyboard;
    bool disable_for_one_frame = false;
};


struct Action
{
    Key_Code keyboard_mapping = Key_Code::NONE;
    Button controller_mapping = Button::NONE;
    
    Button_State state = { 0, 0 };
    bool disabled = false;
    bool in_unknown_state = false;

    
    inline bool Is_Pressed()
    {
        bool result = !disabled && state.Is_Pressed();
        return result;
    }
    
    inline bool Is_Released()
    {
        bool result = !disabled && state.Is_Released();
        return result;
    }
    
    inline bool Is_Down()
    {
        bool result = !disabled && state.Is_Down();
        return result;
    }
    
    inline bool Is_Up()
    {
        bool result = !disabled && state.Is_Up();
        return result;
    }
};


static Action Make_Action(Key_Code keyboard_mapping, Button controller_mapping)
{
    Action result = {keyboard_mapping, controller_mapping};
    return result;
}


static void Update_Actions(
    Platform_Calltable* platform, 
    Action* actions, 
    u32 count, 
    Action_Context* context)
{
    Assert(platform);
    Assert(actions);
    
    Controller_State controller = platform->Get_Controller_State(0);
    
    bool is_focused = Is_Flag_Set(platform->Get_Flags(), (u32)App_Flags::is_focused);
    
    if(is_focused && !context->disable_for_one_frame)
    {
        for(u32 i = 0; i < count; ++i)
        {
            Action* action = actions + i;
            
            bool s1 = (action->keyboard_mapping == Key_Code::NONE)? false : 
                platform->Get_Keyboard_Key_Down(action->keyboard_mapping);
            
            bool s2 = actions->controller_mapping == Button::NONE? false : 
                controller.Get_Button_State(action->controller_mapping).current;
            
            Button_State bs = Button_State{s1 || s2, action->state.current};
            if(action->in_unknown_state)
            {
                if(!bs.current && !bs.previous)
                {
                    action->in_unknown_state = false;
                    action->state = bs;
                }
                else
                {
                    action->state = Button_State{false, false};
                }
            }
            else
                action->state = bs; 
        }    
    }
    else
    {
        for(u32 i = 0; i < count; ++i)
        {
            Action* action = actions + i;
            action->state = Button_State{false, false}; 
            action->in_unknown_state = true;
        }
    }
    
    context->disable_for_one_frame = false;
}