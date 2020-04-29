
 function void
Fleury4RenderCloseBraceAnnotation(Application_Links * app,
                                  Buffer_ID buffer,
                                  Text_Layout_ID text_layout_id,
                                  i64 pos)
{
    ProfileScope(app, "[Fleury] Brace Annotation");
    
    Range_i64 visible_range = text_layout_get_visible_range(app, text_layout_id);
    Token_Array token_array = get_token_array_from_buffer(app, buffer);
    Face_ID face_id = get_face_id(app, buffer);
    
    if (token_array.tokens != 0)
    {
        Token_Iterator_Array it = token_iterator_pos(0, &token_array, pos);
        Token * token = token_it_read(&it);
        
        if (token != 0 && token->kind == TokenBaseKind_ScopeOpen)
        {
            pos = token->pos + token->size;
        }
        else if (token_it_dec_all(&it))
        {
            token = token_it_read(&it);
            if (token->kind == TokenBaseKind_ScopeClose &&
                pos == token->pos + token->size)
            {
                pos = token->pos;
            }
        }
    }
    
    Scratch_Block scratch(app);
    Range_i64_Array ranges = get_enclosure_ranges(app, scratch, buffer, pos, RangeHighlightKind_CharacterHighlight);
    
    for (i32 i = ranges.count - 1; i >= 0; i -= 1)
    {
        Range_i64 range = ranges.ranges[i];
        
        if (range.start >= visible_range.start)
            continue;
        
        Rect_f32 close_scope_rect = text_layout_character_on_screen(app, text_layout_id, range.end);
        Vec2_f32 close_scope_pos = { close_scope_rect.x0 + 12, close_scope_rect.y0 };
        
        // NOTE(rjf): Find token set before this scope begins.
        Token * start_token = 0;
        i64 token_count = 0;
        {
            Token_Iterator_Array it = token_iterator_pos(0, &token_array, range.start-1);
            int paren_nest = 0;
            
            for (;;)
            {
                Token * token = token_it_read(&it);
                if (!token_it_dec_non_whitespace(&it))
                    break;
                
                if (token)
                {
                    token_count += 1;
                    
                    if(token->kind == TokenBaseKind_ParentheticalClose)
                    {
                        ++paren_nest;
                    }
                    else if(token->kind == TokenBaseKind_ParentheticalOpen)
                    {
                        --paren_nest;
                    }
                    else if(paren_nest == 0 &&
                            (token->kind == TokenBaseKind_ScopeClose ||
                             token->kind == TokenBaseKind_StatementClose))
                    {
                        break;
                    }
                    else if((token->kind == TokenBaseKind_Identifier || token->kind == TokenBaseKind_Keyword ||
                             token->kind == TokenBaseKind_Comment) &&
                            !paren_nest)
                    {
                        start_token = token;
                        break;
                    }
                    
                }
                else
                {
                    break;
                }
            }
        }
        
        // NOTE(rjf): Draw.
        if(start_token)
        {
            // NOTE(sammynilla): Color get managed here.
            u32 color = finalize_color(defcolor_comment, 0);
            color &= 0x00ffffff;
            color |= 0x80000000;
            // NOTE(sammynilla): We can add or remove the unicode text ligature here.
            draw_string(app, face_id, string_u8_litexpr("←"), close_scope_pos, color);
            // NOTE(sammynilla): Below is the distance in pixels between the end of the above ligature and the start of the scope definition.
            close_scope_pos.x += 24;
            /*
             * TODO(sammynilla): Currently this only generates the closing brace annotation based on the line number of the first token.
             * However, we want to make it so that the full definition is rendered regardless of whitespace (from paren-to-paren).
             */
            String_Const_u8 start_line = push_buffer_line(app, scratch, buffer,
                                                          get_line_number_from_pos(app, buffer, start_token->pos));
            
            // NOTE(sammynilla): ??
            u64 first_non_whitespace_offset = 0;
            for (u64 c = 0; c < start_line.size; ++c)
            {
                if (start_line.str[c] <= 32)
                    ++first_non_whitespace_offset;
                else
                    break;
            }
            start_line.str += first_non_whitespace_offset;
            start_line.size -= first_non_whitespace_offset;
            
            // NOTE(sammynilla): Draw the actual scope definition
            draw_string(app, face_id, start_line, close_scope_pos, color);
        }
    }
}