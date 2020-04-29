
// NOTE(sammynilla):
//https://en.wikibooks.org/wiki/C_Programming/Language_Reference#Table_of_operators
//https://en.cppreference.com/w/c/keyword
internal FColor
sn_GetTokenColorC(Token token)
{
    Managed_ID color = defcolor_text_default;
    switch (token.kind)
    {
        case TokenBaseKind_Preprocessor:
        {
            color = defcolor_preproc;
            switch (token.sub_kind)
            {
                case TokenCppKind_PPInclude:
                {
                    return fcolor_argb(finalize_color(defcolor_preproc, 1));
                }break;
                case TokenCppKind_PPPragma:
                {
                    color = defcolor_keyword;
                }break;
            }
        }break;
        case TokenBaseKind_Keyword:
        {
            switch (token.sub_kind)
            {
                // NOTE(sammynilla): Preprocessor Keywords
                case TokenCppKind_PPDefined:
                {
                    color = defcolor_preproc;
                }break;
                case TokenCppKind_PPPush:
                case TokenCppKind_PPPop:
                {
                    return fcolor_argb(finalize_color(defcolor_preproc, 1));
                }break;
                case TokenCppKind_PPOnce:
                case TokenCppKind_PPEnable:
                case TokenCppKind_PPDisable:
                {
                    color = defcolor_keyword;
                }break;
                case TokenCppKind_Void:
                case TokenCppKind_Bool: // _Bool (c99)
                case TokenCppKind_Char:
                case TokenCppKind_Int:
                case TokenCppKind_Float:
                case TokenCppKind_Double:
                case TokenCppKind_Long:
                case TokenCppKind_Short:
                case TokenCppKind_Unsigned:
                case TokenCppKind_Signed:
                case TokenCppKind_Volatile:
                case TokenCppKind_Asm:
                case TokenCppKind_Break:
                case TokenCppKind_Case:
                case TokenCppKind_Continue:
                case TokenCppKind_Do:
                case TokenCppKind_For:
                case TokenCppKind_Goto:
                case TokenCppKind_Switch:
                case TokenCppKind_While:
                case TokenCppKind_Typedef:
                case TokenCppKind_Extern:
                case TokenCppKind_Inline:  // (c99)
                case TokenCppKind_Static:
                case TokenCppKind_AlignAs: //_Alignas (c11)
                case TokenCppKind_Register:
                case TokenCppKind_ThreadLocal: //_Thread_local (c11)
                case TokenCppKind_SizeOf:
                case TokenCppKind_AlignOf: //_Alignof (c11)
                case TokenCppKind_Default:
                {
                    color = defcolor_keyword;
                }break;
                case TokenCppKind_If:
                case TokenCppKind_Else:
                case TokenCppKind_Enum:
                case TokenCppKind_Return:
                case TokenCppKind_StaticAssert: // _Static_assert (c11)
                case TokenCppKind_Struct:
                case TokenCppKind_Union:
                {
                    return fcolor_argb(finalize_color(defcolor_keyword, 1));
                }break;
                case TokenCppKind_Const:
                {
                    return fcolor_argb(finalize_color(defcolor_keyword, 2));
                }break;
            }
        }break;
        case TokenBaseKind_Comment:
        {
            color = defcolor_comment;
        }break;
        case TokenBaseKind_LiteralString:
        {
            color = defcolor_str_constant;
        }break;
        case TokenBaseKind_LiteralInteger:
        {
            color = defcolor_int_constant;
        }break;
        case TokenBaseKind_LiteralFloat:
        {
            color = defcolor_float_constant;
        }break;
        default:
        {
            switch (token.sub_kind)
            {
                case TokenCppKind_PPIncludeFile:
                {
                    color = defcolor_include;
                }break;
            }
        }break;
    }
    return(fcolor_id(color));
}
