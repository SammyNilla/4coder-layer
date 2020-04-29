
BUFFER_HOOK_SIG(sn_BeginBuffer)
{
    ProfileScope(app, "[sammynilla] begin buffer");
    Scratch_Block scratch(app);
    
    b32 TreatAsCode = false;
    String_Const_u8 FileName = push_buffer_file_name(app, scratch, buffer_id);
    if (FileName.size > 0)
    {
        String_Const_u8_Array Extensions = global_config.code_exts;
        String_Const_u8 Ext = string_file_extension(FileName);
        for (i32 i = 0; i < Extensions.count; ++i)
        {
            if (string_match(Ext, Extensions.strings[i]))
            {
                TreatAsCode= true;
                break;
            }
        }
    }
    
    Command_Map_ID MapID = (TreatAsCode)?(mapid_code):(mapid_file);
    Managed_Scope Scope = buffer_get_managed_scope(app, buffer_id);
    Command_Map_ID *MapIDPtr =
        scope_attachment(app, Scope, buffer_map_id, Command_Map_ID);
    *MapIDPtr = MapID;
    
    Line_Ending_Kind EOLSetting = guess_line_ending_kind_from_buffer(app, buffer_id);
    Line_Ending_Kind *EOLSettingPtr =
        scope_attachment(app, Scope, buffer_eol_setting, Line_Ending_Kind);
    *EOLSettingPtr = EOLSetting;
    
    // NOTE(allen): Decide buffer settings
    b32 WrapLines = true;
    b32 UseVirtualWhitespace = false;
    b32 UseLexer = false;
    if (TreatAsCode)
    {
        WrapLines= global_config.enable_code_wrapping;
        UseVirtualWhitespace = global_config.enable_virtual_whitespace;
        UseLexer = true;
    }
    
    // NOTE(sammynilla): April 7, 2020
    // Compilation buffer line wrap doesn't work. Allen said he would have it fixed for next release. Until then.
    
    if (UseLexer)
    {
        ProfileBlock(app, "begin buffer kick off lexer");
        Async_Task *LexTaskPtr =
            scope_attachment(app, Scope, buffer_lex_task, Async_Task);
        *LexTaskPtr =
            async_task_no_dep(&global_async_system, do_full_lex_async, make_data_struct(&buffer_id));
    }
    
    {
        b32 *WrapLinesPtr = scope_attachment(app, Scope, buffer_wrap_lines, b32);
        *WrapLinesPtr = WrapLines;
    }
    
    if (UseVirtualWhitespace)
    {
        if (UseLexer)
            buffer_set_layout(app, buffer_id, layout_virt_indent_index_generic);
        else
            buffer_set_layout(app, buffer_id, layout_virt_indent_literal_generic);
    }
    else
    {
        buffer_set_layout(app, buffer_id, layout_generic);
    }
    
    return 0;
}