
CUSTOM_COMMAND_SIG(load_project)
CUSTOM_DOC("Looks for a project.4coder file in the current directory and tries to load it.  Looks in parent directories until a project file is found or there are no more parents.")
{
    ProfileScope(app, "[sammynilla] load project");
    save_all_dirty_buffers(app);
    set_current_project_from_nearest_project_file(app);
    
    if (current_project.loaded)
    {
        char * path = (char *)current_project.dir.str;
        _strlwr_s(path, strlen(path) + 1);
        
        i32 index = 0;
        for (; index < pp_meta.count; ++index)
        {
            if (strcmp(pp_meta.path[index], path) == 0)
                goto finish_load;
        }
        
        push_project_path_metadata(path);
        return;
        
        finish_load:;
        move_project_path_at_index_to_front(index);
    }
}

CUSTOM_COMMAND_SIG(setup_new_project)
CUSTOM_DOC("Queries the user for several configuration options and initializes a new 4coder project with build scripts for every OS.")
{
    project_setup_scripts__generic(app, true, false, false);
    load_project(app);
}