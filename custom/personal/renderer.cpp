
//|
// https://github.com/Colbeyyy/4coder_colby
#include "ext/colby/background.cpp"
#include "ext/colby/line_numbers.cpp"
//|
// https://github.com/ryanfleury/4coder_fleury
#include "ext/fleury/annotations.cpp"
#include "ext/fleury/brace_lines.cpp"
#include "ext/fleury/divider_comments.cpp"
//|
// https://github.com/thevaber/4coder_stuff
#include "ext/thevaber/hex_colors.cpp"
#include "ext/thevaber/if0.cpp"
//|
// https://github.com/Skytrias/4files
#include "ext/skytrias/render_highlight.cpp"
//|
// NOTE(sammynilla): Personal files
#include "filebar.cpp"
#include "token_highlight.cpp"

function void
sn_DrawQueryBar(Application_Links * App, Query_Bar * QueryBar,
                Face_ID FaceID, Rect_f32 Bar)
{
    // NOTE(sammynilla): April 6, 2020
    // Nothing has changed here yet. Planning for the future.
    Scratch_Block Scratch(App);
    Fancy_Line List = {};
    push_fancy_string(Scratch, &List, fcolor_id(defcolor_pop1),
                      QueryBar->prompt);
    push_fancy_string(Scratch, &List, fcolor_id(defcolor_text_default),
                      QueryBar->string);
    Vec2_f32 Padding = V2f32(2.f, 2.f);
    Vec2_f32 Point = Bar.p0 + Padding;
    draw_fancy_line(App, FaceID, fcolor_zero(), &List, Point);
}

internal Rect_f32
sn_BufferRegion(Application_Links * App,
                View_ID ViewID,
                Rect_f32 Region)
{
    Buffer_ID Buffer = view_get_buffer(App, ViewID, Access_Always);
    Face_ID FaceID = get_face_id(App, Buffer);
    Face_Metrics Metrics = get_face_metrics(App, FaceID);
    f32 LineHeight = Metrics.line_height;
    f32 DigitAdvance = Metrics.decimal_digit_advance;
    //|
    // NOTE(allen): File bar
    b64 ShowFileBar =
        view_get_setting(App, ViewID, ViewSetting_ShowFileBar, &ShowFileBar);
    if (ShowFileBar)
    {
        Rect_f32_Pair Pair = layout_file_bar_on_bot(Region, LineHeight);
        Region = Pair.min;
    }
    //|
    // NOTE(allen): Query bars
    {
        Query_Bar * Buf[32];
        Query_Bar_Ptr_Array QueryBars = {};
        QueryBars.ptrs = Buf;
        if (get_active_query_bars(App, ViewID, ArrayCount(Buf), &QueryBars))
        {
            Rect_f32_Pair Pair = layout_query_bar_on_top(Region, LineHeight, QueryBars.count);
            Region = Pair.max;
        }
    }
    //|
    // NOTE(allen): FPS HUD
#if 0
    if (show_fps_hud)
    {
        Rect_f32_Pair pair = layout_fps_hud_on_bottom(region, line_height);
        region = pair.min;
    }
#endif
    //|
    // NOTE(allen): Line numbers
    if (global_config.show_line_number_margins)
    {
        Rect_f32_Pair Pair =
            layout_colby_line_number_margin(App, Buffer, Region, DigitAdvance);
        Region = Pair.max;
    }
    
    return Region;
}


