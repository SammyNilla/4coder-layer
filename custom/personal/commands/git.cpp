
// TODO(sammynilla): Look into making this not go to some random buffer.
// Would prefer to have it displayed in the compilation buffer.
CUSTOM_COMMAND_SIG(git_init)
CUSTOM_DOC("Initializes a git repo in an project's root directory.")
{
    Scratch_Block Scratch(app);
    String_Const_u8 OutputBuffer = SCu8("*messages*");
    String_Const_u8 Command = SCu8("git init");
    String_Const_u8 HotDir = push_hot_directory(app, Scratch);
    
    { // NOTE(sammynilla): Output buffer copy
        u64 size = clamp_top(OutputBuffer.size, sizeof(out_buffer_space));
        block_copy(out_buffer_space, OutputBuffer.str, size);
        out_buffer_space[OutputBuffer.size] = 0;
    }
    
    { // NOTE(sammynilla): Command copy
        u64 size = clamp_top(Command.size, sizeof(command_space));
        block_copy(command_space, Command.str, size);
        command_space[Command.size] = 0;
    }
    
    { // NOTE(sammynilla): Hot directory copy
        u64 size = clamp_top(HotDir.size, sizeof(hot_directory_space));
        block_copy(hot_directory_space, HotDir.str, size);
        hot_directory_space[HotDir.size] = 0;
    }
    
    execute_previous_cli(app);
}
