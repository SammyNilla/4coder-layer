
internal f32
DrawFileBarTriangles(Application_Links * App, Rect_f32 Bar,
                     FColor FillColor, f32 StartPoint, i32 Iter)
{
    Rect_f32 LineRect = {};
    local_const i32 LineCount = 15;
    i32 Index = 0;
    do
    {
        LineRect.x0 = (StartPoint + Index);
        LineRect.x1 = LineRect.x0 + Iter;
        LineRect.y0 = Bar.y0 + (Index * Iter);
        LineRect.y1 = (Bar.y1 - (Index * Iter)) - 3;
        
        draw_rectangle_fcolor(App, LineRect, 0, FillColor);
        Index += Iter;
    } while (((LineCount * Iter) + (-Index)));
    
    return Iter ? LineRect.x1 : LineRect.x0;
}

internal f32
DrawFileBarMode(Application_Links * App, Face_ID FaceID,
                Rect_f32 Bar, String_Const_u8 BufferName,
                b32 IsBuffer, f32 DigitAdvance)
{
    Scratch_Block Scratch(App);
    FColor DarkTextColor = fcolor_argb(finalize_color(defcolor_base, 1));
    FColor PaddingColor = fcolor_argb(finalize_color(defcolor_bar, 1));
    FColor BackgroundColor = fcolor_argb(finalize_color(defcolor_bar, 2));
    
    Fancy_Line Line = {};
    String_Const_u8 Mode = {};
    if (!IsBuffer)
    {
        Mode = string_u8_litexpr("NORMAL");
        if (global_keyboard_macro_is_recording)
        {
            View_ID ActiveViewID = get_active_view(App, Access_Always);
            Buffer_ID ActiveBufferID = view_get_buffer(App, ActiveViewID, Access_Always);
            
            String_Const_u8 ActiveBufferName = push_buffer_unique_name(App, Scratch, ActiveBufferID);
            
            b32 IsSameBuffer = string_match(BufferName, ActiveBufferName);
            if (IsSameBuffer)
            {
                BackgroundColor = fcolor_argb(finalize_color(defcolor_bar, 3));
                Mode = string_u8_litexpr("RECORD");
            }
        }
    }
    else
    {
        BackgroundColor = fcolor_argb(finalize_color(defcolor_bar, 5));
        Mode = string_u8_litexpr("BUFFER");
    }
    
    push_fancy_string(Scratch, &Line, DarkTextColor, Mode);
    
    f32 LineWidth = get_fancy_line_width(App, FaceID, &Line);
    Vec2_f32 Padding = V2f32(DigitAdvance, 2.f);
    
    Rect_f32 BarPartition = Bar;
    Vec2_f32 Point = BarPartition.p0 + Padding;
    BarPartition.x1 = (Point.x + LineWidth + Padding.x);
    
    draw_rectangle_fcolor(App, BarPartition, 0, BackgroundColor);
    
    BarPartition.x0 = BarPartition.x1;
    BarPartition.x1 += Padding.x;
    draw_rectangle_fcolor(App, BarPartition, 0, PaddingColor);
    DrawFileBarTriangles(App, Bar, BackgroundColor, BarPartition.x0, 1);
    
    draw_fancy_line(App, FaceID, fcolor_zero(), &Line, Point);
    
    return BarPartition.x1;
}

internal f32
DrawFileBarBufferName(Application_Links * App,
                      Buffer_ID BufferID, Face_ID FaceID,
                      Rect_f32 Bar, f32 StartPoint, f32 DigitAdvance)
{
    Scratch_Block Scratch(App);
    FColor LightTextColor = fcolor_id(defcolor_base);
    FColor PaddingColor = fcolor_argb(finalize_color(defcolor_bar, 1));
    FColor BackgroundColor = fcolor_id(defcolor_bar);
    
#if 0
    FColor DebugColor = fcolor_id(defcolor_highlight_white);
#endif
    
    Fancy_Line Line = {};
    String_Const_u8 UniqueName = push_buffer_unique_name(App, Scratch, BufferID);
    push_fancy_string(Scratch, &Line, LightTextColor, UniqueName);
    
    { // NOTE(sammynilla): Dirty State Handler
        // TODO(sammynilla): Let's think about adding a dirty section to info instead.
        // For instance, a tag that says DIRTY* or DIRTY!
        Dirty_State DirtyState = buffer_get_dirty_state(App, BufferID);
        u8 Buf[3];
        String_u8 DirtyStr = Su8(Buf, 0, 3);
        
        if (DirtyState != 0)
            string_append(&DirtyStr, string_u8_litexpr(" "));
        if (HasFlag(DirtyState, DirtyState_UnsavedChanges))
            string_append(&DirtyStr, string_u8_litexpr("*"));
        if (HasFlag(DirtyState, DirtyState_UnloadedChanges))
            string_append(&DirtyStr, string_u8_litexpr("!"));
        
        push_fancy_string(Scratch, &Line, fcolor_id(defcolor_pop2), DirtyStr.string);
    }
    
    f32 LineWidth = get_fancy_line_width(App, FaceID, &Line);
    Vec2_f32 Padding = V2f32((DigitAdvance * .6f), 2.f);
    
    Rect_f32 BarPartition = Bar;
    BarPartition.x0 = StartPoint;
    
    draw_rectangle_fcolor(App, BarPartition, 0, BackgroundColor);
    BarPartition.x1 = BarPartition.x0 =
        DrawFileBarTriangles(App, Bar, PaddingColor, StartPoint, 1);
    
    Vec2_f32 Point = BarPartition.p0 + Padding;
    BarPartition.x1 += LineWidth + (Padding.x * 2);
    
#if 0
    draw_rectangle_fcolor(App, BarPartition, 0, DebugColor);
#endif
    
    draw_fancy_line(App, FaceID, fcolor_zero(), &Line, Point);
    return BarPartition.x1;
}

