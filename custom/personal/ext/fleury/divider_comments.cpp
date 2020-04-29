
internal void
Fleury4RenderDividerComments(Application_Links * App, Buffer_ID BufferID,
                             View_ID ViewID, Text_Layout_ID TextLayoutID)
{
    ProfileScope(App, "[Fleury] Divider Comments");
    
    String_Const_u8 divider_comment_signifier = string_u8_litexpr("//|");
    
    Token_Array token_array = get_token_array_from_buffer(App, BufferID);
    Range_i64 visible_range = text_layout_get_visible_range(App, TextLayoutID);
    
    if(token_array.tokens != 0)
    {
        i64 first_index = token_index_from_pos(&token_array, visible_range.first);
        Token_Iterator_Array it = token_iterator_index(0, &token_array, first_index);
        
        Token * token = 0;
        for (;;)
        {
            token = token_it_read(&it);
            
            if (token->pos >= visible_range.one_past_last || !token || !token_it_inc_non_whitespace(&it))
                break;
            
            if (token->kind == TokenBaseKind_Comment)
            {
                Rect_f32 comment_first_char_rect =
                    text_layout_character_on_screen(App, TextLayoutID, token->pos+2);
                
                Range_i64 token_range =
                {
                    token->pos,
                    token->pos + (token->size > (i64)divider_comment_signifier.size
                                  ? (i64)divider_comment_signifier.size
                                  : token->size),
                };
                
                u8 token_buffer[256] = {0};
                buffer_read_range(App, BufferID, token_range, token_buffer);
                String_Const_u8 token_string =
                {
                    token_buffer,
                    (u64)(token_range.end - token_range.start)
                };
                
                if(string_match(token_string, divider_comment_signifier))
                {
                    // NOTE(sammynilla): We increment the vertical midpoint by 1 at the end because it feels like the the position is slightly off when interacting with the | character.
                    f32 CharacterVerticalMidPoint =
                        comment_first_char_rect.y0 + ((comment_first_char_rect.y1 - comment_first_char_rect.y0)) * 0.5f;
                    ++CharacterVerticalMidPoint;
                    // NOTE(sammynilla): We're using the horizontal mid-point because were building our line position based on where the | falls.
                    f32 CharacterHorizontalMidPoint =
                        comment_first_char_rect.x0 + ((comment_first_char_rect.x1 - comment_first_char_rect.x0)) * 0.5f;
                    
                    Face_ID FaceID = get_face_id(App, BufferID);
                    Face_Description FaceDescription = get_face_description(App, FaceID);
                    u32 FontSize = FaceDescription.parameters.pt_size;
                    
                    u32 DividerHeight = Round((FontSize*0.1f)-0.05f);
                    u32 DividerOffset = (DividerHeight < 1) ? 0 : 1;
                    
                    Rect_f32 DividerRect = {};
                    DividerRect.x0 = CharacterHorizontalMidPoint;
                    DividerRect.x1 = 10000;
                    DividerRect.y0 = CharacterVerticalMidPoint - DividerOffset;
                    DividerRect.y1 = DividerRect.y0 + DividerHeight;
                    
                    draw_rectangle_fcolor(App, DividerRect, 0, fcolor_id(defcolor_comment));
                }
            }
        }
    }
}