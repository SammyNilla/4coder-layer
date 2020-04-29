
CUSTOM_UI_COMMAND_SIG(projects_lister)
CUSTOM_DOC("List of all your projects")
{
    Scratch_Block scratch(app);
    char * query = "Load your project:";
    
    Lister_Block lister(app, scratch);
    lister_set_query(lister, query);
    lister_set_default_handlers(lister);
    
    code_index_lock();
    
    // NOTE(sammynilla): Check out String_Const_u8_Array for potential improvement
    String_Const_u8 lister_ptr[PPMETA_MAX_PROJECT_COUNT];
    for (i32 i = 0; i < pp_meta.count; ++i)
    {
        lister_ptr[i] = SCu8(pp_meta.path[i]);
        // NOTE(sammynilla): lister, path, alias, ??, 0
        lister_add_item(lister, SCu8(pp_meta.path[i]), SCu8(), &lister_ptr[i], 0);
    }
    
    code_index_unlock();
    
    Lister_Result l_result = run_lister(app, lister);
    String_Const_u8 * result = 0;
    if (!l_result.canceled && l_result.user_data != 0)
    {
        result = (String_Const_u8 *)l_result.user_data;
        if (result->str != 0)
        {
            set_hot_directory(app, *result);
            close_all_files(app);
            load_project(app);
        }
    }
}
