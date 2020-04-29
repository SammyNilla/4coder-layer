
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