internal void
sn_RenderBuffer(Application_Links * App,
                View_ID ViewID, Face_ID FaceID, Buffer_ID BufferID,
                Text_Layout_ID TextLayoutID,
                Rect_f32 Rect, Frame_Info FrameInfo)
{
    ProfileScope(App, "[sammynilla] render buffer");
    
    View_ID ActiveView = get_active_view(App, Access_Always);
    b32 IsActiveView = (ActiveView == ViewID);
    Rect_f32 PrevClip = draw_set_clip(App, Rect);
    
    Scratch_Block Scratch(App);
    String_Const_u8 BufferName = push_buffer_base_name(App, Scratch, BufferID);
    
    // NOTE(allen): Token colorizing
    Token_Array TokenArray = get_token_array_from_buffer(App, BufferID);
    if (TokenArray.tokens != 0)
    {
        // NOTE(sammynilla): Draw tokens based on file type
        if (string_match_insensitive(string_postfix(BufferName, 4), string_u8_litexpr(".cpp")) ||
            string_match_insensitive(string_postfix(BufferName, 4), string_u8_litexpr(".hpp")) ||
            string_match_insensitive(string_postfix(BufferName, 3), string_u8_litexpr(".cc"))  ||
            string_match_insensitive(string_postfix(BufferName, 2), string_u8_litexpr(".h")))
        {
            sn_HighlightTokens(App, TextLayoutID, &TokenArray, sn_GetTokenColorCPP);
            {
                st_paint_functions(App, BufferID, TextLayoutID,
                                   finalize_color(defcolor_highlight, 1));
                tv_draw_if0_fade(App, BufferID, ViewID, TextLayoutID, &TokenArray);
                Fleury4RenderDividerComments(App, BufferID, ViewID, TextLayoutID);
            }
        }
        else if (string_match_insensitive(string_postfix(BufferName, 2), string_u8_litexpr(".c")))
        {
            sn_HighlightTokens(App, TextLayoutID, &TokenArray, sn_GetTokenColorC);
            {
                st_paint_functions(App, BufferID, TextLayoutID,
                                   finalize_color(defcolor_highlight, 1));
                tv_draw_if0_fade(App, BufferID, ViewID, TextLayoutID, &TokenArray);
                Fleury4RenderDividerComments(App, BufferID, ViewID, TextLayoutID);
            }
        }
        else if (string_match_insensitive(string_postfix(BufferName, 4), string_u8_litexpr(".bat")))
        {
            sn_HighlightTokens(App, TextLayoutID, &TokenArray, sn_GetTokenColorBAT);
        }
        else if (string_match_insensitive(string_postfix(BufferName, 5), string_u8_litexpr(".json")))
        {
            sn_HighlightTokens(App, TextLayoutID, &TokenArray, sn_GetTokenColorJSON);
            sn_PaintKeysJSON(App, BufferID, TextLayoutID);
        }
        
        // NOTE(sammynilla): Scan for TODOs/NOTEs/HACKs
        if (global_config.use_comment_keyword)
        {
            Comment_Highlight_Pair Pairs[] = {
                {string_u8_litexpr("NOTE"), finalize_color(defcolor_comment_pop, 0)},
                {string_u8_litexpr("TODO"), finalize_color(defcolor_comment_pop, 1)},
                {string_u8_litexpr("HACK"), finalize_color(defcolor_comment_pop, 2)},
            };
            draw_comment_highlights(App, BufferID, TextLayoutID,
                                    &TokenArray, Pairs, ArrayCount(Pairs));
        }
    }
    else
    {
        Range_i64 VisibleRange = text_layout_get_visible_range(App, TextLayoutID);
        paint_text_color_fcolor(App, TextLayoutID, VisibleRange,
                                fcolor_id(defcolor_text_default));
        
        
        if (string_match_insensitive(string_prefix(BufferName, 6), string_u8_litexpr("theme-")) &&
            string_match_insensitive(string_postfix(BufferName, 7), string_u8_litexpr(".4coder")))
        {
            tv_colorize_hex_colors(App, BufferID, TextLayoutID);
        }
    }
    
    i64 CursorPos = view_correct_cursor(App, ViewID);
    view_correct_mark(App, ViewID);
    
#if 0
    // NOTE(allen): Scope highlight
    if (global_config.use_scope_highlight)
    {
        Color_Array Colors = finalize_color_array(defcolor_back_cycle);
        draw_scope_highlight(app, buffer, text_layout_id, cursor_pos, colors.vals, colors.count);
    }
#endif
    
    if (global_config.use_error_highlight || global_config.use_jump_highlight)
    {
        // NOTE(allen): Error highlighting
        BufferName = string_u8_litexpr("*compilation*");
        Buffer_ID CompilationBuffer = get_buffer_by_name(App, BufferName, Access_Always);
        if (global_config.use_error_highlight)
        {
            draw_jump_highlights(App, BufferID, TextLayoutID, CompilationBuffer,
                                 fcolor_id(defcolor_highlight_junk));
        }
        // NOTE(allen): Jump highlighting
        if (global_config.use_jump_highlight)
        {
            Buffer_ID JumpBuffer = get_locked_jump_buffer(App);
            if (JumpBuffer != CompilationBuffer)
            {
                draw_jump_highlights(App, BufferID, TextLayoutID, JumpBuffer,
                                     fcolor_id(defcolor_highlight_white));
            }
        }
    }
    
#if 0
    // NOTE(allen): Color parens
    if (global_config.use_paren_helper)
    {
        Color_Array colors = finalize_color_array(defcolor_text_cycle);
        draw_paren_highlight(app, buffer, text_layout_id, cursor_pos, colors.vals, colors.count);
    }
#endif
    
    // NOTE(allen): Line highlight
    if (global_config.highlight_line_at_cursor && IsActiveView)
    {
        i64 LineNumber = get_line_number_from_pos(App, BufferID, CursorPos);
        draw_line_highlight(App, TextLayoutID, LineNumber,
                            fcolor_id(defcolor_highlight_cursor_line));
    }
    
    // NOTE(allen): Cursor shape
    Face_Metrics Metrics = get_face_metrics(App, FaceID);
    f32 CursorRoundness = 1.f;
    f32 MarkThickness = 2.f;
    
    // NOTE(allen): Cursor
    switch (fcoder_mode)
    {
        case FCoderMode_Original:
        {
            draw_original_4coder_style_cursor_mark_highlight(App, ViewID, IsActiveView, BufferID, TextLayoutID, CursorRoundness, MarkThickness);
        }break;
        case FCoderMode_NotepadLike:
        {
            draw_notepad_style_cursor_highlight(App, ViewID, BufferID, TextLayoutID, CursorRoundness);
        }break;
    }
    
    // NOTE(allen): put the actual text on the actual screen
    draw_text_layout_default(App, TextLayoutID);
    
    // TODO(sammynilla): Figure out where we want to place these two functions from Ryan.
    { // NOTE(rjf): Brace annotations
        // TODO(sammynilla): Fix the couple of issues with this that I have
        //Fleury4RenderCloseBraceAnnotation(app, buffer, text_layout_id, cursor_pos);
    }
    
    { // NOTE(rjf): Brace lines
        Fleury4RenderBraceLines(App, BufferID, ViewID, TextLayoutID, CursorPos);
    }
    
    draw_set_clip(App, PrevClip);
}

