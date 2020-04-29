
CUSTOM_COMMAND_SIG(sn_4coder_Startup)
{
    ProfileScope(app, "[sammynilla] startup");
    User_Input input = get_current_input(app);
    if (match_core_code(&input, CoreCode_Startup))
    {
        String_Const_u8_Array file_names = input.event.core.file_names;
        load_themes_default_folder(app);
        default_4coder_initialize(app, file_names);
        
        OpenBuildPanelOrResize(app);
        default_4coder_side_by_side_panels(app, file_names);
        
        //if (global_config.automatically_load_project)
        //load_project(app);
        
        load_project_path_metadata(app);
    }
}

CUSTOM_COMMAND_SIG(sn_4coder_TryExit)
{
    User_Input input = get_current_input(app);
    if (match_core_code(&input, CoreCode_TryExit))
    {
        b32 do_exit = true;
        if (!allow_immediate_close_without_checking_for_changes)
        {
            b32 has_unsaved_changes = false;
            for (Buffer_ID buffer = get_buffer_next(app, 0, Access_Always);
                 buffer != 0;
                 buffer = get_buffer_next(app, buffer, Access_Always))
            {
                Dirty_State dirty = buffer_get_dirty_state(app, buffer);
                if (HasFlag(dirty, DirtyState_UnsavedChanges))
                {
                    has_unsaved_changes = true;
                    break;
                }
            }
            if (has_unsaved_changes)
            {
                View_ID view = get_active_view(app, Access_Always);
                do_exit = do_4coder_close_user_check(app, view);
            }
        }
        if (do_exit)
        {
            save_project_path_metadata();
            hard_exit(app);
        }
    }
}