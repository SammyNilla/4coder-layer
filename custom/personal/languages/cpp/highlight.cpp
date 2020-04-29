
/*
   NOTE(sammynilla):
   Implementation for highlighting namespace things.
   The only known issue is that when defining class functions non-inline,
   the class names will also be highlighted.

   In the below example, foo would be highlighted for the function definition.

   class foo {
     void bar();
   };

   foo::bar() {}

   This is clearly sub-optimal, but we'll save this function just in case.
*/
/*
static
void
sn_paint_namespace(Application_Links * app,
                   Buffer_ID buffer,
                   Text_Layout_ID text_layout_id)
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
            
            if (keyword_length != 0 && token->sub_kind == TokenCppKind_ColonColon)
                end_pos = token->pos;
            
            // search for default text, count up the size
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
            
            // color text
            if (start_pos != 0 && end_pos != 0)
            {
                Range_i64 range = { 0 };
                range.start = start_pos;
                range.end = end_pos;
                
                if (token->sub_kind == TokenCppKind_ColonColon)
                    paint_text_color(app, text_layout_id, range,
                                     fcolor_resolve(fcolor_id(defcolor_bool_constant)));
                
                end_pos = 0;
                start_pos = 0;
            }
            
            if (!token_it_inc_all(&it))
                break;
        }
    }
}*/