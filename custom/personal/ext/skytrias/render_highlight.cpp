
// NOTE(Skytrias): paints all standard text leading to a '('
function void
st_paint_functions(Application_Links * app, Buffer_ID buffer,
                   Text_Layout_ID text_layout_id, ARGB_Color color)
{
    i64 keyword_length = 0;
    i64 start_pos = 0;
    i64 end_pos = 0;
    
	Token_Array array = get_token_array_from_buffer(app, buffer);
    if (array.tokens != 0)
    {
        Range_i64 visible_range = text_layout_get_visible_range(app, text_layout_id);
        i64 first_index = token_index_from_pos(&array, visible_range.first);
        Token_Iterator_Array it = token_iterator_index(0, &array, first_index);
        for (;;)
        {
            Token * token = token_it_read(&it);
            if (token->pos >= visible_range.one_past_last)
                break;
            
			if (keyword_length != 0 && token->kind == TokenBaseKind_ParentheticalOpen)
                end_pos = token->pos;
            
            if (token->kind == TokenBaseKind_Identifier)
            {
                if (keyword_length == 0)
                    start_pos = token->pos;
                
                keyword_length += 1;
            }
            else
            {
                keyword_length = 0;
            }
            
            if (start_pos != 0 && end_pos != 0)
            {
                Range_i64 range = { 0 };
                range.start = start_pos;
                range.end = end_pos;
				
                if (token->sub_kind == TokenCppKind_ParenOp)
                    paint_text_color(app, text_layout_id, range, color);
				
                end_pos = 0;
                start_pos = 0;
            }
            
            if (!token_it_inc_all(&it))
                break;
        }
    }
}