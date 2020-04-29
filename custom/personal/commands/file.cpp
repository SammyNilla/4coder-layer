
internal void
sn_NewCHeaderFile(Application_Links * App, Buffer_ID BufferID, String_Const_u8 FileName)
{
    Scratch_Block Scratch(App);
    
    List_String_Const_u8 GuardList = {};
    for (u64 i = 0; i < FileName.size; ++i){
        u8 c[2] = {};
        u64 c_size = 1;
        u8 ch = FileName.str[i];
        if ('A' <= ch && ch <= 'Z'){
            c_size = 2;
            c[0] = '_';
            c[1] = ch;
        }
        else if ('0' <= ch && ch <= '9'){
            c[0] = ch;
        }
        else if ('a' <= ch && ch <= 'z'){
            c[0] = ch - ('a' - 'A');
        }
        else{
            c[0] = '_';
        }
        String_Const_u8 Part = push_string_copy(Scratch, SCu8(c, c_size));
        string_list_push(Scratch, &GuardList, Part);
    }
    String_Const_u8 Guard = string_list_flatten(Scratch, GuardList);
    
    Buffer_Insertion Insert = begin_buffer_insertion_at_buffered(App, BufferID, 0, Scratch, KB(16));
    insertf(&Insert,
            "#if !defined(%.*s)\n"
            "#define %.*s\n"
            "\n"
            "#endif //%.*s\n",
            string_expand(Guard),
            string_expand(Guard),
            string_expand(Guard));
    end_buffer_insertion(&Insert);
}


internal void
sn_NewBATFile(Application_Links * App, Buffer_ID BufferID, String_Const_u8 FileName)
{
    Scratch_Block Scratch(App);
    
    Buffer_Insertion Insert = begin_buffer_insertion_at_buffered(App, BufferID, 0, Scratch, KB(2));
    insertf(&Insert, "@ECHO OFF\n");
    end_buffer_insertion(&Insert);
}

CUSTOM_COMMAND_SIG(close_all_files)
CUSTOM_DOC("close all files that are opened no matter the extension")
{
    // NOTE(sammynilla): Original command can be found at the link below.
    //https://github.com/exanimusfan/4coder_extensions/blob/master/4coder_load_projects.cpp
    Buffer_ID buffer = get_buffer_next(app, 0, Access_Always);
    while (buffer != 0)
    {
        buffer_kill(app, buffer, BufferKill_AlwaysKill);
        buffer = get_buffer_next(app, buffer, Access_Always);
    }
}