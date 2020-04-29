
 function void
sn_PaintKeysJSON(Application_Links * App,
                 Buffer_ID BufferID,
                 Text_Layout_ID TextLayoutID)
{
	Token_Array TokenArray = get_token_array_from_buffer(App, BufferID);
    Range_i64 VisibleRange = text_layout_get_visible_range(App, TextLayoutID);
    
    if (TokenArray.tokens != 0)
    {
        i64 FirstIndex = token_index_from_pos(&TokenArray, VisibleRange.first);
        Token_Iterator_Array Iter =
            token_iterator_index(0, &TokenArray, FirstIndex);
        
        i64 StartPos = 0;
        for (;;)
        {
            Token * T = token_it_read(&Iter);
            if (T->pos >= VisibleRange.one_past_last || !T || !token_it_inc_non_whitespace(&Iter))
                break;
            
            // get position of literal string key
            if (T->kind == TokenBaseKind_LiteralString)
                StartPos = T->pos;
            
            if (T->sub_kind == TokenCppKind_Colon)
            {
                Range_i64 PaintRange = { StartPos, T->pos };
                paint_text_color(App, TextLayoutID, PaintRange,
                                 fcolor_resolve(fcolor_id(defcolor_keyword)));
            }
        }
    }
}
