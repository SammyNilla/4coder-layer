
internal FColor
sn_GetTokenColorCPP(Token token)
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
                case TokenCppKind_PPHasInclude:
                case TokenCppKind_PPHasCppAttribute:
                {
                    color = defcolor_preproc;
                }break;
                case TokenCppKind_PPPush:
                case TokenCppKind_PPPop:
                {
                    return fcolor_argb(finalize_color(defcolor_preproc, 1));
                }break;
                case TokenCppKind_PPEnable:
                case TokenCppKind_PPDisable:
                case TokenCppKind_PPOnce:
                {
                    color = defcolor_keyword;
                }break;
                // NOTE(sammynilla): Namespaces
                case TokenCppKind_NamespaceSTD:
                {
                    color = defcolor_bool_constant;
                }break;
                // NOTE(sammynilla): Cpp Keywords
                case TokenCppKind_Void:
                case TokenCppKind_Bool:
                case TokenCppKind_Char:
                case TokenCppKind_Int:
                case TokenCppKind_Float:
                case TokenCppKind_Double:
                case TokenCppKind_Long:
                case TokenCppKind_Short:
                case TokenCppKind_Unsigned:
                case TokenCppKind_Signed:
                case TokenCppKind_Auto:
                case TokenCppKind_ConstExpr:
                case TokenCppKind_Volatile:
                case TokenCppKind_Asm:
                case TokenCppKind_Typedef:
                case TokenCppKind_Template:
                case TokenCppKind_Typename:
                case TokenCppKind_Friend:
                case TokenCppKind_Extern:
                case TokenCppKind_Export:
                case TokenCppKind_Inline:
                case TokenCppKind_Static:
                case TokenCppKind_Virtual:
                case TokenCppKind_AlignAs:
                case TokenCppKind_Explicit:
                case TokenCppKind_NullPtr:
                case TokenCppKind_Operator:
                case TokenCppKind_Register:
                case TokenCppKind_ThreadLocal:
                case TokenCppKind_SizeOf:
                case TokenCppKind_AlignOf:
                case TokenCppKind_DeclType:
                case TokenCppKind_TypeID:
                case TokenCppKind_New:
                case TokenCppKind_Using:
                case TokenCppKind_Namespace:
                case TokenCppKind_Class:
                case TokenCppKind_Struct:
                case TokenCppKind_Enum:
                case TokenCppKind_Union:
                {
                    color = defcolor_keyword;
                }break;
                case TokenCppKind_Break:
                case TokenCppKind_Case:
                case TokenCppKind_Catch:
                case TokenCppKind_ConstCast:
                case TokenCppKind_Continue:
                case TokenCppKind_Default:
                case TokenCppKind_Delete:
                case TokenCppKind_Do:
                case TokenCppKind_DynamicCast:
                case TokenCppKind_Else:
                case TokenCppKind_For:
                case TokenCppKind_Goto:
                case TokenCppKind_If:
                case TokenCppKind_NoExcept:
                case TokenCppKind_Private:
                case TokenCppKind_Public:
                case TokenCppKind_Protected:
                case TokenCppKind_ReinterpretCast:
                case TokenCppKind_Return:
                case TokenCppKind_StaticAssert:
                case TokenCppKind_StaticCast:
                case TokenCppKind_Switch:
                case TokenCppKind_This:
                case TokenCppKind_Try:
                case TokenCppKind_While:
                {
                    return fcolor_argb(finalize_color(defcolor_keyword, 1));
                }break;
                
                case TokenCppKind_Const:
                {
                    return fcolor_argb(finalize_color(defcolor_keyword, 2));
                }break;
                // NOTE(sammynilla): Standard Lib
                case TokenCppKind_AutoPtr: // Deprecated
                case TokenCppKind_WeakPtr:
                case TokenCppKind_SharedPtr:
                case TokenCppKind_UniquePtr:
                {
                    color = defcolor_keyword;
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
                case TokenCppKind_LiteralTrue:
                case TokenCppKind_LiteralFalse:
                {
                    color = defcolor_bool_constant;
                }break;
                case TokenCppKind_PPIncludeFile:
                {
                    color = defcolor_include;
                }break;
            }
        }break;
    }
    return fcolor_id(color);
}
