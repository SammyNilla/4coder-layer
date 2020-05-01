
CUSTOM_COMMAND_SIG(execute_previous_cli_in_footer_panel)
CUSTOM_DOC("If the command execute_any_cli has already been used, this will execute a CLI in the footer pannel using most recent buffer name and command.")
{
    String_Const_u8 OutputBuffer = SCu8(out_buffer_space);
    String_Const_u8 Command = SCu8(command_space);
    String_Const_u8 HotDir = SCu8(hot_directory_space);
    
    if (OutputBuffer.size > 0 && Command.size > 0 && HotDir.size > 0)
    {
        View_ID CompilationBuffer = open_build_footer_panel(app);
        Buffer_Identifier BufferID = buffer_identifier(OutputBuffer);
        exec_system_command(app, CompilationBuffer, BufferID, HotDir, Command, CLI_OverlapWithConflict|CLI_CursorAtEnd|CLI_SendEndSignal);
        lock_jump_buffer(app, OutputBuffer);
    }
}