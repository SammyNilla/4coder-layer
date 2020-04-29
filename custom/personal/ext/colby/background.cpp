
function Rect_f32
draw_colby_background(Application_Links * app,
                      View_ID view_id,
                      b32 is_active_view)
{
    Rect_f32 view_rect = view_get_screen_rect(app, view_id);
    draw_rectangle_fcolor(app, view_rect, 0.f, fcolor_id(defcolor_back));
    return view_rect;
}