internal void
DrawFileBarBufferInfo(Application_Links * App,
                      View_ID ViewID, Buffer_ID BufferID, Face_ID FaceID,
                      Rect_f32 Bar, f32 DigitAdvance,
                      f32 RenderBreakpoint, b32 IsBuffer)
{
    Scratch_Block Scratch(App);
    
    FColor LightBackgroundColor = fcolor_argb(finalize_color(defcolor_bar, 2));
    FColor PaddingColor = fcolor_argb(finalize_color(defcolor_bar, 1));
    FColor DarkTextColor = fcolor_argb(finalize_color(defcolor_base, 1));
    
    if (!IsBuffer)
    {
        Fancy_Line Line = {};
        
        i64 CursorPosition = view_get_cursor_pos(App, ViewID);
        Buffer_Cursor Cursor =
            view_compute_cursor(App, ViewID, seek_pos(CursorPosition));
        
#if 0
        Managed_Scope Scope = buffer_get_managed_scope(App, BufferID);
        Line_Ending_Kind *EOLType =
            scope_attachment(App, Scope, buffer_eol_setting, Line_Ending_Kind);
        switch (*EOLType)
        {
            case LineEndingKind_Binary:
            {
                push_fancy_string(Scratch, &Line, BaseDarkColor,
                                  string_u8_litexpr("[bin]"));
            }break;
            case LineEndingKind_LF:
            {
                push_fancy_string(Scratch, &Line, BaseDarkColor,
                                  string_u8_litexpr("[unix]"));
            }break;
            case LineEndingKind_CRLF:
            {
                push_fancy_string(Scratch, &Line, BaseDarkColor,
                                  string_u8_litexpr("[dos]"));
            }break;
        }
#endif
        
        i64 LineCount = buffer_get_line_count(App, BufferID);
        i32 LinePercent = (i32)(((f32)Cursor.line / (f32)LineCount) * 100.f);
        
        push_fancy_stringf(Scratch, &Line, DarkTextColor, "%i%% » %lld: %lld",
                           LinePercent,
                           Cursor.line, Cursor.col);
        
        f32 LineWidth = get_fancy_line_width(App, FaceID, &Line);
        Vec2_f32 Padding = V2f32((DigitAdvance * 2.f), 2.f);
        
        Rect_f32 BarPartition = Bar;
        BarPartition.x0 = ((BarPartition.x1 - LineWidth) - (Padding.x * 2));
        Vec2_f32 Point = BarPartition.p0 + Padding;
        
        if (RenderBreakpoint < (BarPartition.x0 - Padding.x))
        {
            draw_rectangle_fcolor(App, BarPartition, 0, LightBackgroundColor);
            BarPartition.x1 = BarPartition.x0;
            BarPartition.x0 -= DigitAdvance;
            draw_rectangle_fcolor(App, BarPartition, 0, PaddingColor);
            DrawFileBarTriangles(App, Bar, PaddingColor, BarPartition.x0, -1);
            DrawFileBarTriangles(App, Bar, LightBackgroundColor, BarPartition.x1, -1);
            draw_fancy_line(App, FaceID, fcolor_zero(), &Line, Point);
        }
    }
}

function void
sn_DrawFileBar(Application_Links * App,
               View_ID ViewID, Buffer_ID BufferID, Face_ID FaceID,
               Rect_f32 Bar)
{
    ProfileScope(App, "[sammynilla] File Bar Renderer");
    Scratch_Block Scratch(App);
    
    FColor BaseMainColor = fcolor_id(defcolor_base);
    FColor BaseDarkColor = fcolor_argb(finalize_color(defcolor_base, 1));
    
    String_Const_u8 BufferName = push_buffer_unique_name(App, Scratch, BufferID);
    // NOTE(sammynilla): Check if the scratch is non file buffer
    b32 IsBuffer = false;
    if (!string_match(BufferName, string_u8_litexpr("*scratch*")))
    {
        if (string_match_insensitive(string_prefix(BufferName, 1),
                                     string_u8_litexpr("*")))
        {
            IsBuffer = true;
        }
    }
    
    Face_Metrics FaceMetrics = get_face_metrics(App, FaceID);
    f32 DigitAdvance = FaceMetrics.decimal_digit_advance;
    
    f32 NextRenderStartPoint = 0;
    NextRenderStartPoint =
        DrawFileBarMode(App, FaceID,
                        Bar, BufferName, IsBuffer, DigitAdvance);
    
    f32 RenderBreakpoint =
        DrawFileBarBufferName(App, BufferID, FaceID,
                              Bar, NextRenderStartPoint, DigitAdvance);
    
    DrawFileBarBufferInfo(App, ViewID, BufferID, FaceID,
                          Bar, DigitAdvance, RenderBreakpoint, IsBuffer);
    
}