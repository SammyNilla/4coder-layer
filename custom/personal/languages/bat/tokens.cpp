
 internal FColor
sn_GetTokenColorBAT(Token token)
{
    Managed_ID color = defcolor_text_default;
    switch (token.kind)
    {
        case TokenBaseKind_Operator:
        {
            switch (token.sub_kind)
            {
                case TokenCppKind_Less:
                case TokenCppKind_Grtr:
                case TokenCppKind_RightRight:
                case TokenCppKind_Eq:
                case TokenCppKind_Plus:
                case TokenCppKind_PlusEq:
                case TokenCppKind_Minus:
                case TokenCppKind_MinusEq:
                case TokenCppKind_Star:
                case TokenCppKind_StarEq:
                case TokenCppKind_Div:
                case TokenCppKind_DivEq:
                case TokenCppKind_Mod:
                case TokenCppKind_ModEq:
                case TokenCppKind_AndAnd:
                case TokenCppKind_OrOr:
                {
                    return fcolor_argb(finalize_color(defcolor_preproc, 1));
                }break;
            }
        }break;
        case TokenBaseKind_Keyword:
        {
            switch (token.sub_kind)
            {
                case TokenCppKind_If:
                case TokenCppKind_Else:
                case TokenCppKind_For:
                case TokenCppKind_Do:
                {
                    color = defcolor_preproc;
                }break;
                case TokenCppKind_BAT_ASSOC:
                case TokenCppKind_BAT_CLS:
                //case TokenCppKind_BAT_CMD:
                case TokenCppKind_BAT_COMP:
                case TokenCppKind_BAT_CONVERT:
                case TokenCppKind_BAT_DATE:
                case TokenCppKind_BAT_DEL:
                case TokenCppKind_BAT_DIR:
                case TokenCppKind_BAT_DISKPART:
                case TokenCppKind_BAT_DRIVERQUERY:
                case TokenCppKind_BAT_ECHO:
                case TokenCppKind_BAT_EXIT:
                case TokenCppKind_BAT_EXPAND:
                case TokenCppKind_BAT_FC:
                case TokenCppKind_BAT_FIND:
                case TokenCppKind_BAT_FORMAT:
                case TokenCppKind_BAT_HELP:
                case TokenCppKind_BAT_IPCONFIG:
                case TokenCppKind_BAT_LABEL:
                case TokenCppKind_BAT_MD:
                case TokenCppKind_BAT_MORE:
                case TokenCppKind_BAT_MOVE:
                case TokenCppKind_BAT_NET:
                case TokenCppKind_BAT_PATH:
                case TokenCppKind_BAT_PAUSE:
                case TokenCppKind_BAT_PING:
                case TokenCppKind_BAT_RD:
                case TokenCppKind_BAT_REM:
                case TokenCppKind_BAT_REN:
                case TokenCppKind_BAT_SET:
                case TokenCppKind_BAT_SHUTDOWN:
                case TokenCppKind_BAT_SORT:
                case TokenCppKind_BAT_START:
                case TokenCppKind_BAT_SYSTEMINFO:
                case TokenCppKind_BAT_TASKKILL:
                case TokenCppKind_BAT_TASKLIST:
                case TokenCppKind_BAT_TIME:
                case TokenCppKind_BAT_TITLE:
                case TokenCppKind_BAT_TREE:
                case TokenCppKind_BAT_TYPE:
                case TokenCppKind_BAT_VER:
                case TokenCppKind_BAT_VOL:
                case TokenCppKind_BAT_XCOPY:
                {
                    color = defcolor_keyword;
                }break;
                case TokenCppKind_BAT_EQU:
                case TokenCppKind_BAT_NEQ:
                case TokenCppKind_BAT_LSS:
                case TokenCppKind_BAT_LEQ:
                case TokenCppKind_BAT_GTR:
                case TokenCppKind_BAT_GEQ:
                {
                    color = defcolor_comment;
                }break;
            }
        }break;
        case TokenBaseKind_LiteralString:
        {
            color = defcolor_str_constant;
        }break;
        case TokenBaseKind_LiteralInteger:
        {
            color = defcolor_int_constant;
        }break;
        // NOTE(sammynilla): is batch okay with floats?
        case TokenBaseKind_LiteralFloat:
        {
            color = defcolor_float_constant;
        }break;
    }
    return(fcolor_id(color));
}