internal void
sn_RenderCaller(Application_Links * App,
                Frame_Info FrameInfo,
                View_ID ViewID)
{
    ProfileScope(App, "[sammynilla] render caller");
    
    View_ID ActiveView = get_active_view(App, Access_Always);
    b32 IsActiveView = (ActiveView == ViewID);
    
    Rect_f32 Region = draw_colby_background(App, ViewID, IsActiveView);
    Rect_f32 PrevClip = draw_set_clip(App, Region);
    
    Buffer_ID BufferID = view_get_buffer(App, ViewID, Access_Always);
    Face_ID FaceID = get_face_id(App, BufferID);
    Face_Metrics FaceMetrics = get_face_metrics(App, FaceID);
    f32 LineHeight = FaceMetrics.line_height;
    f32 DigitAdvance = FaceMetrics.decimal_digit_advance;
    //|
    // NOTE(allen): File bar
    b64 ShowFileBar =
        view_get_setting(App, ViewID, ViewSetting_ShowFileBar, &ShowFileBar);
    if (ShowFileBar)
    {
        Rect_f32_Pair Pair = layout_file_bar_on_bot(Region, LineHeight);
        sn_DrawFileBar(App, ViewID, BufferID, FaceID, Pair.max);
        Region = Pair.min;
    }
    //|
    // NOTE(Skytrias): faster scrolling like alpha
    Buffer_Scroll Scroll = view_get_buffer_scroll(App, ViewID);
    Buffer_Point_Delta_Result Delta =
        delta_apply(App, ViewID,
                    FrameInfo.animation_dt * 1.125f, Scroll);
    
    if (!block_match_struct(&Scroll.position, &Delta.point))
    {
        block_copy_struct(&Scroll.position, &Delta.point);
        view_set_buffer_scroll(App, ViewID, Scroll, SetBufferScroll_NoCursorChange);
    }
    
    if (Delta.still_animating)
        animate_in_n_milliseconds(App, 0);
    //|
    // NOTE(allen): Query bars
    {
        Query_Bar * Buf[32];
        Query_Bar_Ptr_Array QueryBars = {};
        QueryBars.ptrs = Buf;
        if (get_active_query_bars(App, ViewID, ArrayCount(Buf), &QueryBars))
        {
            for (i32 i = 0; i < QueryBars.count; i += 1)
            {
                Rect_f32_Pair Pair = layout_query_bar_on_top(Region, LineHeight, 1);
                sn_DrawQueryBar(App, QueryBars.ptrs[i], FaceID, Pair.min);
                Region = Pair.max;
            }
        }
    }
    //|
    // NOTE(allen): FPS HUD
#if 0
    if (show_fps_hud)
    {
        Rect_f32_Pair pair = layout_fps_hud_on_bottom(region, line_height);
        draw_fps_hud(app, frame_info, face_id, pair.max);
        region = pair.min;
        animate_in_n_milliseconds(app, 1000);
    }
#endif
    //|
    // NOTE(sammynilla): Layout
    Text_Layout_ID TextLayoutID = {};
    Rect_f32 LineNumberRect = {};
    {
        // NOTE(allen): Line numbers
        {
            if (global_config.show_line_number_margins)
            {
                Rect_f32_Pair Pair =
                    layout_colby_line_number_margin(App, BufferID, Region, DigitAdvance);
                LineNumberRect = Pair.min;
                Region = Pair.max;
            }
        }
        // NOTE(allen): Buffer
        {
            Buffer_Point BufferPoint = Scroll.position;
            TextLayoutID =
                text_layout_create(App, BufferID, Region, BufferPoint);
        }
    }
    //|
    // NOTE(sammynilla): Rendering
    {
        // NOTE(allen): Line Numbers
        {
            if (global_config.show_line_number_margins)
                draw_colby_line_number_margin(App,
                                              ViewID, BufferID, FaceID, TextLayoutID,
                                              LineNumberRect);
        }
        // NOTE(allen): Buffer
        {
            sn_RenderBuffer(App,
                            ViewID, FaceID, BufferID, TextLayoutID,
                            Region, FrameInfo);
        }
    }
    //|
    text_layout_free(App, TextLayoutID);
    draw_set_clip(App, PrevClip);
}