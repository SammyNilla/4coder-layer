
function Rect_f32_Pair
layout_colby_line_number_margin(Application_Links * app,
                                Buffer_ID buffer,
                                Rect_f32 rect,
                                f32 digit_advance)
{
    i64 line_count = buffer_get_line_count(app, buffer);
    i64 line_count_digit_count = digit_count_from_integer(line_count, 10) + 2;
    return(layout_line_number_margin(rect, digit_advance, line_count_digit_count));
}

function void
draw_colby_line_number_margin(Application_Links * app,
                              View_ID view_id, Buffer_ID buffer,
                              Face_ID face_id, Text_Layout_ID text_layout_id,
                              Rect_f32 margin)
{
    Rect_f32 prev_clip = draw_set_clip(app, margin);
    draw_rectangle_fcolor(app, margin, 0.f, fcolor_id(defcolor_line_numbers_back));
    
    Range_i64 visible_range = text_layout_get_visible_range(app, text_layout_id);
    
    FColor line_color = fcolor_id(defcolor_line_numbers_text);
    FColor cursor_line_color =
        fcolor_argb(finalize_color(defcolor_line_numbers_text, 1));
    
    i64 line_count = buffer_get_line_count(app, buffer);
    i64 line_count_digit_count = digit_count_from_integer(line_count, 10);
    
    Scratch_Block scratch(app, Scratch_Share);
    
    Face_Metrics face_metrics = get_face_metrics(app, face_id);
    f32 digit_advance = face_metrics.decimal_digit_advance;
    
    i64 cursor_position = view_get_cursor_pos(app, view_id);
    Buffer_Cursor cursor =
        view_compute_cursor(app, view_id, seek_pos(cursor_position));
    
    Buffer_Cursor visible_first =
        view_compute_cursor(app, view_id, seek_pos(visible_range.first));
    i64 line_number = visible_first.line;
    for (;visible_first.pos <= visible_range.one_past_last;)
    {
        if (line_number > line_count)
            break;
        
        b32 is_on_cursor_line = (cursor.line == line_number);
        
        FColor draw_color =
            (is_on_cursor_line && global_config.highlight_line_at_cursor) ?
            cursor_line_color : line_color;
        
        Temp_Memory_Block temp(scratch);
        Fancy_String *string = push_fancy_stringf(scratch, 0, draw_color,
                                                  "%*lld",
                                                  line_count_digit_count,
                                                  line_number);
        
        Range_f32 line_y =
            text_layout_line_on_screen(app, text_layout_id, line_number);
        Vec2_f32 p = V2f32(margin.x0 + digit_advance, line_y.min);
        // Comment this conditional if you don't want line_number pushed to the side.
        if (is_on_cursor_line && global_config.highlight_line_at_cursor)
            p.x -= digit_advance;
        
        draw_fancy_string(app, face_id, fcolor_zero(), string, p);
        line_number += 1;
    }
    
    draw_set_clip(app, prev_clip);
}