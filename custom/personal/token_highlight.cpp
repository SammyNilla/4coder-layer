
#include "languages/cpp/highlight.cpp"
#include "languages/json/highlight.cpp"

function void
sn_HighlightTokens(Application_Links * App,
                   Text_Layout_ID TextLayoutID,
                   Token_Array * TokenArray,
                   FColor (*TokenColorCallback)(Token))
{
    Range_i64 VisibleRange =
        text_layout_get_visible_range(App, TextLayoutID);
    i64 FirstIndex = token_index_from_pos(TokenArray, VisibleRange.first);
    Token_Iterator_Array Iter =
        token_iterator_index(0, TokenArray, FirstIndex);
    
    for (;;)
    {
        Token * T = token_it_read(&Iter);
        if (T->pos >= VisibleRange.one_past_last)
            break;
        
        FColor TokenColor = TokenColorCallback(*T);
        paint_text_color(App, TextLayoutID,
                         Ii64_size(T->pos, T->size),
                         fcolor_resolve(TokenColor));
        
        if (!token_it_inc_all(&Iter))
            break;
    }
}